#pragma once

#include <functional>
#include <thread>
#include <mutex>
#include <vector>
#include <condition_variable>
#include <chrono>
#include <future>
#include <algorithm>

#include "fire_once.h"

class TimedCallbacks {
    public:
        ~TimedCallbacks() {
            cv.notify_all();
            std::lock_guard<std::mutex> lock(mutex);
            for (auto& it : callbacks) {
                it.thread.join();
            }
        }
        
        template <class Rep, class Period, class Func>
        void add_callback(const std::chrono::duration<Rep, Period>& timeout, Func&& callback) {
            static_assert(std::is_invocable_v<Func>, "error should be void(*)()");
            std::lock_guard<std::mutex> lock(mutex);
            // first we remove dead tasks from the vector
            std::remove_if(callbacks.begin(), callbacks.end(), [](auto& callback) {
                if (callback.future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                    callback.thread.join();
                    return true;
                }
                return false;
            });
            // we then add the callback
            std::promise<void> promise;
            std::future<void> future = promise.get_future();
            std::thread thread([this, timeout, callback = std::move(callback), promise = std::move(promise)]() mutable {
                std::unique_lock<std::mutex> lock(mutex);
                if (cv.wait_for(lock, timeout) == std::cv_status::timeout) {
                    std::invoke(callback);
                }
                promise.set_value_at_thread_exit();
            });
            callbacks.emplace_back(std::move(thread), std::move(future));
        }
    
    private:
        struct Callback {
            Callback(std::thread&& thread, std::future<void>&& future) : thread(std::move(thread)), future(std::move(future)) {}
            std::thread thread;
            std::future<void> future;
        };
        
        std::condition_variable cv;
        std::mutex mutex;
        std::vector<Callback> callbacks;
};
