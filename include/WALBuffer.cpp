//
// Created by w on 2026/11/11.
//

#include "WALBuffer.h"

#include <cstring>

WALBuffer::WALBuffer(size_t capacity)
        :buffer(capacity),write_pos_(0)
{

}

bool WALBuffer::append(const char* data, size_t len)
{
    if (len > available())
    {
        return false;
    }

    memcpy(buffer.data() + write_pos_,data,len);
    write_pos_ += len;
    return true;

}

size_t WALBuffer::available() const
{
    return buffer.size() - write_pos_;
}
//[1字节 Op] + [4字节 klen] + [klen字节 Key] + [4字节 vlen] + [vlen字节 Value]
bool WALBuffer::appendInsert(const std::string& key, const std::string& value)
{
    auto klen = static_cast<uint32_t>(key.size());
    auto vlen = static_cast<uint32_t>(value.size());

    size_t totallen = OP_SIZE + LEN_SIZE + klen + LEN_SIZE + vlen;

    if (totallen > available())
    {
        return false;
    }

    auto op = static_cast<uint8_t>(WalOpType::INSERT);
    append(reinterpret_cast<const char*>(&op),OP_SIZE);

    //转换为大端序
    uint32_t net_klen = htonl(klen);
    append(reinterpret_cast<const char*>(&net_klen),sizeof(net_klen));
    append(key.data(), klen);

    uint32_t net_vlen = htonl(vlen);
    append(reinterpret_cast<const char*>(&net_vlen),sizeof(net_vlen));
    append(value.data(), vlen);

    return true;

}
//[1字节 Op] + [4字节 klen] + [klen字节 Key]
bool WALBuffer::appendDelete(const std::string& key)
{
    auto klen = static_cast<uint32_t>(key.size());

    size_t totallen = OP_SIZE + LEN_SIZE + klen ;

    if (totallen > available())
    {
        return false;
    }

    auto op = static_cast<uint8_t>(WalOpType::DELETE);
    append(reinterpret_cast<const char*>(&op),OP_SIZE);

    //转换为大端序
    uint32_t net_klen = htonl(klen);
    append(reinterpret_cast<const char*>(&net_klen),sizeof(net_klen));
    append(key.data(), klen);

    return true;

}

void WALBuffer::reset()
{
    write_pos_ = 0;
}





