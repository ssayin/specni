find_path(SWISSEPH_INCLUDE_DIR NAMES swisseph/swephexp.h /usr/include
                                     /usr/local/include /opt/local/include)

find_library(SWISSEPH_LIBRARY swe /usr/lib64 /usr/lib /usr/local/lib
             /opt/local/lib)

if(SWISSEPH_INCLUDE_DIR AND SWISSEPH_LIBRARY)
  set(swisseph_FOUND TRUE)
  set(swisseph_LIBRARIES ${SWISSEPH_LIBRARY})
endif()

if(swisseph_FOUND)
  message(STATUS "Found swisseph: ${SWISSEPH_LIBRARY}")
else(swisseph_FOUND)
  if(swisseph_FIND_REQUIRED)
    message(FATAL_ERROR "Could not find libswe")
  endif()
endif()
