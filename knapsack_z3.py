#!/usr/bin/env python3

import pandas as pd
import sys
from io import StringIO
from z3 import *


def knapsack_01(df, capacity):
    I = range(len(df))
    p = [int(x) for x in df['profit']]
    w = [int(x) for x in df['weight']]
    c = int(capacity)

    x = [Int(f'x_{i}') for i in I]

    s = Optimize()

    for x_i in x:
        s.add(Or(x_i == 0, x_i == 1))
        # s.add(x_i >= 0)
        # s.add(x_i <= 1)

    s.add(Sum([x[i] * w[i] for i in I]) <= c)

    s.maximize(Sum([x[i] * p[i] for i in I]))

    r = s.check()

    if r != sat:
        print('Unsat')
        return None

    m = s.model()

    result = pd.DataFrame()
    result['weight'] = w
    result['profit'] = p
    result['take'] = [m.eval(x[i]).as_long() for i in I]

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










