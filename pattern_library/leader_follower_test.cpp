#include <iostream>
#include <unistd.h>

#include "leader_follower.hpp"

void echo_to_fd_continuously(int fd)
{
    char c;
    read(fd, &c, sizeof(c));
    std::cout << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() % 60 << ": Read \"" << c << "\", now resting................................................................................" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

int main(int argc, char **argv)
{
    size_t pool_size = LEADER_FOLLOWER_DEFAULT_POOL_SIZE;
    if (argc > 1)
        pool_size = std::atoll(argv[1]);
    LeaderFollower lf(pool_size);
    std::cout << "LF is up" << std::endl;
    lf.add_fd(STDIN_FILENO, echo_to_fd_continuously);
    std::cout << "Added handler" << std::endl;
    sleep(10);
}
