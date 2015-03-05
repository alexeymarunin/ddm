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
      
      initialize: function() {
        // console.log( 'ddmMap.initialize' );
        var self = this;
        self.el = document.getElementById( 'map_canvas' ); 
        self.map = new google.maps.Map( self.el, {
          zoom: 8,
          center: self._createPoint( 44.4024, -100.459 ),
          mapTypeId: google.maps.MapTypeId.ROADMAP
        });
        
        ddmFilter = window.ddmFilter || {};
        ddmMapView = window.ddmMapView || {};
        
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
        //console.log( 'ddmMap.update' );
        var self = this;
        
        for ( var id in self.counties ) {
          self.hide( id );
        }

        var selection = _.result( ddmFilter, 'selection', [] ) || [];
        console.log( 'ddmMap.update selection.length=' + selection.length );
        _.each( selection, function( item ) {
          if ( !_.has( self.counties, item.id ) ) {
            _.defer( function() {
                self._addCounty( item.id );
                self.show( item.id );
            });
          }
          else {
            self.show( item.id );
          }
        });
        self.selection = selection;
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
        
        var paths = self._createMVCArray();
        var bc = 0, vc = 0;
        _.each( model.boundaries, function( boundary ) {
            var loop = self._createMVCArray();
            _.each( boundary.vertices, function( vertex ) {
                var point = self._createPoint( vertex.y, vertex.x );
                loop.push( point );
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
          paths: paths,
          map: self.map
        });
        console.log( "Building polygons for " + model.name + " (" + bc + " boundaries, " + vc + " vertices)..." );
        
        google.maps.event.addListener( polygon, 'click', function( event ) {
          var p = event.latLng;
          var county_id = this.get( 'county_id' );
          var model = self.counties[county_id].model;
          model.clicked( p.lat(), p.lng() );
          self.click( p );
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
          console.log( model.name + " hidden" );
          polygon.setVisible( false );
        });
        model.shown.connect( function() {
          console.log( model.name + " shown" );
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
          model: model
        };
        return this;
      },
      
      _createPoint: function( lat, lng ) {
        return new google.maps.LatLng( lat, lng );
      },
      
      _createMVCArray: function() {
        return new google.maps.MVCArray();
      },
      
      dummy: 1
      
    }; // window.ddmMap
    
    window.ddmMap.initialize();
}

