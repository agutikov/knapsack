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


using item_t = item<double>;


bool density_gt(const item_t& left, const item_t& right)
{
    double d1 = left.profit / left.weight;
    double d2 = right.profit / right.weight;
    if (d1 == d2) {
        return left.weight < right.weight;
    }
    return d1 > d2;
}

struct knapsack_result_t
{
    knapsack_result_t(std::vector<item_t>& items) :
        items(items), picks(items.size(), false)
    {}

    knapsack_result_t& operator=(const knapsack_result_t& other)
    {
        items = other.items;
        picks = other.picks;
        count = other.count;
        P = other.P;
        W = other.W;
        return *this;
    }

    std::vector<item_t>& items;

    std::vector<bool> picks;
    size_t count = 0;
    double P = 0;
    double W = 0;

    void take(size_t index)
    {
        const item_t& item = items[index];
        count++;
        P += item.profit;
        W += item.weight;
        picks[index] = true;
    }

    void discard(size_t index)
    {
        const item_t& item = items[index];
        count--;
        P -= item.profit;
        W -= item.weight;
        picks[index] = false;
    }

    std::vector<std::pair<item_t, bool>> result() const
    {
        std::vector<std::pair<item_t, bool>> r;
        for (size_t i = 0; i < items.size(); i++) {
            r.emplace_back(items[i], picks[i]);
        }
        return r;
    }
};


struct knapsack_01_bnb
{
    std::vector<item_t> items;
    double capacity;

    knapsack_01_bnb(const std::vector<item_t>& _items, double c) :
        items(_items), capacity(c),
        max_p(items.size()), min_w(items.size()),
        state(items), state_max(items), level_stats(items.size())
    {
        std::sort(items.begin(), items.end(), [c](const item_t& left, const item_t& right) {
            if (left.weight > c && right.weight > c) {
                return left.weight < right.weight;
            }
            if (left.weight > c) {
                return false;
            }
            if (right.weight > c) {
                return true;
            }
            return density_gt(left, right);
        });

        double w_min = std::numeric_limits<double>::max();
        double p_sum = 0;
        for (int i = items.size() - 1; i >= 0; i--) {
            w_min = std::min(w_min, items[i].weight);
            min_w[i] = w_min;
            p_sum += items[i].profit;
            max_p[i] = p_sum;
        }
    }

    std::vector<double> max_p;
    std::vector<double> min_w;

    knapsack_result_t state;

    knapsack_result_t state_max;


    bool fit(size_t index) const
    {
        const item_t& item = items[index];
        return item.weight <= capacity - state.W;
    }

    size_t handle_result_calls = 0;
    size_t result_copies = 0;
    size_t cycles = 0;
    size_t leafs_touched = 0;

    struct stat_t
    {
        size_t node_enter_down = 0;
        size_t node_enter_up = 0;
        size_t left_branch = 0;
        size_t right_branch = 0;
        size_t bound = 0;
        size_t min_w = 0;
    };

    stat_t total_stats;
    std::vector<stat_t> level_stats;

    void handle_node_enter_down(size_t level)
    {
        total_stats.node_enter_down++;
        level_stats[level].node_enter_down++;
    }

    void handle_left_branch(size_t level)
    {
        total_stats.left_branch++;
        level_stats[level].left_branch++;
    }

    void handle_right_branch(size_t level)
    {
        total_stats.right_branch++;
        level_stats[level].right_branch++;
    }

    void handle_node_enter_up(size_t level)
    {
        total_stats.node_enter_up++;
        level_stats[level].node_enter_up++;
    }
    void handle_node_bound(size_t level)
    {
        total_stats.bound++;
        level_stats[level].bound++;
    }
    void handle_node_min_w(size_t level)
    {
        total_stats.min_w++;
        level_stats[level].min_w++;
    }

    void handle_leaf()
    {
        leafs_touched++;
    }

