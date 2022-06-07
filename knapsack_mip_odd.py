#!/usr/bin/env python3

import pandas as pd
import sys
from io import StringIO
from mip import Model, xsum, maximize, BINARY, CBC, ConstrsGenerator, CutPool, OptimizationStatus



def knapsack_01(df, capacity):
    I = range(len(df))
    p = [int(x) for x in df['profit']]
    w = [int(x) for x in df['weight']]

    m = Model("knapsack")

    x = [m.add_var(var_type=BINARY) for i in I]

    m.objective = maximize(xsum(p[i] * x[i] for i in I))

    m += xsum(w[i] * x[i] for i in I) <= capacity

    m.optimize()

    result = pd.DataFrame()
    result['weight'] = w
    result['profit'] = p
    result['take'] = [1 if x[i].x > 0.99 else 0 for i in I]

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










