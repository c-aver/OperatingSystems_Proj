#include <thread>
#include <vector>
#include <poll.h>
#include <algorithm>
#include <unistd.h>
#include <iostream>
#include <variant>
#include <vector>

#include "reactor.hpp"

// #define DEBUG

/**
 * @brief Constructor for the Reactor class
 * sets running to false, thread to nullptr and fds_count to 0
 */
Reactor::Reactor() : running(false), thread(nullptr), fds_count(0) {}

Reactor::~Reactor()
{
    this->stop();
}

/**
 * @brief Adds a file descriptor to the reactor
 * @param fd The file descriptor to add
 * @param handler The handler to call when an event occurs on the file descriptor
 */
bool Reactor::add_fd(int fd, Handler handler)
{
    std::lock_guard<std::mutex> vectors_guard(vectors_mutex); // Lock the vectors, will be unlocked when the scope ends

    // Check if the file descriptor is already in the reactor
    if (pfds.end() != std::find_if(pfds.begin(), pfds.end(), [fd](struct pollfd pfd)
                                   { return pfd.fd == fd; }))
    {
        return false;
    }
    this->pfds.push_back((struct pollfd){
        .fd = fd,
        .events = POLLIN,
        .revents = 0});
    this->handlers.push_back(handler);
    this->fds_count += 1;
    return true;
}

bool Reactor::remove_fd(int fd)
{
#ifdef DEBUG
    std::cout << "Removing fd\n";
    std::cout << "fds_count: " << fds_count << std::endl;
    std::cout << "pfds.size: " << pfds.size() << std::endl;
#endif
    std::lock_guard<std::mutex> vectors_guard(vectors_mutex); // Lock the vectors, will be unlocked when the scope ends
    for (size_t i = 0; i < fds_count; ++i)
    {
#ifdef DEBUG
        std::cout << "Checking fd: " << pfds[i].fd << " VS " << fd << std::endl;
#endif
        if (pfds[i].fd == fd) // If the file descriptor is found
        {
#ifdef DEBUG
            std::cout << "Found!\n";
#endif
            pfds.erase(pfds.begin() + i);
            handlers.erase(handlers.begin() + i);
            fds_count -= 1;
            return true; // Return true if the file descriptor was removed
        }
    }
    return false; // Return false if the file descriptor was not found
}

void Reactor::start()
{
    stop(); // Stop the reactor if it is already running
    {
        std::lock_guard<std::mutex> running_guard(running_mutex); // Lock the running mutex, will be unlocked when the scope ends
        running = true;                                           // Set running to true
    }
    thread = new std::thread(&Reactor::reactor_main, this); // Create a new thread with the reactor_main function
}

void Reactor::stop()
{
    if (!thread) // If the thread is not running, exit
        return;
    {
        std::lock_guard<std::mutex> running_guard(running_mutex); // Lock the running mutex, will be unlocked when the scope ends
        running = false;                                          // Set running to false, will be checked in the reactor_main function
    }
    thread->join(); // Join the thread, will wait for the thread to finish
    delete thread;
    thread = nullptr;
    {
        std::lock_guard<std::mutex> vectors_guard(vectors_mutex); // Lock the vectors, will be unlocked when the scope ends
        handlers.clear();
        pfds.clear();
        fds_count = 0;
    }
}

struct handler_action
{
    Reactor::Handler handler;
    int param;
    void operator()()
    {
        handler(param);
    }
};

struct remove_action
{
    Reactor *reactor;
    int param;
    void operator()()
    {
        reactor->remove_fd(param);
    }
};

void Reactor::reactor_main()
{
    bool still_running;
    {
        std::lock_guard<std::mutex> running_guard(running_mutex); // Lock the running mutex, will be unlocked when the scope ends
        still_running = running;
    }
    // Reactor main loop
    while (still_running)
    {
        std::vector<std::variant<handler_action, remove_action>> actions; // Actions to perform after the poll, each is either a handler or removal of an fd
        {
            std::lock_guard<std::mutex> vectors_guard(vectors_mutex);
            poll(pfds.data(), pfds.size(), 0);
            for (size_t i = 0; i < this->fds_count; ++i)
            {
                if (pfds[i].revents & POLLIN) // Check if the file descriptor has information available for reading
                {
                    actions.push_back(handler_action{handlers[i], pfds[i].fd});
                }
                else if (pfds[i].revents & POLLNVAL)
                {
                    actions.push_back(remove_action{this, pfds[i].fd});
                }
            }
        }

        // Iterate over the actions and perform them
        for (auto &action : actions)
        {
            // Call the function object stored in the variant
            // std::visit will call the () operator of the function object
            std::visit([](auto &arg)
                       { arg(); }, action);
        }

        {
            std::lock_guard<std::mutex> running_guard(running_mutex);
            still_running = running; // If stop was called by another thread, running will be false
        }
    }
}
