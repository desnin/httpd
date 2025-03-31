#include <iostream>
#include <chrono> // 包含C++标准库中的chrono头文件，用于处理时间和日期
#include <thread>
#include <vector>
#include "threadpool.h"
using namespace std;
// 定义一个名为add的函数，用于计算两个整数的和
int add(int a , int b){
    // 使用this_thread::sleep_for函数使当前线程休眠1秒钟
    // this_thread::sleep_for是C++标准库中的一个函数，用于暂停当前线程的执行
    // chrono::seconds(1)表示休眠时间为1秒
    this_thread::sleep_for(chrono::seconds(1));
    // 返回两个整数的和
    // a和b是函数的参数，表示要相加的两个整数
    return a+ b;
}

// 定义一个名为multiply的函数，用于计算两个整数的乘积
int multiply(int a , int b){
    // 使用this_thread::sleep_for函数使当前线程休眠1秒钟
    // 这通常用于模拟耗时操作，例如网络请求或复杂计算
    this_thread::sleep_for(chrono::seconds(1));
    // 返回两个整数的乘积
    return a*b;
}

int main(){
    // 创建一个包含4个线程的线程池
    ThreadPool pool(4);
    //提交加法任务到线程沉
    future<int> result1 = pool.enqueue(add, 10,20);
    future<int> result2 = pool.enqueue(multiply ,30,2);
    future<int> result3 = pool.enqueue(multiply ,30,5);
    future<int> result3 = pool.enqueue(multiply ,40,5);
    future<int> 4 = pool.enqueue(multiply ,30,5);
    cout << "在线程运行的过程中还有运行其他程序。。。。" << endl;
    // 获取任务的结果
    std::cout << "Result of add: " << result1.get() << std::endl;
    std::cout << "Result of multiply: " << result2.get() << std::endl;

    // 等待线程池中的线程全部完成
    return 0;
}