#pragma once
#include <queue>
#include <utility>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <thread>
#include <type_traits>
#include <iostream>

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
        std::lock_guard<std::mutex> lock(other.queueLock);
        _mQueue = other._mQueue;
    }

    CommandQueue(CommandQueue && other){
        std::lock_guard<std::mutex> lock(other.queueLock);
        _mQueue = std::move(other._mQueue);
    }

    CommandQueue& operator=(const CommandQueue & other){
        //Need to lock both mutexes to avoid deadlock at the same time
        //Do not lock each mutex separately, as it may cause deadlock
        std::lock_guard<std::mutex> lock(queueLock, other.queueLock);
        _mQueue = other._mQueue;
        return *this;
    }

    CommandQueue& operator=(CommandQueue && other){
        std::lock_guard<std::mutex> lock(queueLock, other.queueLock);
        _mQueue = std::move(other._mQueue);
        return *this;
    }


    void push(T const &command)
    {
        std::lock_guard<std::mutex> lock(queueLock);
        _mQueue.push(command);
        holdVar.notify_one();
        return;
    }

    // motor direction 
    // 0        0        TILT UP
    // 0        1        TILT DOWN
    // 1        0        PAN DOWN(right)
    // 1        1        PAN UP(left)
    std::shared_ptr<T> pop()
    {
        std::lock_guard<std::mutex> lock(queueLock);
        if (_mQueue.empty())
        {
            return nullptr;
        }
        std::shared_ptr<T> command{std::make_shared<T>(_mQueue.front())};
        _mQueue.pop();
        return command;
    }

    std::shared_ptr<T> waitAndPop()
    {
        std::unique_lock<std::mutex> lock(queueLock);
        holdVar.wait(lock, [this]
                     { return !_mQueue.empty(); });
        std::shared_ptr<T> command{std::make_shared<T>(_mQueue.front())};
        _mQueue.pop();
        return command;
    }

    std::shared_ptr<T> top()
    {
        std::lock_guard<std::mutex> lock(queueLock);
        if (_mQueue.empty())
        {
            return nullptr;
        }
        std::shared_ptr<T> command{std::make_shared<T>(_mQueue.front())};
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
    std::queue<T> _mQueue;
    std::mutex queueLock;
    std::condition_variable holdVar;
};