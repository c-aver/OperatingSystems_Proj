#pragma once

#include <functional>
#include <map>
#include <thread>
#include <mutex>
#include <vector>
#include <condition_variable>

/**
 * This class implements the leader follower pool design pattern.
 * The pool saves a list of file descriptors and handlers.
 * The pool leader continuously polls the file descriptors for events.
 * When an event occurs on one of them (currently only event is information available for reading) the handler is called.
 * Once that happens, a new leader is promoted.
 */
class LeaderFollower
{
public:
    /**
     * The type used as a handler for reactions to file descriptor events.
     * The function receives as an argument the file descriptor on which the event occured.
     */
    using Handler = std::function<void(int)>; // TODO: allow different handlers?
private:
    bool has_leader; // TODO: pointer to leader thread?
    std::mutex leader_mutex;
    std::condition_variable leader_cond;

    size_t fds_count;
    std::vector<Handler> handlers;
    std::vector<struct pollfd> pfds;
    std::mutex vectors_mutex;
    void reactor_main(); // TODO: needed?
    int follow();
    void promote_leader();
    void lead();
public:
    /**
     * Creates a new leader follower pool, you can add file descriptors to it to start handling events.
     */
    LeaderFollower();
    /**
     * Destroys the pool. // TODO: does what?
     */
    ~LeaderFollower();

    /**
     * Adds a new file descriptor to the reactor, when events occur on it handler will be called.
     */
    bool add_fd(int fd, Handler handler);
    /**
     * Removes from the reactor the file descriptor and its handler.
     */
    bool remove_fd(int fd);
};
