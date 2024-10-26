# Tesi di Laurea Triennale

## Titolo

Implementazione in C++ dell'algoritmo del simplesso primale con soluzioni esatte usando i numeri razionali.

## Setup

### GMPlib
Installa `gmplib`
```bash
sudo pacman -Sy lib32-gmp # ubuntu
```
```bash
sudo apt-get install libgmp3-dev # archlinux
```

### Repository
```bash
git clone git@github.com:sypeeoui/ExactSimplex.git
cd ExactSimplex
git submodule update --init --recursive
```

## Build

Metti il codice in `mainFolder/`
aggiungendo
```c++
#include "Simplex.h"
```

Poi per compilare
```bash
cmake CMakeLists.txt
make
```

Se si vuole usare un tipo `(DOUBLE, FLOAT, RATIONAL_INT, RATIONAL_LONG_LONG, MPQ, DEFAULT)` diverso usare la flag `-DDUSE_TYPE=MPQ`
```bash
cmake -DUSE_TYPE=DEFAULT CMakeLists.txt
make
```

nel caso di MPF è possibile specificare la precisione con la flag `-DPRECISION=256`
```bash
cmake -DUSE_TYPE=MPF -DPRECISION=256 CMakeLists.txt
make
```

Gli eseguiti verranno prodotti nella cartella `out/`.

## Solve PL

Per risolvere un problema di PL della forma
$$ \max c^T x \quad \text{vincolato da} \quad Ax \le b$$

Creare un file di testo contenente i seguenti parametri (solo numeri interi) in ordine separati da spazi o a capo
```
len(c)
len(b)
b
A
c
max_iterations
```

Eseguire con 
```bash
./out/solvePL < testcases/test1.txt
```
Il programma avvierà prima il simplesso primale ausiliario per trovare una base ammissibile e poi il simplesso primale e ritornerà la soluzione ottima.

## Klee-Minty

Script in python per generare i testcases di Klee-Minty
```bash
python3 testcases/klee_minty.py n
```

**Esempio**

```bash
python3 testcases/klee_minty.py 25 | ./out/solvePL
```

## Bland-Trick

Script in python per generare i testcases di complessità esponenziale
```bash
python3 testcases/bland_trick.py n
```

**Esempio**

```bash
python3 testcases/bland_trick.py 20 | ./out/solvePL
```

## Random Feasible

Script in python per generare un problema di PL casuale con soluzione ammissibile. Utile per creare testcase pesanti.
```bash
python3 testcases/random_feasible.py len(c) len(b)
```

**Esempio**

```bash
python3 testcases/random_feasible.py 10 35 | ./out/solvePL
```


## Autore

Suqi Chen

## Università

Università di Pisa

## Dipartimento

Ingegneria informatica

## Relatore

Marco Cococcioni

## TODO

- [X] Preliminari
  - [X] Leggere i paper
  - [X] Scegliere le librerie da usare -> **Eigen, tcb/rational, MPQ**
- [X] Implementazione l'algoritmo del simplesso
  - [X] setup/cMake per Eigen e tcb/rational
  - [X] Implementazione pSimplex
  - [X] Implementazione pSimplexAux
  - [X] Rendere il codice funzionante per i numeri razionali/double
  - [X] Uso di MPQ e MPF per implementare razionali/reali a precisione arbitraria
- [X] Testing e applicazioni (confronto con la versione floating point)
  - [X] Implementazione solvePL per testcase
  - [X] Testcase da prove d'esame di Ricerca Operativa
  - [X] Implementazione Klee-Minty per generare testcase
  - [X] Implementazione random_feasible per generare testcase pesanti

- [X] Relazione / Presentazione
