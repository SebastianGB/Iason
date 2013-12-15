#!/bin/bash


#install summon-arm-toolchain

#install dependencies
apt-get install flex bison libgmp3-dev libmpfr-dev libncurses5-dev libmpc-dev autoconf texinfo build-essential libftdi-dev zlib1g-dev git zlib1g-dev python-yaml

git clone https://github.com/esden/summon-arm-toolchain.git
cd *summon-arm-toolchain*
./summon-arm-toolchain
echo 'PATH=$PATH:~/sat/bin' >> ~/.bashrc

#insatll st-link (zum flashen)
git clone git@github.com:texane/stlink.git

#only needs libusb as dependency, should already be there on a ubuntu	
cd stlink
./autogen.sh
./configure
make





