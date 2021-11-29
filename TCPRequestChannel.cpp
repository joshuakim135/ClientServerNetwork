#include "TCPRequestChannel.h"

void TCPRequestChannel::serverSetUp(const string port) {

}
void TCPRequestChannel::clientSetUp(const string host, const string port) {

}

TCPRequestChannel::TCPRequestChannel (const string host_name, const string port_no) {
    // if host name empty string -> server side; else -> client side
    (host_name.empty()) ? serverSetUp(port_no) : clientSetUp(host_name, port_no);
}
TCPRequestChannel::TCPRequestChannel(int) {

}
TCPRequestChannel::~TCPRequestChannel() {
    
}
int TCPRequestChannel::cread(void* msgbuf, int buflen) {

}
int TCPRequestChannel::cwrite(void* msgbuf, int msglen) {

}