#!/usr/bin/env bash

DIR_BUILD=/home/vagrant/build

cmake --build ${DIR_BUILD} --target clean -- -j 4
