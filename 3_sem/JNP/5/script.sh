for ((test = 1; test <= 3; test++))
do
	g++ -std=c++14 -DTEST=${test} -O2 compile_err_tests.cpp 2>/dev/null
	if [[ $? == 1 ]]; then
		printf "OK\n"
	else
		printf "BAD\n"
	fi	
done

