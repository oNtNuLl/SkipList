//
// Created by w on 2026/11/11.
//

#ifndef SSKIPKV_WALBUFFER_H
#define SSKIPKV_WALBUFFER_H
#include <vector>
#include <cstddef>
#include <string>
#include <arpa/inet.h>

// 单条记录类型
enum class WalOpType : uint8_t {
    INSERT = 1,
    DELETE = 2
};

static constexpr size_t OP_SIZE = sizeof(uint8_t);
static constexpr size_t LEN_SIZE = sizeof(uint32_t);

class WALBuffer
{
public:
    WALBuffer(size_t capacity = 4 * 1024 * 1024);
    //追加一条

    // 剩余空间
    size_t available() const;

    const char * data() const { return buffer.data();}

    size_t size() const { return write_pos_;}

    bool appendInsert(const std::string& key, const std::string& value);

    bool appendDelete(const std::string& key);

    //重置索引
    void reset();

private:
    bool append(const char * data,size_t len);
    std::vector<char> buffer;
    size_t write_pos_ = 0;
};


#endif //SSKIPKV_WALBUFFER_H