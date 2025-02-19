#!/bin/bash

# Check if the library paths already exist in .bashrc
if ! grep -q "libqtc" $HOME/.bashrc; then
  echo "
# Paths for loading the shared library - libqtc.so -
# $(date)
#----------------------------------------------------------
export PATH_TO_QTC=$PWD
export LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:\$PATH_TO_QTC/lib
export Iqtc=-I\$PATH_TO_QTC/include
export Lqtc=-L\$PATH_TO_QTC/lib\ -lqtc
export useqtc=\$Iqtc\ \$Lqtc
#----------------------------------------------------------" | cat >>$HOME/.bashrc
  echo "Library paths have been added to .bashrc"
  bash; source $HOME/.bashrc
else
  echo "Library paths are already present in .bashrc"
fi
