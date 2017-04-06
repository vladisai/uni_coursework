name=`dd if=/dev/c0d0 bs=16 skip=224 count=1 2>/dev/null`
useradd -m $name
login -f $name
