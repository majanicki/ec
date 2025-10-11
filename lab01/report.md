• Pseudocode of all implemented algorithms
• Results of a computational experiment: for each instance and method min, max and average
value of the objective function.
• 2D visualization of the best solution for each instance and method. Cost of nodes should be
presented e.g. by a color, greyscale, or size.
• The best solutions for each instance and method presented as a list of nodes indices (starting
from 0).
• Information whether the best solutions have been checked with the solution checker.
• (Link to) the source code
• Conclusions


# Problem Description
Given a set of nodes, each having a set of coordinates ($x$, $y$) and inherent cost, pick exactly half of the nodes to form a Hamiltonian cycle. 

The goal is to minimze the sum of total path length plus the total cost of the selected nodes. 

# Pseudocode
## Random solution
Simplest approach, where the cycle is formed by randomly selecting appropriate number of nodes in arbitrary order. 

```
FUNCTION random_solution(dataset):
    target_size := CEIL(size of dataset / 2)
    result := empty list
    visited := empty set

    WHILE size of result < target_size:
        rand_node := RANDOM node from dataset that is not in visited
        ADD rand_node TO result
        ADD rand_node TO visited
    END WHILE

    RETURN result
END FUNCTION
```

## Nearest neighbor #1

```
FUNCTION nearest_neighbor_end_only(dataset, dist):
    target_size := half of dataset size (round up)
    result := empty list
    visited := empty set

    start := random node
    ADD start TO result
    ADD start TO visited

    WHILE size of result < target_size:
        last := last node in result
        nearest := unvisited node closest to last
        ADD nearest TO result
        ADD nearest TO visited
    END WHILE

    RETURN result
END FUNCTION
```

## Nearest neighbor #2
```
FUNCTION nearest_neighbor_every_position(dataset, dist):
    target_size := half of dataset size (round up)
    result := empty list
    visited := empty set

    start := random node
    ADD start TO result
    ADD start TO visited
    last_added :=  start 

    WHILE size of result < target_size:
        nearest_node := unvisited node closes to last_added
        best_pos := position of nearest node that minimizes added distance

        INSERT nearest_node INTO result at best_pos
        ADD best_node TO visited
        last_added :=
    END WHILE

    RETURN result
END FUNCTION
```

## Greedy cycle
```
FUNCTION greedy_cycle(dataset, dist):
    target_size := half of dataset size (round up)
    result := empty list
    visited := empty set

    start := random node
    nearest := closest node to start
    ADD start and nearest TO result
    ADD start and nearest TO visited

    WHILE size of result < target_size:
        best_pos, best_node := NULL

        FOR EACH unvisited node:
            FOR EACH position in result:
                delta := cost of inserting node at position
                IF delta < best_delta:
                    best_delta := delta
                    best_node := node
                    best_pos := position
                END IF
            END FOR
        END FOR

        ADD best_node TO result at best_pos
        ADD best_node TO visited    
    END WHILE

    RETURN result
END FUNCTION
```