#!/bin/sh
OPT=$1

cd ui/
for md in *md; do
	base=${md%.*}
	markdown $md > ${base}.html
done

if [ $OPT -eq "restart" ]; then
	cd ../back/
	make clean all
	killall fastcgi_proxy
	killall map_server
	spawn-fcgi -p 8000 -n fastcgi_proxy &
	./map_server /opt/hmp/useragent.sock &
fi
