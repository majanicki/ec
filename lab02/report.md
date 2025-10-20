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

## Nearest Neighbor (extended) Regret

```
FUNCTION nearest_neighbor_regret(dataset, dist, start):
    target_size := ceil(dataset.size / 2)
    result := [dataset[start]]
    used := {start}

    WHILE result.size < target_size:
        max_regret, best_node, best_pos := -INFINITY, null, 0

        FOR each node IN dataset IF not used:
            min_cost, second_min_cost, insert_pos := INF, INF, 0

            FOR i from -1 to result.size - 1:
                cost :=
                    if i == -1: dist(node, result[0])
                    else if i == result.size - 1: dist(result[i], node)
                    else: dist(result[i], node) 
                          + dist(node, result[i+1]) 
                          - dist(result[i], result[i+1])

                if cost < min_cost:
                    second_min_cost := min_cost
                    min_cost := cost
                    insert_pos := i
                else if cost < second_min_cost:
                    second_min_cost := cost

            regret := second_min_cost - min_cost
            if regret > max_regret:
                max_regret := regret
                best_node := node
                best_pos := insert_pos

        INSERT best_node into result at best_pos + 1
        ADD best_node to used

    RETURN result
```

\newpage

## Nearest Neighbor (extended) Regret weighted

```
FUNCTION nearest_neighbor_regret(dataset, dist, start, weight_cost, weight_regret):
    target_size := ceil(dataset.size / 2)
    result := [dataset[start]]
    used := {start}

    WHILE result.size < target_size:
        max_weighted_sum, best_node, best_pos := -INFINITY, null, 0

        FOR each node IN dataset IF not used:
            min_cost, second_min_cost, insert_pos := INF, INF, 0

            FOR i from -1 to result.size - 1:
                cost :=
                    if i == -1: dist(node, result[0])
                    else if i == result.size - 1: dist(result[i], node)
                    else: dist(result[i], node) 
                          + dist(node, result[i+1]) 
                          - dist(result[i], result[i+1])


                if cost < min_cost:
                    second_min_cost := min_cost
                    min_cost := cost
                    insert_pos := i
                else if cost < second_min_cost:
                    second_min_cost := cost

            regret := second_min_cost - min_cost
            weighted_sum := -min_cost * weight_cost + regret * weight_regret
            if weighted_sum > max_weighted_sum:
                max_weighted_sum := weighted_sum
                best_node := node
                best_pos := insert_pos

        INSERT best_node into result at best_pos + 1
        ADD best_node to used

    RETURN result
```

\newpage

## Greedy Cycle Regret

```
FUNCTION greedy_cycle_regret(dataset, dist, start):
    target_size := ceil(dataset.size / 2)
    result := [dataset[start]]
    used := {start}

    WHILE size of result < target_size:
        max_regret, best_node, best_pos := -INFINITY, null, 0

        FOR each node IN dataset IF not used:
            min_cost, second_min_cost, insert_pos := INFINITY, INFINITY, 0

            FOR EACH position in result:
                current_node := result[position]
                next_node := result[(position + 1) MOD result.size]

                cost_current_edge := dist(current_node, next_node)
                cost_new_edge := dist(current_node, dataset[node]) + dist(dataset[node], next_node)
                cost := cost_new_edge - cost_current_edge

                IF cost < min_cost THEN:
                    second_min_cost := min_cost
                    min_cost := cost
                    insert_pos := position
                ELSE IF cost < second_min_cost THEN:
                    second_min_cost := cost
            regret := second_min_cost - min_cost

            IF regret >= max_regret THEN
                max_regret := regret
                best_node := dataset[node]
                best_pos := insert_pos

        INSERT best_node into result at position best_pos + 1
        ADD best_pos TO used
    END WHILE

    RETURN result
END FUNCTION
```

\newpage

## Greedy Cycle Regret Weighted

