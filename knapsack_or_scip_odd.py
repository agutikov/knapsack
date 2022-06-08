#!/usr/bin/env python3

import pandas as pd
import sys
from io import StringIO
from ortools.linear_solver import pywraplp



def knapsack_01(df, capacity):
    I = range(len(df))
    p = [int(x) for x in df['profit']]
    w = [int(x) for x in df['weight']]

    solver = pywraplp.Solver.CreateSolver('SCIP')
    if solver is None:
        print('SCIP solver unavailable.')
        return None

    x = [solver.BoolVar(f'x_{i}') for i in I]

    solver.Add(sum(x[i] * w[i] for i in I) <= capacity)

    if len(x) % 2 == 0:
        solver.Add(sum(x) <= len(x) - 1)

    objective = solver.Objective()
    for i in I:
        objective.SetCoefficient(x[i], p[i])
    objective.SetMaximization()

    while True:
        status = solver.Solve()

        if status != pywraplp.Solver.OPTIMAL:
            print('Non-optimal solution')
            return None

        count = sum(int(x[i].solution_value()) for i in I)
        if count % 2 == 0:
            print(f"count={count}, add constraint and continue")
            solver.Add(sum(x) <= count-1)
        else:
            print(f'count={count}')
            break

    result = pd.DataFrame()
    result['weight'] = w
    result['profit'] = p
    result['take'] = [int(x[i].solution_value()) for i in I]

    return result


if __name__ == '__main__':
    input_csv_filename = sys.argv[1]
    capacity = float(sys.argv[2])

    df = pd.read_csv(input_csv_filename)

    data = knapsack_01(df, capacity)

    output_stdout = len(sys.argv) == 3
    output = StringIO()
    if not output_stdout:
        output = sys.argv[3]

    data.to_csv(output, index=False)

    if output_stdout:
        output.seek(0)
        print(output.read(), end='')










