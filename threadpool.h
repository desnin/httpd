#pragma once // 确保该头文件只被包含一次，防止重复定义
#ifndef THREADPOOL_H // 如果未定义THREADPOOL_H
#define THREADPOOL_H // 定义THREADPOOL_H，防止重复包含
#include <vector> // 包含向量库，用于存储线程
#include <queue> // 包含队列库，用于任务队列
#include <memory> // 包含智能指针库，用于管理动态分配的内存
#include <thread> // 包含线程库，用于创建和管理线程
#include <mutex> // 包含互斥锁库，用于线程同步
#include <condition_variable> // 包含条件变量库，用于线程间的条件同步
#include <future> // 包含未来库，用于异步任务和结果获取
#include <functional> // 包含函数库，用于存储可调用对象
#include <stdexcept> // 包含标准异常库，用于异常处理


class ThreadPool
{
public:
 
    // 构造函数，初始化线程池，参数为线程数量
    ThreadPool(size_t);
  
    // 析构函数，用于销毁线程池
    ~ThreadPool();

    // 模板函数，用于将任务加入线程池队列
    // F 为函数类型，Args 为函数参数类型
    // 返回值为 std::future，用于获取任务执行结果
    template<typename F, class... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;
private:

    // 存储线程的容器
    std::vector<std::thread> workers;

    // 存储任务的队列，任务类型为 std::function<void()>
    std::queue<std::function<void()>> tasks;

    // 互斥锁，用于保护任务队列的线程安全
    std::mutex tasks_mutex;

    // 条件变量，用于线程间的同步
    std::condition_variable condition;
    // 标志位，用于控制线程池的停止
    bool stop;
};

// 定义ThreadPool类的构造函数，参数threads指定线程池中的线程数量
inline ThreadPool::ThreadPool(size_t threads) :
    stop(false) // 初始化stop标志为false，表示线程池未停止
{
    // 循环创建指定数量的工作线程
    for(size_t i = 0; i < threads; i++)
    {
        // 将新创建的线程添加到workers容器中
        workers.emplace_back(
                    [this] // 捕获当前ThreadPool对象的this指针
            {
                // 无限循环，直到线程池被停止且任务队列为空
                for(;;)
                {
                    std::function<void()> task; // 定义一个任务变量
                    {
                        // 创建一个unique_lock对象，用于锁定tasks_mutex互斥量
                        std::unique_lock<std::mutex> lock(this->tasks_mutex);
                        // 等待条件变量condition，直到stop为true或tasks队列不为空
                        this->condition.wait(lock,
                                             [this]{ return this->stop || !this->tasks.empty(); });
                        // 如果线程池已停止且任务队列为空，则退出线程
                        if(this->stop && this->tasks.empty())
                        {
                            return;
                        }
                        // 从任务队列中取出一个任务
                        task = std::move(this->tasks.front());
                        // 移除任务队列中的第一个任务
                        this->tasks.pop();
                    }
                    // 执行取出的任务
                    task();
                }
            }
        );
    }
}

inline ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(tasks_mutex);
        stop = true;
    }
    condition.notify_all();
    for(std::thread & worker : workers)
    {
        worker.join();
    }
}

template <typename F, typename... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;

    //std::forward参数完美转发: https://zhuanlan.zhihu.com/p/92486757
    auto task = std::make_shared<std::packaged_task<return_type()>>(
                            std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(tasks_mutex);
        //停止池后不允许排队
        if(stop)
        {
            throw std::runtime_error("enqueue on stopped ThreadPool!");
        }
        tasks.emplace([task](){ (*task)(); });
    }
    condition.notify_one();
    return res;
}

#endif // THREADPOOL_H
