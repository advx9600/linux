import subprocess
import  sys
import os

my_dir="my_dir"
board=""
imgType=""
branch=""
fileName=""

if (len(sys.argv) != 3):
	print "argc err"
	print "board imgType"
	sys.exit("")

board =sys.argv[1]
imgType=sys.argv[2]

p = subprocess.Popen(['git','branch'],stdout=subprocess.PIPE)
out =p.communicate()[0]
branch=out.split(' ')[1].strip()

fileName=my_dir+"/"+imgType+"_"+branch+"_version.xml"
print "xmlFile:",fileName


imgFiles=[]
imgDir=""
zipName=""
if (cmp(imgType,"uboot")==0):
	imgFiles=["u-boot.bin"]
	zipName="u-boot.zip"	
elif (cmp(imgType,"kernel")==0):
	os.system("cp arch/arm/boot/zImage arch/arm/boot/kernel.img");
	imgDir="arch/arm/boot/"
	imgFiles=["kernel.img"]
	zipName="kernel.zip"
elif (cmp(imgType,"system")==0):
	imgDir="out/target/product/"+board+"/"
	imgFiles=["ramdisk-yaffs.img","userdata.img","system.img"]
	zipName="system.zip"

zipName=board+"_"+zipName;
print "target file:",zipName


allZipFiles=fileName
for  i in imgFiles:
	allZipFiles += " "+imgDir+i

if os.path.exists(zipName):
	os.remove(zipName)
os.system("zip "+zipName+" -j "+allZipFiles)
