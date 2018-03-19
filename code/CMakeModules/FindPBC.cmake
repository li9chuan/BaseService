# - Locate PBC library
# This module defines
# PBC_INCLUDE_DIRS   - where to find pbc/pbc.h, etc.
# PBC_LIBRARIES      - List of libraries when using pbc.
# PBC_FOUND          - True if pbc found.
# PBC_VERSION_STRING - the version of pbc found (since CMake 2.8.8)

FIND_PATH(PBC_INCLUDE_DIR pbc.h
  PATH_SUFFIXES pbc
  $ENV{PBC_DIR}/include
  /usr/local/include
  /usr/include
  /sw/include
  /opt/local/include
  /opt/csw/include
  /opt/include
  /usr/local/include/pbc
  /mingw/include
)

SET(LIBRARY_NAME_RELEASE pbc)
SET(LIBRARY_NAME_DEBUG pbc)

FIND_LIBRARY(PBC_LIBRARY_RELEASE
  NAMES ${LIBRARY_NAME_RELEASE}
  PATHS
  $ENV{PBC_DIR}/lib
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
  /usr/local/lib/pbc
  /mingw/lib
)

FIND_LIBRARY(PBC_LIBRARY_DEBUG
  NAMES ${LIBRARY_NAME_DEBUG}
  PATHS
  $ENV{PBC_DIR}/lib
  /usr/local/lib
  /usr/lib
  /usr/local/X11R6/lib
  /usr/X11R6/lib
  /sw/lib
  /opt/local/lib
  /opt/csw/lib
  /opt/lib
  /usr/freeware/lib64
  /usr/local/lib/pbc
  /mingw/lib
)

IF(PBC_INCLUDE_DIR)
  IF(PBC_LIBRARY_RELEASE AND PBC_LIBRARY_DEBUG)
    # Case where both Release and Debug versions are provided
    SET(PBC_FOUND TRUE)
    SET(PBC_LIBRARY optimized ${PBC_LIBRARY_RELEASE} debug ${PBC_LIBRARY_DEBUG})
  ELSEIF(PBC_LIBRARY_RELEASE)
    # Normal case
    SET(PBC_FOUND TRUE)
    SET(PBC_LIBRARY ${PBC_LIBRARY_RELEASE})
  ELSEIF(PBC_LIBRARY_DEBUG)
    # Case where PBC is compiled from sources (debug version is compiled by default)
    SET(PBC_FOUND TRUE)
    SET(PBC_LIBRARY ${PBC_LIBRARY_DEBUG})
  ENDIF(PBC_LIBRARY_RELEASE AND PBC_LIBRARY_DEBUG)
ENDIF(PBC_INCLUDE_DIR)

IF(PBC_FOUND)
    MESSAGE(STATUS "Found PBC: ${PBC_INCLUDE_DIR} ${PBC_LIBRARY}")
ELSE(PBC_FOUND)
    MESSAGE(STATUS "Warning: Unable to find PBC! INCLUDE: ${PBC_INCLUDE_DIR}  LIB:${PBC_LIBRARY}  ")
ENDIF(PBC_FOUND)

MARK_AS_ADVANCED(PBC_LIBRARY_RELEASE PBC_LIBRARY_DEBUG)
