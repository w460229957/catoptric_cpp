#pragma once
#include <queue>
#include <utility>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <iostream>
#include <functional>

struct Command
{
    int rowNum;
    int mirrorID;
    int whichMotor;
    int direction;
    int step;
    Command(int r, int m, int w, int d, int s) : rowNum(r), mirrorID(m), whichMotor(w), direction(d), step(s){};
};

/***
 *@author: ZhengYuan Zhang
 *@email: zhang.zhengyuan@wustl.edu
 *@brief: A thread-safe working thread queue for storing commands
 */

template <typename T>
class CommandQueue
{
public:
    CommandQueue() = default;
    ~CommandQueue() = default;

    CommandQueue(const CommandQueue &other)
    {
        std::lock_guard<std::mutex> lock(other.queueLock, this->lock);
        _mQueue = other._mQueue;
    }

    CommandQueue &operator=(const CommandQueue &) = delete;

    auto push(T const &command) -> void
    {
        std::unique_lock<std::mutex> lock(queueLock);
        _mQueue.emplace(command);
        lock.unlock();
        std::atomic_thread_fence(std::memory_order_acquire);
        holdVar.notify_one();
    }

    auto pop() -> std::shared_ptr<T>
    {
        std::lock_guard<std::mutex> lock(queueLock);
        if (_mQueue.empty())
        {
            return nullptr;
        }
        auto command = std::make_shared<T>(_mQueue.front());
        _mQueue.pop();
        return command;
    }

    auto wait() -> void
    {
        std::unique_lock<std::mutex> lock(queueLock);
        holdVar.wait(lock, [this]{return !this->isEmpty();});
    }

    auto wait_and_pop() -> std::shared_ptr<T>
    {
        std::unique_lock<std::mutex> lock(queueLock);
        holdVar.wait(lock, [this]{return !this->isEmpty();});
        auto command = std::make_shared<T>(_mQueue.front());
        _mQueue.pop();
        return command;
    }

    auto top() -> std::shared_ptr<T>
    {
        std::lock_guard<std::mutex> lock(queueLock);
        if (_mQueue.empty())
        {
            return nullptr;
        }
        auto command = std::make_shared<T>(_mQueue.front());
        return command;
    }

    bool isEmpty() const
    {
        return _mQueue.empty();
    }

    int size() const
    {
        return _mQueue.size();
    }

private:
    std::atomic<int> waitCount;    
    std::queue<T> _mQueue;
    std::mutex queueLock;
    std::condition_variable holdVar;
};