set (TARGET_NAME freetype)
set (ROOT_DIR "${SLIB_PATH}/external/src/freetype")

set (
 SRC_LIST
 
 "${ROOT_DIR}/src/base/ftapi.c"
 "${ROOT_DIR}/src/base/ftbdf.c"
 "${ROOT_DIR}/src/base/ftbitmap.c"
 "${ROOT_DIR}/src/base/ftcid.c"
 "${ROOT_DIR}/src/base/ftdebug.c"
 "${ROOT_DIR}/src/base/ftfstype.c"
 "${ROOT_DIR}/src/base/ftgasp.c"
 "${ROOT_DIR}/src/base/ftglyph.c"
 "${ROOT_DIR}/src/base/ftgxval.c"
 "${ROOT_DIR}/src/base/ftinit.c"
 "${ROOT_DIR}/src/base/ftmm.c"
 "${ROOT_DIR}/src/base/ftotval.c"
 "${ROOT_DIR}/src/base/ftpatent.c"
 "${ROOT_DIR}/src/base/ftpfr.c"
 "${ROOT_DIR}/src/base/ftstroke.c"
 "${ROOT_DIR}/src/base/ftsynth.c"
 "${ROOT_DIR}/src/base/ftsystem.c"
 "${ROOT_DIR}/src/base/fttype1.c"
 "${ROOT_DIR}/src/base/ftwinfnt.c"

 "${ROOT_DIR}/src/base/ftbase.c"
 "${ROOT_DIR}/src/autofit/autofit.c"
 "${ROOT_DIR}/src/bdf/bdf.c"
 "${ROOT_DIR}/src/cache/ftcache.c"
 "${ROOT_DIR}/src/cff/cff.c"
 "${ROOT_DIR}/src/cid/type1cid.c"
 "${ROOT_DIR}/src/gxvalid/gxvalid.c"
 "${ROOT_DIR}/src/gzip/ftgzip.c"
 "${ROOT_DIR}/src/lzw/ftlzw.c"
 "${ROOT_DIR}/src/otvalid/otvalid.c"
 "${ROOT_DIR}/src/pcf/pcf.c"
 "${ROOT_DIR}/src/pfr/pfr.c"
 "${ROOT_DIR}/src/psaux/psaux.c"
 "${ROOT_DIR}/src/pshinter/pshinter.c"
 "${ROOT_DIR}/src/psnames/psnames.c"
 "${ROOT_DIR}/src/raster/raster.c"
 "${ROOT_DIR}/src/sfnt/sfnt.c"
 "${ROOT_DIR}/src/smooth/smooth.c"
 "${ROOT_DIR}/src/truetype/truetype.c"
 "${ROOT_DIR}/src/type1/type1.c"
 "${ROOT_DIR}/src/type42/type42.c"
 "${ROOT_DIR}/src/winfonts/winfnt.c"
)

include ("${CMAKE_CURRENT_LIST_DIR}/common.inc.cmake")

target_include_directories (
 ${TARGET_NAME}
 PRIVATE "${ROOT_DIR}/include"
)
if (ANDROID)
 target_compile_options (
  ${TARGET_NAME}
  PRIVATE -Wno-tautological-constant-compare
 )
endif ()
