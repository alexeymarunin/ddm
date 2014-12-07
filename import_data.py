#!/bin/env/python
# -*- coding: utf-8 -*-

#########################################################################
###  Импорт данных по графствам США в единую локальную базу SQLite      #
###                                                                     #
###  Авторы: Марунин Алексей, Цалко Тарас                               #
###  Дата:   04.12.2014                                                 #
###                                                                     #
#########################################################################
 
import os
import os.path
import xlrd
import sqlite3
import hashlib
import math
from xml.dom.minidom import parseString as parseXML

class DDM_Model() :

	########################################################################################################################
	# Конструктор
	def __init__( self ) :
		self.connect_db( 'ddm.sqlite' )
		
		# Если не находим таблицу с графствами, то запускаем парсер
		self.load_counties()
		if len( self.counties ) == 0 :
			self.load_counties_base( os.path.join( 'data', 'United States Counties.xlsx' ) )
			self.load_counties()
		
		# Вычисляем расстояния между графствами
		print( 'Calculating county distances...' )
		self.calc_county_distances()
		print( 'Inserting county distances...(%d)' % len( self.county_distances ) )
		self.cursor.executemany( 'INSERT INTO ddm_county_distances ( county_from, county_to, distance ) VALUES ( :county_from, :county_to, :distance )', self.county_distances )
		self.conn.commit()

		#center = self.find_county_center( 2 )
		#print( 'center: %s' % center )

	########################################################################################################################
	# 
	def connect_db( self, filename = 'ddm.sqlite' ) :
		self.conn = sqlite3.connect( filename )
		self.cursor = self.conn.cursor()
		self.init_db()


	########################################################################################################################
	# 
	def load_counties( self ) :
		self.counties = []
		self.cursor.execute( 'SELECT id, name FROM ddm_counties' )
		rows = self.cursor.fetchall()
		for row in rows :
			county_id = int( row[0] )
			county_name = row[1]
			self.counties.append({ 'id':county_id, 'name':county_name, 'center':None })
		return len( self.counties )


	########################################################################################################################
	# 
	def load_counties_base( self, filename ) :
		print( os.path.abspath( filename ) )
		print( 'Loading...' )
		book = xlrd.open_workbook( filename )
		sheet = book.sheet_by_index( 0 )

		self.points = []
		self.boundaries = []
		self.boundary_points = []
		self.county_boundaries = []
		
		print( 'Parsing rows...' )
		row_count = sheet.nrows
		for row in range( 1, row_count - 1 ) :
			
			# Выбираем нужные значения из ячеек
			county_name     = sheet.cell_value( row, 8  )
			state_abbr      = sheet.cell_value( row, 3  )
			xml_coordinates = sheet.cell_value( row, 4 )
			county_value    = sheet.cell_value( row, 5  )
			county_geoid    = sheet.cell_value( row, 6  )
			county_geoid2   = sheet.cell_value( row, 7  )
			geographic_name = sheet.cell_value( row, 8  )
			state_num       = sheet.cell_value( row, 9  )
			county_num      = sheet.cell_value( row, 10 )
			fips_formula    = sheet.cell_value( row, 11 )
			county_error    = sheet.cell_value( row, 12 )
			
			if len( geographic_name ) == 0 : raise # Это уникальное имя, оно должно быть!
			
			county_title = geographic_name.split( ',' )[0]
			state_name = geographic_name.split( ',' )[1].strip()
			
			print( '[ROW %d]: %s' % ( row + 1, geographic_name ) )
			
			# Добавляем штат
			state_id = self.insert_state({
				'name' : state_name,
				'abbr' : state_abbr,
				'title': state_name
			})
			
			# Добавляем графство
			county_id = self.insert_county({
				'abbr' : state_abbr,
				'name' : county_name,
				'title' : county_title,
				'state_id' : state_id,
				'geographic_name' : geographic_name,
				'state_num' : state_num,
				'num' : county_num,
				'fips_formula' : fips_formula,
				'value' : county_value,
				'geoid' : county_geoid,
				'geoid2' : county_geoid2,
				'error' : county_error
			})
			
			# Получаем координаты
			if len( xml_coordinates.strip() ) > 0 :
				( point_count, boundary_count ) = self.parse_county_coordinates( county_id, xml_coordinates )
				print( '%d points, %d boundaries parsed' % ( point_count, boundary_count ) )
				
			else :
				print( 'WARNING! No boundaries found!' )

		# Вставляем данные в БД
		print( 'Inserting points (%d)...' % len( self.points ) )
		self.cursor.executemany( 'INSERT INTO ddm_points ( id, x, y ) VALUES ( :id, :x, :y )', self.points )
		self.conn.commit()
		
		print( 'Inserting boundaries...(%d)' % len( self.boundaries ) )
		self.cursor.executemany( 'INSERT INTO ddm_boundaries ( id, outer, square, center_id ) VALUES ( :id, :outer, :square, :center_id )', self.boundaries )
		self.conn.commit()
		
		print( 'Inserting boundary points...(%d)' % len( self.boundary_points ) )
		self.cursor.executemany( 'INSERT INTO ddm_boundary_points ( point_id, boundary_id ) VALUES ( :point_id, :boundary_id )', self.boundary_points )
		self.conn.commit()
		
		print( 'Inserting county boundaries...(%d)' % len( self.county_boundaries ) )
		self.cursor.executemany( 'INSERT INTO ddm_county_boundaries ( county_id, boundary_id ) VALUES ( :county_id, :boundary_id )', self.county_boundaries )
		self.conn.commit()
		

	########################################################################################################################
	# 
	def parse_county_coordinates( self, county_id, xml ) :
		total_boundaries = 0 # число найденных контуров
		total_points     = 0 # число найденных вершин
		
		dom = parseXML( xml )
		polygons = dom.getElementsByTagName( 'Polygon' )
		
		for polygon in polygons :
			# Внешние границы графства
			outer_boundaries = polygon.getElementsByTagName( 'outerBoundaryIs' )
			( boundary_count, point_count ) = self.parse_boundaries( county_id, outer_boundaries, 1 )
			total_boundaries += boundary_count
			total_points += point_count
			
			# Границы "вырезов" внутри графства
			inner_boundaries = polygon.getElementsByTagName( 'innerBoundaryIs' )
			( boundary_count, point_count ) = self.parse_boundaries( county_id, inner_boundaries, 0 )
			total_boundaries += boundary_count
			total_points += point_count

			# TODO: здесь мы вычисляем центр графства и его площадь

		return ( total_points, total_boundaries )


	########################################################################################################################
	# 
	def append_point( self, x, y ) :
		point_id = len( self.points ) + 1
		self.points.append({ 'id':point_id, 'x':x, 'y':y })
		return point_id

	def get_point( self, point_id ) :
		return self.points[point_id - 1]

	########################################################################################################################
	# 
	def append_boundary( self, county_id, outer ) :
		boundary_id = len( self.boundaries ) + 1
		self.boundaries.append({ 'id':boundary_id, 'outer':outer, 'square':0, 'center_id':0, 'points':[] });
		self.county_boundaries.append({ 'boundary_id':boundary_id, 'county_id':county_id });
		return boundary_id

	def get_boundary( self, boundary_id ) :
		return self.boundaries[boundary_id - 1]

	def set_boundary( self, boundary_id, center_id = 0, square = 0 ) :
		if center_id > 0 : 
			self.boundaries[boundary_id - 1]['center_id'] = center_id
		if square > 0 : 
			self.boundaries[boundary_id - 1]['square'] = square

	########################################################################################################################
	# 
	def parse_boundaries( self, county_id, xml_boundaries, outer ) :
		point_count = 0
		boundary_count = 0
		if xml_boundaries :
			for boundary in xml_boundaries :
				boundary_id = self.append_boundary( county_id, outer )
				coordinates = boundary.getElementsByTagName( 'coordinates' )[0].childNodes[0].data.split( ' ' )
				vertices = []
				for point in coordinates :
					#print( '[%s]' % point )
					( x, y ) = point.split( ',' )
					point_id = self.append_point( x, y )
					self.boundary_points.append({ 'point_id':point_id, 'boundary_id':boundary_id })
					vertices.append({ 'x':x, 'y':y })
				
				( x, y ) = self.calc_centroid( vertices )
				center_id = self.append_point( x, y )
				square = self.calc_polygon_square( vertices )
				self.set_boundary( boundary_id, center_id, square )
				
				point_count += len( coordinates );
			
			boundary_count = len( xml_boundaries )
		
		return ( boundary_count, point_count )


	########################################################################################################################
	# 
	def insert_state( self, state ) :
		self.cursor.execute( 'SELECT id FROM ddm_states WHERE abbr = :abbr', { 'abbr' : state['abbr'] } )
		res = self.cursor.fetchone()
		if res :
			return res[0]
		else :
			self.cursor.execute( 'INSERT INTO ddm_states ( name, abbr, title ) VALUES ( :name, :abbr, :title )', state )
			self.conn.commit()
			return self.cursor.lastrowid 


	########################################################################################################################
	# 
	def insert_county( self, county ) :
		self.cursor.execute( 'SELECT id FROM ddm_counties WHERE name = :name', { 'name' : county['name'] } )
		res = self.cursor.fetchone()
		if res :
			return res[0]
		else :
			self.cursor.execute( '''
			INSERT INTO ddm_counties 
				(  name,  abbr,  title,  state_id,  geographic_name,  state_num,  num,  fips_formula,  value,  geoid,  geoid2,  error ) 
				VALUES 
				( :name, :abbr, :title, :state_id, :geographic_name, :state_num, :num, :fips_formula, :value, :geoid, :geoid2, :error )
			''', county )
			self.conn.commit()
			return self.cursor.lastrowid 


	########################################################################################################################
	# 
	def insert_point( self, point ) :
		self.cursor.execute( 'SELECT id FROM ddm_points WHERE x = :x AND y = :y', point )
		res = self.cursor.fetchone()
		if res :
			return res[0]
		else :
			self.cursor.execute( 'INSERT INTO ddm_points ( x, y ) VALUES ( :x, :y )', point )
			self.conn.commit()
			return self.cursor.lastrowid 


	########################################################################################################################
	# Вычисляет площадь полигона по координатам его вершин
	# Взято отсюда - http://ru.wikihow.com/%D0%BD%D0%B0%D0%B9%D1%82%D0%B8-%D0%BF%D0%BB%D0%BE%D1%89%D0%B0%D0%B4%D1%8C-%D0%BC%D0%BD%D0%BE%D0%B3%D0%BE%D1%83%D0%B3%D0%BE%D0%BB%D1%8C%D0%BD%D0%B8%D0%BA%D0%B0
	def calc_polygon_square( self, vertices ) :

		v = list( vertices )
		v.append( v[0] )
		nvetrices = len( v )
		n = nvetrices - 1
		s = 0.0
		for i in range( n ) :
			x1 = float( v[i]['x'] )
			y1 = float( v[i]['y'] ) 
			x2 = float( v[i+1]['x'] if i < n else v[0]['x'] )
			y2 = float( v[i+1]['y'] if i < n else v[0]['y'] )
			s += ( x1 * y2 - x2 * y1 )

		return abs( 0.5 * s )


	########################################################################################################################
	# Вычисляет центр полигона по координатам его вершин
	def calc_centroid( self, vertices ):
	
		# Дальше работаем с копией массива
		v = list( vertices )
		v.append( v[0] )
		Cx = 0.0
		Cy = 0.0

		n = len( v ) - 1
		for i in range( n ) :
			x1 = float( v[i]['x'] )
			y1 = float( v[i]['y'] ) 
			x2 = float( v[i+1]['x'] if i < n else v[0]['x'] )
			y2 = float( v[i+1]['y'] if i < n else v[0]['y'] )
			Cx += ( x1 + x2 ) * ( x1 * y2 - x2 * y1 )
			Cy += ( y1 + y2 ) * ( x1 * y2 - x2 * y1 )

		# Вычисляем площадь
		S = self.calc_polygon_square( vertices )
		x = Cx / ( 6 * S )
		y = Cy / ( 6 * S )
		
		return ( x, y )


	########################################################################################################################
	# Вычисляет расстояние между двумя точками
	# Результат - в метрах 
	# Взято отсюда - http://www.kobzarev.com/programming/calculation-of-distances-between-cities-on-their-coordinates.html
	def calc_point_distance( self, p1, p2 ) :
		x1    = float( p1['x'] )
		y1    = float( p1['y'] )
		lat1  = x1 * math.pi / 180.0
		long1 = y1 * math.pi / 180.0
		sinl1 = math.sin( lat1 )
		cosl1 = math.cos( lat1 )
		
		x2    = float( p2['x'] )
		y2    = float( p2['y'] )
		lat2  = x2 * math.pi / 180.0
		long2 = y2 * math.pi / 180.0
		sinl2 = math.sin( lat2 )
		cosl2 = math.cos( lat2 )
		
		dl    = long2 - long1
		sindl = math.sin( dl )
		cosdl = math.cos( dl )
		
		a = cosl2 * sindl
		b = cosl1 * sinl2 - sinl1 * cosl2 * cosdl
		y = math.sqrt( a*a + b*b )
		x = sinl1 * sinl2 + cosl1 * cosl2 * cosdl
		d = math.atan2( y, x ) * 6372795 # радиус Земли
		
		#print( "d=%d" % ( d ) )
		return d


	########################################################################################################################
	# Находит центр графства
	# Выбирает точку - центр наибольшего внешнего контура графства
	def find_county_center( self, county_id ) :
		self.cursor.execute( '''
			SELECT p.x, p.y FROM ddm_county_boundaries AS cb
			LEFT JOIN ddm_counties AS c ON c.id = cb.county_id
			LEFT JOIN ddm_boundaries AS b ON b.id = cb.boundary_id
			LEFT JOIN ddm_points AS p ON p.id = b.center_id
			WHERE c.id = :county_id AND b.outer = 1
			ORDER BY b.square DESC
			LIMIT 1
		''', { 'county_id':county_id } )
		row = self.cursor.fetchone()
		
		point = None
		if row :
			x = float( row[0] )
			y = float( row[1] )
			point = { 'x':x, 'y':y } 
		#print( "center=%d" % ( point ) )
		return point


	########################################################################################################################
	# Вычисляет расстояния между графствами
	def calc_county_distances( self ) :

		centers = []
		ncounties = len( self.counties )
		for county in self.counties :
			county_id = county['id']
			county_name = county['name']
			center = self.find_county_center( county_id )
			centers.append({ 'county_id':county_id, 'center':center })
		
		self.county_distances = []
		for i in range( ncounties - 1 ) :
			county_from = centers[i]['county_id']
			p1 = centers[i]['center']
			for j in range( i + 1, ncounties ) :
				if i == j : continue
				county_to = centers[j]['county_id']
				p2 = centers[j]['center']
				distance = self.calc_point_distance( p1, p2 )
				self.county_distances.append({
					'county_from':county_from,
					'county_to':county_to,
					'distance':distance
				})
				
		return ncounties

	########################################################################################################################
	# 
	def init_db( self ) :
		self.cursor.execute( 'PRAGMA foreign_keys = ON' )
		
		# Таблица ddm_states
		self.cursor.execute( '''
			CREATE TABLE IF NOT EXISTS ddm_states ( 
				id              INTEGER PRIMARY KEY AUTOINCREMENT,
				title           VARCHAR(50) NOT NULL UNIQUE,
				name            VARCHAR(50) NOT NULL UNIQUE,
				abbr            VARCHAR(2) NOT NULL UNIQUE
			)
		''' )
		
		# Таблица ddm_counties
		self.cursor.execute( '''
			CREATE TABLE IF NOT EXISTS ddm_counties (
				id              INTEGER PRIMARY KEY AUTOINCREMENT,
				title           VARCHAR(50) NOT NULL DEFAULT "",
				name            VARCHAR(50) NOT NULL,
				abbr            VARCHAR(2) NOT NULL,
				state_id        INTEGER,
				geographic_name VARCHAR(100),
				state_num       INTEGER, 
				num             INTEGER, 
				fips_formula    DOUBLE, 
				value           VARCHAR(20), 
				geoid           VARCHAR(20), 
				geoid2          VARCHAR(20), 
				error           VARCHAR(100),
				square          DOUBLE NOT NULL DEFAULT 0,
				center_id       INTEGER NOT NULL DEFAULT 0,
				FOREIGN KEY(state_id) REFERENCES ddm_states(id)
			)
		''' )
		
		# Таблица ddm_points
		self.cursor.execute( '''
			CREATE TABLE IF NOT EXISTS ddm_points (
				id              INTEGER PRIMARY KEY,
				x               VARCHAR(20) NOT NULL,
				y               VARCHAR(20) NOT NULL
			)
		''' )
		
		# Таблица ddm_boundaries
		self.cursor.execute( '''
			CREATE TABLE IF NOT EXISTS ddm_boundaries (
				id              INTEGER PRIMARY KEY,
				square          DOUBLE NOT NULL DEFAULT 0,
				center_id       INTEGER NOT NULL DEFAULT 0,
				outer           SMALLINT(1) NOT NULL DEFAULT 1
			)
		''' )
		
		# Таблица ddm_boundary_points
		self.cursor.execute( '''
			CREATE TABLE IF NOT EXISTS ddm_boundary_points (
				id              INTEGER PRIMARY KEY AUTOINCREMENT,
				boundary_id     INTEGER NOT NULL,
				point_id        INTEGER NOT NULL,
				FOREIGN KEY(boundary_id) REFERENCES ddm_boundaries(id),
				FOREIGN KEY(point_id)    REFERENCES ddm_points(id)
			)
		''' )
		
		# Таблица ddm_county_boundaries
		self.cursor.execute( '''
			CREATE TABLE IF NOT EXISTS ddm_county_boundaries (
				id              INTEGER PRIMARY KEY AUTOINCREMENT,
				county_id       INTEGER NOT NULL,
				boundary_id     INTEGER NOT NULL,
				FOREIGN KEY(county_id)      REFERENCES ddm_counties(id),
				FOREIGN KEY(boundary_id)    REFERENCES ddm_boundaries(id)
			)
		''' )
		
		# Issue #3
		# Таблица ddm_county_distances
		self.cursor.execute( '''DROP TABLE IF EXISTS ddm_county_distances''' )
		self.cursor.execute( '''
			CREATE TABLE IF NOT EXISTS ddm_county_distances (
				id              INTEGER PRIMARY KEY AUTOINCREMENT,
				county_from     INTEGER NOT NULL,
				county_to       INTEGER NOT NULL,
				distance        DOUBLE NOT NULL DEFAULT 0,
				FOREIGN KEY(county_from)    REFERENCES ddm_counties(id),
				FOREIGN KEY(county_to)      REFERENCES ddm_counties(id)
			)
		''' )
		
		# Создаем индексы для более быстрого доступа
		self.cursor.executescript( '''
			CREATE UNIQUE INDEX IF NOT EXISTS geographic_name_idx ON ddm_counties( geographic_name );
		''' )


	########################################################################################################################
	# Деструктор
	def __del__( self ) :
		if ( self.conn ) :
			self.conn.close()


model = DDM_Model()
