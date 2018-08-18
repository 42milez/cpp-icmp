#!/usr/bin/env sh

DIR_BUILD=/home/vagrant/build
DIR_ICMP1=/home/vagrant/icmp1
DIR_ICMP2=/home/vagrant/icmp2

if [ ! -e ${DIR_BUILD} ]; then
  mkdir ${DIR_BUILD}
fi

if [ ! -e ${DIR_ICMP1} ]; then
  mkdir ${DIR_ICMP1}
fi

if [ ! -e ${DIR_ICMP2} ]; then
  mkdir ${DIR_ICMP2}
fi

cd ${DIR_BUILD}

# https://cmake.org/cmake/help/v3.12/generator/CodeBlocks.html
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=/usr/bin/ninja -DCMAKE_C_COMPILER=/usr/bin/clang-6.0 -DCMAKE_CXX_COMPILER=/usr/bin/clang++-6.0 -DCMAKE_CXX_FLAGS="-g" -G "CodeBlocks - Ninja" /vagrant
cmake --build . --target icmp -- -j 2

cd

if [ ! -e /home/vagrant/build/icmp/icmp ]; then
  exit
fi

cp /home/vagrant/build/icmp/icmp ${DIR_ICMP1}
cp /home/vagrant/build/icmp/icmp ${DIR_ICMP2}

chmod 0700 ${DIR_ICMP1}/icmp
chmod 0700 ${DIR_ICMP2}/icmp

cp /vagrant/config.template.json ${DIR_ICMP1}/config.json
cp /vagrant/config.template.json ${DIR_ICMP2}/config.json

DEVICE=enp0s3
GATEWAY=10.0.2.2
VIP1=10.0.2.100
VIP2=10.0.2.101
VMAC1=02:00:00:00:00:01
VMAC2=02:00:00:00:00:02
VMASK=255.255.255.0

sed -i -e "s/\"device\": \"\"/\"device\": \"${DEVICE}\"/g" ${DIR_ICMP1}/config.json
sed -i -e "s/\"device\": \"\"/\"device\": \"${DEVICE}\"/g" ${DIR_ICMP2}/config.json

sed -i -e "s/\"gateway\": \"\"/\"gateway\": \"${GATEWAY}\"/g" ${DIR_ICMP1}/config.json
sed -i -e "s/\"gateway\": \"\"/\"gateway\": \"${GATEWAY}\"/g" ${DIR_ICMP2}/config.json

sed -i -e "s/\"vip\": \"\"/\"vip\": \"${VIP1}\"/g" ${DIR_ICMP1}/config.json
sed -i -e "s/\"vip\": \"\"/\"vip\": \"${VIP2}\"/g" ${DIR_ICMP2}/config.json

sed -i -e "s/\"vmac\": \"\"/\"vmac\": \"${VMAC1}\"/g" ${DIR_ICMP1}/config.json
sed -i -e "s/\"vmac\": \"\"/\"vmac\": \"${VMAC2}\"/g" ${DIR_ICMP2}/config.json

sed -i -e "s/\"vmask\": \"\"/\"vmask\": \"${VMASK}\"/g" ${DIR_ICMP1}/config.json
sed -i -e "s/\"vmask\": \"\"/\"vmask\": \"${VMASK}\"/g" ${DIR_ICMP2}/config.json
