function ddm_load()
{
    window.ddmMap = { 
      el: null,
      loaded: false,
      map: false,
      center: false,
      marker: false,
      counties: {},
      selection: [],
      bounds: false,
      popup: false,
      
      initialize: function() {
        //console.log( 'ddmMap.initialize' );
        var self = this;
        self.el = document.getElementById( 'map_canvas' ); 
        self.map = new google.maps.Map( self.el, {
          zoom: 4,
          center: self._createPoint( 44.4024, -100.459 ),
          mapTypeId: google.maps.MapTypeId.ROADMAP
        });
        
        self.popup = new google.maps.InfoWindow({
        });
        
        ddmFilter = window.ddmFilter || {};
        ddmMapView = window.ddmMapView || {};
        
        self.updateVisualzation = window.ddmFilter.updateVisualzation;
        google.maps.event.addListener( self.map, 'idle', function() {
          // console.log( 'self.map.idle' );
          if ( !self.loaded ) {
            self.loaded = self.load();
          }
        });

        google.maps.event.addListener( self.map, 'click', function( event ) {
          // console.log( 'self.map.click' );
          self.panTo( event.latLng );
        });
        
        google.maps.event.addListener( self.map, 'mousemove', function( event ) {
          // console.log( 'self.map.mousemove' );
          // console.log( event.latLng.toString() );
          self.mousemove( event.latLng );
        });
        
        if ( _.has( ddmFilter, 'selectionUpdated' ) ) {
          ddmFilter.selectionUpdated.connect( self, self.update );
        }

        if ( _.has( ddmMapView, 'resized' ) ) {
          ddmMapView.resized.connect( self, self.resize );
        }
        
        self.bounds = new google.maps.LatLngBounds();
            
        return this;
      },
      
      panTo: function( lat, lng ) {
        // console.log( 'ddmMap.panTo' );
        var self = this;
        var center = lat instanceof google.maps.LatLng ? lat : self._createPoint( lat, lng );
        self.center = center;
        self.map.panTo( center );
        return this;
      },
      
      mousemove: function( lat, lng ) {
        // console.log( 'ddmMap.mousemove' );
        var self = this;
        var point = lat instanceof google.maps.LatLng ? lat : self._createPoint( lat, lng );
        if ( _.has( ddmMapView, 'mousemove' ) ) {
            ddmMapView.mousemove( point.lat(), point.lng() );
        }
        return this;
      },
      
      click: function( lat, lng ) {
        // console.log( 'ddmMap.click' );
        this.panTo( lat, lng );
        return this;
      },
      
      setMarker: function( lat, lng ) {
        // console.log( 'ddmMap.setMarker' );
        var self = this;
        var point = lat instanceof google.maps.LatLng ? lat : self._createPoint( lat, lng );
        if ( !self.marker ) {
          self.marker = new google.maps.Marker({
            position: point,
            map: self.map
          });
        }
        self.marker.setPosition( point );
        self.map.panTo( point );
        return this;
      },
      
      load: function() {
        // console.log( 'ddmMap.load' );
        var self = this;
        if ( _.has( ddmMapView, 'loaded' ) ) {
          ddmMapView.loaded();
          // console.log( 'ddmMapView.loaded' );
        }
        self.update();
        return true;
      },
      
      update: function() {
        // console.log( 'ddmMap.update' );
        var self = this;
                
        for ( var id in self.counties ) {
          self.hide( id );
        }
        
        var selection = _.result( ddmFilter, 'selection', [] ) || [];
        //console.log( 'ddmMap.update selection.length=' + selection.length );
        var bounds = new google.maps.LatLngBounds();
        _.each( selection, function( item ) {
          if ( !_.has( self.counties, item.id ) ) {
            //_.defer( function() {
                county = self._addCounty( item.id );
                bounds.union( county.bounds );
            //});
          }
          else {
            self.show( item.id );
            county = self.counties[id];
            bounds.union( county.bounds );
          }
        });
        self.selection = selection;
        self.bounds = bounds;
        // console.log( bounds.toString() );
             
        if ( _.has( ddmMapView, 'updateVisualization' ) ) {
            ddmMapView.updateVisualization();
        }
        self.fitSelection();
        return this;
      },
      
      resize: function( width, height ) {
        // console.log( 'ddmMap.resize (width=' + width + ', height=' + height + ')' );
        var self = this;
        self.el.style.width = width + 'px';
        self.el.style.height = height + 'px';
        //google.maps.event.trigger( self.map, 'resize' );
        //self.panTo( self.center );
      },
      
      show: function( id, flag ) {
        var self = this;
        var county = self.counties[id];
        if ( county ) {
          county.polygon.setVisible( arguments.length == 1 ? true : flag );
        }
      },
      
      hide: function( id ) {
        return this.show( id, false );
      },
      
      zoom_in: function() {
        var map = this.map;
        var zoom = map.getZoom();
        this.map.setZoom( zoom + 1 );
        return this;
      },
      
      zoom_out: function() {
        var map = this.map;
        var zoom = map.getZoom();
        this.map.setZoom( zoom - 1 );
        return this;
      },
      
      fitCounty: function( id ) {
        var self = this;
        // console.log( 'id=' + id );
        var county = self.counties[id];
        if ( !county ) {
          self._addCounty( id );
        }
        county = self.counties[id];
        // console.log( county.bounds.toString() );
        self.map.fitBounds( county.bounds );
        return this;
      },
      
      fitSelection: function() {
        var self = this;
        if ( !self.bounds.isEmpty() ) {
          self.map.fitBounds( self.bounds );
        }
        return this;
      },
      
      _findCounty: function( id ) {
        // console.log( 'ddmMap._findCounty' );
        return _.find( ddmFilter.model.counties, function( county ) {
          return ( county.id == id );
        });
      },
      
      _addCounty: function( id ) {
        // console.log( 'ddmMap._addCounty (id=' + id + ')' );
        var self = this;
        var model = self._findCounty( id );
        
        var bounds = new google.maps.LatLngBounds();
        var paths = self._createMVCArray();
        var bc = 0, vc = 0;
        _.each( model.boundaries, function( boundary ) {
            var loop = self._createMVCArray();
            _.each( boundary.vertices, function( vertex ) {
                var point = self._createPoint( vertex.y, vertex.x );
                loop.push( point );
                bounds.extend( point );
            });
            bc++;
            vc += loop.length;
            paths.push( loop );
        });
        var polygon = new google.maps.Polygon({
          fillColor: model.fillColor,
          fillOpacity: model.fillOpacity,
          strokeColor: model.strokeColor,
          strokeWeight: model.strokeWeight,
          strokeOpacity: model.strokeOpacity,
          visible: false,
          paths: paths,
          map: self.map
        });
        //console.log( "Building polygons for " + model.name + " (" + bc + " boundaries, " + vc + " vertices)..." );
        
        google.maps.event.addListener( polygon, 'click', function( event ) {
          var p = event.latLng;
          var county_id = this.get( 'county_id' );
          var model = self.counties[county_id].model;
          model.clicked( p.lat(), p.lng() );
          self.click( p );
          self.showPopup( p, 
            '<dl>' + 
              '<dt>' + model.name + '</dt>' + 
              '<dd>Население: ' + model.population + ' чел.</dd>' + 
              '<dd>Кол-во приехавших: ' + model.in_sum + ' чел.</dd>' + 
              '<dd>Кол-во уехавших: ' + model.out_sum + ' чел.</dd>' + 
              '<dd>Среднее трение: ' + Number( model.f_mid.toFixed( 2 ) ) + '</dd>' + 
            '</dl>'
          );
        });
        google.maps.event.addListener( polygon, 'mousemove', function( event ) {
          self.mousemove( event.latLng );
        });
        google.maps.event.addListener( polygon, 'mouseover', function( event ) {
          model.mouseover();
        });
        google.maps.event.addListener( polygon, 'mouseout', function( event ) {
          model.mouseout();
        });

        polygon.set( 'county_id', model.id );
        
        model.hidden.connect( function() {
          // console.log( model.name + " hidden" );
          polygon.setVisible( false );
        });
        model.shown.connect( function() {
          // console.log( model.name + " shown" );
          polygon.setVisible( true );
        });
        model.repaint.connect( function() {
          polygon.setOptions({
            fillColor: model.fillColor,
            fillOpacity: model.fillOpacity,
            strokeColor: model.strokeColor,
            strokeWeight: model.strokeWeight,
            strokeOpacity: model.strokeOpacity,
          });
        });
        
        self.counties[id] = {
          polygon: polygon,
          model: model,
          bounds: bounds
        };
        
        polygon.setVisible( true );
        
        return self.counties[id];
      },
      
      showPopup: function( pos, content ) {
        var self = this;
        self.popup.setPosition( pos );
        self.popup.setContent( content );
        self.popup.open( self.map );
      },
      
      _createPoint: function( lat, lng ) {
        return new google.maps.LatLng( lat, lng );
      },
      
      _createMVCArray: function() {
        return new google.maps.MVCArray();
      },
          
      drawArrow: function( beginX, beginY, endX, endY, width ) {
        var self = this;
        var len = self._calc_distance( beginX, beginY, endX, endY );
        var h = 0.1 * len;
        var inv_len = 1./len;
                        
        var ex = ( endX - beginX ) * inv_len;
        var ey = ( endY - beginY ) * inv_len;
                
        var ox = endX - ex * h;
        var oy = endY - ey * h;
        var nx = beginY - endY;  
        var ny = endX - beginX;
        var inv_len_nx = 1./ Math.sqrt( ( ( endX - beginX ) * ( endX - beginX ) + ( endY - beginY ) * ( endY - beginY ) ) );
        nx = nx * inv_len_nx;
        ny = ny * inv_len_nx; 

        var arrowX1 = width * nx + ox;
        var arrowY1 = width * ny + oy;
        var arrowX2 = -width * nx + ox;
        var arrowY2 = -width * ny + oy;                         
                     
        var flightPlanCoordinates = [];
        flightPlanCoordinates.push( new google.maps.LatLng( beginX, beginY ) );
        flightPlanCoordinates.push( new google.maps.LatLng( ox, oy) );
        flightPlanCoordinates.push( new google.maps.LatLng( arrowX1, arrowY1) );
        flightPlanCoordinates.push( new google.maps.LatLng( endX, endY ) );
        flightPlanCoordinates.push( new google.maps.LatLng( arrowX2, arrowY2) );
        flightPlanCoordinates.push( new google.maps.LatLng( ox, oy) );
        flightPlanCoordinates.push( new google.maps.LatLng( arrowX1, arrowY1) );
        var polylineOptions = { path: flightPlanCoordinates, strokeWeight: 0.5 };
        var flightPath = new google.maps.Polyline( polylineOptions ); 
        flightPath.setMap( self.map );
      },
      
        _calc_distance:function(  x1, y1, x2, y2 ) {
          
          pi = Math.PI.toFixed( 20 );  //3.14159265358979311600
          lat1  = x1 * pi / 180.0;
          long1 = y1 * pi / 180.0;
          sinl1 = Math.sin( lat1 );
          cosl1 = Math.cos( lat1 );
                
          lat2  = x2 * pi / 180.0;
          long2 = y2 * pi / 180.0;
          sinl2 = Math.sin( lat2 );
          cosl2 = Math.cos( lat2 );
                
          dl    = long2 - long1;
          sindl = Math.sin( dl );
          cosdl = Math.cos( dl );
                
          a = cosl2 * sindl;
          b = cosl1 * sinl2 - sinl1 * cosl2 * cosdl;
          y = Math.sqrt( a*a + b*b );
          x = sinl1 * sinl2 + cosl1 * cosl2 * cosdl;
          d = Math.atan2( y, x ) * 6372795; // радиус Земли
          return d;
        },
      dummy: 1
      
    }; // window.ddmMap
        
    
    window.ddmMap.initialize();
}

