#!/usr/bin/env sh

# myeth1

DIR_MYETH_1A=/home/vagrant/myeth1a
DIR_MYETH_1B=/home/vagrant/myeth1b

cd myeth1
make
mkdir ${DIR_MYETH_1A} ${DIR_MYETH_1B}
cp myeth ${DIR_MYETH_1A}
chmod 0700 ${DIR_MYETH_1A}/myeth
cp myeth ${DIR_MYETH_1B}
chmod 0700 ${DIR_MYETH_1B}/myeth
cp myeth1a.ini ${DIR_MYETH_1A}
cp myeth1b.ini ${DIR_MYETH_1B}

