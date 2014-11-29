#include <boost/asio.hpp>
#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include "fcgio.h"

using namespace std;
using boost::asio::local::stream_protocol;


int main(void) {
    // Backup the stdio streambufs
    streambuf * cin_stream  = cin.rdbuf();
    streambuf * cout_stream = cout.rdbuf();
    streambuf * cerr_stream = cerr.rdbuf();
    char *buf_write;
    char buf_read [1024];

    FCGX_Request request;

    FCGX_Init();
    FCGX_InitRequest(&request, 0, 0);


    ofstream logfile;
    logfile.open ("/home/gurio/logfile.log");

    while (FCGX_Accept_r(&request) == 0) {
        fcgi_streambuf cin_fcgi_stream(request.in);
        fcgi_streambuf cout_fcgi_stream(request.out);
        fcgi_streambuf cerr_fcgi_stream(request.err);

        cin.rdbuf(&cin_fcgi_stream);
        cout.rdbuf(&cout_fcgi_stream);
        cerr.rdbuf(&cerr_fcgi_stream);

        boost::asio::io_service service;
        stream_protocol::endpoint ep("/opt/hmp/useragent.sock");
        stream_protocol::socket socket(service);
        socket.connect(ep);
        
        buf_write = FCGX_GetParam("HTTP_USER_AGENT", request.envp); 

        socket.write_some(boost::asio::buffer(buf_write, strlen(buf_write)));
        memset(buf_read, 0, 1024);
        socket.read_some(boost::asio::buffer(buf_read));
        socket.close();
        cout << "Content-type: text/html\r\n\r\n"
             << "    <html>"
             << buf_read
             << "</html>\n";


    }

    // restore stdio streambufs
    cin.rdbuf(cin_stream);
    cout.rdbuf(cout_stream);
    cerr.rdbuf(cerr_stream);

    logfile.close();

    return 0;
}
