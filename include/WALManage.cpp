//
// Created by w on 2026/11/11.
//

#include "WALManage.h"

WALManage::WALManage(const std::string& fileName)
    :file_name_(fileName),is_running_(true),wal_io_(fileName),current_buffer_(new WALBuffer()),flush_buffer_(new WALBuffer()){

    bg_thread_ = std::thread([this]()
    {
        while (is_running_)
        {
            {
            unique_lock<mutex> lock(mutex_);

            condition_.wait_for(lock,chrono::seconds(1),[&]
            {
                return !is_running_ || current_buffer_->size() > 0;
            });

             if (flush_buffer_->size() == 0 && current_buffer_->size() > 0)
             {
                 swap(current_buffer_, flush_buffer_);
             }
            }
            if (flush_buffer_->size() > 0)
            {
                //调用io
                wal_io_.write(flush_buffer_->data(),flush_buffer_->size());
                flush_buffer_->reset();
            }
        }
    });

}

WALManage::~WALManage()
{
    is_running_ = false;
    condition_.notify_one();

    if (bg_thread_.joinable())
    {
        bg_thread_.join();
    }
}

void WALManage::append(const std::string& key, const std::string& value)
{
    unique_lock<mutex> lock(mutex_);
    if (!current_buffer_->appendInsert(key,value))
    {
        swap(current_buffer_, flush_buffer_);
        condition_.notify_one();
        current_buffer_->appendInsert(key,value);
    }
}

void WALManage::remove(const std::string& key)
{
    unique_lock<mutex> lock(mutex_);
    if (! current_buffer_->appendDelete(key))
    {
        swap(current_buffer_, flush_buffer_);
        condition_.notify_one();
        current_buffer_->appendDelete(key);
    }
}

bool WALManage::reload(SkipList<std::string, std::string>& list)
{
    if (!wal_io_.open_for_read("wal.log"))
        return false;

    WalOpType op;
    string key, value;

    while (wal_io_.read_record(op,key,value))
    {
        if (op == WalOpType::INSERT)
        {
            list.insert(key, value);
        }else if (op == WalOpType::DELETE)
        {
            list.remove(key);
        }
    }
    wal_io_.read_close();
    return true;
}