```
FUNCTION greedy_cycle_regret_weighted(dataset, dist, start, weight_cost, weight_regret):
    target_size := ceil(dataset.size / 2)
    result := [dataset[start]]
    used := {start}

    WHILE size of result < target_size:
        best_weighted_sum, best_node, best_pos := -INFINITY, null, 0

        FOR each node IN dataset IF not used:
            min_cost, second_min_cost, insert_pos := INFINITY, INFINITY, 0

            FOR EACH position in result:
                current_node := result[position]
                next_node := result[(position + 1) MOD result.size]

                cost_current_edge := dist(current_node, next_node)
                cost_new_edge := dist(current_node, dataset[node]) + dist(dataset[node], next_node)
                cost := cost_new_edge - cost_current_edge

                IF cost < min_cost THEN:
                    second_min_cost := min_cost
                    min_cost := cost
                    insert_pos := position
                ELSE IF cost < second_min_cost THEN:
                    second_min_cost := cost

            regret := second_min_cost - min_cost
            weighted_sum += -(min_cost * weight_cost) + (regret * weight_regret)

            IF weighted_sum >= best_weighted_sum THEN
                best_weighted_sum := weighted_sum
                best_node := dataset[node]
                best_pos := insert_pos

        INSERT best_node into result at position best_pos + 1
        ADD best_pos TO used
    END WHILE

    RETURN result
END FUNCTION
```

\newpage

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

# Visualizations

## TSPA

### Nearest Neighbor Regret

67, 108, 18, 199, 22, 103, 195, 159, 193, 96, 42, 181, 160, 48, 147, 177, 10, 190, 4, 112, 126, 29, 127, 194, 135, 6, 154, 173, 133, 151, 45, 161, 123, 24, 149, 77, 116, 197, 59, 72, 51, 141, 66, 176, 80, 122, 63, 182, 121, 100, 97, 101, 75, 2, 120, 44, 25, 78, 16, 171, 113, 50, 188, 157, 17, 196, 91, 57, 172, 52, 106, 185, 40, 174, 90, 27, 71, 164, 7, 21, 14, 102, 144, 132, 73, 64, 114, 186, 23, 89, 183, 153, 170, 117, 93, 36, 68, 46, 198, 115, 67

![Visualization of Nearest Neighbor Regret best solution for TSPA](nearest_neighbor_regret_a.png)

\newpage

### Greedy Cycle Regret

70, 127, 123, 24, 156, 112, 4, 190, 10, 177, 104, 54, 48, 160, 34, 103, 146, 22, 20, 199, 69, 108, 67, 36, 140, 93, 117, 143, 153, 183, 89, 23, 83, 64, 15, 62, 37, 111, 172, 129, 92, 57, 55, 3, 138, 14, 144, 132, 21, 7, 164, 71, 27, 90, 39, 8, 119, 174, 81, 196, 17, 157, 38, 85, 113, 175, 171, 16, 78, 25, 44, 120, 2, 75, 101, 26, 121, 182, 63, 122, 80, 176, 66, 141, 60, 46, 198, 115, 96, 42, 43, 77, 65, 197, 59, 72, 151, 133, 173, 135, 70

![Visualization of Greedy Cycle Regret best solution for TSPA](greedy_cycle_regret_a.png)

\newpage

### Nearest Neighbor Regret Weighted

#### 1. Cost: 0.5, Regret: 0.5

36, 140, 93, 68, 46, 139, 115, 118, 59, 65, 116, 43, 42, 5, 41, 193, 159, 69, 108, 18, 22, 146, 181, 34, 160, 48, 54, 177, 184, 35, 84, 4, 112, 127, 123, 162, 151, 133, 135, 70, 154, 180, 53, 121, 100, 26, 86, 75, 101, 1, 97, 152, 2, 120, 44, 25, 16, 171, 175, 113, 56, 31, 78, 145, 179, 92, 129, 57, 55, 52, 185, 40, 196, 81, 90, 39, 165, 106, 178, 14, 49, 102, 144, 62, 9, 148, 124, 94, 63, 79, 80, 176, 137, 23, 186, 89, 183, 143, 0, 117, 36

![Visualization of Nearest Neighbor Regret Weighted best solution for TSPA](nearest_neighbor_regret_weighted_a.png)

\newpage

#### 2. Cost: 0.3, Regret: 0.7

