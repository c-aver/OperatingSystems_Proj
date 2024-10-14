#include <iostream>
#include <vector>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdexcept>
#include <string>
#include <set>
#include <algorithm>
#include <sstream>
#include <fcntl.h>

#include "pattern_library/leader_follower.hpp"
#include "algorithms/Kruskal.hpp"
#include "algorithms/MSTFactory.hpp"
#include "algorithms/Utilities.hpp"
#include "graphs/Graph.hpp"
#include "graphs/AdjacencyGraph.hpp"

#define PORT 9034
#define MAX_USERS 5
#define MAX_SEGMENT_SIZE 65535

using std::cin, std::cout, std::set, std::string;

Graph *g = nullptr;
LeaderFollower lf;

/**
 * @brief Receive a message from the client
 * @return The received message
 * @throw Throws runtime_error if an error occurred
 */
string receive_message(int fd)
{
    char buffer[MAX_SEGMENT_SIZE];
    ssize_t bytes_read = read(fd, buffer, MAX_SEGMENT_SIZE);
    if (bytes_read < 0)
    {
        throw std::runtime_error("Something went wrong when trying to read from the socket");
    }
    if (bytes_read == 0)
    {
        throw std::runtime_error("Connection closed by the client");
    }
    buffer[bytes_read] = '\0'; // Null-terminate the buffer if it's a string
    return buffer;
}

bool send_message(int fd, string message)
{
    return write(fd, message.c_str(), message.length() + 1) < 0;
}

bool handle_user_input(int fd, string input)
{
    std::istringstream is(input);
    string command;
    std::getline(is, command, ' ');
    if (command == "Kosaraju") // TODO: replace
    {
        if (!g)
        {
            if (send_message(fd, "Please create a graph using Newgraph <n>,<m> first\n"))
            {
                throw std::runtime_error("Error sending a message to the client");
            }
            return false;
        }
        // auto comps = kosaraju(*g);
        string message;
        // message += "The strongly connected components are: \n";
        // for (auto comp : comps)
        // {
        //     for (vertex v : comp)
        //     {
        //         message += std::to_string(v) + ' ';
        //     }
        //     message += '\n';
        // }
        if (send_message(fd, message))
        {
            throw std::runtime_error("Error sending a message to the client");
        }
        return false;
    }
    else if (command == "Newgraph")
    {
        std::string param1, param2;
        std::getline(is, param1, ',');
        std::getline(is, param2);
        if (param1.length() == 0 || param2.length() == 0)
        {
            if (send_message(fd, "Not enough parameters detected, command ignored\n"))
            {
                throw std::runtime_error("Error sending a message to the client");
            }
        }
        size_t n = strtoull(param1.c_str(), nullptr, 10), m = strtoull(param2.c_str(), nullptr, 10);
        if (g)
            delete g;
        std::vector<std::pair<vertex, vertex>> edges;
        for (size_t i = 0; i < m; ++i)
        {
            vertex src, dst;
            string received_edge = receive_message(fd);
            cout << "Received edge: " << received_edge << std::endl;
            char *space;
            src = strtoull(received_edge.c_str(), &space, 10); // TODO: parse weight
            dst = strtoull(space + 1, nullptr, 10);
            cout << "Parsed edge: " << src << " " << dst << std::endl;
            edges.push_back(std::make_pair(src, dst));
        }
        // g = new AdjacencyGraph(n, edges);
        return false;
    }
    else if (command == "Newedge")
    {
        if (!g)
        {
            if (send_message(fd, "Please create a graph using Newgraph <n>,<m> first\n"))
            {
                throw std::runtime_error("Error sending a message to the client");
            }
            return false;
        }
        std::string param1, param2; // TODO: parse weight
        std::getline(is, param1, ',');
        std::getline(is, param2);
        if (param1.length() == 0 || param2.length() == 0)
        {
            if (send_message(fd, "Not enough parameters detected, command ignored\n"))
            {
                throw std::runtime_error("Error sending a message to the client");
            }
        }
        vertex src = strtoull(param1.c_str(), nullptr, 10), dst = strtoull(param2.c_str(), nullptr, 10);
        if (!(g)->set_edge(src, dst, 0)) // TODO: replace 0 with parsed weight
        {
            if (send_message(fd, "Edge already exists\n"))
            {
                throw std::runtime_error("Error sending a message to the client");
            }
        }
        else
        {
            if (send_message(fd, "Edge was created successfuly\n"))
            {
                throw std::runtime_error("Error sending a message to the client");
            }
        }
        return false;
    } // TODO: add Updateedge command?
    else if (command == "Removeedge")
    {
        if (!g)
        {
            if (send_message(fd, "Please create a graph using Newgraph <n>,<m> first\n"))
            {
                throw std::runtime_error("Error sending a message to the client");
            }
            return false;
        }
        std::string param1, param2;
        std::getline(is, param1, ',');
        std::getline(is, param2);
        if (param1.length() == 0 || param2.length() == 0)
        {
            if (send_message(fd, "Not enough parameters detected, command ignored\n"))
            {
                throw std::runtime_error("Error sending a message to the client");
            }
        }
        vertex src = strtoull(param1.c_str(), nullptr, 10), dst = strtoull(param2.c_str(), nullptr, 10);
        if (!(g)->remove_edge(src, dst))
        {
            if (send_message(fd, "Edge does not exist\n"))
            {
                throw std::runtime_error("Error sending a message to the client");
            }
        }
        else
        {
            if (send_message(fd, "Edge was removed successfuly\n"))
            {
                throw std::runtime_error("Error sending a message to the client");
            }
        }
        return false;
    }
    else if (command == "Exit")
    {
        std::cout << "Connection closed by the client" << std::endl;
        close(fd);
        lf.remove_fd(fd);
        return true;
    }
    else
    {
        if (send_message(fd, "Unknown command\n"))
        {
            throw std::runtime_error("Error sending a message to the client");
        }
        return false;
    }
}

void message_handler(int fd)
{
    string input;
    try
    {
        input = receive_message(fd);
    }
    catch (const std::runtime_error &e)
    {
        cout << e.what() << std::endl;
        close(fd);
        return;
    }
    input.pop_back(); // Remove the newline character
    if(handle_user_input(fd, input))
    {
        return; // The user requested to close the connection, so we don't need to send a message
    }
    if (send_message(fd, "Enter command: "))
    {
        std::cerr << ("Error sending a message to the client\n");
        close(fd);
        return;
    }
}

int main()
{
    int server_fd = -1;
    int new_socket = -1;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    std::cout << "Waiting for incoming connections..." << std::endl;

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 9034
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the network address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, MAX_USERS) < 0)
    {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Set the socket to non-blocking mode
    if (fcntl(server_fd, F_SETFL, O_NONBLOCK) < 0)
    {
        perror("fcntl(F_SETFL | O_NONBLOCK)");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    std::cout << "Server is listening on port " << PORT << std::endl;

    while (true)
    {
        new_socket = -1;
        // Accept an incoming connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0 && errno != EAGAIN)
        {
            perror("accept");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        if (new_socket > 0)
        {
            std::cout << "New connection accepted" << std::endl;
            lf.add_fd(new_socket, message_handler);
            if (send_message(new_socket, "Enter command: "))
            {
                close(new_socket);
                close(server_fd);
                throw std::runtime_error("Error sending a message to the client");
            }
        }
    }
    return 0;
}
