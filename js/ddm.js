function ddm_map( $, config ) {

    var DDM = {};

    var CountyModel = Backbone.Model.extend({
        polygon: null,
        defaults: {
            id: 0,
            name: '',
            selected: false,
            hover: false,
            boundaries: [],
            center: { x:0, y:0 }
        },
        
        initialize: function() {
            var self = this;
            var boundaries = self.get( 'boundaries' );
            
            // Создаем объект полигона
            var polygon = new google.maps.Polygon( config.polygon.clean );
            var paths = new google.maps.MVCArray();
            _.each( boundaries, function( boundary ) {
                var loop = new google.maps.MVCArray();
                _.each( boundary.vertices, function( vertex ) {
                    var point = new google.maps.LatLng( vertex.y, vertex.x );
                    loop.push( point );
                    /*
                    var marker = new google.maps.Marker({
                      //position: new google.maps.LatLng( 59.3462, -135.0306 ),
                      position: point,
                      map: DDM.Map
                    });
                    */
                });
                paths.push( loop );
            });
            polygon.setPaths( paths );
            polygon.setMap( DDM.Map );
            
            // Вешаем обработчики событий
            google.maps.event.addListener( polygon, 'click', function() {
                self.select();
            });
            google.maps.event.addListener( polygon, 'mouseover', function() {
                self.mouseover( event );
            });
            google.maps.event.addListener( polygon, 'mouseout', function() {
                self.mouseout();
            });
            
            google.maps.event.addListener( polygon, 'mousemove', function (event) {
                self.mousemove( event );
            });
            
            this.polygon = polygon;
            
            this.on( 'change:selected change:hover', this.paint, this );
            return this;
        },
        
        // Перерисовка полигона
        paint: function() {
            var selected = this.get( 'selected' );
            var hover = this.get( 'hover' );
            var options = {};
            if ( selected ) {
                if ( hover ) { 
                    options = _.extend( options, config.polygon.select, config.polygon.hover );
                }
                else {
                    options = _.extend( options, config.polygon.select );
                }
            }
            else {
                if ( hover ) {
                    options = _.extend( options, config.polygon.clean, config.polygon.hover );
                }
                else {
                    options = _.extend( options, config.polygon.clean );
                }
            }
            this.polygon.setOptions( options );
            return this;
        },

        select: function() {
            this.set( 'selected', true );
            var countyId = this.get( 'id' );
            DDM.setCurrentCountyFromJS( countyId );
            return this;
        },
        
        unselect: function() {
            this.set( 'selected', false );
            return this;
        },
        
        mouseover: function() {
            this.set( 'hover', true );
            return this;
        },
        
        mousemove: function( event ) {
             DDM.getCurrentCoordsFromJS( event.latLng.lat() ,event.latLng.lng() );
             return this;
        },
        
        mouseout: function() {
            this.set( 'hover', false );
            return this;
        },
        
        show: function() {
            this.polygon.setVisible( true );
            return this;
        },
        
        hide: function() {
            this.polygon.setVisible( false );
            return this;
        }
    });
    
    /**
     *  Описание класса представления карты
     */
    var MapView = Backbone.View.extend({
        map: null,
        active: null,
        
        initialize: function( options ) {
        
            var self = this;
            this.map = DDM.Map;

            var collection = new Backbone.Collection( [], { model:CountyModel });
            collection.on( 'change:selected', function( model ) {
                var select = model.get( 'selected' ); 
                if ( select ) {
                    if ( self.active ) self.active.unselect();
                    self.active = model;
                }
            });
            
            collection.add( config.counties || [] );
            this.collection = collection;
            
            return this;
        },
        
        render: function() {
            return this;
        }
        
    });

    /**
     *  Инициализация карты
     */
    function ddm_init() {
        
        // Элемент DOM, на котором будет отрисовываться карта
        var el = $( config.el );
        DDM.Map = new google.maps.Map( el[0], config.map );

        // Модель, которая пришла от приложения
        DDM.Model = ddm_model.props;
        DDM.setCurrentCountyFromJS = ddm_model.setCurrentCountyFromJS;
        DDM.getCurrentCoordsFromJS  = ddm_model.getCurrentCoordsFromJS;
        
        // Представление DDM
        DDM.View = new MapView({ el:el });
        DDM.View.render();
        
        // 
        DDM.Collection = DDM.View.collection;
        
        // Вешаем обработчик события 
        google.maps.event.addDomListener( window, 'resize', ddm_resize );

        window.DDM = DDM;
        

        if ( _.isFunction( config.success ) ) {
            config.success.apply( DDM );
        }
        /*
        marker = new google.maps.Marker({
          //position: new google.maps.LatLng( 59.3462, -135.0306 ),
          position: new google.maps.LatLng( 54.75864, -159.42634 ),
          map: DDM.View.map
        });
        */
        return DDM;
        
    }

    /**
     *  Обновление карты после изменения размеров окна
     */
    function ddm_resize() {
        var map = DDM.View.map;
        var center = map.getCenter();
        google.maps.event.trigger( map, 'resize' );
        map.setCenter( center );
    }
    
    google.maps.event.addDomListener( window, 'load', ddm_init );
    
}

