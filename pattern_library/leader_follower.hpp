#pragma once

#include <functional>
#include <map>
#include <thread>
#include <mutex>
#include <vector>
#include <condition_variable>

constexpr size_t LEADER_FOLLOWER_DEFAULT_POOL_SIZE = 10;

constexpr int LEADER_FOLLOWER_POLL_TIMEOUT = 10;

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
    std::vector<std::thread> pool;

    volatile bool running; // not protected by mutex, should be fine since it is only ever written once when destructing

    bool has_leader; // TODO: pointer to leader thread?
    std::mutex leader_mutex;
    std::condition_variable leader_cond;

    size_t fds_count;
    std::vector<Handler> handlers;
    std::vector<struct pollfd> pfds;
    std::mutex vectors_mutex;
    void reactor_main(); // TODO: needed?
    void follow();
    void promote_leader();
    void lead();
public:
    /**
     * Creates a new leader follower pool, you can add file descriptors to it to start handling events.
     * @param pool_size the number of threads to create in the pool, default is LEADER_FOLLOWER_DEFAULT_POOL_SIZE
     */
    LeaderFollower(size_t pool_size = LEADER_FOLLOWER_DEFAULT_POOL_SIZE);
    /**
     * Destroys the pool, will no longer process events in the given fds.
     * Can take up to LEADER_FOLLOWER_POLL_TIMEOUT milliseconds for the leader to stop.
     * Will block until leader stops and all running handlers finish. // TODO: can make so it doesn't block and just hope they die
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
