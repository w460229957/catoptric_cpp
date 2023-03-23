#pragma once
#include <mutex>
#include <condition_variable>
#include <streambuf>
#include <climits>


//Define a counting semaphore class
//Disable inheritance and copy/move constructors(implicitly deleted)
///@author Zhengyuan Zhang
class CountingSemaphore final{
public:
    explicit CountingSemaphore(int count = 0) : count(count){}

    void wait() {
        //Unique lock is used to ensure that the mutex is unlocked when the thread is suspended
        //wait() will unlock the mutex and suspend the thread until the condition variable is notified
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock, [this]{return count > 0;});//Lambda function takes this pointer as an argument
        --count;
    }

    void signal() {
        std::unique_lock<std::mutex> lock(mutex);
        ++count;
        cv.notify_one();
    }

    void signal_all() {
        //Need to make sure this is the only thread that is modifying the count variable
        std::unique_lock<std::mutex> lock(mutex);
        count = INT_MAX;
        //Set count to INT_MAX to ensure that all threads that are not in the critial section will not be blocked
        cv.notify_all();
    }

private:
    std::mutex mutex;
    std::condition_variable cv;
    int count;
};