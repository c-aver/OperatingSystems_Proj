#include <thread>
#include <vector>
#include <poll.h>
#include <algorithm>
#include <unistd.h>
#include <iostream>
#include <variant>

#include "leader_follower.hpp"

#define DEBUG

LeaderFollower::LeaderFollower() : has_leader(false), fds_count(0) {}

LeaderFollower::~LeaderFollower() {} // TODO: what to do?

bool LeaderFollower::add_fd(int fd, Handler handler)
{
    std::lock_guard<std::mutex> vectors_guard(vectors_mutex);
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
    std::cout << "Removing fd\n";
    std::cout << "fds_count: " << fds_count << std::endl;
    std::cout << "pfds.size: " << pfds.size() << std::endl;
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

struct handler_action
{
    LeaderFollower::Handler handler;
    int param;
    void operator()()
    {
        handler(param);
    }
}; // TODO: remove?

struct remove_action
{
    LeaderFollower *LeaderFollower;
    int param;
    void operator()()
    {
        LeaderFollower->remove_fd(param);
    }
}; // TODO: remove?

int LeaderFollower::follow()
{
    std::unique_lock<std::mutex> leader_lock(leader_mutex); // lock the mutex to check if a leader exists
    while (true)                                            // infinite loop to conitnuously try to be the leader (shows initiative)
    {
        if (has_leader) // if there is a leader already
        {
            leader_cond.wait(leader_lock, [this]       // wait for it to wake you up
                             { return !has_leader; }); // make sure there is actually no leader
        }
        has_leader = true;    // you are now the leader
        leader_lock.unlock(); // unlock the mutex so others can follow while you lead
        lead();               // perform the leaders duties // TODO: handle errors by dying?
        leader_lock.lock();   // after you are done, lock the mutex to check again
    }
}

void LeaderFollower::promote_leader()
{
    std::unique_lock<std::mutex> leader_lock(leader_mutex); // lock the mutex to set has_leader
    has_leader = false;                                     // set to false so others know there is a vacancy
    leader_cond.notify_one();                               // wake up one of them
}

void LeaderFollower::lead()
{
    std::lock_guard<std::mutex> vectors_guard(vectors_mutex); // lock the vectors, the leader is their sole owner
    poll(pfds.data(), pfds.size(), 0); // poll the fds for events // TOOD: loop on this? // TODO: select instead?
    for (size_t i = 0; i < this->fds_count; ++i) // go through the fds
    {
        if (pfds[i].revents & POLLIN) // if found one with data in
        {
            promote_leader(); // promote a new leader (you are now busy)
            handlers[i](pfds[i].fd); // handle the fd
            break; // break to become a follower again
        }
        else if (pfds[i].revents & POLLNVAL) // if found one with error
        {
            remove_fd(pfds[i].fd); // remove it from the set
        }
    }
}