28, 184, 160, 34, 181, 42, 43, 116, 65, 59, 118, 115, 46, 68, 139, 41, 193, 159, 146, 22, 20, 18, 108, 140, 93, 117, 0, 143, 183, 89, 23, 186, 114, 15, 148, 9, 62, 144, 14, 49, 178, 106, 52, 55, 185, 40, 165, 39, 95, 21, 7, 164, 71, 27, 90, 81, 196, 179, 145, 78, 31, 113, 175, 171, 16, 25, 44, 120, 92, 57, 129, 2, 75, 101, 1, 152, 97, 26, 86, 53, 180, 154, 70, 135, 133, 79, 63, 94, 80, 176, 51, 151, 162, 123, 112, 4, 190, 10, 177, 54, 28

![Visualization of Nearest Neighbor Regret Weighted best solution for TSPA](nearest_neighbor_regret_weighted_a_03_07.png)

\newpage

#### 3. Cost: 0.7, Regret: 0.3

199, 69, 108, 18, 22, 146, 159, 193, 41, 139, 68, 46, 115, 5, 42, 181, 34, 160, 48, 54, 177, 10, 190, 4, 112, 84, 184, 43, 116, 65, 59, 118, 51, 151, 133, 162, 123, 127, 70, 135, 180, 154, 53, 100, 26, 86, 75, 44, 25, 16, 171, 175, 113, 56, 31, 78, 145, 92, 57, 179, 40, 196, 81, 90, 165, 185, 106, 178, 14, 144, 62, 9, 148, 102, 49, 52, 55, 129, 120, 2, 101, 1, 97, 152, 124, 94, 63, 79, 80, 176, 137, 23, 186, 89, 183, 143, 0, 117, 93, 140, 199

![Visualization of Nearest Neighbor Regret Weighted best solution for TSPA](nearest_neighbor_regret_weighted_a_07_03.png)

\newpage

### Greedy Cycle Regret Weighted

#### 1. Cost: 0.5, Regret: 0.5

0, 46, 68, 139, 193, 41, 115, 5, 42, 181, 159, 69, 108, 18, 22, 146, 34, 160, 48, 54, 177, 10, 190, 4, 112, 84, 184, 43, 116, 65, 59, 118, 51, 151, 133, 162, 123, 127, 70, 135, 154, 180, 53, 121, 100, 26, 86, 75, 44, 25, 16, 171, 175, 113, 56, 31, 78, 145, 179, 196, 81, 90, 40, 165, 185, 106, 178, 138, 14, 144, 62, 9, 148, 102, 49, 52, 55, 92, 57, 129, 82, 120, 2, 101, 1, 97, 152, 124, 94, 63, 79, 80, 176, 137, 23, 186, 89, 183, 143, 117, 0

![Visualization of Greedy Cycle Regret Weighted best solution for TSPA](greedy_cycle_regret_weighted_a.png)

\newpage

#### 2. Cost: 0.3, Regret: 0.7

186, 89, 183, 143, 0, 117, 93, 140, 68, 46, 115, 139, 193, 41, 159, 108, 18, 22, 146, 181, 34, 160, 48, 54, 177, 10, 190, 4, 112, 84, 35, 184, 42, 43, 116, 65, 59, 118, 51, 151, 133, 162, 123, 127, 70, 135, 154, 180, 53, 121, 100, 26, 86, 75, 44, 120, 25, 16, 171, 175, 113, 56, 31, 78, 145, 179, 196, 81, 90, 40, 165, 185, 106, 178, 138, 14, 144, 62, 9, 148, 167, 49, 52, 55, 57, 92, 129, 2, 101, 1, 97, 152, 124, 94, 63, 79, 80, 176, 137, 23, 186

![Visualization of Greedy Cycle Regret Weighted best solution for TSPA](greedy_cycle_regret_weighted_a_03_07.png)

\newpage

#### 3. Cost: 0.7, Regret: 0.3

23, 137, 186, 114, 15, 9, 62, 144, 14, 138, 165, 40, 185, 90, 81, 196, 179, 145, 78, 31, 56, 113, 175, 171, 16, 25, 44, 120, 2, 129, 92, 57, 55, 52, 106, 178, 49, 102, 148, 124, 94, 152, 97, 1, 101, 75, 86, 26, 100, 53, 154, 70, 135, 180, 63, 79, 133, 127, 123, 162, 151, 80, 176, 51, 118, 59, 65, 116, 43, 184, 84, 112, 4, 190, 10, 177, 54, 48, 160, 34, 146, 22, 18, 108, 69, 159, 181, 42, 5, 115, 41, 193, 139, 68, 46, 0, 117, 143, 183, 89, 23

