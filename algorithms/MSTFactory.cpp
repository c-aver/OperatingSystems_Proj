#include "MSTFactory.hpp"

mst_strat_t *mst_strategy() // TODO: parameters
{
    mst_strat_t *result = &kruskal;
    return result;
};
