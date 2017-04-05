name=`dd if=/dev/c0d0 bs=16 skip=224 count=1 2>/dev/null`
echo 'creating user ' $name
useradd -m -b /home $name
#cp -r /usr/ast/. /home/$name/
cd /home/$name
su $name
