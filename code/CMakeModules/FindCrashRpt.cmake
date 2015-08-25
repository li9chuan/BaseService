# - Locate CrashRpt library
# This module defines
#  CrashRpt_LIBRARIES, the libraries to link against
#  CrashRpt_FOUND, if false, do not try to link to CrashRpt
#  CrashRpt_INCLUDE_DIR, where to find headers.

IF(CrashRpt_LIBRARIES AND CrashRpt_INCLUDE_DIR)
  # in cache already
  SET(CrashRpt_FIND_QUIETLY TRUE)
ENDIF(CrashRpt_LIBRARIES AND CrashRpt_INCLUDE_DIR)

FIND_PATH(CrashRpt_INCLUDE_DIR CrashRpt.h
  PATH_SUFFIXES crashrpt CrashRpt
  $ENV{CrashRpt_DIR}/include
)

FIND_LIBRARY(CrashRpt_LIBRARIES
  NAMES CrashRpt1402
  PATHS
  $ENV{CrashRpt_DIR}/lib
)

IF(CrashRpt_INCLUDE_DIR)
  IF(CrashRpt_LIBRARIES)
    SET(CrashRpt_FOUND TRUE)
  ENDIF(CrashRpt_LIBRARIES)
ENDIF(CrashRpt_INCLUDE_DIR)

IF(CrashRpt_FOUND)
  IF(NOT CrashRpt_FIND_QUIETLY)
    MESSAGE(STATUS "Found CrashRpt: ${CrashRpt_INCLUDE_DIR} ${CrashRpt_LIBRARIES}")
  ENDIF(NOT CrashRpt_FIND_QUIETLY)
ELSE(CrashRpt_FOUND)
  IF(NOT CrashRpt_FIND_QUIETLY)
    MESSAGE(STATUS "Warning: Unable to find CrashRpt!")
  ENDIF(NOT CrashRpt_FIND_QUIETLY)
ENDIF(CrashRpt_FOUND)

MARK_AS_ADVANCED(CrashRpt_LIBRARIES)
