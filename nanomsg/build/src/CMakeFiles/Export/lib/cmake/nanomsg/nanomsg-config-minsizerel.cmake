#----------------------------------------------------------------
# Generated CMake target import file for configuration "MinSizeRel".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "nanomsg" for configuration "MinSizeRel"
set_property(TARGET nanomsg APPEND PROPERTY IMPORTED_CONFIGURATIONS MINSIZEREL)
set_target_properties(nanomsg PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_MINSIZEREL "pthread"
  IMPORTED_LOCATION_MINSIZEREL "${_IMPORT_PREFIX}/lib/libnanomsg.5.1.0.dylib"
  IMPORTED_SONAME_MINSIZEREL "@rpath/libnanomsg.5.dylib"
  )

list(APPEND _IMPORT_CHECK_TARGETS nanomsg )
list(APPEND _IMPORT_CHECK_FILES_FOR_nanomsg "${_IMPORT_PREFIX}/lib/libnanomsg.5.1.0.dylib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
