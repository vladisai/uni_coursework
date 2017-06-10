#! /usr/bin/env bash
cd changed
./make_patch.sh
cd ..
scp changed/ws374078.patch so:/usr/src
scp a.c so:~/
scp prepare.sh so:~/
scp run.sh so:~/
