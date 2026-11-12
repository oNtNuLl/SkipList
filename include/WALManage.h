//
// Created by w on 2026/11/11.
//

#ifndef SKIPKV_WALMANAGE_H
#define SKIPKV_WALMANAGE_H
#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include "SkipList.h"
#include "WALBuffer.h"
#include "WALIO.h"

using namespace std;
class WALManage
{
    public:
    WALManage(const std::string &fileName  = "wal.log");
    ~WALManage();

    void append(const std::string &key, const std::string &value);
    void remove(const std::string &key);
    bool reload(SkipList<std::string,std::string> & list);

    private:
    //前台写入buffer
    unique_ptr<WALBuffer> current_buffer_;
    //落盘buffer
    unique_ptr<WALBuffer> flush_buffer_;

    mutex mutex_;
    condition_variable condition_;
    atomic<bool> is_running_;
    thread bg_thread_;
    string file_name_;
    WALIO  wal_io_;

};


#endif //SKIPKV_WALMANAGE_H