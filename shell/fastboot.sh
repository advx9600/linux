#!/bin/sh
type=$1
android_out=android4.0_realv210_ver_1_0/out/target/product/smdkv210
if [ $type = "bootloader" ]
then
	fastboot flash bootloader u-boot-samsung-dev/u-boot.bin
elif [ $type = "kernel" ]
then
	fastboot flash kernel kernel/arch/arm/boot/zImage
elif [ $type = "ramdisk" ]
then
	fastboot flash ramdisk $android_out/ramdisk-yaffs.img
elif [ $type = "system" ]
then
	fastboot flash system $android_out/system.img
elif [ $type = "userdata" ]
then
	fastboot flash userdata $android_out/userdata.img
fi
