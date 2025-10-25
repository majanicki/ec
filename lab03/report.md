---
geometry: margin=30mm
...

# Report Laboratories 2

Maciej Janicki 156073
Jakub Kubiak 156049

[Source code](https://github.com/majanicki/ec/tree/trunk/lab02)

# Problem Description

Given a set of nodes, each having a set of coordinates ($x$, $y$) and inherent cost, pick exactly half of the nodes to form a Hamiltonian cycle.

The goal is to minimze the sum of total path length plus the total cost of the selected nodes.

# Pseudocode



# Result Comparison

## TSPA

| Algorithm                                   | Cost (Mean (Min, Max))     |
| ------------------------------------------- | -------------------------- |
| Random                                      | 265,135 (241,347, 291,966) |
| Nearest Neighbor #1                         | 85,108.5 (83,182, 89,433)  |
| Nearest Neighbor #2                         | 73,601.7 (71,868, 75,953)  |
| Greedy Cycle                                | 72,646.4 (71,488, 74,410)  |
| Nearest Neighbor Regret                     | 117,516 (107,945, 128,071) |
| Greedy Cycle Regret                         | 115,137 (106,052, 123,750) |
| Nearest Neighbor Regret Weighted (0.5, 0.5) | 73,566.7 (70,894, 75,929)  |
| Greedy Cycle Regret Weighted (0.5, 0.5)     | 72,137.6 (71,108, 73,395)  |
| Nearest Neighbor Regret Weighted (0.3, 0.7) | 75,562.1 (72,155, 78,661)  |
| Greedy Cycle Regret Weighted (0.3, 0.7)     | 73,010.6 (70,475, 75,365)  |
| Nearest Neighbor Regret Weighted (0.7, 0.3) | 73,412.8 (71,519, 75,234)  |
| Greedy Cycle Regret Weighted (0.7, 0.3)     | 72,437.2 (71,163, 73,759)  |

## TSPB

| Algorithm                                   | Cost (Mean (Min, Max))     |
| ------------------------------------------- | -------------------------- |
| Random                                      | 213,771 (190,834, 241,363) |
| Nearest Neighbor #1                         | 54,390.4 (52,319, 59,030)  |
| Nearest Neighbor #2                         | 48,723.2 (44,609, 57,315)  |
| Greedy Cycle                                | 51,400.6 (49,001, 57,324)  |
| Nearest Neighbor Regret                     | 73,512.7 (67,345, 78,889)  |
| Greedy Cycle Regret                         | 73,316.1 (67,729, 77,498)  |
| Nearest Neighbor Regret Weighted (0.5, 0.5) | 49,847 (44,901, 57,036)    |
| Greedy Cycle Regret Weighted (0.5, 0.5)     | 50,827.2 (47,144, 55,700)  |
| Nearest Neighbor Regret Weighted (0.3, 0.7) | 51,784.2 (47,543, 59,409)  |
| Greedy Cycle Regret Weighted (0.3, 0.7)     | 52,420.1 (49,944, 56,179)  |
| Nearest Neighbor Regret Weighted (0.7, 0.3) | 48,816.6 (45,153, 53,425)  |
| Greedy Cycle Regret Weighted (0.7, 0.3)     | 51,173.3 (49,326, 53,437)  |


\newpage

## Execution Time

| Algorithm                             | Time (Mean (Min, Max)) [ms] |
| Greedy + Intra Nodes + Random Start   |       47.52 (18.46, 117.33) |
| Greedy + Intra Nodes + Greedy Start   |        1.15 ( 0.38,   4.16) |
| Greedy + Intra Edges + Random Start   |       64.23 (30.26,  95.29) |
| Greedy + Intra Edges + Greedy Start   |        2.18 ( 0.57,   4.25) |
| Steepest + Intra Nodes + Random Start |       13.11 (10.19,  17.17) |
| Steepest + Intra Nodes + Greedy Start |        0.37 (0.08,    0.93) |
| Steepest + Intra Edges + Random Start |       11.21 (8.76,   18.02) |
| Steepest + Intra Edges + Greedy Start |        0.89 (0.22,    1.60) |

\newpage
