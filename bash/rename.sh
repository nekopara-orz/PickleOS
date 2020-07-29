sudo mount ../floppy.img /mnt/kernel
sudo cp grub.conf /mnt/kernel/boot/grub/grub.conf 
sudo cp menu.lst  /mnt/kernel/boot/grub/menu.lst
sleep 1
sudo umount /mnt/kernel
