#!/usr/bin/env python3
import argparse
import numpy as np

def generate_random_feasible_lp_integers(n, m):
    # Calculate the range based on the product of len(c) * len(b)
    product_len = (n * m)**2
    min_val = -product_len
    max_val = product_len
    
    # Generate a random feasible solution x with integer values within the range
    x = np.random.randint(min_val, max_val + 1, size=n)

    # Generate a random matrix A (m x n) with integer values within the range
    A = np.random.randint(min_val, max_val + 1, size=(m, n))

    # Calculate b as A * x to ensure feasibility
    b = A @ x
    # Add noise to m - n elements of b 
    noised_indices = np.random.choice(m, m - n, replace=False)
    noise = np.random.randint(0, max_val**2, size=m - n)
    b[noised_indices] += noise
    
    # Generate random objective function coefficients c with integer values within the range
    c = np.random.randint(min_val, max_val + 1, size=n)
    # for i in range(n):
    #     c[i] = c[i] * np.sign(x[i])

    # Maximum number of iterations (arbitrary for random LP)
    max_iterations = n*m

    return c, b, A, max_iterations

def print_problem(c, b, A, max_iterations):
    # Print lengths of c and b
    print(len(c))
    print(len(b))
    
    # Print vector b
    print(" ".join(map(str, c)))
    
    # Print matrix A row by row
    for row in A:
        print(" ".join(map(str, row)))
    
    # Print vector c
    print(" ".join(map(str, b)))
    
    # Print maximum iterations
    print(max_iterations)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate a random feasible LP problem with integer values based on len(c) * len(b)")
    parser.add_argument("n", type=int, help="Number of variables (dimension of x)")
    parser.add_argument("m", type=int, help="Number of constraints (number of rows in A)")
    args = parser.parse_args()
    
    c, b, A, max_iterations = generate_random_feasible_lp_integers(args.n, args.m)
    print_problem(c, b, A, max_iterations)
