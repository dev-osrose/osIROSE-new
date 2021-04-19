
set(CPACK_GENERATOR "NSIS64" "ZIP" "7Z")
set(CPACK_PACKAGE_VENDOR "osrose")
set(CPACK_PACKAGE_VERSION_MAJOR 0)
set(CPACK_PACKAGE_VERSION_MINOR 3)
set(CPACK_PACKAGE_VERSION_PATCH 1)


add_compiler_flags(-DASIO_STANDALONE)

add_compiler_flags(-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS) # Remove all of the really annoying c++17 deprection warnings

if("${CMAKE_GENERATOR}" MATCHES "Ninja")
  set(NINJA TRUE)
elseif("${CMAKE_GENERATOR}" MATCHES "Make")
  set(MAKEFILES TRUE)
elseif("${CMAKE_GENERATOR}" MATCHES "Visual Studio")
  set(MSBUILD TRUE)
endif()

set(COPY_SCRIPT_PATH ${CMAKE_SCRIPT_PATH}/robocopy.bat)
set(COPY_FLAT_SCRIPT_PATH ${CMAKE_SCRIPT_PATH}/robocopy_flat.bat)

if(MAKEFILES AND "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  set(OFFICIAL_BUILD ON CACHE BOOL "" FORCE)
  set(BUILD_TOOLS OFF CACHE BOOL "" FORCE)
  set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(WITH_GTEST OFF CACHE BOOL "" FORCE )
  set(WITH_GMOCK OFF CACHE BOOL "" FORCE )
  set(DEBUG OFF CACHE BOOL "" FORCE)
endif()

macro(use_unicode_here)
  add_definitions(-D_UNICODE -DUNICODE)
endmacro()

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  add_definitions(-DDEBUG -D_DEBUG)
  message(STATUS "Configured for Debug")
else()
  add_definitions(-DNDEBUG -D_NDEBUG)
  message(STATUS "Configured for Release")
  
  #This is needed if we want a pdb file to be generated
  if(WITH_CRASH_REPORTS)
    add_compile_options(/Zi)
    add_linker_flags(/DEBUG /OPT:REF /OPT:ICF)
  endif()
endif()

add_definitions(-DNOMINMAX)

#add_linker_flags(/LTCG /INCREMENTAL:NO)

# ignore some warnings
add_linker_flags(/ignore:4075)
add_compiler_flags(/wd4503)
add_compiler_flags(/MP) # build with multi processors
add_compiler_flags(/FS) # build with multi processors
add_compiler_flags(/std:c++17)
add_compiler_flags(/EHsc)
add_compiler_flags(/bigobj)

set(WIN_TARGET 0x0601) # Windows XP SP2
set(WIN_SDK_MIN 0x0601) # Windows Vista
add_definitions(-DWINVER=${WIN_TARGET}
                -D_WIN32_WINNT=${WIN_SDK_MIN})

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(64BIT TRUE)
  message("-- detected 64bit")
else()
  set(64BIT FALSE)
  message("-- detected 32bit")
endif()

if(64BIT)
  set(BUILD_PLATFORM "x64")
else()
  set(BUILD_PLATFORM "Win32")
  add_linker_flags(/LARGEADDRESSAWARE)
endif()

macro(SetSharedRuntime target)
  if(DEBUG)
    set_target_properties(${target} PROPERTIES COMPILE_FLAGS "/MDd")
  else()
    set_target_properties(${target} PROPERTIES COMPILE_FLAGS "/MD")
  endif()
endmacro()

macro(SetStaticRuntime target)
  if(DEBUG)
    set_target_properties(${target} PROPERTIES COMPILE_FLAGS "/MTd")
  else()
    set_target_properties(${target} PROPERTIES COMPILE_FLAGS "/MT")
  endif()
endmacro()

set(MSVC TRUE)
