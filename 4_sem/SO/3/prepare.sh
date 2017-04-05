dd if=/dev/c0d0 of=/dev/c0d0 bs=512 count=1 seek=2
dd if=loop of=/dev/c0d0 bs=512 count=1 seek=1
dd if=jumper of=/dev/c0d0 bs=446 count=1
dd if=/dev/c0d0 of=out bs=512 count=5
echo 'bash ./usr.sh' >> .profile
#hexdump -C out | vim -
