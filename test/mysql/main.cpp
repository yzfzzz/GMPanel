#include <iostream>
#include <memory>
#include <thread>
#include "ConnectionPool.h"
#include "MysqlConn.h"
/*
1.单线程: 使用/不使用连接池
2.多线程: 使用/不使用连接池
*/

void no_pool(int begin, int end) {
    for (int i = begin; i < end; i++) {
        MysqlConn conn;
        conn.connect("debian-sys-maint", "SQ0CTalwPPoAhCnh", "data",
                     "host.docker.internal");
        char sql[1024] = {0};
        sprintf(sql, "insert into score values(%d,'Mike')", i);
        conn.update(sql);
    }
}

void use_pool(ConnectionPool* pool, int begin, int end) {
    for (int i = begin; i < end; i++) {
        std::shared_ptr<MysqlConn> conn_ptr = pool->getConnection();
        char sql[1024] = {0};
        sprintf(sql, "insert into score values(%d,'Mike')", i);
        conn_ptr->update(sql);
    }
}

// 单线程: 使用/不使用连接池
void test1() {
#if 1
    std::chrono::steady_clock::time_point start =
        std::chrono::steady_clock::now();
    no_pool(0, 5000);
    std::chrono::steady_clock::time_point end =
        std::chrono::steady_clock::now();
    auto len = end - start;

    std::cout << "[1] one pthread no pool: " << len.count() << " ns"
              << " " << len.count() / 1000000 << " ms" << std::endl;
    std::cout << "------------------------------" << std::endl;
    // [1] one pthread no pool: 36289870044 ns 36289 ms
#else
    ConnectionPool* pool = ConnectionPool::getConnectPool();
    std::chrono::steady_clock::time_point start =
        std::chrono::steady_clock::now();
    use_pool(pool, 0, 5000);
    std::chrono::steady_clock::time_point end =
        std::chrono::steady_clock::now();
    auto len = end - start;

    std::cout << "[2] one pthread use pool: " << len.count() << " ns"
              << " " << len.count() / 1000000 << " ms" << std::endl;
    // [2] one pthread use pool: 19991372806 ns 19991 ms
#endif
}

// 2.多线程: 使用/不使用连接池
void test2() {
#if 0
    MysqlConn conn;
    conn.connect("debian-sys-maint", "uUakoE1X8Ei6TqLJ", "data",
                     "host.docker.internal");
    std::chrono::steady_clock::time_point start =
        std::chrono::steady_clock::now();
    std::thread t1(no_pool, 0, 1000);
    std::thread t2(no_pool, 1000, 2000);
    std::thread t3(no_pool, 2000, 3000);
    std::thread t4(no_pool, 3000, 4000);
    std::thread t5(no_pool, 4000, 5000);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    std::chrono::steady_clock::time_point end =
        std::chrono::steady_clock::now();
    auto len = end - start;

    std::cout << "[3] muti pthread no pool: " << len.count() << " ns"
              << " " << len.count() / 1000000 << " ms" << std::endl;
    // [3] muti pthread no pool: 14109221266 ns 14109 ms

#else

    ConnectionPool* pool = ConnectionPool::getConnectPool();
    std::chrono::steady_clock::time_point start =
        std::chrono::steady_clock::now();
    std::thread t1(use_pool, pool, 0, 1000);
    std::thread t2(use_pool, pool, 1000, 2000);
    std::thread t3(use_pool, pool, 2000, 3000);
    std::thread t4(use_pool, pool, 3000, 4000);
    std::thread t5(use_pool, pool, 4000, 5000);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    std::chrono::steady_clock::time_point end =
        std::chrono::steady_clock::now();
    auto len = end - start;

    std::cout << "[4] muti pthread use pool: " << len.count() << " ns"
              << " " << len.count() / 1000000 << " ms" << std::endl;
    // [4] muti pthread use pool: 7520603022 ns 7520 ms

#endif
}

int main() {
    test1();
    return 0;
}
