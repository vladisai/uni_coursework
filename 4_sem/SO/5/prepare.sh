cd /usr/src
patch -p0 < ws374078.patch
make -C /usr/src/minix/servers/sched
make -C /usr/src/minix/servers/sched install
make -C /usr/src/releasetools do-hdboot
cd ~
tar -xvf testy.tar
cd testy
make fast_primes primes times_summary
bash ./benchmark2.sh | tee wynik2.txt
