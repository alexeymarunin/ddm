#!/bin/env/python
# -*- coding: utf-8 -*-

import os
import os.path
import re
import fnmatch
import sys

version_file = "ddmVersion.h"

if not os.path.isfile( version_file ) :
	print( 'No version file found! Check %s file' % version_file )
	raise 

def find_files( dir, pattern ) :
	result = []
	for root, subdirs, files in os.walk( dir ) :
		for file in files :
			filepath = os.path.join( root, file )
			if fnmatch.fnmatch( filepath, '*/.git/*' ) : continue
			if fnmatch.fnmatch( filepath, '*/bld/*' ) : continue
			# print ( filepath )
			if fnmatch.fnmatch( filepath, pattern ) :
				result.append( filepath )

	return result

has_changes = False
project_dir = os.getcwd()
sources = ( 
	find_files( project_dir, '*.cpp' ) + 
	find_files( project_dir, '*.h'   ) + 
	find_files( project_dir, '*.ui'  )
)
for src in sources :
	if os.path.getmtime( src ) > os.path.getmtime( version_file ) :
		# print( src )
		has_changes = True

if not has_changes :
	print( 'No files changed' )
	sys.exit()

file = open( version_file, 'r' )
content = file.read() 
file.close()

m = re.search( "\#define\s+DDM_MAJOR_VERSION\s+([0-9]+)", content )
if not m :
	print( 'No major version found! Check DDM_MAJOR_VERSION variable in %s' % version_file )
	raise 
major_version = int( m.group( 1 ) )

m = re.search( "\#define\s+DDM_MINOR_VERSION\s+([0-9]+)", content )
if not m :
	print( 'No minor version found! Check DDM_MINOR_VERSION variable in %s' % version_file )
	raise 
minor_version = int( m.group( 1 ) )

m = re.search( "\#define\s+DDM_BUILD_NUMBER\s+([0-9]+)", content )
if not m :
	print( 'No build number found! Check DDM_BUILD_NUMBER variable in %s' % version_file )
	raise 
build_num = int( m.group( 1 ) )

build_num = build_num + 1
print ( 'Version %d.%d, build %d' % ( major_version, minor_version, build_num ) )

content = re.sub('\#define\s+DDM_BUILD_NUMBER\s+[0-9]+', '#define DDM_BUILD_NUMBER  %d' % build_num, content )

file = open( version_file, 'w' )
file.write( content ) 
file.close()
