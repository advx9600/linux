import versionupdate
import os
import sys

#print "(0 u-boot,1-kernel,2-system) (s5pv210...)"
board=sys.argv[1]

img_type=sys.argv[2]
img_type=int(img_type)

ver_file = versionupdate.getOpenVerName(img_type)
#print ver_file
	
img_dir=""
img_files=[];
zipname=""

if img_type == 0:
	img_files=["u-boot.bin"]
	zipname="u-boot.zip"
elif img_type == 1:
	os.system("cp arch/arm/boot/zImage arch/arm/boot/kernel.img");
	img_dir="arch/arm/boot/"
	img_files=["kernel.img"]
	zipname="kernel.zip"
elif img_type == 2:
	img_dir="out/target/product/smdkv210/"
	img_files=["ramdisk-yaffs.img","userdata.img","system.img"]
	zipname="system.zip"


zipname=board+"_"+zipname;

print "target file:",zipname

all_files = ver_file
for i in img_files:
	all_files += " "+img_dir+i

#print all_files

if os.path.exists(zipname):
	os.remove(zipname)
os.system("zip "+zipname+" -j "+all_files)
