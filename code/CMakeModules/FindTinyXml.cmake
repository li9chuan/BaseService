# - Locate TinyXml library
# This module defines
#  TINYXML_LIBRARIES, the libraries to link against
#  TINYXML_FOUND, if false, do not try to link to TINYXML
#  TINYXML_INCLUDE_DIR, where to find headers.

IF(TINYXML_LIBRARIES AND TINYXML_INCLUDE_DIR)
  # in cache already
  SET(TINYXML_FIND_QUIETLY TRUE)
ENDIF(TINYXML_LIBRARIES AND TINYXML_INCLUDE_DIR)

FIND_PATH(TINYXML_INCLUDE_DIR tinyxml.h
  PATH_SUFFIXES tinyxml
  $ENV{TINYXML_DIR}/include
  /usr/local/include
  /usr/include
  /sw/include
  /opt/local/include
  /opt/csw/include
  /opt/include
)

SET(LIBRARY_NAME_RELEASE tinyxml libtinyxml)
SET(LIBRARY_NAME_DEBUG tinyxml_d tinyxmld libtinyxml_d libtinyxmld)

FIND_LIBRARY(TINYXML_LIBRARY_RELEASE
  NAMES ${LIBRARY_NAME_RELEASE}
  PATHS
  $ENV{TINYXML_DIR}/lib
  /usr/local/lib
  /usr/lib
  /usr/local/X11R6/lib
  /usr/X11R6/lib
  /sw/lib
  /opt/local/lib
  /opt/csw/lib
  /opt/lib
  /usr/freeware/lib64
)

FIND_LIBRARY(TINYXML_LIBRARY_DEBUG
  NAMES ${LIBRARY_NAME_DEBUG}
  PATHS
  $ENV{TINYXML_DIR}/lib
  /usr/local/lib
  /usr/lib
  /usr/local/X11R6/lib
  /usr/X11R6/lib
  /sw/lib
  /opt/local/lib
  /opt/csw/lib
  /opt/lib
  /usr/freeware/lib64
)

IF(TINYXML_INCLUDE_DIR)
  IF(TINYXML_LIBRARY_RELEASE AND TINYXML_LIBRARY_DEBUG)
    # Case where both Release and Debug versions are provided
    SET(TINYXML_FOUND TRUE)
    SET(TINYXML_LIBRARIES optimized ${TINYXML_LIBRARY_RELEASE} debug ${TINYXML_LIBRARY_DEBUG})
  ELSEIF(TINYXML_LIBRARY_RELEASE)
    # Normal case
    SET(TINYXML_FOUND TRUE)
    SET(TINYXML_LIBRARIES ${TINYXML_LIBRARY_RELEASE})
  ELSEIF(TINYXML_LIBRARY_DEBUG)
    # Case where TinyXml is compiled from sources (debug version is compiled by default)
    SET(TINYXML_FOUND TRUE)
    SET(TINYXML_LIBRARIES ${TINYXML_LIBRARY_DEBUG})
  ENDIF(TINYXML_LIBRARY_RELEASE AND TINYXML_LIBRARY_DEBUG)
ENDIF(TINYXML_INCLUDE_DIR)

IF(TINYXML_FOUND)
  IF(NOT TINYXML_FIND_QUIETLY)
    MESSAGE(STATUS "Found TinyXml: ${TINYXML_INCLUDE_DIR} ${TINYXML_LIBRARIES}")
  ENDIF(NOT TINYXML_FIND_QUIETLY)
ELSE(TINYXML_FOUND)
  IF(NOT TINYXML_FIND_QUIETLY)
    MESSAGE(STATUS "Warning: Unable to find TinyXml!")
  ENDIF(NOT TINYXML_FIND_QUIETLY)
ENDIF(TINYXML_FOUND)

MARK_AS_ADVANCED(TINYXML_LIBRARY_RELEASE TINYXML_LIBRARY_DEBUG)
