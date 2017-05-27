#! /usr/bin/env bash

cd ..
./mnt.sh
cd changed

if [ ! -d ../../fs/usr/src/minix/servers/sched_old ]; then
    mv ./../fs/usr/src/minix/servers/sched/ ./../fs/usr/src/minix/servers/sched_old/
fi

/bin/rm -r ./../fs/usr/src/minix/servers/sched

cp -r ./sched ./../fs/usr/src/minix/servers
