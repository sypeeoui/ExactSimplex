#!/usr/bin/env python3
import argparse
import numpy as np

def klee_minty_problem(n, k):
    # Vector c
    c = np.array([0]*(n-1) + [1])

    if k == -1:
        k = 10
    # Vector b
    b = np.array([-1] + [0]*(n-1) + [k-1] + [k]*(n-1))

    # Matrix A
    A = np.zeros((2*n, n))
    for i in range(n):
        A[i, i] = -k
        if (i > 0):
            A[i, i-1] = 1

        A[n+i, i] = k
        if (i > 0):
            A[n+i, i-1] = 1
    
    max_iterations = 2**n - 1
    
    return c, b, A, max_iterations

def print_problem(n, k):
    c, b, A, max_iterations = klee_minty_problem(n, k)
    
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
    parser.add_argument("n", type=int, help="Dimension of the problem")
    parser.add_argument("k", type=int, nargs='?', default=-1, help="factor k")

    args = parser.parse_args()

    print_problem(args.n, args.k)
