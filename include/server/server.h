//
// Created by w on 2026/11/13.
//

#ifndef SKIPKV_SERVER_H
#define SKIPKV_SERVER_H
#include "SkipList.h"
#include "WALManage.h"
#include "NetWork.pb.h"
#include <atomic>

using namespace std;

class server
{
public:
    server(int port);
    ~server();

    void start();

    void stop();

private:

    void handle_client(int client_fd);
    bool read_exact(int fd, char* buf, size_t n);

    bool send_response(int fd, const network::result& resp);
    int port;
    int server_fd_;
    atomic<bool> is_running;

    SkipList<string,string> db_;
    WALManage wal_manage_;
};


#endif //SKIPKV_SERVER_H