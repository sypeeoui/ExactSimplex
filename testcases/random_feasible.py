#!/usr/bin/env python3
import argparse
import numpy as np

def generate_random_feasible_lp_integers(n, m, max_val):
    # Genero un problema di PL casuale con valori interi
    product_len = (n * m)**2
    if max_val == -1:
        min_val = -product_len
        max_val = product_len
    else:
        min_val = -max_val
    # print(f"Min val: {min_val}, Max val: {max_val}")
    
    # Genero un vettore x interi casuali nell'intervallo
    x = np.random.randint(min_val, max_val + 1, size=n)

    # Genero una matrice A con valori interi casuali nell'intervallo
    A = np.random.randint(min_val, max_val + 1, size=(m, n))

    # Calcolo il vettore b per garantire che il problema sia ammissibile
    b = A @ x
    # Aggiungo rumore a b
    noised_indices = np.random.choice(m, m - n, replace=False)
    noise = np.random.randint(0, max_val**2, size=m - n)
    b[noised_indices] += noise
    
    # Genero un vettore c con valori interi casuali nell'intervallo
    c = np.random.randint(min_val, max_val + 1, size=n)

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
    parser.add_argument("max_val", type=int, nargs='?', default=-1, help="Maximum value for the random coefficients")
    args = parser.parse_args()
    
    c, b, A, max_iterations = generate_random_feasible_lp_integers(args.n, args.m, args.max_val)
    print_problem(c, b, A, max_iterations)
