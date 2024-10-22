#include "MSTFactory.hpp"
#include <string>
#include <cctype>
#include <iostream>
#include <algorithm>

// #define DEBUG

using std::string;
mst_strat_t *mst_strategy(string algo_name)
{
    std::transform(algo_name.begin(), algo_name.end(), algo_name.begin(),
    [](unsigned char c){ return std::tolower(c); });            // Convert the string to lowercase
    
#ifdef DEBUG
    std::cout << "algo_name: " << algo_name << std::endl;
#endif

    if(algo_name == "kruskal")
    {
        return &kruskal;
    }
    else if(algo_name == "prim")
    {
        return &prim;
    }
    else
    {
        return nullptr;
    }
};
