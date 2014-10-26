###TODO:

1. write simple web server printing all header info
2. parse user agent
3. store this info in some hashtable with time and request_num
4. write nginx config, to:
    * pass UA to cpp server
    * give back static
    * pass delete command if it nescessary
5. write some information handler, with on-timer event of memory serialization.
6. on-startup file storage check. And Loader/Creator
7. use bootstrap