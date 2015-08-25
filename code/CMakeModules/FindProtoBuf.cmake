# - Locate ProtoBuf library
# This module defines
#  PROTOBUF_LIBRARIES, the libraries to link against
#  PROTOBUF_FOUND, if false, do not try to link to PROTO_BUF
#  PROTOBUF_INCLUDE_DIR, where to find headers.

IF(PROTOBUF_LIBRARIES AND PROTOBUF_INCLUDE_DIR)
  # in cache already
  SET(PROTOBUF_FIND_QUIETLY TRUE)
ENDIF(PROTOBUF_LIBRARIES AND PROTOBUF_INCLUDE_DIR)

FIND_PATH(PROTOBUF_INCLUDE_DIR generated_message_util.h
  PATH_SUFFIXES google/protobuf
  $ENV{PROTOBUF_DIR}/include
  /usr/local/include
  /usr/include
  /sw/include
  /opt/local/include
  /opt/csw/include
  /opt/include
  /usr/local/include/google/protobuf
  /usr/local/include/google-protobuf
)

SET(LIBRARY_NAME_RELEASE protobuf libprotobuf)
SET(LIBRARY_NAME_DEBUG protobufd libprotobufd)
	
IF(WITH_STLPORT AND WIN32)
	SET(LIBRARY_NAME_RELEASE libprotobuf_stlport ${LIBRARY_NAME_RELEASE})
	SET(LIBRARY_NAME_DEBUG libprotobuf_stlportd ${LIBRARY_NAME_RELEASE})
ENDIF(WITH_STLPORT AND WIN32)

SET(PROTOBUF_LIBRARY_RELEASE NOTFOUND)
SET(PROTOBUF_LIBRARY_DEBUG NOTFOUND)

FIND_LIBRARY(PROTOBUF_LIBRARY_RELEASE
  NAMES ${LIBRARY_NAME_RELEASE}
  PATHS
  $ENV{PROTOBUF_DIR}/lib
  /usr/local/lib
  /usr/lib
  /usr/local/X11R6/lib
  /usr/X11R6/lib
  /sw/lib
  /opt/local/lib
  /opt/csw/lib
  /opt/lib
  /usr/freeware/lib64
  /usr/local/lib/google-protobuf
)

FIND_LIBRARY(PROTOBUF_LIBRARY_DEBUG
  NAMES ${LIBRARY_NAME_DEBUG}
  PATHS
  $ENV{PROTOBUF_DIR}/lib
  /usr/local/lib
  /usr/lib
  /usr/local/X11R6/lib
  /usr/X11R6/lib
  /sw/lib
  /opt/local/lib
  /opt/csw/lib
  /opt/lib
  /usr/freeware/lib64
  /usr/local/lib/google-protobuf
)

IF(PROTOBUF_INCLUDE_DIR)
  IF(PROTOBUF_LIBRARY_RELEASE AND PROTOBUF_LIBRARY_DEBUG)
    # Case where both Release and Debug versions are provided
    SET(PROTOBUF_FOUND TRUE)
    SET(PROTOBUF_LIBRARIES optimized ${PROTOBUF_LIBRARY_RELEASE} debug ${PROTOBUF_LIBRARY_DEBUG})
  ELSEIF(PROTOBUF_LIBRARY_RELEASE)
    # Normal case
    SET(PROTOBUF_FOUND TRUE)
    SET(PROTOBUF_LIBRARIES ${PROTOBUF_LIBRARY_RELEASE})
  ELSEIF(PROTOBUF_LIBRARY_DEBUG)
    # Case where ProtoBuf is compiled from sources (debug version is compiled by default)
    SET(PROTOBUF_FOUND TRUE)
    SET(PROTOBUF_LIBRARIES ${PROTOBUF_LIBRARY_DEBUG})
  ENDIF(PROTOBUF_LIBRARY_RELEASE AND PROTOBUF_LIBRARY_DEBUG)
ENDIF(PROTOBUF_INCLUDE_DIR)

IF(PROTOBUF_FOUND)
  IF(NOT PROTOBUF_FIND_QUIETLY)
    MESSAGE(STATUS "Found ProtoBuf: ${PROTOBUF_INCLUDE_DIR} ${PROTOBUF_LIBRARIES}")
  ENDIF(NOT PROTOBUF_FIND_QUIETLY)
ELSE(PROTOBUF_FOUND)
  IF(NOT PROTOBUF_FIND_QUIETLY)
    MESSAGE(STATUS "Warning: Unable to find ProtoBuf!")
  ENDIF(NOT PROTOBUF_FIND_QUIETLY)
ENDIF(PROTOBUF_FOUND)

MARK_AS_ADVANCED(PROTOBUF_LIBRARY_RELEASE PROTOBUF_LIBRARY_DEBUG)
