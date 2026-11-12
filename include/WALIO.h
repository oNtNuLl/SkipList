//
// Created by w on 2026/11/11.
//

#ifndef SKIPKV_WALIO_H
#define SKIPKV_WALIO_H

#include <fstream>
#include <iostream>

#include "WALBuffer.h"
using namespace std;
class WALIO
{
public:
    WALIO(string file_name);
    ~WALIO();

    bool write(const char* data,size_t len);

    bool open_for_read(string file_path);
    bool read_record(WalOpType& op, string& key, string& value);
    void read_close();

private:
    string file_name;
    ofstream write_file;
    ifstream read_file;
};


#endif //SKIPKV_WALIO_H