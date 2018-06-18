add_library (
 ${TARGET_NAME} STATIC
 ${SRC_LIST}
)

set_target_properties (
 ${TARGET_NAME}
 PROPERTIES
 ARCHIVE_OUTPUT_DIRECTORY "${SLIB_LIB_PATH}"
)
if (CMAKE_BUILD_TYPE STREQUAL Debug)
 target_compile_options (
  ${TARGET_NAME}
  PRIVATE -O3
 )
endif ()
