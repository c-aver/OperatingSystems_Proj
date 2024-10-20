#include "MSTFactory.hpp"
#include <string>

using std::string;
mst_strat_t *mst_strategy(string algo_name) // TODO: parameters
{
    (void)algo_name;
    mst_strat_t *result = &kruskal;
    return result;
};
