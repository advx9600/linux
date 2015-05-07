echo 0 > /sys/class/android_usb/android0/enable
echo 18d1 >/sys/class/android_usb/android0/idVendor
echo D002 > /sys/class/android_usb/android0/idProduct
echo mass_storage,adb > /sys/class/android_usb/android0/functions
echo "/dev/sdcard" >/sys/class/android_usb/android0/f_mass_storage/lun/file
echo 1 > /sys/class/android_usb/android0/enable
