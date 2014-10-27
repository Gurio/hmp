#!/bin/sh
CLEAN=$1

cd back/
make $CLEAN all
killall fastcgi_proxy
killall map_server
spawn-fcgi -p 8000 -n fastcgi_proxy &
./map_server /opt/hmp/useragent.sock &