    void print_total_stats() const
    {
        std::cout
        << "cycles = " << cycles << std::endl
        << "handle_result_calls = " << handle_result_calls << std::endl
        << "result_copies = " << result_copies << std::endl
        << "leafs_touched = " << leafs_touched << std::endl
        << "node_enter_down = " << total_stats.node_enter_down << std::endl
        << "node_enter_up = " << total_stats.node_enter_up << std::endl
        << "left_branch = " << total_stats.left_branch << std::endl
        << "right_branch = " << total_stats.right_branch << std::endl
        << "bound = " << total_stats.bound << std::endl
        << "min_w = " << total_stats.min_w << std::endl
        ;
        std::cout << "count=" << state_max.count << ", W=" << state_max.W << ", P=" << state_max.P << std::endl;
    }

    void print_level_stats() const
    {
        std::cout << "level, weight, profit, take,      max_p,    min_w,"
            << "     down,       up,     left,    right,    bound,    min_w"
            << std::endl;
        for (size_t index = 0; index < items.size(); index++) {
            const item_t& item = items[index];
            std::cout << std::setw(5) << index << ", "
                      << std::setw(6) << item.weight << ", "
                      << std::setw(6) << item.profit << ", "
                      << std::setw(4) << state_max.picks[index] << ", "
                      << std::setw(10) << max_p[index] << ", "
                      << std::setw(8) << min_w[index] << ", "
                      << std::setw(8) << level_stats[index].node_enter_down << ", "
                      << std::setw(8) << level_stats[index].node_enter_up << ", "
                      << std::setw(8) << level_stats[index].left_branch << ", "
                      << std::setw(8) << level_stats[index].right_branch << ", "
                      << std::setw(8) << level_stats[index].bound << ", "
                      << std::setw(8) << level_stats[index].min_w
                      << std::endl;
        }
    }

    std::function<bool(const knapsack_result_t& state)> result_constraint;

    void handle_result()
    {
        handle_result_calls++;

        if (result_constraint) {
            if (!result_constraint(state)) {
                return;
            }
        }

        if (state.P > state_max.P
            || (state.P == state_max.P && state.W < state_max.W))
        {
            state_max = state;
            result_copies++;
        }
    }

    void optimize()
    {
        size_t index = 0;
        bool up = false;

        while (true) {
            cycles++;

            if (up) {
                if (index == 0) {
                    // end of three walk-through
                    break;
                }
                // go up
                index--;
                handle_node_enter_up(index);
                if (state.picks[index]) {
                    // handle result before discard item
                    handle_result();

                    // right branch
                    state.discard(index);

                    // go down
                    up = false;
                    handle_right_branch(index);
                    index++;
                } else {
                    // continue moving up
                    continue;
                }
            }

            if (index >= items.size()) {
                // no more items to process
                handle_leaf();
                up = true;
                continue;
            }
            if (state_max.P >= state.P + max_p[index]) {
                // potential profit is less than already found one
                handle_node_bound(index);
                up = true;
                continue;
            }
            if (state.W + min_w[index] > capacity) {
                // no one next item (including current) will fit
                handle_node_min_w(index);
                up = true;
                continue;
            }

            handle_node_enter_down(index);

            if (fit(index)) {
                // left branch
                state.take(index);
                handle_left_branch(index);
            } else {
                handle_right_branch(index);
            }

            // go down
            index++;
        }
    }

    auto result() const
    {
        return state_max.result();
    }
};


int main(int argc, const char* argv[])
{
    auto items = read_csv<double>(argv[1]);
    double capacity = std::stod(argv[2]);

    knapsack_01_bnb k{ items, capacity };

    if (false) {
        k.result_constraint = [](const knapsack_result_t& state) { return state.count % 2 != 0; };
    }

    k.optimize();
    k.print_level_stats();
    k.print_total_stats();
    std::cout << std::endl;

    auto r = k.result();
    save_csv(argv[3], r, 0);


    return 0;
}
