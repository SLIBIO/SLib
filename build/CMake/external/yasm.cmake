if (NOT ANDROID)

set (TARGET_NAME libyasm)
set (ROOT_DIR "${SLIB_PATH}/external/src/yasm")

set (
 SRC_LIST
 
 "${ROOT_DIR}/libyasm/assocdat.c"
 "${ROOT_DIR}/libyasm/bc-align.c"
 "${ROOT_DIR}/libyasm/bc-data.c"
 "${ROOT_DIR}/libyasm/bc-incbin.c"
 "${ROOT_DIR}/libyasm/bc-org.c"
 "${ROOT_DIR}/libyasm/bc-reserve.c"
 "${ROOT_DIR}/libyasm/bitvect.c"
 "${ROOT_DIR}/libyasm/bytecode.c"
 "${ROOT_DIR}/libyasm/errwarn.c"
 "${ROOT_DIR}/libyasm/expr.c"
 "${ROOT_DIR}/libyasm/file.c"
 "${ROOT_DIR}/libyasm/floatnum.c"
 "${ROOT_DIR}/libyasm/genmodule.c"
 "${ROOT_DIR}/libyasm/hamt.c"
 "${ROOT_DIR}/libyasm/insn.c"
 "${ROOT_DIR}/libyasm/intnum.c"
 "${ROOT_DIR}/libyasm/inttree.c"
 "${ROOT_DIR}/libyasm/linemap.c"
 "${ROOT_DIR}/libyasm/md5.c"
 "${ROOT_DIR}/libyasm/mergesort.c"
 "${ROOT_DIR}/libyasm/phash.c"
 "${ROOT_DIR}/libyasm/section.c"
 "${ROOT_DIR}/libyasm/strcasecmp.c"
 "${ROOT_DIR}/libyasm/strsep.c"
 "${ROOT_DIR}/libyasm/symrec.c"
 "${ROOT_DIR}/libyasm/valparam.c"
 "${ROOT_DIR}/libyasm/value.c"
 "${ROOT_DIR}/libyasm/xmalloc.c"
 "${ROOT_DIR}/libyasm/xstrdup.c"

 "${ROOT_DIR}/gas-token.c"
 "${ROOT_DIR}/lc3bid.c"
 "${ROOT_DIR}/module.c"
 "${ROOT_DIR}/nasm-token.c"
 "${ROOT_DIR}/x86cpu.c"
 "${ROOT_DIR}/x86regtmod.c"

 "${ROOT_DIR}/modules/arch/lc3b/lc3barch.c"
 "${ROOT_DIR}/modules/arch/lc3b/lc3bbc.c"
 "${ROOT_DIR}/modules/arch/x86/x86arch.c"
 "${ROOT_DIR}/modules/arch/x86/x86bc.c"
 "${ROOT_DIR}/modules/arch/x86/x86expr.c"
 "${ROOT_DIR}/modules/arch/x86/x86id.c"
 "${ROOT_DIR}/modules/dbgfmts/codeview/cv-dbgfmt.c"
 "${ROOT_DIR}/modules/dbgfmts/codeview/cv-symline.c"
 "${ROOT_DIR}/modules/dbgfmts/codeview/cv-type.c"
 "${ROOT_DIR}/modules/dbgfmts/dwarf2/dwarf2-aranges.c"
 "${ROOT_DIR}/modules/dbgfmts/dwarf2/dwarf2-dbgfmt.c"
 "${ROOT_DIR}/modules/dbgfmts/dwarf2/dwarf2-info.c"
 "${ROOT_DIR}/modules/dbgfmts/dwarf2/dwarf2-line.c"
 "${ROOT_DIR}/modules/dbgfmts/null/null-dbgfmt.c"
 "${ROOT_DIR}/modules/dbgfmts/stabs/stabs-dbgfmt.c"
 "${ROOT_DIR}/modules/listfmts/nasm/nasm-listfmt.c"
 "${ROOT_DIR}/modules/objfmts/bin/bin-objfmt.c"
 "${ROOT_DIR}/modules/objfmts/coff/coff-objfmt.c"
 "${ROOT_DIR}/modules/objfmts/coff/win64-except.c"
 "${ROOT_DIR}/modules/objfmts/dbg/dbg-objfmt.c"
 "${ROOT_DIR}/modules/objfmts/elf/elf.c"
 "${ROOT_DIR}/modules/objfmts/elf/elf-objfmt.c"
 "${ROOT_DIR}/modules/objfmts/elf/elf-x86-amd64.c"
 "${ROOT_DIR}/modules/objfmts/elf/elf-x86-x32.c"
 "${ROOT_DIR}/modules/objfmts/elf/elf-x86-x86.c"
 "${ROOT_DIR}/modules/objfmts/macho/macho-objfmt.c"
 "${ROOT_DIR}/modules/objfmts/rdf/rdf-objfmt.c"
 "${ROOT_DIR}/modules/objfmts/xdf/xdf-objfmt.c"
 "${ROOT_DIR}/modules/parsers/gas/gas-parse.c"
 "${ROOT_DIR}/modules/parsers/gas/gas-parse-intel.c"
 "${ROOT_DIR}/modules/parsers/gas/gas-parser.c"
 "${ROOT_DIR}/modules/parsers/nasm/nasm-parse.c"
 "${ROOT_DIR}/modules/parsers/nasm/nasm-parser.c"
 "${ROOT_DIR}/modules/preprocs/cpp/cpp-preproc.c"
 "${ROOT_DIR}/modules/preprocs/gas/gas-eval.c"
 "${ROOT_DIR}/modules/preprocs/gas/gas-preproc.c"
 "${ROOT_DIR}/modules/preprocs/nasm/nasm-eval.c"
 "${ROOT_DIR}/modules/preprocs/nasm/nasmlib.c"
 "${ROOT_DIR}/modules/preprocs/nasm/nasm-pp.c"
 "${ROOT_DIR}/modules/preprocs/nasm/nasm-preproc.c"
 "${ROOT_DIR}/modules/preprocs/raw/raw-preproc.c"
)

