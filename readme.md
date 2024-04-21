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

Metti il codice in mainFolder/
aggiungendo 
```c++
#include "Simplex.h"
```

Poi per compilare
```bash
cmake CMakeLists.txt
make
```
The executables will be in the out/ directory.

## Autore

Suqi Chen

## Università

Università di Pisa

## Dipartimento

Ingegneria informatica

## Relatore

Marco Cococcioni

## TODO

- [ ] Preliminari
  - [X] Leggere i paper
  - [X] Scegliere le librerie da usare -> **Eigen, tcb/rational**
  - [ ] 
- [ ] Implementazione l'algoritmo del simplesso
  - [X] setup/cMake per Eigen e tcb/rational
  - [ ] Definizione elementi della classe `Simplex`
  - [ ] Implementazione della funzione `solve`
  - [ ] 
  - [ ] Aggiunta numeri razionali
- [ ] Testing e applicazioni (confronto con la versione floating point)
- [ ] Relazione / Presentazione
