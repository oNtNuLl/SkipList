//
// Created by w on 2026/11/11.
//

#include "WALIO.h"

WALIO::WALIO(string file_name)
    :file_name(file_name)
{
    write_file = ofstream(file_name,ios::binary|ios::app);
}

WALIO::~WALIO()
{
    write_file.close();
    read_file.close();
}

bool WALIO::write(const char* data, size_t len)
{
    write_file.write(data, len);
    write_file.flush();
    return true;
}

bool WALIO::open_for_read(string file_path)
{
    if (file_path.empty()) file_path = file_name;
    read_file = ifstream(file_path, ios::binary);
    return read_file.is_open();
}

bool WALIO::read_record(WalOpType& op, string& key, string& value)
{

    if (!read_file.read((char*)&op, OP_SIZE))
        return false;

    uint32_t net_key_len = 0;
    if (!read_file.read(reinterpret_cast<char*>(&net_key_len),LEN_SIZE))
        return false;

    uint32_t key_len = ntohl(net_key_len);
    key.resize(key_len);
    if (!read_file.read(&key[0],key_len))
        return false;

    if (op == WalOpType::INSERT)
    {
        uint32_t net_val_len = 0;
        if (!read_file.read(reinterpret_cast<char*>(&net_val_len),LEN_SIZE))
            return false;

        uint32_t val_len = ntohl(net_val_len);
        value.resize(val_len);
        if (!read_file.read(&value[0],val_len))
            return false;

        return true;
    }

    value.clear();
    return true;

}

void WALIO::read_close()
{
    read_file.close();
}
