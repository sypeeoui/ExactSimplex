#!/usr/bin/env python3
import argparse
import numpy as np

def klee_minty_problem(n):
    # Vector c
    c = np.array([int(2**(n-i)) for i in range(1, n+1)])

    # Vector b
    b = np.array([5**i for i in range(1, n+1)] + [0]*n)

    # Matrix A
    A = np.zeros((2*n, n))
    for i in range(n):
        for j in range(i+1):
            A[i, j] = int(2**(i-j+1))

    for i in range(n):
        A[i, i] = 1
        A[n+i, i] = -1
    
    # Maximum number of iterations for simplex (2^n - 1)
    max_iterations = 2**n - 1
    
    return c, b, A, max_iterations

def print_problem(n):
    c, b, A, max_iterations = klee_minty_problem(n)
    
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
    parser = argparse.ArgumentParser(description="Generate the Klee-Minty LP problem for given n")
    parser.add_argument("n", type=int, help="Dimension of the Klee-Minty problem")
    args = parser.parse_args()

    print_problem(args.n)