include ("${CMAKE_CURRENT_LIST_DIR}/common.inc.cmake")

target_include_directories (
 ${TARGET_NAME}
 PRIVATE "${ROOT_DIR}"
 PRIVATE "${ROOT_DIR}/gen_inc"
)
target_compile_definitions (
 ${TARGET_NAME}
 PRIVATE HAVE_CONFIG_H
)
set_target_properties (
 ${TARGET_NAME}
 PROPERTIES
 ARCHIVE_OUTPUT_NAME "yasm"
)


set (TARGET_NAME yasm)

add_executable (
 ${TARGET_NAME}
 "${ROOT_DIR}/frontends/yasm/yasm.c"
 "${ROOT_DIR}/frontends/yasm/yasm-options.c"
 "${ROOT_DIR}/frontends/yasm/yasm-plugin.c"
)

set_target_properties (
 ${TARGET_NAME}
 PROPERTIES
 RUNTIME_OUTPUT_DIRECTORY "${SLIB_BIN_PATH}"
)
target_include_directories (
 ${TARGET_NAME}
 PRIVATE "${ROOT_DIR}"
 PRIVATE "${ROOT_DIR}/gen_inc"
)
target_compile_definitions (
 ${TARGET_NAME}
 PRIVATE HAVE_CONFIG_H
)
if (CMAKE_BUILD_TYPE STREQUAL Debug)
 target_compile_options (
  ${TARGET_NAME}
  PRIVATE -O3
 )
endif ()
target_link_libraries (
 ${TARGET_NAME}
 libyasm dl
)

set(CMAKE_ASM_NASM_COMPILER "${SLIB_BIN_PATH}/yasm")
if (NOT EXISTS "${SLIB_BIN_PATH}/yasm")
 file (
  WRITE "${SLIB_BIN_PATH}/yasm"
  "# DummyFile to bypass CMake error"
 )
endif ()
enable_language(ASM_NASM)
set (
 CMAKE_ASM_NASM_COMPILE_OBJECT
 "<CMAKE_ASM_NASM_COMPILER> <FLAGS> -o <OBJECT> <SOURCE>"
)

endif ()
