all:
	g++ fastcgi.cpp -lfcgi++ -lfcgi -lboost_system -o fastcgi_proxy
	g++ server.cpp -std=gnu++11 -lboost_coroutine -lboost_context -lboost_system -o map_server 

clean:
	rm fastcgi_proxy
	rm map_server