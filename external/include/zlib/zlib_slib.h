#ifndef ZLIB_SLIB_H
#define ZLIB_SLIB_H

#ifndef SLIB_ZLIB_NO_PREFIX

#  define _dist_code            slib_z__dist_code
#  define _length_code          slib_z__length_code
#  define _tr_align             slib_z__tr_align
#  define _tr_flush_bits        slib_z__tr_flush_bits
#  define _tr_flush_block       slib_z__tr_flush_block
#  define _tr_init              slib_z__tr_init
#  define _tr_stored_block      slib_z__tr_stored_block
#  define _tr_tally             slib_z__tr_tally
#  define adler32               slib_z_adler32
#  define adler32_combine       slib_z_adler32_combine
#  define adler32_combine64     slib_z_adler32_combine64
#  define adler32_z             slib_z_adler32_z
#  ifndef Z_SOLO
#    define compress              slib_z_compress
#    define compress2             slib_z_compress2
#    define compressBound         slib_z_compressBound
#  endif
#  define crc32                 slib_z_crc32
#  define crc32_combine         slib_z_crc32_combine
#  define crc32_combine64       slib_z_crc32_combine64
#  define crc32_z               slib_z_crc32_z
#  define deflate               slib_z_deflate
#  define deflateBound          slib_z_deflateBound
#  define deflateCopy           slib_z_deflateCopy
#  define deflateEnd            slib_z_deflateEnd
#  define deflateGetDictionary  slib_z_deflateGetDictionary
#  define deflateInit2_         slib_z_deflateInit2_
#  define deflateInit_          slib_z_deflateInit_
#  define deflateParams         slib_z_deflateParams
#  define deflatePending        slib_z_deflatePending
#  define deflatePrime          slib_z_deflatePrime
#  define deflateReset          slib_z_deflateReset
#  define deflateResetKeep      slib_z_deflateResetKeep
#  define deflateSetDictionary  slib_z_deflateSetDictionary
#  define deflateSetHeader      slib_z_deflateSetHeader
#  define deflateTune           slib_z_deflateTune
#  define deflate_copyright     slib_z_deflate_copyright
#  define get_crc_table         slib_z_get_crc_table
#  ifndef Z_SOLO
#    define gz_error              slib_z_gz_error
#    define gz_intmax             slib_z_gz_intmax
#    define gz_strwinerror        slib_z_gz_strwinerror
#    define gzbuffer              slib_z_gzbuffer
#    define gzclearerr            slib_z_gzclearerr
#    define gzclose               slib_z_gzclose
#    define gzclose_r             slib_z_gzclose_r
#    define gzclose_w             slib_z_gzclose_w
#    define gzdirect              slib_z_gzdirect
#    define gzdopen               slib_z_gzdopen
#    define gzeof                 slib_z_gzeof
#    define gzerror               slib_z_gzerror
#    define gzflush               slib_z_gzflush
#    define gzfread               slib_z_gzfread
#    define gzfwrite              slib_z_gzfwrite
#    define gzgetc_               slib_z_gzgetc_
#    define gzgets                slib_z_gzgets
#    define gzoffset              slib_z_gzoffset
#    define gzoffset64            slib_z_gzoffset64
#    define gzopen                slib_z_gzopen
#    define gzopen64              slib_z_gzopen64
#    ifdef _WIN32
#      define gzopen_w              slib_z_gzopen_w
#    endif
#    define gzprintf              slib_z_gzprintf
#    define gzputc                slib_z_gzputc
#    define gzputs                slib_z_gzputs
#    define gzread                slib_z_gzread
#    define gzrewind              slib_z_gzrewind
#    define gzseek                slib_z_gzseek
#    define gzseek64              slib_z_gzseek64
#    define gzsetparams           slib_z_gzsetparams
#    define gztell                slib_z_gztell
#    define gztell64              slib_z_gztell64
#    define gzungetc              slib_z_gzungetc
#    define gzvprintf             slib_z_gzvprintf
#    define gzwrite               slib_z_gzwrite
#  endif
#  define inflate               slib_z_inflate
#  define inflateBack           slib_z_inflateBack
#  define inflateBackEnd        slib_z_inflateBackEnd
#  define inflateBackInit_      slib_z_inflateBackInit_
#  define inflateCodesUsed      slib_z_inflateCodesUsed
#  define inflateCopy           slib_z_inflateCopy
#  define inflateEnd            slib_z_inflateEnd
#  define inflateGetDictionary  slib_z_inflateGetDictionary
#  define inflateGetHeader      slib_z_inflateGetHeader
#  define inflateInit2_         slib_z_inflateInit2_
#  define inflateInit_          slib_z_inflateInit_
#  define inflateMark           slib_z_inflateMark
#  define inflatePrime          slib_z_inflatePrime
#  define inflateReset          slib_z_inflateReset
#  define inflateReset2         slib_z_inflateReset2
#  define inflateResetKeep      slib_z_inflateResetKeep
#  define inflateSetDictionary  slib_z_inflateSetDictionary
#  define inflateSync           slib_z_inflateSync
#  define inflateSyncPoint      slib_z_inflateSyncPoint
#  define inflateUndermine      slib_z_inflateUndermine
#  define inflateValidate       slib_z_inflateValidate
#  define inflate_copyright     slib_z_inflate_copyright
#  define inflate_fast          slib_z_inflate_fast
#  define inflate_table         slib_z_inflate_table
#  ifndef Z_SOLO
#    define uncompress            slib_z_uncompress
#    define uncompress2           slib_z_uncompress2
#  endif
#  define zError                slib_z_zError
#  ifndef Z_SOLO
#    define zcalloc               slib_z_zcalloc
#    define zcfree                slib_z_zcfree
#  endif
#  define zlibCompileFlags      slib_z_zlibCompileFlags
#  define zlibVersion           slib_z_zlibVersion

/* all zlib typedefs in zlib.h and zconf.h */
#  define Byte                  slib_z_Byte
#  define Bytef                 slib_z_Bytef
#  define alloc_func            slib_z_alloc_func
#  define charf                 slib_z_charf
#  define free_func             slib_z_free_func
#  ifndef Z_SOLO
#    define gzFile                slib_z_gzFile
#  endif
#  define gz_header             slib_z_gz_header
#  define gz_headerp            slib_z_gz_headerp
#  define in_func               slib_z_in_func
#  define intf                  slib_z_intf
#  define out_func              slib_z_out_func
#  define uInt                  slib_z_uInt
#  define uIntf                 slib_z_uIntf
#  define uLong                 slib_z_uLong
#  define uLongf                slib_z_uLongf
#  define voidp                 slib_z_voidp
#  define voidpc                slib_z_voidpc
#  define voidpf                slib_z_voidpf

/* all zlib structs in zlib.h and zconf.h */
#  define gz_header_s           slib_z_gz_header_s
#  define internal_state        slib_z_internal_state

#define z_errmsg slib_z_errmsg

#endif

#endif