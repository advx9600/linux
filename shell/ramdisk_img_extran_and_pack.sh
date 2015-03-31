mv ramdisk.img ramdisk.img.gz
gunzip ramdisk.img.gz
mkdir ramdisk
cd ramdisk
cpio -i -F ../ramdisk.img


rm -f ../ramdisk.img
find . | cpio -o -H newc > ../ramdisk.img.unzip
cd ..
gzip -c ./ramdisk.img.unzip > ./ramdisk.img.gz
rm -f ./ramdisk.img.unzip
mv ./ramdisk.img.gz ./ramdisk.img
