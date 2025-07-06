# High Performance Branch Target Buffer for Dynamic Branch Prediction

Branch Target Buffer (BTB) with 2-bit saturating counter

## Overview

### What is a Branch Target Buffer (BTB)?
- Pipelined processors suffer a performance penalty for prefetching instructions that follow a taken branch
- A **branch target buffer** is a cache for mapping a branch instruction's address to the address it wants to branch to
- A dynamic branch prediction strategy lets each cache entry categorise itself at runtime for the likelihood of taking its branch

### Dynamic Branch Prediction & BTB Implementation
- **2-bit saturating counter** with four prediction states (Strongly Not Taken, Weakly Not Taken, Weakly Taken, Strongly Taken)
- **Branch handling**: Supports both invariant (B-type) and variant (R, D, M, L-type) branches
- **Doubly-linked list LRU ordering & hashmap** caching strategy and implemenation of BTB

### Performance Analysis (more in scripts/branch_prediction_report.pdf)
- **Configurable BTB sizes** from 8 to 1024+ entries
- **Comprehensive statistics** on hit and miss rates
- **CSV output support** for data visualisation and graphing

## Implementation

### BTB Design
The Branch Target Buffer implements a Least Recently Used (LRU) cache eviction strategy:

```
┌─────────────────┐    ┌──────────────────┐    ┌─────────────────┐
│   Hash Map      │    │  Doubly-Linked   │    │  2-bit Counter  │
│   (Fast Lookup) │◄──►│  List (LRU)      │◄──►│  (Prediction)   │
└─────────────────┘    └──────────────────┘    └─────────────────┘
```

- **O(1) average lookup** via std::unordered_map
- **O(1) LRU updates** via doubly-linked list manipulation
- **Automatic memory management** via C++ smart pointers

### Prediction Algorithm
1. **Initialisation**: Enter unseen branch instructions with WEAKLY_TAKEN
2. **Cache Hit**: Return predicted target if confidence = WEAKLY_TAKEN or STRONGLY_TAKEN
3. **Cache Miss**: Predict not taken and evict LRU entry
4. **Update**: Adjust 2-bit counter based on actual branch outcome

### Code
- **btb.cpp**: Implementation of branch target buffer with LRU cache & 2-bit counter updating logic
- **profiler.cpp**: Parses and processes profile of a program's runtime branch behaviour
- **main.cpp**: Simulates program execution to record performance improvement with BTB

## To compile and run:
Developed using **g++ 13.3.0** on **Linux Ubuntu 24.04**
```sh
g++ src/main.cpp src/btb.cpp src/profiler.cpp -o main
./main
```


### Sample Output
```
Branch Target Buffer Statistics:
Size: 32 entries
Hits: 317223
Misses: 279048
Hit Rate: 53.2011%
```