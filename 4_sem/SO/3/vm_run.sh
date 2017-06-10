#! /usr/bin/env bash
qemu-system-x86_64 -curses -drive file=minix_m.img -localtime -net user,hostfwd=tcp::15881-:22 -net nic,model=virtio -m 1024M -cpu host -smp 4 -enable-kvm

