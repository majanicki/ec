---
geometry: margin=10mm
fontsize: 4pt
...

# Report Laboratories 4

Maciej Janicki 156073
Jakub Kubiak 156049

[Source code](https://github.com/majanicki/ec/tree/trunk/lab04)

# Problem Description

Given a set of nodes, each having a set of coordinates ($x$, $y$) and inherent cost, pick exactly half of the nodes to form a Hamiltonian cycle.

The goal is to minimze the sum of total path length plus the total cost of the selected nodes.

Current implementation is about improving the efficiency of the steepest local search with the use of candidate moves.

# Pseudocode

## Candidate moves


# Result Comparison

| Algorithm                                                | TSPA Cost (Mean (Min, Max))      | TSPB Cost (Mean (Min, Max)) |
| -------------------------------------------------------- | -------------------------------- | -------------------------- |
| Random                                                   | 265,135 (241,347, 291,966)       | 213,771 (190,834, 241,363) |
| Nearest Neighbor #1                                      | 85,108.5 (83,182, 89,433)        | 54,390.4 (52,319, 59,030)  |
| Nearest Neighbor #2                                      | 73,601.7 (71,868, 75,953)        | 48,723.2 (44,609, 57,315)  |
| Greedy Cycle                                             | 72,646.4 (71,488, 74,410)        | 51,400.6 (49,001, 57,324)  |
| Nearest Neighbor Regret                                  | 117,516 (107,945, 128,071)       | 73,512.7 (67,345, 78,889)  |
| Greedy Cycle Regret                                      | 115,137 (106,052, 123,750)       | 73,316.1 (67,729, 77,498)  |
| Nearest Neighbor Regret Weighted (0.5, 0.5)              | 73,566.7 (70,894, 75,929)        | 49,847 (44,901, 57,036)    |
| Greedy Cycle Regret Weighted (0.5, 0.5)                  | 72,137.6 (71,108, 73,395)        | 50,827.2 (47,144, 55,700)  |
| Nearest Neighbor Regret Weighted (0.3, 0.7)              | 75,562.1 (72,155, 78,661)        | 51,784.2 (47,543, 59,409)  |
| Greedy Cycle Regret Weighted (0.3, 0.7)                  | 73,010.6 (70,475, 75,365)        | 52,420.1 (49,944, 56,179)  |
| Nearest Neighbor Regret Weighted (0.7, 0.3)              | 73,412.8 (71,519, 75,234)        | 48,816.6 (45,153, 53,425)  |
| Greedy Cycle Regret Weighted (0.7, 0.3)                  | 72,437.2 (71,163, 73,759)        | 51,173.3 (49,326, 53,437)  |
| Local Search Greedy + Intra Nodes + Random Start         | 83,804.35 (77,418, 90,972)       | 59,076.75 (52,869, 67,067) |
| Local Search Greedy + Intra Nodes + Greedy Start         | 72,806.49 (71,034, 74,987)       | 45,468.38 (43,989, 51,040) |
| Local Search Greedy + Intra Edges + Random Start         | 73,387.63 (71,248, 78,480)       | 48,161.21 (45,907, 51,461) |
| Local Search Greedy + Intra Edges + Greedy Start         | 71,067.07 (70,046, 73,486)       | 45,053.77 (43,947, 50,319) |
| Local Search Steepest + Intra Nodes + Random Start       | 87,883.71 (79,593, 98,325)       | 62,947.23 (54,122, 71,598) |
| Local Search Steepest + Intra Nodes + Greedy Start       | 72,807.32 (71,034, 74,904)       | 45,414.50 (43,826, 50,876) |
| Local Search Steepest + Intra Edges + Random Start       | 73,938.93 (71,428, 77,903)       | 48,323.99 (45,670, 51,667) |
| Local Search Steepest + Intra Edges + Greedy Start       | 70,975.96 (69,864, 73,068)       | 44,974.89 (43,921, 50,319) |
| Candidate Local Search Steepest (k=10)                   |                                  |                            |
| Candidate Local Search Steepest (k=5)                    |                                  |                            |
| Candidate Local Search Steepest (k=15)                   |                                  |                            |

\newpage

# Execution Time Comparison


| Algorithm                                          | TSPA Time (Mean (Min, Max)) [ms] | TSPB Time (Mean (Min, Max)) [ms] |
|--------------------------------------------------- | ---------------------------------| -------------------------------- |
| Local Search Greedy + Intra Nodes + Random Start   | 36.36 (16.00, 69.92)             | 35.84 (19.78, 59.54)             |
| Local Search Greedy + Intra Nodes + Greedy Start   | 2.49 (1.80, 5.13)                | 2.67 (1.85, 7.62)                |
| Local Search Greedy + Intra Edges + Random Start   | 48.81 (23.56, 70.97)             | 42.71 (22.79, 62.96)             |
| Local Search Greedy + Intra Edges + Greedy Start   | 3.28 (1.95, 5.24)                | 2.92 (1.77, 7.81)                |
| Local Search Steepest + Intra Nodes + Random Start | 21.89 (16.96, 31.65)             | 22.20 (17.60, 29.57)             |
| Local Search Steepest + Intra Nodes + Greedy Start | 2.10 (1.55, 3.00)                | 2.31 (1.81, 4.95)                |
| Local Search Steepest + Intra Edges + Random Start | 15.45 (13.59, 19.09)             | 15.45 (13.17, 17.90)             |
| Local Search Steepest + Intra Edges + Greedy Start | 2.78 (1.83, 3.85)                | 2.36 (1.86, 4.88)                |
| Candidate Local Search Steepest (k=10)             |                                  |                                  |
| Candidate Local Search Steepest (k=5)              |                                  |                                  |
| Candidate Local Search Steepest (k=15)             |                                  |                                  |


\newpage

# Visualizations

## TSPA

### Steepest Local Search + Intra Edges + Random Start

![](baseline_steepest_local_search_a.png)

### Candidate Moves approach (k=10)

![](candidate_steepest_local_search_10_a.png)

### Candidate Moves approach (k=5)

![](candidate_steepest_local_search_5_a.png)

### Candidate Moves approach (k=15)

![](candidate_steepest_local_search_15_a.png)

## TSPB

### Steepest Local Search + Intra Edges + Random Start

![](baseline_steepest_local_search_b.png)

### Candidate Moves approach (k=10)

![](candidate_steepest_local_search_10_b.png)

### Candidate Moves approach (k=5)

![](candidate_steepest_local_search_5_b.png)

### Candidate Moves approach (k=15)

![](candidate_steepest_local_search_15_b.png)


All solutions were checked with solution checker.

# Conclusions

- Evaluating only a subset of 'nearest' neighbors is enough to capture the good moves
- Candidate approach might be a good alternative for the greedy approaches, especially for larger instances, where the number of neighbors to evaluate is too large to be feasible.
