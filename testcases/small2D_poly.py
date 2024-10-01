#!/usr/bin/env python3
import argparse
import numpy as np

def small2D_poly(n, k):
    # Vector c
    c = np.array([1, 1])
    if k == -1:
        k = n//2
    # Vector b
    b = np.array([0, n*n, 0, n, 0])
    # Matrix A
    A = [
        [-1, 0],
        [-n*k +n +k, n*k],
        [0, -1],
        [-n + 1, n],
        [1, -1],
    ]
    max_iterations = 100
    return c, b, A, max_iterations


def print_problem(n, k):
    c, b, A, max_iterations = small2D_poly(n, k)
    
    # Printing the outputs in the desired format
    print(len(c))
    print(len(b))
    print(" ".join(map(str, c)))
    
    for row in A:
        row = list(map(int, row))
        print(" ".join(map(str, row)))
    
    print(" ".join(map(str, b)))
    print(max_iterations)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate the a problem given n")
    parser.add_argument("e", type=int, help="Dimension of the problem")
    parser.add_argument("k", type=int, nargs='?', default=-1, help="factor k")

    args = parser.parse_args()
    n = 10**args.e

    print_problem(n, args.k)
