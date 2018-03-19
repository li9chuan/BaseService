# - Locate LibEvent library
# This module defines
# LIBEVENT_INCLUDE_DIRS   - where to find libevent/libevent.h, etc.
# LIBEVENT_LIBRARIES      - List of libraries when using libevent.
# LIBEVENT_FOUND          - True if libevent found.
# LIBEVENT_VERSION_STRING - the version of libevent found (since CMake 2.8.8)

IF(LIBEVENT_LIBRARY AND LIBEVENT_INCLUDE_DIR)
  # in cache already
  SET(LIBEVENT_FIND_QUIETLY TRUE)
ENDIF(LIBEVENT_LIBRARY AND LIBEVENT_INCLUDE_DIR)

FIND_PATH(LIBEVENT_INCLUDE_DIR evutil.h
  PATH_SUFFIXES libevent
  $ENV{LIBEVENT_DIR}/include
  /usr/local/include
  /usr/include
  /sw/include
  /opt/local/include
  /opt/csw/include
  /opt/include
  /usr/local/include/libevent
  /mingw/include
)

SET(LIBRARY_NAME_RELEASE libevent)
SET(LIBRARY_NAME_DEBUG libevent)

FIND_LIBRARY(LIBEVENT_LIBRARY_RELEASE
  NAMES ${LIBRARY_NAME_RELEASE}
  PATHS
  $ENV{LIBEVENT_DIR}/lib
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
  /usr/local/lib/libevent
  /mingw/lib
)

FIND_LIBRARY(LIBEVENT_LIBRARY_DEBUG
  NAMES ${LIBRARY_NAME_DEBUG}
  PATHS
  $ENV{LIBEVENT_DIR}/lib
  /usr/local/lib
  /usr/lib
  /usr/local/X11R6/lib
  /usr/X11R6/lib
  /sw/lib
  /opt/local/lib
  /opt/csw/lib
  /opt/lib
  /usr/freeware/lib64
  /usr/local/lib/libevent
  /mingw/lib
)

IF(LIBEVENT_INCLUDE_DIR)
  IF(LIBEVENT_LIBRARY_RELEASE AND LIBEVENT_LIBRARY_DEBUG)
    # Case where both Release and Debug versions are provided
    SET(LIBEVENT_FOUND TRUE)
    SET(LIBEVENT_LIBRARY optimized ${LIBEVENT_LIBRARY_RELEASE} debug ${LIBEVENT_LIBRARY_DEBUG})
  ELSEIF(LIBEVENT_LIBRARY_RELEASE)
    # Normal case
    SET(LIBEVENT_FOUND TRUE)
    SET(LIBEVENT_LIBRARY ${LIBEVENT_LIBRARY_RELEASE})
  ELSEIF(LIBEVENT_LIBRARY_DEBUG)
    # Case where LibEvent is compiled from sources (debug version is compiled by default)
    SET(LIBEVENT_FOUND TRUE)
    SET(LIBEVENT_LIBRARY ${LIBEVENT_LIBRARY_DEBUG})
  ENDIF(LIBEVENT_LIBRARY_RELEASE AND LIBEVENT_LIBRARY_DEBUG)
ENDIF(LIBEVENT_INCLUDE_DIR)

IF(LIBEVENT_FOUND)
  IF(NOT LIBEVENT_FIND_QUIETLY)
    MESSAGE(STATUS "Found LibEvent: ${LIBEVENT_INCLUDE_DIR} ${LIBEVENT_LIBRARY}")
  ENDIF(NOT LIBEVENT_FIND_QUIETLY)
ELSE(LIBEVENT_FOUND)
  IF(NOT LIBEVENT_FIND_QUIETLY)
    MESSAGE(STATUS "Warning: Unable to find LibEvent! INCLUDE: ${LIBEVENT_INCLUDE_DIR}  LIB:${LIBEVENT_LIBRARY}  ")
  ENDIF(NOT LIBEVENT_FIND_QUIETLY)
ENDIF(LIBEVENT_FOUND)

MARK_AS_ADVANCED(LIBEVENT_LIBRARY_RELEASE LIBEVENT_LIBRARY_DEBUG)
