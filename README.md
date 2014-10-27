hmp
===

###About
This is web application written in C++ language. Idea is to parse UserAgent, and then show to visitor is he first on such browser or not. __It is overengineered for such simple task__. It could be much simplier, but its purpose to show some C++ skills.

###Nginx
Nginx is used to give html page, and to run fastCGI C++ listener. Listener extracts UserAgent and then passes it to C++ useragent-info-server, via unix domain socket

###Boost
Boost is used for useragent-info-server. I've used boost coroutines implementation (spawn) and boost domain socket to write async server. Look back/server.cpp.


