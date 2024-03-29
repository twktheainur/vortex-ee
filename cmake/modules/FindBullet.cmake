# Find Bullet includes and library
#
# This module defines
#  BULLET_INCLUDE_DIR
#  BULLET_LIBRARIES, the libraries to link against to use Bullet.
#  BULLET_LIB_DIR, the location of the libraries
#  BULLET_FOUND, If false, do not try to use Bullet
#
# Copyright � 2007, Matt Williams
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

IF (BULLET_LIBRARIES AND BULLET_INCLUDE_DIR)
	SET(BULLET_FIND_QUIETLY TRUE) # Already in cache, be silent
ENDIF (BULLET_LIBRARIES AND BULLET_INCLUDE_DIR)

IF (WIN32) #Windows
	MESSAGE(STATUS "Looking for Bullet")
	##SET(OGRESDK $ENV{OGRE_HOME})
	SET(BULLETSOURCE $ENV{BULLET_HOME})
	#IF (OGRESDK)
	#	MESSAGE(STATUS "Using OGRE SDK")
	#	STRING(REGEX REPLACE "[\\]" "/" OGRESDK "${OGRESDK}")
	#	SET(OGRE_INCLUDE_DIR ${OGRESDK}/include)
	#	SET(OGRE_LIB_DIR ${OGRESDK}/lib)
	#	SET(OGRE_LIBRARIES debug OgreMain_d optimized OgreMain)
	#ENDIF (OGRESDK)
	IF (BULLETSOURCE)
		MESSAGE(STATUS "Using Bullet built from source")
		SET(BULLET_INCLUDE_DIR $ENV{BULLET_HOME}/src $ENV{BULLET_HOME}/Extras/GIMPACT/include)
		SET(BULLET_LIB_DIR $ENV{BULLET_HOME}/out/Debug8/libs $ENV{BULLET_HOME}/out/Release8/libs)
		SET(BULLET_LIBRARIES debug libbulletcollision_d debug libbulletdynamics_d debug libbulletmath_d debug libGIMPACT_d optimized libbulletcollision optimized libbulletdynamics optimized libbulletmath optimized libGIMPACT)
	ENDIF (BULLETSOURCE)
#ELSE (WIN32) #Unix
#	CMAKE_MINIMUM_REQUIRED(VERSION 2.4.7 FATAL_ERROR)
#	FIND_PACKAGE(PkgConfig)
#	PKG_SEARCH_MODULE(OGRE OGRE)
#	SET(OGRE_INCLUDE_DIR ${OGRE_INCLUDE_DIRS})
#	SET(OGRE_LIB_DIR ${OGRE_LIBDIR})
#	SET(OGRE_LIBRARIES ${OGRE_LIBRARIES} CACHE STRING "")
ENDIF (WIN32)

#Do some preparation
SEPARATE_ARGUMENTS(BULLET_INCLUDE_DIR)
SEPARATE_ARGUMENTS(BULLET_LIBRARIES)

SET(BULLET_INCLUDE_DIR ${BULLET_INCLUDE_DIR} CACHE PATH "")
SET(BULLET_LIBRARIES ${BULLET_LIBRARIES} CACHE STRING "")
SET(BULLET_LIB_DIR ${BULLET_LIB_DIR} CACHE PATH "")

IF (BULLET_INCLUDE_DIR AND BULLET_LIBRARIES)
	SET(BULLET_FOUND TRUE)
ENDIF (BULLET_INCLUDE_DIR AND BULLET_LIBRARIES)

IF (BULLET_FOUND)
	IF (NOT BULLET_FIND_QUIETLY)
		MESSAGE(STATUS "  libraries : ${BULLET_LIBRARIES} from ${BULLET_LIB_DIR}")
		MESSAGE(STATUS "  includes  : ${BULLET_INCLUDE_DIR}")
	ENDIF (NOT BULLET_FIND_QUIETLY)
ELSE (BULLET_FOUND)
	IF (BULLET_FIND_REQUIRED)
		MESSAGE(FATAL_ERROR "Could not find Bullet")
	ENDIF (BULLET_FIND_REQUIRED)
ENDIF (BULLET_FOUND)
