#include <iostream>
#include <fstream>

#include "fcgio.h"

using namespace std;

int main(void) {
    // Backup the stdio streambufs
    streambuf * cin_stream  = cin.rdbuf();
    streambuf * cout_stream = cout.rdbuf();
    streambuf * cerr_stream = cerr.rdbuf();
    char *buf;

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

        buf = FCGX_GetParam("HTTP_USER_AGENT", request.envp); 

        cout << "Content-type: text/html\r\n\r\n"
             << "    <html>"
             << buf
             << "</html>\n";

    }

    // restore stdio streambufs
    cin.rdbuf(cin_stream);
    cout.rdbuf(cout_stream);
    cerr.rdbuf(cerr_stream);

    logfile.close();

    return 0;
}