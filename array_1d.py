#!/usr/bin/env python3
"""Generate array of integer numbers

Usage:
  array_1d.py <size> (const | linear | square) [-o FILE]
  array_1d.py <size> uniform [--min=<min>] [--max=<max>] [-o FILE]
  array_1d.py <size> normal <mean> <std> [--min=<min>] [--max=<max>] [-o FILE]
  array_1d.py (-h | --help)

Options:
  -h --help            Show help.
  -o FILE              Output csv file.
  --min=<min>          Min value, [default: 1].
  --max=<max>          Max value, [default: 1000].
"""
from docopt import docopt
import numpy as np
import sys


def linear(size):
    return np.arange(1, size+1, 1)

def square(size):
    return np.square(np.arange(1, size+1, 1))

def const(size):
    return np.full((size), 1)

def uniform(size, min, max):
    return np.random.uniform(min, max, size)

def normal(size, mean, std, min, max):
    return np.clip(np.random.normal(mean, std, size), min, max)


if __name__ == '__main__':
    args = docopt(__doc__)

    size = int(args['<size>'])
    if size <= 0:
        print('<size> should be greater than 0')
        exit(1)

    data = None
    if args['linear']:
        data = linear(size)
    elif args['const']:
        data = const(size)
    elif args['square']:
        data = square(size)
    elif args['uniform']:
        min = int(args['--min'])
        max = int(args['--max'])
        data = uniform(size, min, max)
    elif args['normal']:
        mean = int(args['<mean>'])
        std = int(args['<std>'])
        min = int(args['--min'])
        max = int(args['--max'])
        data = normal(size, mean, std, min, max)

    d = [int(x) for x in data]

    output = args['-o']
    if output is None:
        output = stdout

    np.savetxt(output, d, delimiter=',', fmt='%d')




