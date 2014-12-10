#!/bin/env/python
# -*- coding: utf-8 -*-

#########################################################################
###  Импорт данных по графствам США в единую локальную базу SQLite      #
###                                                                     #
###  Авторы: Марунин Алексей, Цалко Тарас                               #
###  Дата:   04.12.2014                                                 #
###                                                                     #
#########################################################################
 
import sys
import os
import os.path
import xlrd
import sqlite3
import hashlib
import math
import time
from xml.dom.minidom import parseString as parseXML
from optparse import OptionParser

# Имя файла с БД
DB_FILE = 'ddm.sqlite'

class DDM_Model() :

	########################################################################################################################
	# Конструктор
	def __init__( self, options ) :
		self.conn = None # чтобы корректно отрабатывал деструктор в случае ошибки

		rebuild_db = options.rebuild_db == True
		drop_counties = rebuild_db or options.drop_counties == True
		drop_county_distances = drop_counties or options.drop_county_distances == True
		drop_residences = drop_counties or options.drop_residences == True
		drop_migrations = drop_counties or options.drop_migrations == True
		county_id = int( options.county_id )
		verbose = options.verbose
		
		self.config = {
			'county_id'             : county_id,
			'drop_counties'         : drop_counties,
			'drop_county_distances' : drop_county_distances,
			'drop_residences'       : drop_residences,
			'drop_migrations'       : drop_migrations
		}
		self.verbose = verbose
		
		if rebuild_db :
			print( 'Full rebuild database.' )
			if os.path.isfile( DB_FILE ) :
				os.remove( DB_FILE )
		
		# Подключаемся к БД
		self.connect_db( DB_FILE )
		
		# Если не находим таблицу с графствами, то запускаем парсер
		self.load_counties()
		if len( self.counties ) == 0 :
			self.load_counties_base( os.path.join( 'data', 'United States Counties.xlsx' ) )
			self.load_counties()
		
		if drop_county_distances :
			# Вычисляем расстояния между графствами
			print( 'Calculating county distances...' )
			self.calc_county_distances()
			print( 'Inserting county distances...(%d)' % len( self.county_distances ) )
			self.cursor.executemany( 'INSERT INTO ddm_county_distances ( county_from, county_to, distance ) VALUES ( :county_from, :county_to, :distance )', self.county_distances )
			self.conn.commit()

		if drop_residences :
			self.load_counties_residences( os.path.join( 'data', 'County-to-County 2008-2012 - Current Residence Sort.xlsx' ) )
		
		if drop_migrations :
			self.load_counties_migrations( os.path.join( 'data', 'County-to-County 2008-2012 - Ins Outs Nets Gross.xlsx' ) )


	########################################################################################################################
	# 
	def connect_db( self, filename ) :
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
		print( 'File: %s' % os.path.abspath( filename ) )
		print( 'Loading...' )
		book = xlrd.open_workbook( filename )
		sheet = book.sheet_by_index( 0 )

		self.points = []
		self.boundaries = []
		self.boundary_points = []
		self.county_boundaries = []
		
		print( 'Parsing counties...' )
		target_county = self.config['county_id']
		rows_range = range( target_county, target_county + 1 ) if target_county > 0 else range( 1, sheet.nrows )
		for row in rows_range :
			
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
				if self.verbose :
					print( '%s: %d points, %d boundaries' % ( geographic_name, point_count, boundary_count ) )
			else :
				print( 'WARNING! No boundaries found for %s (row %d)!' % ( geographic_name, row + 1 ) )

		# Освобождаем память
		book.release_resources()
		
		# Вставляем данные в БД
		print( 'Inserting points (%d)...' % len( self.points ) )
		self.cursor.executemany( 'INSERT INTO ddm_points ( id, x, y ) VALUES ( :id, :x, :y )', self.points )
		self.conn.commit()
		
		print( 'Inserting boundaries (%d)...' % len( self.boundaries ) )
		self.cursor.executemany( 'INSERT INTO ddm_boundaries ( id, outer, square, center_id ) VALUES ( :id, :outer, :square, :center_id )', self.boundaries )
		self.conn.commit()
		
		print( 'Inserting boundary points (%d)...' % len( self.boundary_points ) )
		self.cursor.executemany( 'INSERT INTO ddm_boundary_points ( point_id, boundary_id ) VALUES ( :point_id, :boundary_id )', self.boundary_points )
		self.conn.commit()
		
		print( 'Inserting county boundaries (%d)...' % len( self.county_boundaries ) )
		self.cursor.executemany( 'INSERT INTO ddm_county_boundaries ( county_id, boundary_id ) VALUES ( :county_id, :boundary_id )', self.county_boundaries )
		self.conn.commit()


	########################################################################################################################
	# 
	def load_counties_residences( self, filename ) :
		
		self.residences = []
		
		print( 'File: %s' % os.path.abspath( filename ) )
		print( 'Loading...' )
		book = xlrd.open_workbook( filename, on_demand = True )
		for sheet in book.sheets() :
			state_name = sheet.name
			row_count = sheet.nrows
			print( 'Parsing %s (%d rows)...' % ( state_name, row_count ) )
			prev_county = None
			for row in range( row_count ) :
				ph = sheet.cell_value( row, 0 )
				if not ph.isnumeric() : continue
				
				state_name = sheet.cell_value( row, 4 )
				county_title = sheet.cell_value( row, 5 )
				
				if prev_county == county_title : continue 
				
				( county_id, county_name ) = self.find_county( state_name, county_title )
				if not county_id :
					print( '[ROW %d]: Undefined %s, %s' % ( row + 1, county_name, state_name ) )
					raise 
				else :
					popul_est = int( sheet.cell_value( row, 6 ) )
					popul_moe = int( sheet.cell_value( row, 7 ) )
					if self.verbose : 
						print( '[ROW %d]: County - %s(%d), population - %d/%d' % ( row + 1, county_name, county_id, popul_est, popul_moe ) )
					self.residences.append({ 'county_id':county_id, 'popul_est':popul_est, 'popul_moe':popul_moe })
				
				prev_county = county_title
				
		# Освобождаем память
		book.release_resources()
		
		print( 'Inserting county residences (%d)...' % len( self.residences ) )
		self.cursor.executemany( 'INSERT INTO ddm_residences ( county_id, popul_est, popul_moe ) VALUES ( :county_id, :popul_est, :popul_moe )', self.residences )
		self.conn.commit()


	########################################################################################################################
	# 
	def load_counties_migrations( self, filename ) :
		
		self.migrations = []
		
		print( 'File: %s' % os.path.abspath( filename ) )
		print( 'Loading...' )
		book = xlrd.open_workbook( filename, on_demand = True )
		for sheet in book.sheets() :
			state_name = sheet.name
			row_count = sheet.nrows
			print( 'Parsing %s (%d rows)...' % ( state_name, row_count ) )
			for row in range( row_count ) :
				ph = sheet.cell_value( row, 0 )
				if not ph.isnumeric() : continue
				ph = sheet.cell_value( row, 2 )
				if not ph.isnumeric() : continue
				
				state1  = sheet.cell_value( row, 4 )
				county1 = sheet.cell_value( row, 5 )
				state2  = sheet.cell_value( row, 6 )
				county2 = sheet.cell_value( row, 7 )

				( county_id1, county_name1 ) = self.find_county( state1, county1 )
				if not county_id1 :
					print( '[ROW %d]: Undefined county A (%s, %s)!' % ( row + 1, county_name1, state1 ) )
					raise 
				( county_id2, county_name2 ) = self.find_county( state2, county2 )
				if not county_id2 :
					print( '[ROW %d]: Undefined county B (%s, %s)!' % ( row + 1, county_name2, state2 ) )
					raise 
				
				county_a       = county_id1
				county_b       = county_id2
				in_estimate    = int( sheet.cell_value( row, 8  ) )
				in_moe         = int( sheet.cell_value( row, 9  ) )
				out_estimate   = int( sheet.cell_value( row, 10 ) )
				out_moe        = int( sheet.cell_value( row, 11 ) )
				net_migr_est   = int( sheet.cell_value( row, 12 ) )
				net_migr_moe   = int( sheet.cell_value( row, 13 ) )
				gross_migr_est = int( sheet.cell_value( row, 14 ) )
				gross_migr_moe = int( sheet.cell_value( row, 15 ) )
				
				if self.verbose :
					print( '[ROW %d]:' % ( row + 1 ) )
					print( '    County A - %s(%d)' % ( county_name1, county_id1 ) )
					print( '    County B - %s(%d)' % ( county_name2, county_id2 ) )
					print( '    Flow from B to A: %d/%d' % ( in_estimate, in_moe ) )
					print( '    Flow from B to A: %d/%d' % ( out_estimate, out_moe ) )
					print( '    Net Migration from B to A: %d/%d' % ( net_migr_est, net_migr_moe ) )
					print( '    Gross Migration between A and B: %d/%d' % ( gross_migr_est, gross_migr_moe ) )
					print( ' ' )
				
				self.migrations.append({ 
					'county_a'       : county_a, 
					'county_b'       : county_b, 
					'in_estimate'    : in_estimate, 
					'in_moe'         : in_moe, 
					'out_estimate'   : out_estimate, 
					'out_moe'        : out_moe, 
					'net_migr_est'   : net_migr_est, 
					'net_migr_moe'   : net_migr_moe, 
					'gross_migr_est' : gross_migr_est, 
					'gross_migr_moe' : gross_migr_moe 
				})
				
		# Освобождаем память
		book.release_resources()
		
		print( 'Inserting county migrations (%d)...' % len( self.migrations ) )
		self.cursor.executemany( '''
			INSERT INTO ddm_mirgations ( 
				county_a, county_b, 
				in_estimate, in_moe, 
				out_estimate, out_moe, 
				net_migr_est, net_migr_moe, 
				gross_migr_est, gross_migr_moe 
			) 
			VALUES ( 
				:county_a, :county_b, 
				:in_estimate, :in_moe,  
				:out_estimate, :out_moe,  
				:net_migr_est, :net_migr_moe,  
				:gross_migr_est, :gross_migr_moe
			)''', self.migrations )
		self.conn.commit()


	########################################################################################################################
	# 
	def find_county( self, state_name, county_raw_title ) :
		county_title = county_raw_title.replace( 'ñ', 'n' ).replace( 'ó', 'o' ).replace( 'á', 'a' ).replace( 'í', 'i' ).replace( 'ü', 'u' )
		self.cursor.execute( '''
			SELECT c.id, c.title FROM ddm_counties AS c
			LEFT JOIN ddm_states AS s ON s.id = c.state_id
			WHERE s.name = :state_name AND c.title = :county_title
		''', { 'state_name':state_name, 'county_title':county_title }
		)
		row = self.cursor.fetchone()
		if not row : return ( 0, county_title )
		
		county_id = int( row[0] )
		county_name = row[1]
		return ( county_id, county_name )


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
				#print( x, y ) # тест результатов работы функции
				center_id = self.append_point( x, y )
				square = abs( self.calc_polygon_square( vertices ) )
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

		return 0.5 * s


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
			Cy += ( x1 + x2 ) * ( x1 * y2 - x2 * y1 )
			Cx += ( y1 + y2 ) * ( x1 * y2 - x2 * y1 )

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
		if self.config['drop_counties'] == True :
			self.cursor.executescript( '''
				DROP TABLE IF EXISTS ddm_counties;
				DROP INDEX IF EXISTS county_title_state_idx;
				DROP INDEX IF EXISTS geographic_name_idx;
			''' )
		self.cursor.executescript( '''
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
			);
			CREATE UNIQUE INDEX IF NOT EXISTS county_title_state_idx ON ddm_counties( title, state_id );
			CREATE UNIQUE INDEX IF NOT EXISTS geographic_name_idx ON ddm_counties( geographic_name );
		''' )
		
		# Таблица ddm_points
		if self.config['drop_counties'] == True :
			self.cursor.execute( '''DROP TABLE IF EXISTS ddm_points''' )
		self.cursor.execute( '''
			CREATE TABLE IF NOT EXISTS ddm_points (
				id              INTEGER PRIMARY KEY,
				x               VARCHAR(20) NOT NULL,
				y               VARCHAR(20) NOT NULL
			)
		''' )
		
		# Таблица ddm_boundaries
		if self.config['drop_counties'] == True :
			self.cursor.execute( '''DROP TABLE IF EXISTS ddm_boundaries''' )
		self.cursor.execute( '''
			CREATE TABLE IF NOT EXISTS ddm_boundaries (
				id              INTEGER PRIMARY KEY,
				square          DOUBLE NOT NULL DEFAULT 0,
				center_id       INTEGER NOT NULL DEFAULT 0,
				outer           SMALLINT(1) NOT NULL DEFAULT 1
			)
		''' )
		
		# Таблица ddm_boundary_points
		if self.config['drop_counties'] == True :
			self.cursor.execute( '''DROP TABLE IF EXISTS ddm_boundary_points''' )
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
		if self.config['drop_counties'] == True :
			self.cursor.execute( '''DROP TABLE IF EXISTS ddm_county_boundaries''' )
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
		if self.config['drop_county_distances'] == True :
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
		
		# Issue #6
		# Таблица ddm_residences
		if self.config['drop_residences'] == True :
			self.cursor.executescript( '''
				DROP TABLE IF EXISTS ddm_residences;
				DROP INDEX IF EXISTS county_id_idx;
			''' )
		self.cursor.executescript( '''
			CREATE TABLE IF NOT EXISTS ddm_residences (
				id              INTEGER PRIMARY KEY AUTOINCREMENT,
				county_id       INTEGER NOT NULL,
				popul_est       INTEGER NOT NULL DEFAULT 0,
				popul_moe       INTEGER NOT NULL DEFAULT 0,
				FOREIGN KEY(county_id)       REFERENCES ddm_counties(id)
			);
			CREATE UNIQUE INDEX IF NOT EXISTS county_id_idx ON ddm_residences( county_id );
		''' )
		
		# Issue #7
		# Таблица ddm_mirgations
		if self.config['drop_migrations'] == True :
			self.cursor.execute( '''DROP TABLE IF EXISTS ddm_mirgations''' )
		self.cursor.execute( '''
			CREATE TABLE IF NOT EXISTS ddm_mirgations (
				id              INTEGER PRIMARY KEY AUTOINCREMENT,
				county_a        INTEGER NOT NULL,
				county_b        INTEGER NOT NULL,
				in_estimate     INTEGER NOT NULL DEFAULT 0,
				in_moe          INTEGER NOT NULL DEFAULT 0,
				out_estimate    INTEGER NOT NULL DEFAULT 0,
				out_moe         INTEGER NOT NULL DEFAULT 0,
				net_migr_est    INTEGER NOT NULL DEFAULT 0,
				net_migr_moe    INTEGER NOT NULL DEFAULT 0,
				gross_migr_est  INTEGER NOT NULL DEFAULT 0,
				gross_migr_moe  INTEGER NOT NULL DEFAULT 0,
				FOREIGN KEY(county_a)       REFERENCES ddm_counties(id),
				FOREIGN KEY(county_b)       REFERENCES ddm_counties(id)
			)
		''' )
		
		# Представления
		self.cursor.execute( '''DROP VIEW IF EXISTS ddm_county_centers''' )
		self.cursor.execute( '''
			CREATE VIEW IF NOT EXISTS ddm_county_centers AS SELECT c.id, c.name, p.x, p.y, b.square FROM ddm_county_boundaries AS cb
			LEFT JOIN ddm_counties AS c ON c.id = cb.county_id
			LEFT JOIN ddm_boundaries AS b ON b.id = cb.boundary_id
			LEFT JOIN ddm_points AS p ON p.id = b.center_id
			WHERE b.outer = 1
			GROUP BY c.id
			ORDER BY c.id, b.square DESC
		''' )


	########################################################################################################################
	# Деструктор
	def __del__( self ) :
		if ( self.conn ) :
			self.conn.close()

