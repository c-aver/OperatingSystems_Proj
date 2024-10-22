#include <iostream>
#include <unistd.h>

#include "active_object.hpp"

void print_double(int i)
{
    std::cout << i*2 << std::endl;
    sleep(5);
}
int return_double(int i)
{
    return i*2;
}

int main()
{
    ActiveObject<void(int)> ao1(print_double);
    ActiveObject<int(int)> ao2(return_double);
    ao1(5);
    ao1(20);
    std::future<int> fut = ao2(10);
    fut.wait();
    std::cout << fut.get() << std::endl;
    sleep(10);
}