#pragma once

#include <queue>
#include <tuple>
#include <future>
#include <thread>
#include <mutex>
#include <condition_variable>

constexpr std::chrono::duration<signed long, std::milli> ACTIVE_OBJECT_WAIT_TIMEOUT = std::chrono::milliseconds(10);

template <typename R, typename... Args>
class ActiveObject;

template <typename R, typename... Args>
class ActiveObject<R(Args...)>
{
private:
    using method = std::function<R(Args...)>;
    struct MethodObject
    {
        std::promise<R> promise;
        std::tuple<Args...> args;
        void dispatch(method &func)
        {
            promise.set_value(std::apply(func, args));
        }
        MethodObject(std::tuple<Args...> args) : args(args) {}
    };
    std::thread dispatcher_thread;

    std::queue<MethodObject> queue;
    std::mutex queue_mutex;
    std::condition_variable queue_cond;

    method func;

    volatile bool done;

    void processing_loop()
    {
        while (!done)
        {
            std::unique_lock<std::mutex> queue_lock(queue_mutex);
            if (queue.empty())
                queue_cond.wait_for(queue_lock, ACTIVE_OBJECT_WAIT_TIMEOUT, [this]
                                    { return !queue.empty(); }); // wait until there is something to pop
            if (queue.empty())
                continue; // if timed out, check again if done
            MethodObject obj = std::move(queue.front());
            queue.pop();
            queue_lock.unlock();
            obj.dispatch(func);
        }
    }

public:
    ActiveObject(method f)
    {
        done = false;
        func = f;
        dispatcher_thread = std::thread(&ActiveObject::processing_loop, this);
    }

    ~ActiveObject()
    {
        done = true;
        dispatcher_thread.join();
    }

    std::future<R> invoke(Args... args)
    {
        std::unique_lock<std::mutex> queue_lock(queue_mutex);
        queue.emplace(std::make_tuple(args...));
        queue_cond.notify_all();
        return queue.back().promise.get_future();
    }

    std::future<R> operator()(Args... args)
    {
        return invoke(args...);
    }
};

template <typename... Args>
class ActiveObject<void(Args...)>
{
private:
    using method = std::function<void(Args...)>;
    struct MethodObject
    {
        std::promise<void> promise;
        std::tuple<Args...> args;
        void dispatch(method &func)
        {
            std::apply(func, args);
        }
        MethodObject(std::tuple<Args...> args) : args(args) {}
    };
    std::thread dispatcher_thread;

    std::queue<MethodObject> queue;
    std::mutex queue_mutex;
    std::condition_variable queue_cond;

    method func;

    volatile bool done;

    void processing_loop()
    {
        while (!done)
        {
            std::unique_lock<std::mutex> queue_lock(queue_mutex);
            if (queue.empty())
                queue_cond.wait_for(queue_lock, ACTIVE_OBJECT_WAIT_TIMEOUT, [this]
                                    { return !queue.empty(); }); // wait until there is something to pop
            if (queue.empty())
                continue; // if timed out, check again if done
            MethodObject obj = std::move(queue.front());
            queue.pop();
            queue_lock.unlock();
            obj.dispatch(func);
        }
    }

public:
    ActiveObject(method f)
    {
        done = false;
        func = f;
        dispatcher_thread = std::thread(&ActiveObject::processing_loop, this);
    }

    ~ActiveObject()
    {
        done = true;
        dispatcher_thread.join();
    }

    std::future<void> invoke(Args... args)
    {
        std::unique_lock<std::mutex> queue_lock(queue_mutex);
        queue.emplace(std::make_tuple(args...));
        queue_cond.notify_all();
        return queue.back().promise.get_future();
    }

    std::future<void> operator()(Args... args)
    {
        return invoke(args...);
    }
};
