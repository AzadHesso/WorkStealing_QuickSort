#pragma once
#include <thread>
#include <vector>
#include "BlockedQueue.h"
#include <queue>
#include <future>
#include <condition_variable>

using namespace std;

class ThreadPool
{
public:
    ThreadPool();

    void start();

    void stop();

    res_type push_task(FuncType func, int* arr, long arg1, long arg2);

    void threadFunc(int qindex);

private:
    // структура, объединяющая задачу для выполнения и промис
    struct TaskWithPromise {
        task_type task;
        promise<void> prom;
    };
    mutex m_locker;

    int m_thread_count;

    vector<thread> m_threads;
    
    vector<BlockedQueue<TaskWithPromise>> m_thread_queues;

    unsigned m_index;
};