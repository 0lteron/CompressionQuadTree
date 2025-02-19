#!/bin/bash

if grep -q "libqtc.so" ~/.bashrc; then
  cp ~/.bashrc ~/.bashrc.bak
  echo "Backup of .bashrc created as .bashrc.bak"

  sed -i '/libqtc\.so/,+8d' ~/.bashrc

  unset useqtc
  unset Lqtc
  unset Iqtc
  unset PATH_TO_QTC
  export LD_LIBRARY_PATH=$(echo $LD_LIBRARY_PATH | sed -e "s|\$PATH_TO_QTC/lib:||g" -e "s|:\$PATH_TO_QTC/lib||g" -e "s|\$PATH_TO_QTC/lib||g")

  echo "Removed the block related to libqtc.so from ~/.bashrc"
  bash; source $HOME/.bashrc
else
  echo "No libqtc.so related paths found in ~/.bashrc"
fi
