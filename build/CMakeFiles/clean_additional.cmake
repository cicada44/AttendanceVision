# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "Attendance_autogen"
  "CMakeFiles\\Attendance_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Attendance_autogen.dir\\ParseCache.txt"
  )
endif()
