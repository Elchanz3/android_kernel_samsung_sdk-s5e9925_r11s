#!/bin/bash

#!/bin/bash

mkdir kout
mkdir images
mkdir builds
export CDIR="$(pwd)"
export LOG_FILE=compilation-log.log
export OUT_DIR="/home/chanz22/Documentos/GitHub/android_kernel_samsung_sdk-s5e9925_r11s/kout"
export AK3="/home/chanz22/Documentos/GitHub/android_kernel_samsung_sdk-s5e9925_r11s/AnyKernel3"
export PATH=/home/chanz22/Documentos/GitHub/clang-r416183b/bin/
export PATH=/home/chanz22/Documentos/GitHub/gas/linux-x86z/
export CROSS_COMPILE=/home/chanz22/Documentos/GitHub/clang-r416183b/bin/clang
export TARGET_SOC=s5e9925
export IMAGE_NAME=HarmmoniX_Kernel
export KERNELZIP="HarmmoniX_Kernel.zip"
export KERNELVERSION=0.2

export PLATFORM_VERSION=12
export ANDROID_MAJOR_VERSION=s
export ARCH=arm64
export SEC_BUILD_CONF_VENDOR_BUILD_OS=14

export CCACHE=ccache

DATE_START=$(date +"%s")

make O="$OUT_DIR" chanz_c1s_defconfig
make O="$OUT_DIR" LLVM=1 LLVM_IAS=1 "$TARGET_SOC" -j12 2>&1 | tee "../$LOG_FILE"

cd /home/chanz22/Documentos/GitHub/android_kernel_samsung_sdk-s5e9925_r11s/toolchain/

./mkdtimg cfg_create "$AK3"/dtb.img $(pwd)/dtconfigs/s5e9925.cfg -d "$OUT_DIR"/arch/arm64/boot/dts/exynos
./mkdtimg cfg_create "$AK3"/dtbo.img $(pwd)/dtconfigs/r11s.cfg -d "$OUT_DIR"/arch/arm64/boot/dts/samsung

cd /home/chanz22/Documentos/GitHub/android_kernel_samsung_sdk-s5e9925_r11s/

cp "$OUT_DIR"/arch/arm64/boot/Image "$AK3"/Image

export IMAGE="$AK3"/Image

echo ""
echo ""
echo "******************************************"
echo ""
echo "Checking for required files..."
echo ""
echo "******************************************"

if [ ! -f "$IMAGE" ]; then
    echo "Compilation failed. Required file '$IMAGE' not found. Check logs."
    exit 1
else
    echo "File '$IMAGE' found. Proceeding to the next step."
fi

echo "Required files found. Proceeding to the next step."

echo ""
echo ""
echo "******************************************"
echo ""
echo "Generating Anykernel3 zip..."
echo ""
echo "******************************************"

rm -r AnyKernel3/*.zip

if [[ -f "$IMAGE" ]]; then
    KERNELZIP="HarmoniX.zip"

    (cd "AnyKernel3" && zip -r9 "$KERNELZIP" .) || error_exit "Error creating the AnyKernel package"

    echo "Zip done..."
fi

    cd AnyKernel3

    mv "$KERNELZIP" "$CDIR"/builds/HarmoniX"$KERNELVERSION".zip

echo ""
echo "proceeding to step 2..."
echo ""
echo "******************************************"
echo ""
echo "generating flashable image..."
echo ""
echo "******************************************"

# clean up previous images
cd "$CDIR"/AIK
./cleanup.sh
./unpackimg.sh --nosudo

# back to main dir
cd "$CDIR"

# move generated files to temporary directory
mv "$AK3"/dtb.img "$CDIR"/images/boot.img-dtb
mv "$AK3"/Image "$CDIR"/images/Image
mv "$CDIR"/images/Image "$CDIR"/images/boot.img-kernel

# cleanup past files and move new ones
rm "$CDIR"/AIK/split_img/boot.img-kernel
rm "$CDIR"/AIK/split_img/boot.img-dtb
mv "$CDIR"/images/boot.img-kernel "$CDIR"/AIK/split_img/boot.img-kernel
mv "$CDIR"/images/boot.img-dtb "$CDIR"/AIK/split_img/boot.img-dtb

# delete images dir
rm -r "$CDIR"/images

# goto AIK dir and repack boot.img as not sudo
cd "$CDIR"/AIK
./repackimg.sh --nosudo

# goto main dir
cd "$CDIR"

# move generated image to builds dir renamed as HarmoniX
mv "$CDIR"/AIK/image-new.img "$CDIR"/builds/"$IMAGE_NAME".img

if [ -d "kout" ]; then
    rm -rf "kout"
    echo "directory removed.."
else
    echo "pff. There is no 'kout' directory."
fi

echo "image done.."

    DATE_END=$(date +"%s")
    DIFF=$(($DATE_END - $DATE_START))
   
   echo -e "\nElapsed time: $(($DIFF / 60)) minute(s) and $(($DIFF % 60)) seconds.\n"

echo "find your zip and image in build dir..."
