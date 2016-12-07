#! /bin/bash
pwd=$(pwd)
cpl_unit_path=$CPL_UNIT_PATH
outdir=$pwd
cppdir= #
outname=a.out
objsdir=$pwd/objs_dir
premacro= #
includes= #
libsdir= #
libs= #

#if [ -z $(find ./ -name $objsdir) ] ; then
#   mkdir $objsdir
#fi 

rm -drf $objsdir
mkdir $objs_dir

make -f $cpl_unit_path/my-make  cpp_dir:=$cppdir out_dir:=$outdir out_name:=$outname objs_dir:=$objsdir includes:=$includes pre_macro:=$premacro libs_dir:=$libsdir libs:=$libs



