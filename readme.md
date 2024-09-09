# Tesi di Laurea Triennale

## Titolo

Implementazione in C++ dell'algoritmo del simplesso primale con soluzioni esatte usando i numeri razionali.

## Descrizione

TODO

## Setup

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

Se si vuole usare un tipo `(DOUBLE, FLOAT, RATIONAL_INT, DEFAULT)` diverso usare la flag `-DDUSE_TYPE=DOUBLE`
```bash
cmake -DUSE_TYPE=DOUBLE CMakeLists.txt
make
```
Gli eseguiti verranno prodotti nella cartella `out/`.

## Solve PL

Per risolvere un problema di PL della forma
$$ \min c^T x \quad \text{s.t.} \quad Ax = b, x \geq 0 $$

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
./out/solvePL testcases/test1.txt
```
Il programma avvierà prima il simplesso primale ausiliario per trovare una base ammissibile e poi il simplesso primale e ritornerà la soluzione ottima.

## Klee-Minty

Script in python per generare i testcases di Klee-Minty
```bash
python3 testcases/klee_minty.py n
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
  - [X] Scegliere le librerie da usare -> **Eigen, tcb/rational**
- [X] Implementazione l'algoritmo del simplesso
  - [X] setup/cMake per Eigen e tcb/rational
  - [X] Implementazione pSimplex
  - [X] Implementazione pSimplexAux
  - [X] Rendere il codice funzionante per i numeri razionali/double
- [ ] Testing e applicazioni (confronto con la versione floating point)
  - [X] Implementazione solvePL per testcase
  - [X] Testcase da prove d'esame di Ricerca Operativa
  - [X] Implementazione Klee-Minty per generare testcase
- [ ] Relazione / Presentazione
