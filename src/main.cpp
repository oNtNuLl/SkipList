//
// Created by w on 2026/3/15.
//

#include <chrono>
#include <iostream>
#include <random>

#include "../include/SkipList.h"


int main()
{
    srand(static_cast<unsigned>(time(nullptr)));

    // SkipList<int, std::string> my_db(12);
    //
    //
    //
    // for (int i = 0; i < 10; i++) {
    //
    //     int random_key = rand() % 10;
    //
    //
    //     std::string val = "Data_" + std::to_string(random_key);
    //
    //     my_db.insert(random_key, val);
    // }
    //
    //
    // my_db.display_list();
    //
    // int test_keys[] = {5,6,7,8,9,10};
    //
    // for(int i = 0; i < 5; i++) {
    //     if(my_db.search(test_keys[i])) {
    //         std::cout << " 成功秒杀: " << test_keys[i] << std::endl;
    //     } else {
    //         std::cout << " 库中没有: " << test_keys[i] << std::endl;
    //     }
    // }
    // my_db.remove(1);
    // my_db.remove(3);
    // my_db.display_list();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 10000000);
    SkipList<int, int> my_db(16); //
    int test_count = 1000000;     //

    std::cout << "插入 " << test_count << " 条乱序数据..." << std::endl;

    // 1. 记录开始时间
    auto start = std::chrono::high_resolution_clock::now();


    for (int i = 0; i < test_count; i++) {
        int random_key = distrib(gen);
        my_db.insert(random_key, random_key * 10);
    }


    auto end = std::chrono::high_resolution_clock::now();


    std::chrono::duration<double, std::milli> elapsed = end - start;
    double seconds = elapsed.count() / 1000.0;



    std::cout << "写入完成！" << std::endl;
    std::cout << " 总耗时: " << seconds << " 秒" << std::endl;
    std::cout << " QPS: " << static_cast<int>(test_count / seconds) << " 次操作/秒" << std::endl;

}