def main() :
	
	parser = OptionParser()
	
	parser.add_option( "-f", "--force", 
		action = "store_true", dest = "rebuild_db", default = False,
		help = "force rebuild DB"
	)
	
	# Сброс данных о графствах
	parser.add_option( "", "--drop-counties", 
		action = "store_true", dest = "drop_counties", default = False,
		help = "drop ddm_counties table"
	)
	
	# Сброс данных о расстояниях между графствами
	parser.add_option( "", "--drop-distances", 
		action = "store_true", dest = "drop_county_distances", default = False,
		help = "drop ddm_county_distances table"
	)
	
	# Сброс данных о резидентах (численности)
	parser.add_option( "", "--drop-residences", 
		action = "store_true", dest = "drop_residences", default = False,
		help = "drop ddm_drop_residences table"
	)
	
	# Сброс данных о резидентах (численности)
	parser.add_option( "", "--drop-migrations", 
		action = "store_true", dest = "drop_migrations", default = False,
		help = "drop ddm_mirgations table"
	)
	parser.add_option( "-i", "--id", 
		type="int", dest = "county_id", default = 0,  metavar = "ID",
		help = "target county id"
	)
	
	parser.add_option( "-v", "--verbose", 
		action = "store_true", dest = "verbose", default = False,
		help = "print verbose information"
	)
	
	( options, args ) = parser.parse_args()
	# print( options )

	model = DDM_Model( options )
	print( 'Ending...' )
	
if __name__ == "__main__" :
	start_time = time.time()
	try :
		main()
	except :
		print( '%s' % sys.exc_info() )
	else :
		end_time = time.time()
		print( '\n\n--- %f seconds ---' % ( end_time - start_time ) )
