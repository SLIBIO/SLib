cd $(dirname $0)
LIBVPX_PATH=`pwd`

generate_arm() {
	local path=$1
	local file=$2
	local outfile=$path/$file.S
	if [ -n "$3" ]
	then
		outfile=$3
	else
		mkdir -p "$LIBVPX_PATH/gen/asm_apple/armv7/$path"
		mkdir -p "$LIBVPX_PATH/gen/asm/armv7/$path"
	fi
	cat "$LIBVPX_PATH/$path/$file" | perl "$LIBVPX_PATH/build/make/ads2gas_apple.pl" > "$LIBVPX_PATH/gen/asm_apple/armv7/$outfile"
	cat "$LIBVPX_PATH/$path/$file" | perl "$LIBVPX_PATH/build/make/ads2gas.pl" > "$LIBVPX_PATH/gen/asm/armv7/$outfile"
}

generate_arm vpx_dsp/arm idct_neon.asm
generate_arm vpx_dsp/arm idct4x4_1_add_neon.asm
generate_arm vpx_dsp/arm idct4x4_add_neon.asm
generate_arm vpx_dsp/arm intrapred_neon_asm.asm
generate_arm vpx_dsp/arm loopfilter_4_neon.asm
generate_arm vpx_dsp/arm loopfilter_8_neon.asm
generate_arm vpx_dsp/arm loopfilter_16_neon.asm
generate_arm vpx_dsp/arm save_reg_neon.asm
generate_arm vpx_dsp/arm vpx_convolve_avg_neon_asm.asm
generate_arm vpx_dsp/arm vpx_convolve_copy_neon_asm.asm
generate_arm vpx_dsp/arm vpx_convolve8_avg_neon_asm.asm
generate_arm vpx_dsp/arm vpx_convolve8_neon_asm.asm

generate_arm "" vpx_config_arm.asm vpx_config.asm
generate_arm "" vpx_config_common.asm vpx_config_common.S

mkdir -p "$LIBVPX_PATH/gen/asm/i386"
cp "$LIBVPX_PATH/vpx_config_x86.asm" "$LIBVPX_PATH/gen/asm/i386/vpx_config.asm"
mkdir -p "$LIBVPX_PATH/gen/asm/x86_64"
cp "$LIBVPX_PATH/vpx_config_x86_64.asm" "$LIBVPX_PATH/gen/asm/x86_64/vpx_config.asm"
