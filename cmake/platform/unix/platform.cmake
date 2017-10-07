add_compiler_flags(-DASIO_STANDALONE)
add_compiler_flags(-Wall -Wextra)

if(DEBUG)
  add_definitions(-DDEBUG)
else()
  add_definitions(-DNDEBUG)
endif()