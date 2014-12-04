#!/bin/env/python
# -*- coding: utf-8 -*-

#########################################################################
###  Импорт данных по графствам США в единую локальную базу SQLite      #
###                                                                     #
###  Автор: Марунин Алексей                                             #
###  Дата: 04.12.2014                                                   #
###                                                                     #
#########################################################################
 
import os
import os.path
import xlrd
import sqlite3
import hashlib
from xml.dom.minidom import parseString as parseXML

class DDM_Model() :

	########################################################################################################################
	# Конструктор
	def __init__( self ) :
		self.connect_db( 'ddm.sqlite' )
		self.load_counties_base( os.path.join( 'data', 'United States Counties.xlsx' ) )

	########################################################################################################################
	# 
	def connect_db( self, filename = 'ddm.sqlite' ) :
		self.conn = sqlite3.connect( filename )
		self.cursor = self.conn.cursor()
		self.init_db()


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
			county_name     = sheet.cell_value( row, 0  )
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
			
			if len( geographic_name ) == 0 : continue # 91 строка
			
			county_title = geographic_name.split( ',' )[0]
			state_name = geographic_name.split( ',' )[1].strip()
			
			print( '[ROW %d]: %s' % ( row+1, county_title ) )
			
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
		
		print( 'Inserting boundaries...(%d)...' % len( self.boundaries ) )
		self.cursor.executemany( 'INSERT INTO ddm_boundaries ( id, outer, square, center_id ) VALUES ( :id, :outer, :square, :center_id )', self.boundaries )
		self.conn.commit()
		
		print( 'Inserting boundary points...(%d)...' % len( self.boundary_points ) )
		self.cursor.executemany( 'INSERT INTO ddm_boundary_points ( point_id, boundary_id ) VALUES ( :point_id, :boundary_id )', self.boundary_points )
		self.conn.commit()
		
		print( 'Inserting county boundaries...(%d)...' % len( self.county_boundaries ) )
		self.cursor.executemany( 'INSERT INTO ddm_county_boundaries ( county_id, boundary_id ) VALUES ( :county_id, :boundary_id )', self.county_boundaries )
		self.conn.commit()


	########################################################################################################################
	# 
	def parse_county_coordinates( self, county_id, xml ) :
		# if county_id == 69 : return ( 0, 0 ) 
		
		dom = parseXML( xml )
		polygons = dom.getElementsByTagName( 'Polygon' )
		point_count = 0 # возвращаемое значение
		boundary_count = 0 # возвращаемое значение
		
		for polygon in polygons :
			# Внешние границы графства
			outerBoundaries = polygon.getElementsByTagName( 'outerBoundaryIs' )
			if outerBoundaries :
				for boundary in outerBoundaries :
					boundary_id = len( self.boundaries ) + 1
					coordinates = boundary.getElementsByTagName( 'coordinates' )[0].childNodes[0].data.split()
					vertices = []
					for point in coordinates :
						point_count = point_count + 1
						point_id = len( self.points ) + 1
						( x, y ) = point.split( ',' )
						self.points.append({ 'id':point_id, 'x':x, 'y':y })
						self.boundary_points.append({ 'point_id':point_id, 'boundary_id':boundary_id })
						vertices.append({ 'x':x, 'y':y })
					square = self.calc_polygon_square( vertices )
					#print( 's=%s' % ( square ) )

					self.boundaries.append({ 'id':boundary_id, 'outer':1, 'square':square, 'center_id':0, 'vertices':vertices });
					self.county_boundaries.append({ 'boundary_id':boundary_id, 'county_id':county_id });
					boundary_count = boundary_count + 1

			# Границы "вырезов" внутри графства
			innerBoundaries = polygon.getElementsByTagName( 'innerBoundaryIs' )
			if innerBoundaries :
				for boundary in innerBoundaries :
					boundary_id = len( self.boundaries ) + 1
					self.boundaries.append({ 'id':boundary_id, 'outer':0, 'square':0, 'center_id':0, 'points':[] });
					self.county_boundaries.append({ 'boundary_id':boundary_id, 'county_id':county_id });
					boundary_count = boundary_count + 1
					
					coordinates = boundary.getElementsByTagName( 'coordinates' )[0].childNodes[0].data.split( ' ' )
					for point in coordinates :
						point_count = point_count + 1
						( x, y ) = point.split( ',' )
						point_id = len( self.points ) + 1
						self.points.append({ 'id':point_id, 'x':x, 'y':y })
						self.boundary_points.append({ 'point_id':point_id, 'boundary_id':boundary_id })
			
			# TODO: здесь мы вычисляем центр графства и его площадь

		return ( point_count, boundary_count )


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
	# 
	def calc_polygon_square( self, vertices ) :
		nvetrices = len( vertices )
		n = nvetrices - 1
		s1 = 0
		s2 = 0
		for i in range( 0, n ) :
			x1 = float( vertices[i]['x'] )
			y1 = float( vertices[i+1]['y'] if i < n else vertices[0]['y'] )
			x2 = float( vertices[i+1]['x'] if i < n else vertices[0]['x'] )
			y2 = float( vertices[i]['y'] )
			s1 = s1 + x1 * y1
			s2 = s2 + x2 * y2
		s = s1 - s2
		return s


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
		# CREATE INDEX state_id_idx ON ddm_counties(state_id)
		
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
		# CREATE INDEX boundary_point_idx ON ddm_boundary_points(boundary_id,point_id)
		
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
		


	########################################################################################################################
	# Деструктор
	def __del__( self ) :
		if ( self.conn ) :
			self.conn.close()

model = DDM_Model()
