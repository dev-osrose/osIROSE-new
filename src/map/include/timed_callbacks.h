#pragma once

#include <functional>
#include <mutex>
#include <list>
#include <chrono>

#include "fire_once.h"
#include "network_thread_pool.h"

class TimedCallbacks {
    private:
        using Timer = boost::asio::steady_timer;
        using error_code = boost::system::error_code;

    public:
        struct Wrapper {
            Wrapper(std::weak_ptr<Timer> timer) : timer(timer) {}

            void cancel() {
                if (auto tmp = timer.lock()) {
                    tmp->cancel();
                }
            }
            private:
                std::weak_ptr<Timer> timer;
        };

        template <class Rep, class Period, class Func>
        Wrapper add_callback(const std::chrono::duration<Rep, Period>& timeout, Func&& callback) {
            static_assert(std::is_invocable_v<Func>, "timer functions should be void(*)()");
            auto instance = Core::NetworkThreadPool::GetInstance();
            auto *io_context = instance.Get_IO_Service();
            auto t = std::make_shared<Timer>(*io_context, timeout);
            std::lock_guard<std::mutex> guard(mutex);
            t.async_wait([this, pos = timers.begin(), callback = std::forward<Func>(callback)](const error_code& error) mutable {
                if (error != boost::asio::error::operation_aborted) {
                    std::invoke(std::forward<Func>(callback));
                    std::lock_guard<std::mutex> guard(mutex);
                    timers.erase(pos);
                }
            });
            timers.push_front(t);
            return t;
        }
    
        template <class Rep, class Period, class Func>
        Wrapper add_recurrent_callback(const std::chrono::duration<Rep, Period>& timeout, Func&& callback) {
            static_assert(std::is_invocable_v<Func>, "timer functions should be void(*)()");
            auto instance = Core::NetworkThreadPool::GetInstance();
            auto *io_context = instance.Get_IO_Service();
            auto t = std::make_shared<Timer>(*io_context, timeout);
            t.async_wait(std::bind(&TimedCallbacks::recurrent_task, std::placeholders::_1, t.get(), timeout, std::forward<Func>(callback)));
            std::lock_guard<std::mutex> guard(mutex);
            timers.push_front(t);
            return t;
        }
    
    private:
        template <class Rep, class Period, class Func>
        static void recurrent_task(const error_code& error, Timer* t, const std::chrono::duration<Rep, Period>& timeout, Func&& callback) {
            if (error != boost::asio::error::operation_aborted) {
                std::invoke(callback);
                t->expires_at(t->expiry() + timeout);
                t->async_wait(std::bind(&TimedCallbacks::recurrent_task, std::placeholders::_1, t, timeout, std::forward<Func>(callback)));
            }
        }

        std::mutex mutex;
        std::vector<std::shared_ptr<Timer>> timers;
};
