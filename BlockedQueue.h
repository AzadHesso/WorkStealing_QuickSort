#pragma once
#include <mutex>
#include <queue>
#include <condition_variable>
#include <functional>
#include <future>

using namespace std;

typedef function<void()> task_type;

// будет символизировать тип результата метода push_task
typedef future<void> res_type;

typedef void (*FuncType) (int* array, long left, long right);

template<class T>
class BlockedQueue 
{
public:
    void push(T& item)
    {
        lock_guard<mutex> l(m_locker);

        m_task_queue.push(std::move(item));

        m_event_holder.notify_one();
    }

    void pop(T& item)
    {
        unique_lock<mutex> l(m_locker);

        if (m_task_queue.empty()) {
            m_event_holder.wait(l, [this] {return !m_task_queue.empty(); });
        }

        item = std::move(m_task_queue.front());
        m_task_queue.pop();
    }

    bool fast_pop(T& item)
    {
        lock_guard<mutex> l(m_locker);

        if (m_task_queue.empty()) {
            return false;
        }

        item = std::move(m_task_queue.front());
        m_task_queue.pop();

        return true;
    }

private:
    mutex m_locker;

    queue<T> m_task_queue;

    condition_variable m_event_holder;
};