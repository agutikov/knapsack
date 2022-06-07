# knapsack
**Solve knapsack problem with different solvers**


```shell
# Install requirements:
pip3 install -r ./requirements.txt

# Generate problems' data:
./gen_problems.sh

# Solve one problem, 1000 items, capacity=1000000, output into "./data/result.csv":
./knapsack_mip.py ./data/p/1000_square_linear.csv 1000000 ./data/result.csv

# Calculate result weight and profit:
./stat.py ./data/result.csv
```


## Links

* Wikipedia: https://en.wikipedia.org/wiki/Knapsack_problem
* Basic algorithms:
  * Branch and Bound:
    * Algorithm: https://www.geeksforgeeks.org/0-1-knapsack-using-branch-and-bound/
    * C++ Implementation: https://www.geeksforgeeks.org/implementation-of-0-1-knapsack-using-branch-and-bound/
  * DP:
* High-level tools: wrappers, frameworks, etc...
  * Python-MIP: https://docs.python-mip.com/en/latest/examples.html
  * OR-Tools: https://developers.google.com/optimization/introduction/overview
* Solvers:
  * CP-SAT - CP/SAT solver: https://github.com/google/or-tools/tree/stable/ortools/sat#readme
  * SCIP - MIP solver: https://www.scipopt.org/
  * CBC - MIP solver: https://www.coin-or.org/Cbc/
  * Z3 - SMT solver:
    * Python API docs: https://z3prover.github.io/api/html/namespacez3py.html
    * Python Z3 tutorial: https://colab.research.google.com/github/philzook58/z3_tutorial/blob/master/Z3%20Tutorial.ipynb
  * CVC4 - SMT solver: (correct me if I'm wrong) does not have model optimization


## Abbreviations

* OR - [Operations Research](https://en.wikipedia.org/wiki/Operations_research)
* CP - [Constraint Programming](https://en.wikipedia.org/wiki/Constraint_programming)
* DP - [Dynamic Programming](https://en.wikipedia.org/wiki/Dynamic_programming)
* MIP - [Mixed Integer Programming](https://en.wikipedia.org/wiki/Integer_programming)
* SAT - **SAT**isfiability [Boolean satisfiability problem](https://en.wikipedia.org/wiki/Boolean_satisfiability_problem), [SAT solver](https://en.wikipedia.org/wiki/SAT_solver)
* SMT - [Satisfiability Modulo Theories](https://en.wikipedia.org/wiki/Satisfiability_modulo_theories)

