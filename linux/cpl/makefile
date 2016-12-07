##traveral all of additional dependence .h files
cpl:=g++
cpl_flag:=-g
pre_macro:=#
out_dir:=./
cpp_dir:=./
out_name:=default.out
objs_dir:=./
includes:=#
libs_dir:=# 
libs:=#

##inputed param process.
local_null:=#
local_out_dir:=#

ifeq ($(out_dir), ./)
	local_out_dir:=$(out_dir)
else
	local_out_dir:=$(out_dir:/=)
	local_out_dir:=$(dir $(out_dir))$(notdir $(out_dir))/
endif

local_objs_dir:=#

ifeq ($(objs_dir), ./)
	local_objs_dir:=$(objs_dir)
else
	local_objs_dir:=$(objs_dir:/=)
	local_objs_dir:=$(dir $(objs_dir))$(notdir $(objs_dir))/
endif

#local_includes:=$(foreach tmp, $(includes), -I$(tmp))
local_includes:=$(subst :, , $(includes))
local_includes:=$(foreach tmp, $(local_includes), -I$(tmp))

local_pre_macro:=$(subst :, , $(pre_macro))
local_pre_macro:=$(foreach tmp, $(local_pre_macro), -D$(tmp))

local_libs:=$(subst .a,,$(libs))
local_libs:=$(subst :, , $(local_libs))
local_libs:=$(foreach tmp, $(local_libs), -l$(strip $(subst lib, $(local_null), $(tmp))))

local_libs_dir:=$(subst :, ,$(libs_dir))
local_libs_dir:=$(foreach tmp, $(local_libs_dir), -L$(tmp))

##Traversal all of cpp file to be compile.
local_cpps:=$(shell find $(cpp_dir) -name '*.cpp')
local_objs:=$(local_cpps:.cpp=.o)
####
VPATH:=$(foreach tmp, $(local_objs),$(dir $(tmp)))
VPATH+=$(objs_dir):$(out_dir)

##Link those objects are generated
ifeq ( , $(and $(findstring lib,$(out_name)),$(findstring .a, $(out_name)) ))

$(out_name):$(notdir $(local_objs))
	g++ $(foreach tmp, $(notdir $(local_objs)), $(local_objs_dir)$(tmp)) -o $(local_out_dir)$(out_name) $(local_libs_dir) $(local_libs)
else

$(out_name):$(notdir $(local_objs))
	ar rcs $(local_out_dir)$(out_name) $(foreach tmp, $(notdir $(local_objs)), $(local_objs_dir)$(tmp)) 
endif

%.o:%.cpp %.h
	$(cpl) $(cpl_flag) $(local_pre_macro) -c $< -o $(local_objs_dir)$(notdir $@)  $(local_includes)

%.o:%.cpp
	$(cpl) $(cpl_flag) $(local_pre_macro) -c $< -o $(local_objs_dir)$(notdir $@)  $(local_includes)

clean:
	rm -rf $(local_out_dir)$(out_name)
	rm -rf $(objs_dir)*.o

test:

#	echo $(includes)
	echo $(local_libs)
#	echo $(pre_macro)
#	echo $(local_pre_macro)
#	echo $(local_includes)
#	echo $(local_includes)
#	echo $(local_objs_dir)
#	echo $(objs_dir)
#	echo $(cpp_dir)
#i	echo  $(shell pwd)
#	echo $(local_out_dir)
#	echo $(dir ./)
#	echo $(notdir ./)








