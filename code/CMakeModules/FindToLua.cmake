# - Locate ToLua library
# This module defines
#  TOLUA_LIBRARIES, the libraries to link against
#  TOLUA_FOUND, if false, do not try to link to TOLUA
#  TOLUA_INCLUDE_DIR, where to find headers.

IF(TOLUA_LIBRARIES AND TOLUA_INCLUDE_DIR)
  # in cache already
  SET(TOLUA_FIND_QUIETLY TRUE)
ENDIF(TOLUA_LIBRARIES AND TOLUA_INCLUDE_DIR)

FIND_PACKAGE(Lua51 REQUIRED)

FIND_PATH(TOLUA_INCLUDE_DIR tolua++.h
  PATH_SUFFIXES tolua lua51
  $ENV{TOLUA_DIR}/include
  /usr/local/include
  /usr/include
  /sw/include
  /opt/local/include
  /opt/csw/include
  /opt/include
  /usr/local/include/lua51
  /mingw/include
)

SET(LIBRARY_NAME_RELEASE tolua++5.1 libtolua++.a libtolua++5.1.a libtolua++.so)
SET(LIBRARY_NAME_DEBUG tolua++5.1 libtolua++.a libtolua++5.1.a libtolua++.so)

FIND_LIBRARY(TOLUA_LIBRARY_RELEASE
  NAMES ${LIBRARY_NAME_RELEASE}
  PATHS
  $ENV{TOLUA_DIR}/lib
  /usr/local/lib
  /usr/lib
  /usr/lib64
  /usr/local/X11R6/lib
  /usr/X11R6/lib
  /sw/lib
  /opt/local/lib
  /opt/csw/lib
  /opt/lib
  /usr/freeware/lib64
  /usr/local/lib/lua51
  /mingw/lib
)

FIND_LIBRARY(TOLUA_LIBRARY_DEBUG
  NAMES ${LIBRARY_NAME_DEBUG}
  PATHS
  $ENV{TOLUA_DIR}/lib
  /usr/local/lib
  /usr/lib
  /usr/local/X11R6/lib
  /usr/X11R6/lib
  /sw/lib
  /opt/local/lib
  /opt/csw/lib
  /opt/lib
  /usr/freeware/lib64
  /usr/local/lib/lua51
  /mingw/lib
)

IF(TOLUA_INCLUDE_DIR)
  IF(TOLUA_LIBRARY_RELEASE AND TOLUA_LIBRARY_DEBUG)
    # Case where both Release and Debug versions are provided
    SET(TOLUA_FOUND TRUE)
    SET(TOLUA_LIBRARIES optimized ${TOLUA_LIBRARY_RELEASE} debug ${TOLUA_LIBRARY_DEBUG})
  ELSEIF(TOLUA_LIBRARY_RELEASE)
    # Normal case
    SET(TOLUA_FOUND TRUE)
    SET(TOLUA_LIBRARIES ${TOLUA_LIBRARY_RELEASE})
  ELSEIF(TOLUA_LIBRARY_DEBUG)
    # Case where ToLua is compiled from sources (debug version is compiled by default)
    SET(TOLUA_FOUND TRUE)
    SET(TOLUA_LIBRARIES ${TOLUA_LIBRARY_DEBUG})
  ENDIF(TOLUA_LIBRARY_RELEASE AND TOLUA_LIBRARY_DEBUG)
ENDIF(TOLUA_INCLUDE_DIR)

IF(TOLUA_FOUND)
  IF(NOT TOLUA_FIND_QUIETLY)
    MESSAGE(STATUS "Found ToLua: ${TOLUA_INCLUDE_DIR} ${TOLUA_LIBRARIES}")
  ENDIF(NOT TOLUA_FIND_QUIETLY)
ELSE(TOLUA_FOUND)
  IF(NOT TOLUA_FIND_QUIETLY)
    MESSAGE(STATUS "Warning: Unable to find ToLua! INCLUDE: ${TOLUA_INCLUDE_DIR}  LIB:${TOLUA_LIBRARIES}  ")
  ENDIF(NOT TOLUA_FIND_QUIETLY)
ENDIF(TOLUA_FOUND)

MARK_AS_ADVANCED(TOLUA_LIBRARY_RELEASE TOLUA_LIBRARY_DEBUG)
