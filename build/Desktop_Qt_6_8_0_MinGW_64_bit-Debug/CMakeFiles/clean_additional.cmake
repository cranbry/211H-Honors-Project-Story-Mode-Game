# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "211-Honors-Project_autogen"
  "CMakeFiles\\211-Honors-Project_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\211-Honors-Project_autogen.dir\\ParseCache.txt"
  )
endif()
