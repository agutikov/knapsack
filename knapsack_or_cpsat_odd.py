#!/usr/bin/env python3

import pandas as pd
import sys
from io import StringIO
from ortools.sat.python import cp_model



def knapsack_01(df, capacity):
    I = range(len(df))
    p = [int(x) for x in df['profit']]
    w = [int(x) for x in df['weight']]
    c = int(capacity)

    model = cp_model.CpModel()

    x = [model.NewIntVar(0, 1, f'x_{i}') for i in I]

    model.Add(sum(x[i] * w[i] for i in I) <= c)

    for i in range(2, len(I) + 1):
        if i % 2 == 0:
            model.Add(sum(x) != i)

    model.Maximize(sum(x[i] * p[i] for i in I))

    solver = cp_model.CpSolver()
    status = solver.Solve(model)

    if status != cp_model.OPTIMAL:
        print('Non-optimal solution')
        return None

    result = pd.DataFrame()
    result['weight'] = w
    result['profit'] = p
    result['take'] = [int(solver.Value(x[i])) for i in I]

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










