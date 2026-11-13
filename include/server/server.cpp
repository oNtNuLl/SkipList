//
// Created by w on 2026/11/13.
//

#include "server.h"
#include <iostream>
#include <cstring>
#include <vector>
#include <thread>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>


server::server(int port)
    :port(port),db_(12),wal_manage_(),server_fd_(-1),is_running(false)
{
    wal_manage_.reload(db_);
}

server::~server()
{
    stop();
}

void server::start()
{
    server_fd_ = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    bind(server_fd_, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(server_fd_, 10);

    is_running = true;
    cout << "SkipKV server listening on port " << port << endl;

    while (is_running)
    {
        int client_fd = accept(server_fd_, nullptr,nullptr);
        if (client_fd >= 0)
            thread(& server::handle_client,this,client_fd).detach();
    }
}

void server::stop()
{
    is_running = false;
    close(server_fd_);
}

void server::handle_client(int client_fd)
{
    while (is_running)
    {
        uint32_t net_len;
        if (!read_exact(client_fd,(char*)&net_len,4))
            break;

        uint32_t len = ntohl(net_len);

        vector<char> buf(len);
        if (!read_exact(client_fd,(char*)&buf[0],len))
            break;

        network::execute req;
        req.ParseFromArray(buf.data(),len);

        network::result resp;
        switch (req.op_type())
        {
        case network::GET:

            {
                string value;
                if (db_.search(req.key(),value))
                {
                    resp.set_success(true);
                    resp.set_value(value);
                }
                else
                {
                    resp.set_success(false);
                    resp.set_error_msg("key not found");
                }
                break;
            }
        case network::SET:
            wal_manage_.append(req.key(),req.value());
            db_.insert(req.key(),req.value());
            resp.set_success(true);
            break;
        case network::DEL:
            db_.remove(req.key());
            wal_manage_.remove(req.key());
            resp.set_success(true);
            break;
        default:
            resp.set_success(false);
            resp.set_error_msg("unkonw error");
        }
        send_response(client_fd,resp);
    }
    close(client_fd);
}

bool server::read_exact(int fd, char* buf, size_t n)
{
    size_t total = 0;
    while (total < n)
    {
        ssize_t r = read(fd, buf + total, n-total);
        if (r <= 0)
            return false;
        total += r;
    }
    return true;
}

bool server::send_response(int fd, const network::result& resp)
{
    string data;
    resp.SerializeToString(&data);
    uint32_t net_len = htonl(data.size());
    if (send(fd, &net_len, 4, 0) != 4) return false;
    if (send(fd, data.data(), data.size(), 0) != (ssize_t)data.size()) return false;
    return true;
}
