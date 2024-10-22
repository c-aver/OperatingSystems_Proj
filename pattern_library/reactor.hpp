#pragma once

#include <functional>
#include <map>
#include <thread>
#include <mutex>
#include <vector>

/**
 * This class implements the reactor design pattern.
 * The reactor saves a list of file descriptors and handlers.
 * While running the reactor continuously polls the file descriptors for events.
 * When an event occurs on one of them (currently only event is information available for reading) the handler is called.
 */
class Reactor
{
public:
    /**
     * The type used as a handler for reactions to file descriptor events.
     * The function receives as an argument the file descriptor on which the event occured.
     */
    using Handler = std::function<void (int)>;
private:
    bool running;
    std::mutex running_mutex;
    std::thread *thread;    // The thread that runs the reactor
    size_t fds_count;       // The number of file descriptors
    std::vector<Handler> handlers;  // The handlers for the file descriptors
    std::vector<struct pollfd> pfds;    // The file descriptors to poll
    std::mutex vectors_mutex;           // Mutex for the vectors
    void reactor_main();                // The main function of the reactor
public:
    /**
     * Creates a new reactor, intially not running, you can add file descriptors to it and use start() to start it.
     */
    Reactor();
    /**
     * Destroys the reactor, calls stop().
     */
    ~Reactor();
    /**
     * Starts the reactor in a separate thread. If the reactor was already running it is stopped first.
     */
    void start();
    /**
     * Stops the reactor, blocks until all currently polled operations are finished.
     */
    void stop();
    /**
     * Adds a new file descriptor to the reactor, when events occur on it handler will be called.
     */
    bool add_fd(int fd, Handler handler);
    /**
     * Removes from the reactor the file descriptor and its handler.
     */
    bool remove_fd(int fd);
};
