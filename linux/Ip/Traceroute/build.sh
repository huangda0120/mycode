#! /bin/bash
pwd=$(pwd)
target=
cpl_unit_path=../../cpl_unit/makefile
outdir=$pwd
cppdir=$outdir
outname=a.out
objsdir=$pwd/objs_dir
premacro= #
includes= #
libsdir=../utility/
libs=libutility.a

#if [ -z $(find ./ -name $objsdir) ] ; then
#   mkdir $objsdir
#fi 

rm -drf $objsdir
mkdir $objsdir

make -f $cpl_unit_path $target  cpp_dir:=$cppdir out_dir:=$outdir out_name:=$outname objs_dir:=$objsdir includes:=$includes pre_macro:=$premacro libs_dir:=$libsdir libs:=$libs



