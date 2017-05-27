#! /usr/bin/env bash

./vm_setup.sh
./vm_run.sh &disown;

clear

sleep 5 # booting

./upload.sh
ssh so ./test0.sh log > clean0
ssh so ./test1.sh log > clean1

ssh so ./prepare.sh &

sleep 15 # rebooting

ssh so ./test0.sh log > patched0
ssh so ./test1.sh log > patched1

ssh so poweroff
