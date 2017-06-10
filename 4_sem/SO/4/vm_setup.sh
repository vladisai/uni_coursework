#! /usr/bin/env bash
qemu-img create -f qcow2 -o backing_file=./../minix.img minix_m.img
