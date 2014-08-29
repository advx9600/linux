#!/bin/sh
type=$1

total_out=/home/abc/Public/android/android4.0/s5pv210
uboot_out=$total_out/u-boot-samsung-dev
kernel_out=$total_out/kernel
android_out=$total_out/android4.0_realv210_ver_1_0/out/target/product/smdkv210
fast_boot="sudo /home/abc/Public/tools/android-sdk/android-sdk-linux/platform-tools/fastboot"

if [ $type = "bootloader" ]
then
	$fast_boot flash bootloader u-boot-samsung-dev/u-boot.bin
elif [ $type = "kernel" ]
then
	$fast_boot flash kernel kernel/arch/arm/boot/zImage
elif [ $type = "ramdisk" ]
then
	$fast_boot flash ramdisk $android_out/ramdisk-yaffs.img
elif [ $type = "system" ]
then
	$fast_boot flash system $android_out/system.img
elif [ $type = "userdata" ]
then
	$fast_boot flash userdata $android_out/userdata.img
fi
