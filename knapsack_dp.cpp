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








int main(int argc, const char* argv[])
{
    auto items = read_csv(argv[1]);
    double capacity = std::stod(argv[2]);






    // save_csv(argv[3], r);


    return 0;
}
