#!/bin/env/python
# -*- coding: utf-8 -*-

#########################################################################
#  Добавление информации о графствах США, являющихся соседями,          #
#  в единую локальную базу SQLite                                       #
#                                                                       #
#  Автор:  Марунин Алексей                                              #
#  Дата:   16.03.2015                                                   #
#                                                                       #
#########################################################################

import sys
import os
import os.path
import sqlite3
import math
import time
from optparse import OptionParser


# Имя файла с БД
DB_FILE = 'ddm.sqlite'

# Открываем БД
conn = sqlite3.connect( DB_FILE )
cursor = conn.cursor()

# Файл с данными
DATA_FILE = 'data/county_adjacency.txt'

# Чтобы нумерация пошла с 1, добавляем один фейковый элемент
countries = [ 'unknown' ]

county_neighbors = []

def find_county( name ) :
	
	if len( countries ) == 1 :
		cursor.execute( 'SELECT id, title, abbr FROM ddm_counties' )
		rows = cursor.fetchall()
		for row in rows :
			county_id = int( row[0] )
			county_name = '%s, %s' % ( row[1], row[2] )
			countries.append( county_name )
	
	return countries.index( name )

def append_neighbor( county_a, county_b ) :
	for neighbors in county_neighbors :
		if ( ( neighbors['county_a'] == county_a and neighbors['county_b'] == county_b ) or ( neighbors['county_a'] == county_b and neighbors['county_b'] == county_a ) ) : return False
	
	county_neighbors.append( { 'county_a':county_a, 'county_b':county_b } )
	return len( county_neighbors )

def parse_neighbors( options ) : 

	cursor.execute( 'PRAGMA foreign_keys = ON' )
	
	if options.rebuild_neighbors :
		# Удаляем предыдущие результаты
		cursor.executescript( '''
			DROP INDEX IF EXISTS county_neighbors_idx;
			DROP TABLE IF EXISTS ddm_county_neighbors;
		''' )
		cursor.executescript( '''
			CREATE TABLE IF NOT EXISTS ddm_county_neighbors ( 
				id              INTEGER PRIMARY KEY AUTOINCREMENT,
				county_a        INTEGER,
				county_b        INTEGER
			);
			CREATE UNIQUE INDEX IF NOT EXISTS county_neighbors_idx ON ddm_county_neighbors( county_a, county_b );
		''' )
	
	data_file = open( DATA_FILE, "r" ).read()
	
	current_county = ''
	neighbor = ''
	county_a = 0
	county_b = 0
	b = 0
	lines = data_file.split( "\n" )
	cp = 0
	for line in lines :
		parts = line.split( "\t", 3 )
		ccp = int( 100.0 * b / len( lines ) )
		if ccp > cp : 
			print ( '%d%%' % ccp )
			cp = ccp
		b = b + 1
		# if b == 30 : break
		
		if parts[0] != '' : 
			current_county = parts[0]
			county_a = find_county( current_county )
			# print ( '%d: %s' % ( county_a, current_county ) )
		
		neighbor = parts[2]
		if neighbor == current_county : continue
		
		#print ( '    %s' % neighbor )
		county_b = find_county( neighbor )
		lastid = append_neighbor( county_a, county_b )
		# if lastid : print ( lastid )

def main() :
	parser = OptionParser()

	parser.add_option( "-f", "--force", 
		action = "store_true", dest = "rebuild_neighbors", default = True,
		help = "force rebuild neighbors"
	)

	( options, args ) = parser.parse_args()
	parse_neighbors( options )

	cursor.executemany( '''INSERT INTO ddm_county_neighbors ( county_a, county_b ) VALUES ( :county_a, :county_b )''', county_neighbors ) 
	conn.commit()

main()