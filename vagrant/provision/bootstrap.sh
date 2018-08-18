#!/usr/bin/env sh

apt install -y build-essential
apt install -y clang-6.0
apt install -y cmake
apt install -y lldb-6.0
apt install -y ninja-build

cp /vagrant/vagrant/profile /home/vagrant/.profile
chmod 0644 /home/vagrant/.profile