![Visualization of Greedy Cycle Regret Weighted best solution for TSPA](greedy_cycle_regret_weighted_a_07_03.png)

\newpage

## TSPB

### Nearest Neighbor Regret

93, 75, 137, 127, 103, 163, 129, 180, 88, 194, 166, 86, 110, 181, 83, 62, 124, 106, 143, 35, 0, 39, 160, 33, 138, 182, 25, 123, 177, 171, 21, 82, 68, 111, 41, 14, 81, 153, 146, 97, 141, 91, 79, 36, 7, 5, 78, 46, 190, 105, 31, 151, 117, 30, 156, 1, 16, 38, 92, 63, 100, 40, 17, 44, 10, 178, 147, 71, 120, 51, 98, 118, 74, 2, 134, 6, 65, 132, 13, 145, 15, 70, 84, 155, 184, 53, 174, 183, 140, 149, 28, 59, 20, 60, 148, 47, 94, 57, 52, 99, 93

![Visualization of Nearest Neighbor Regret best solution for TSPB](nearest_neighbor_regret_b.png)

\newpage

### Greedy Cycle Regret

113, 114, 137, 75, 93, 48, 166, 52, 172, 57, 66, 47, 60, 20, 59, 28, 199, 140, 183, 95, 185, 86, 110, 128, 124, 62, 83, 55, 34, 170, 53, 184, 155, 84, 3, 15, 145, 195, 132, 65, 6, 192, 147, 134, 43, 139, 11, 138, 182, 118, 158, 121, 131, 125, 191, 115, 10, 133, 17, 107, 100, 63, 102, 27, 197, 24, 42, 196, 30, 117, 151, 164, 105, 80, 46, 78, 5, 123, 177, 7, 36, 91, 21, 82, 8, 56, 33, 39, 35, 37, 111, 50, 81, 77, 97, 146, 187, 186, 163, 176, 113

![Visualization of Greedy Cycle Regret best solution for TSPB](greedy_cycle_regret_b.png)

\newpage

### Nearest Neighbor Regret Weighted

#### Cost: 0.5, Regret: 0.5

105, 190, 80, 162, 175, 78, 5, 177, 25, 182, 138, 139, 11, 29, 109, 35, 0, 160, 33, 144, 104, 8, 82, 21, 36, 61, 91, 141, 77, 81, 153, 187, 163, 89, 127, 103, 113, 176, 194, 166, 86, 185, 99, 22, 179, 172, 57, 66, 94, 47, 148, 60, 20, 28, 149, 4, 140, 183, 130, 95, 128, 106, 124, 62, 18, 55, 34, 170, 152, 184, 155, 3, 70, 15, 145, 195, 168, 13, 132, 169, 188, 6, 147, 90, 51, 121, 131, 122, 135, 63, 38, 27, 16, 1, 156, 198, 117, 54, 31, 193, 105

![Visualization of Nearest Neighbor Regret Weighted best solution for TSPB](nearest_neighbor_regret_weighted_b.png)

\newpage

#### Cost: 0.3, Regret: 0.7

65, 188, 169, 132, 13, 70, 3, 15, 145, 195, 168, 139, 134, 6, 192, 147, 115, 10, 133, 122, 72, 107, 40, 63, 102, 135, 131, 90, 51, 121, 112, 19, 151, 117, 193, 31, 54, 73, 136, 190, 80, 162, 175, 78, 5, 177, 36, 61, 91, 141, 77, 81, 153, 187, 163, 89, 127, 103, 113, 176, 194, 166, 86, 95, 185, 179, 172, 57, 66, 94, 47, 148, 60, 20, 59, 28, 149, 4, 140, 183, 152, 170, 34, 55, 18, 62, 124, 106, 143, 35, 109, 0, 29, 160, 33, 138, 104, 8, 82, 21, 65

![Visualization of Nearest Neighbor Regret Weighted best solution for TSPB](nearest_neighbor_regret_weighted_b_03_07.png)

\newpage

#### Cost: 0.7, Regret: 0.3

