#! /usr/bin/env bash
rm -r mfs
mkdir mfs
mkfs.mfs /dev/c0d1
mount /dev/c0d1 mfs
