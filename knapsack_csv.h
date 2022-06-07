#pragma once

#include <cstddef>
#include <vector>
#include <utility>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <iostream>


struct item_t
{
    size_t id;
    double weight;
    double profit;
};


std::vector<item_t> read_csv(const std::string& filename)
{
    std::vector<item_t> items;

    std::ifstream ifs(filename);

    bool header = false;
    std::string line;
    size_t id = 0;
    while (std::getline(ifs, line)) {
        if (line.size() == 0) {
            continue;
        }
        if (items.size() == 0 && !header) {
            if (line != "weight,profit") {
                throw std::runtime_error("invalid csv header: \"" + line + "\"");
            } else {
                header = true;
                continue;
            }
        }
        std::stringstream ss(line);
        double w, p;
        ss >> w;
        while (ss.peek() == ',' || ss.peek() == ' ') {
            ss.ignore();
        }
        ss >> p;

        items.emplace_back(item_t{id, w, p});
        id++;
    }

    return items;
}


void save_csv(const char* filename, const std::vector<std::pair<item_t, bool>>& r)
{
    auto data = r;
    std::sort(data.begin(), data.end(), [](const std::pair<item_t, bool>& p1, const std::pair<item_t, bool>& p2) {
        return p1.first.id < p2.first.id;
    });

    std::ostream* os = &std::cout;
    std::ofstream ofs;
    if (filename) {
        ofs.open(filename, std::ios::out | std::ios::trunc);
        os = &ofs;
    }

    *os << "weight,profit,take" << std::endl;

    for (const auto& p : data) {
        *os << std::fixed << std::setprecision(1) << p.first.weight << ","
            << std::fixed << std::setprecision(1) << p.first.profit << ","
            << p.second << std::endl;
    }

    if (ofs.is_open()) {
        ofs.close();
    }
}


