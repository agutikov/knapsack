#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cstddef>
#include <algorithm>
#include <iostream>
#include <limits>
#include <iomanip>
#include <stack>
#include <fstream>
#include <sstream>
#include <functional>
#include "knapsack_csv.h"


using item_t = item<size_t>;


struct knapsack_01_dp
{
    std::vector<item_t> items;
    size_t capacity;

    using table_item_t = std::pair<size_t, std::vector<bool>>;
    std::vector<std::vector<table_item_t>> table;


    knapsack_01_dp(const std::vector<item_t>& _items, size_t c) :
        items(_items), capacity(c)
    {
        table.resize(2);

        for (auto& row : table) {
            row.resize(capacity + 1);
            for (auto &item : row) {
                item.first = 0;
                item.second.resize(items.size());
            }
        }
    }

    // TODO: weight > capacity
    // TODO: capacity >= sum(weights)

    // TODO: bit vector for bool vector
    // TODO: sort??

    void optimize()
    {
        for (size_t item_i = 0; item_i < items.size(); item_i++) {
            const item_t& item = items[item_i];
            size_t i = item_i + 1;
            size_t i_1 = i % 2;
            size_t i_0 = (i - 1) % 2;

            for (size_t c = 1; c <= capacity; c++) {
                if (item.weight > c) {
                    table[i_1][c] = table[i_0][c];
                } else {
                    size_t p1 = table[i_0][c].first;
                    size_t p2 = table[i_0][c - item.weight].first + item.profit;

                    if (p1 >= p2) {
                        table[i_1][c] = table[i_0][c];
                    } else {
                        table[i_1][c].first = p2;
                        table[i_1][c].second = table[i_0][c - item.weight].second;
                        table[i_1][c].second[item_i] = true;
                    }
                }

                //printf(" %lu", table[i_1][c].first);
            }
            //printf("\n");
        }
    }

    std::vector<std::pair<item_t, bool>> result() const
    {
        std::vector<std::pair<item_t, bool>> r;

        const auto &selected = table[items.size() % 2][capacity].second;

        for (size_t i = 0; i < items.size(); i++) {
            r.emplace_back(items[i], selected[i]);
        }

        return r;
    }
};




int main(int argc, const char* argv[])
{
    auto items = read_csv<size_t>(argv[1]);
    size_t capacity = std::stol(argv[2]);

    knapsack_01_dp k{ items, capacity };


    k.optimize();


    auto r = k.result();
    save_csv(argv[3], r, 0);


    return 0;
}
