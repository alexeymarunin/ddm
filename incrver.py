#!/bin/env/python
# -*- coding: utf-8 -*-

import os
import os.path
import re

version_file = "ddmVersion.h"

if not os.path.isfile( version_file ) :
	print( 'No version file found! Check %s file' % version_file )
	raise 

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
