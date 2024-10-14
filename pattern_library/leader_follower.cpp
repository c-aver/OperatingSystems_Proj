#include <thread>
#include <vector>
#include <poll.h>
#include <algorithm>
#include <unistd.h>
#include <iostream>
#include <variant>

#include "leader_follower.hpp"

#define DEBUG

LeaderFollower::LeaderFollower(size_t pool_size) : running(true), has_leader(false), fds_count(0)
{
    for (size_t i = 0; i < pool_size; ++i)
    {
        pool.push_back(std::thread(&LeaderFollower::follow, this));
    }
}

LeaderFollower::~LeaderFollower()
{
    running = false;            // set to false so all threads know to die
    has_leader = false;         // so that all followers will actually wake up
    leader_cond.notify_all();   // wake up all the followers
    for (std::thread &t : pool) // wait for all threads to finish up
    {
        t.join();
    }
}

bool LeaderFollower::add_fd(int fd, Handler handler)
{
    std::lock_guard<std::mutex> vectors_guard(vectors_mutex);
#ifdef DEBUG
    std::cout << "Adding fd: " << fd << "\n";
    std::cout << "Current fds_count: " << fds_count << std::endl;
#endif
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

bool LeaderFollower::remove_fd(int fd)
{
#ifdef DEBUG
    std::cout << "Removing fd " << fd << "\n";
    std::cout << "Current fds_count: " << fds_count << std::endl;
#endif
    std::lock_guard<std::mutex> vectors_guard(vectors_mutex);
    for (size_t i = 0; i < fds_count; ++i)
    {
#ifdef DEBUG
        std::cout << "Checking fd: " << pfds[i].fd << " VS " << fd << std::endl;
#endif
        if (pfds[i].fd == fd)
        {
#ifdef DEBUG
            std::cout << "Found!\n";
#endif
            pfds.erase(pfds.begin() + i);
            handlers.erase(handlers.begin() + i);
            fds_count -= 1;
            return true;
        }
    }
    return false;
}

void LeaderFollower::follow()
{
    std::unique_lock<std::mutex> leader_lock(leader_mutex); // lock the mutex to check if a leader exists
#ifdef DEBUG
    std::cout << "Thread " << std::this_thread::get_id() << " is following." << std::endl;
#endif
    while (running) // infinite loop to conitnuously try to be the leader (shows initiative)
    {
        if (has_leader) // if there is a leader already
        {
#ifdef DEBUG
            std::cout << "Thread " << std::this_thread::get_id() << " is waiting." << std::endl;
#endif
            leader_cond.wait(leader_lock, [this]       // wait for it to wake you up
                             { return !has_leader; }); // make sure there is actually no leader
        }
        if (!running)
            return;
        has_leader = true;    // you are now the leader
        leader_lock.unlock(); // unlock the mutex so others can follow while you lead
        lead();               // perform the leaders duties // TODO: handle errors by dying?
        leader_lock.lock();   // after you are done, lock the mutex to check again
    }
}

void LeaderFollower::promote_leader()
{
#ifdef DEBUG
    std::cout << "Thread " << std::this_thread::get_id() << " wants to promote a new leader." << std::endl;
#endif
    std::unique_lock<std::mutex> leader_lock(leader_mutex); // lock the mutex to set has_leader
    has_leader = false;                                     // set to false so others know there is a vacancy
    leader_cond.notify_one();                               // wake up one of them
}

void LeaderFollower::lead()
{
#ifdef DEBUG
    std::cout << "Thread " << std::this_thread::get_id() << " is leading." << std::endl;
#endif
    while (running) // make sure the pool is still running
    {
        std::unique_lock<std::mutex> vectors_lock(vectors_mutex); // lock the vectors, the leader is their sole owner
#ifdef DEBUG
                                                                  // std::cout << "Thread " << std::this_thread::get_id() << " is polling (first is " << pfds[0].fd << ")." << std::endl;
#endif
        poll(pfds.data(), pfds.size(), LEADER_FOLLOWER_POLL_TIMEOUT); // poll the fds for events, timeout to check running one in a while
        for (size_t i = 0; i < this->fds_count; ++i)                  // go through the fds
        {
            if (pfds[i].revents & POLLIN) // if found one with data in
            {
                promote_leader(); // promote a new leader (you are now busy)
                vectors_lock.unlock();
                handlers[i](pfds[i].fd); // handle the fd
#ifdef DEBUG
                std::cout << "Thread " << std::this_thread::get_id() << " finished handling and is back to following." << std::endl;
#endif
                return; // break to become a follower again
            }
            else if (pfds[i].revents & POLLNVAL) // if found one with error
            {
                remove_fd(pfds[i].fd); // remove it from the set
            }
        }
    }
}
