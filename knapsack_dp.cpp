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
#include <cstring>
#include "knapsack_csv.h"


using item_t = item<size_t>;


struct bool_vector_t
{
    bool_vector_t() = default;

    bool_vector_t(size_t sz)
    {
        resize(sz);
        clear(false);
    }

    bool_vector_t(const bool_vector_t& other)
    {
        resize(other.size);
        memcpy(bits.get(), other.bits.get(), size/8);
    }

    bool_vector_t& operator=(const bool_vector_t& other)
    {
        resize(other.size);
        memcpy(bits.get(), other.bits.get(), size/8);
        return *this;
    }

    bool_vector_t(bool_vector_t&& other)
    {
        bits.swap(other.bits);
        size = other.size;
    }

    bool_vector_t& operator=(bool_vector_t&& other)
    {
        bits.swap(other.bits);
        size = other.size;
        other.bits.reset();
        other.size = 0;
        return *this;
    }

    void clear(bool value = false)
    {
        memset(bits.get(), value ? 0xFF : 0x00, (size + 7) / 8);
    }

    void resize(size_t sz)
    {
        sz += 7;
        sz &= ~size_t(0x07);
        if (sz <= size) {
            return;
        }
        if (bits) {
            uint8_t* ptr = bits.release();
            ptr = (uint8_t*)realloc(ptr, sz/8);
            bits.reset(ptr);
        } else {
            bits.reset((uint8_t*)malloc(sz/8));
        }
        memset(bits.get() + size/8, 0, sz/8 - size/8);
        if (!bits) {
            throw std::bad_alloc{};
        }
        size = sz;
    }

    bool operator[] (size_t index) const
    {
        if (index >= size) {
            throw std::out_of_range{"bool_vector operator[] index out of range"};
        }
        return (bits.get()[index / 8] & (1 << (index % 8))) != 0;
    }

    void set(size_t index)
    {
        if (index >= size) {
            throw std::out_of_range{"bool_vector set index out of range"};
        }
        bits.get()[index / 8] |= 1 << (index % 8);
    }

    void unset(size_t index)
    {
        if (index >= size) {
            throw std::out_of_range{"bool_vector unset index out of range"};
        }
        bits.get()[index / 8] &= ~(1 << (index % 8));
    }

    size_t size = 0;
    std::unique_ptr<uint8_t[]> bits;
};




struct knapsack_01_dp
{
    std::vector<item_t> items;
    size_t capacity;

    using table_item_t = std::pair<size_t, bool_vector_t>;
    std::vector<std::vector<table_item_t>> table;


    knapsack_01_dp(const std::vector<item_t>& _items, size_t c) :
        items(_items), capacity(c)
    {
        table.resize(2);

        for (auto& row : table) {
            row.resize(capacity + 1);
            for (auto &item : row) {
                item.first = 0;
                item.second = bool_vector_t(items.size());
            }
        }
    }

    // TODO: weight > capacity
    // TODO: capacity >= sum(weights)

    // TODO: sort??

    void optimize()
    {
        for (size_t item_i = 0; item_i < items.size(); item_i++) {
            const item_t& item = items[item_i];
            size_t i_0 = item_i % 2;
            size_t i_1 = (i_0 + 1) % 2;

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
                        table[i_1][c].second.set(item_i);
                    }
                }
            }
        }
    }

    std::vector<std::pair<item_t, bool>> result() const
    {
        std::vector<std::pair<item_t, bool>> r;

        const auto& selected = table[items.size() % 2][capacity].second;

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
