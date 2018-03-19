# - Locate Curl library
# This module defines
# CURL_INCLUDE_DIRS   - where to find curl/curl.h, etc.
# CURL_LIBRARIES      - List of libraries when using curl.
# CURL_FOUND          - True if curl found.
# CURL_VERSION_STRING - the version of curl found (since CMake 2.8.8)

IF(CURL_LIBRARY AND CURL_INCLUDE_DIR)
  # in cache already
  SET(CURL_FIND_QUIETLY TRUE)
ENDIF(CURL_LIBRARY AND CURL_INCLUDE_DIR)

FIND_PATH(CURL_INCLUDE_DIR curl.h
  PATH_SUFFIXES curl
  $ENV{CURL_DIR}/include
  /usr/local/include
  /usr/include
  /sw/include
  /opt/local/include
  /opt/csw/include
  /opt/include
  /usr/local/include/curl
  /mingw/include
)

SET(LIBRARY_NAME_RELEASE curl libcurl)
SET(LIBRARY_NAME_DEBUG curl libcurl)

FIND_LIBRARY(CURL_LIBRARY_RELEASE
  NAMES ${LIBRARY_NAME_RELEASE}
  PATHS
  $ENV{CURL_DIR}/lib
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
  /usr/local/lib/curl
  /mingw/lib
)

FIND_LIBRARY(CURL_LIBRARY_DEBUG
  NAMES ${LIBRARY_NAME_DEBUG}
  PATHS
  $ENV{CURL_DIR}/lib
  /usr/local/lib
  /usr/lib
  /usr/local/X11R6/lib
  /usr/X11R6/lib
  /sw/lib
  /opt/local/lib
  /opt/csw/lib
  /opt/lib
  /usr/freeware/lib64
  /usr/local/lib/curl
  /mingw/lib
)

IF(CURL_INCLUDE_DIR)
  IF(CURL_LIBRARY_RELEASE AND CURL_LIBRARY_DEBUG)
    # Case where both Release and Debug versions are provided
    SET(CURL_FOUND TRUE)
    SET(CURL_LIBRARY optimized ${CURL_LIBRARY_RELEASE} debug ${CURL_LIBRARY_DEBUG})
  ELSEIF(CURL_LIBRARY_RELEASE)
    # Normal case
    SET(CURL_FOUND TRUE)
    SET(CURL_LIBRARY ${CURL_LIBRARY_RELEASE})
  ELSEIF(CURL_LIBRARY_DEBUG)
    # Case where Curl is compiled from sources (debug version is compiled by default)
    SET(CURL_FOUND TRUE)
    SET(CURL_LIBRARY ${CURL_LIBRARY_DEBUG})
  ENDIF(CURL_LIBRARY_RELEASE AND CURL_LIBRARY_DEBUG)
ENDIF(CURL_INCLUDE_DIR)

IF(CURL_FOUND)
  IF(NOT CURL_FIND_QUIETLY)
    MESSAGE(STATUS "Found Curl: ${CURL_INCLUDE_DIR} ${CURL_LIBRARY}")
  ENDIF(NOT CURL_FIND_QUIETLY)
ELSE(CURL_FOUND)
  IF(NOT CURL_FIND_QUIETLY)
    MESSAGE(STATUS "Warning: Unable to find Curl! INCLUDE: ${CURL_INCLUDE_DIR}  LIB:${CURL_LIBRARY}  ")
  ENDIF(NOT CURL_FIND_QUIETLY)
ENDIF(CURL_FOUND)

MARK_AS_ADVANCED(CURL_LIBRARY_RELEASE CURL_LIBRARY_DEBUG)
