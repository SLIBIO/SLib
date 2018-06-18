cd $(dirname $0)
LIBVPX_PATH=`pwd`

mkdir -p "$LIBVPX_PATH/gen"
mkdir -p "$LIBVPX_PATH/gen/rtcd"

generate_rtcd() {
	local arch=$1
	local sym=$2
	local file=$3
	"$LIBVPX_PATH/build/make/rtcd.pl" \
		--arch=$arch \
		--sym=$sym \
		--config="$LIBVPX_PATH/generate-rtcd.config" \
		"$file" \
		> "$LIBVPX_PATH/gen/rtcd/$arch/$sym.h"
}

generate_rtcd_per_arch() {
	local arch=$1
	mkdir -p "$LIBVPX_PATH/gen/rtcd/$arch"
	generate_rtcd $arch vp8_rtcd "$LIBVPX_PATH/vp8/common/rtcd_defs.pl"
	generate_rtcd $arch vp9_rtcd "$LIBVPX_PATH/vp9/common/vp9_rtcd_defs.pl"
	generate_rtcd $arch vpx_dsp_rtcd "$LIBVPX_PATH/vpx_dsp/vpx_dsp_rtcd_defs.pl"
	generate_rtcd $arch vpx_scale_rtcd "$LIBVPX_PATH/vpx_scale/vpx_scale_rtcd.pl"
}

generate_rtcd_per_arch x86
generate_rtcd_per_arch x86_64
generate_rtcd_per_arch armv7
generate_rtcd_per_arch arm64