150, 147, 6, 188, 169, 132, 13, 70, 3, 15, 145, 195, 168, 43, 139, 11, 138, 33, 160, 29, 0, 109, 35, 143, 106, 124, 62, 18, 55, 34, 170, 152, 183, 140, 4, 149, 28, 20, 60, 148, 47, 94, 66, 179, 185, 99, 130, 95, 86, 166, 194, 176, 113, 103, 127, 89, 163, 187, 153, 81, 77, 141, 91, 61, 36, 21, 82, 111, 8, 104, 177, 5, 45, 142, 78, 175, 162, 80, 190, 136, 73, 193, 31, 54, 117, 198, 156, 1, 131, 121, 51, 90, 10, 133, 122, 135, 63, 100, 107, 40, 150

![Visualization of Nearest Neighbor Regret Weighted best solution for TSPB](nearest_neighbor_regret_weighted_b_07_03.png)

\newpage

### Greedy Cycle Regret Weighted

#### Cost: 0.5, Regret: 0.5

199, 183, 140, 95, 130, 99, 22, 179, 185, 86, 166, 194, 113, 176, 26, 103, 114, 137, 127, 89, 163, 187, 153, 81, 77, 141, 91, 61, 36, 175, 78, 142, 45, 5, 177, 21, 82, 111, 8, 104, 138, 182, 139, 168, 195, 145, 15, 3, 70, 13, 132, 169, 188, 6, 147, 115, 10, 133, 122, 63, 135, 38, 1, 117, 193, 31, 54, 131, 90, 51, 121, 118, 74, 134, 11, 33, 160, 29, 0, 109, 35, 143, 106, 124, 128, 62, 18, 55, 34, 170, 152, 4, 149, 28, 20, 60, 94, 66, 47, 148, 199

![Visualization of Greedy Cycle Regret Weighted best solution for TSPB](greedy_cycle_regret_weighted_b.png)

\newpage

#### Cost: 0.3, Regret: 0.7

56, 144, 111, 50, 77, 81, 153, 146, 187, 163, 89, 127, 103, 26, 113, 176, 194, 166, 86, 185, 179, 172, 57, 66, 94, 47, 148, 60, 20, 59, 28, 149, 4, 140, 183, 9, 99, 130, 95, 128, 55, 34, 18, 62, 124, 106, 143, 35, 0, 109, 29, 160, 33, 49, 11, 134, 74, 118, 98, 51, 90, 121, 131, 151, 54, 31, 193, 117, 198, 156, 1, 38, 135, 102, 63, 40, 107, 72, 122, 133, 10, 115, 147, 192, 6, 188, 169, 132, 13, 70, 3, 15, 145, 195, 168, 139, 182, 138, 104, 8, 56

![Visualization of Greedy Cycle Regret Weighted best solution for TSPB](greedy_cycle_regret_weighted_b_03_07.png)

\newpage

#### Cost: 0.7, Regret: 0.3

199, 183, 140, 95, 130, 99, 22, 179, 185, 86, 166, 194, 113, 176, 180, 26, 103, 114, 137, 127, 89, 163, 165, 187, 153, 97, 141, 77, 81, 111, 82, 21, 61, 36, 5, 177, 8, 104, 138, 182, 139, 168, 195, 145, 15, 3, 70, 161, 13, 132, 169, 188, 6, 147, 71, 191, 90, 178, 10, 133, 122, 100, 107, 40, 63, 102, 135, 131, 121, 51, 98, 118, 74, 134, 11, 33, 160, 29, 0, 109, 35, 143, 106, 124, 128, 62, 18, 55, 34, 170, 152, 4, 149, 28, 20, 60, 94, 66, 47, 148, 199

![Visualization of Greedy Cycle Regret Weighted best solution for TSPB](greedy_cycle_regret_weighted_b_07_03.png)

Solutions have been checked with solution checker.

# Conclusions

- In case of our problem (finding TSP solution containing 50% of nodes) pure regret heuristic tends to unecessarily include nodes with high cost when the "regretted" node could be ommitted entirely. 

- Including a regret measure (especially properly weighted) helps in avoiding short-sighted decision. Nodes that would later create expensive insertions are avoided.

- Greedy Cycle Regret slightly outperforms Extended Nearest Neighbor Regret on average across both datasets.

- Weighted methods perform better than their unweighted counterparts.

- Giving higher weight to node insertion cost than to regret provides better results. 
