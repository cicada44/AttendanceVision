# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "AttendanceVision_autogen"
  "CMakeFiles\\AttendanceVision_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\AttendanceVision_autogen.dir\\ParseCache.txt"
  )
endif()
