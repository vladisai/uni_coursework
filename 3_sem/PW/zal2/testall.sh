for i in `seq 1 48`; do
    echo $i
    time ./brandes $i bigin /dev/null
done
