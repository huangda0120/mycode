# /bin/bash
service iptables stop
#here, need to indicate a aboslute path.
export CPL_UNIT_PATH=

rm -rf ../sysfuntags
mkdir ../sysfuntags
ctags -I __THROW --file-scope=yes --langmap=c:+.h --languages=c,c++ --links=yes --c-kinds=+p --fields=+S  -R -f ../sysfuntags/tags /usr/include
