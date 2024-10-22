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
#include <mutex>
#include <shared_mutex>

#include "pattern_library/leader_follower.hpp"
#include "algorithms/Kruskal.hpp"
#include "algorithms/MSTFactory.hpp"
#include "algorithms/Utilities.hpp"
#include "graphs/Graph.hpp"
#include "graphs/AdjacencyGraph.hpp"

#define PORT 9034
#define MAX_USERS 5
#define MAX_SEGMENT_SIZE 65535

#define DEBUG

using std::cin, std::cout, std::set, std::string;

Graph *g = nullptr;
LeaderFollower lf;
std::shared_mutex graph_mutex;

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
    if (command == "MST")
    {
        {
            std::shared_lock<std::shared_mutex> graph_lock(graph_mutex);
            if (!g)
            {
                if (send_message(fd, "Please create a graph using Newgraph <n>,<m> first\n"))
                {
                    throw std::runtime_error("Error sending a message to the client");
                }
                return false;
            }
            if(!is_connected(g))
            {
                if (send_message(fd, "The graph is not connected, therefore has no MST\n"))
                {
                    throw std::runtime_error("Error sending a message to the client");
                }
                return false;
            }
        }
        
        string algo_name;
        std::getline(is, algo_name);
        std::stringstream buffer;
        {
            std::shared_lock<std::shared_mutex> graph_lock(graph_mutex);
            mst_strat_t *algo = mst_strategy(algo_name);
            Subgraph MST_Graph = algo(g);
            #ifdef DEBUG
            std::cout << "MST Graph:" << std::endl;
            for (auto &[u, v, w] : MST_Graph.get_edges())
            {
                std::cout << "(" << u << ", " << v << ") With weight: " << w << '\n';
            }
            #endif
            double total_MST_weight = total_weight(&MST_Graph);
            double average_distance = average_distance_between_two_vertices(&MST_Graph);
            double shortest_distance = shortest_distance_between_two_vertices(&MST_Graph);
            double longest_distance = longest_distance_between_two_vertices(&MST_Graph);

            buffer << "The MST is: \n";
            for (auto &[u, v, w] : MST_Graph.get_edges())
            {
                buffer << "(" << u << ", " << v << ") With weight: " << w << '\n';
            }

            buffer << "The total weight of the MST is: " << total_MST_weight << '\n';
            buffer << "The average distance between two vertices in the MST is: " << average_distance << '\n';
            buffer << "The shortest distance between two vertices in the MST is: " << shortest_distance << '\n';
            buffer << "The longest distance between two vertices in the MST is: " << longest_distance << '\n';
        }
        if (send_message(fd, buffer.str()))
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
        {
            std::unique_lock<std::shared_mutex> graph_lock(graph_mutex);
            if (g)
                delete g;

            std::vector<Graph::edge> edges;
            try
            {
                for (size_t i = 0; i < m; ++i)
                {
                    vertex src, dst;
                    weight w;
                    string received_edge = receive_message(fd);
                    std::istringstream edge_stream(received_edge);
                    cout << "Received edge: " << received_edge << std::endl;
                    if (edge_stream >> src >> dst >> w)
                    {
                        cout << "Parsed edge: " << src << " " << dst << " " << w << std::endl;
                        edges.push_back(Graph::edge(src, dst, w));
                    }
                    else
                    {
                        cout << "Failed to parse edge: " << received_edge << std::endl;
                    }
                if(send_message(fd, "Enter edge and weight: "))
                {
                    throw std::runtime_error("Error sending a message to the client");
                }
                vertex src, dst;
                weight w;
                string received_edge = receive_message(fd);
                std::istringstream edge_stream(received_edge);
                cout << "Received edge: " << received_edge << std::endl;
                if (edge_stream >> src >> dst >> w)
                {
                    cout << "Parsed edge: " << src << " <-> " << dst << " With weight: "<< w <<  std::endl;
                    edges.push_back(Graph::edge(src, dst, w));
                }
                else
                {
                    cout << "Failed to parse edge: " << received_edge << std::endl;
                }
            }
            catch (std::runtime_error &e)
            {
                std::cout << "Client closed while talking" << std::endl;
            }
            g = new AdjacencyGraph(n, edges);
        }
        return false;
    }
    else if (command == "Newedge" || command == "Updateedge")
    {
        {
            std::shared_lock<std::shared_mutex> graph_lock(graph_mutex);

            if (!g)
            {
                if (send_message(fd, "Please create a graph using Newgraph <n>,<m> first\n"))
                {
                    throw std::runtime_error("Error sending a message to the client");
                }
                return false;
            }
        }
        std::string param1, param2, edge_weight;
        std::getline(is, param1, ',');
        std::getline(is, param2, ',');
        std::getline(is, edge_weight);
        if (param1.length() == 0 || param2.length() == 0 || edge_weight.length() == 0)
        {
            if (send_message(fd, "Not enough parameters detected, command ignored\n"))
            {
                throw std::runtime_error("Error sending a message to the client");
            }
        }
        vertex src = strtoull(param1.c_str(), nullptr, 10), dst = strtoull(param2.c_str(), nullptr, 10);
        weight w = strtod(edge_weight.c_str(), nullptr);
        {
            std::unique_lock<std::shared_mutex> graph_lock(graph_mutex);

            if (!(g)->set_edge(src, dst, w))
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
        }
        return false;
    }
    else if (command == "Removeedge")
    {
        {
            std::shared_lock<std::shared_mutex> graph_lock(graph_mutex);

            if (!g)
            {
                if (send_message(fd, "Please create a graph using Newgraph <n>,<m> first\n"))
                {
                    throw std::runtime_error("Error sending a message to the client");
                }
                return false;
            }
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
        {
            std::unique_lock<std::shared_mutex> graph_lock(graph_mutex);

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
        if (send_message(fd, "Unknown command: " + command + "\n"))
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
    if (handle_user_input(fd, input))
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
