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


# Result Comparison

| Algorithm                       | Min Cost | Mean Cost | Max Cost |
|---------------------------------|-----------:|-----------:|-----------:|
| Random                          | 241,347    | 265,135    | 291,966    |
| Nearest Neighbor #1       | 83,182     | 85,108.5   | 89,433     |
| Nearest Neighbor #2  | 74,252     | 77,517.7   | 80,442     |
| Greedy Cycle                    | 71,488     | 72,646.4   | 74,410     |


## Best solutions 

This section presents best solution found by each method. 
In visualizations greener nodes have lower cost than red nodes.

### Random

Random solution achieved lowest cost of `241,347`.

```
38
0
80
183
196
21
73
78
127
147
48
82
23
177
10
86
56
160
28
146
31
175
135
163
2
157
97
126
65
62
184
17
94
115
52
161
189
118
162
149
139
191
30
25
15
199
138
165
53
156
188
169
151
143
18
110
84
60
74
198
181
43
170
72
108
153
79
14
12
58
71
32
89
166
99
131
192
6
145
140
85
39
133
77
178
83
46
51
37
75
193
164
187
129
137
4
167
40
81
117
38
```

![Visualization of Random Algorithm](random.png "Random")

### Nearest neighbor #1

The lowest cost is `83,182`.

```
124
94
63
53
180
154
135
123
65
116
59
115
139
193
41
42
160
34
22
18
108
69
159
181
184
177
54
30
48
43
151
176
80
79
133
162
51
137
183
143
0
117
46
68
93
140
36
163
199
146
195
103
5
96
118
149
131
112
4
84
35
10
190
127
70
101
97
1
152
120
78
145
185
40
165
90
81
113
175
171
16
31
44
92
57
106
49
144
62
14
178
52
55
129
2
75
86
26
100
121
124
```

![Visualization of Nearest Neighbor #1](nearest_neighbor_end_only.png "Nearest Neighbor #1")

### Nearest neighbor #2

The lowest cost is `74,252`.

```
118
51
80
176
151
133
79
63
94
152
97
1
2
129
92
57
55
52
49
102
148
15
9
62
144
14
178
106
185
165
90
40
81
31
113
175
171
16
145
78
44
120
75
101
86
53
180
154
135
162
123
127
112
4
84
35
149
131
65
116
42
181
195
159
193
41
139
96
5
43
184
177
190
10
30
54
48
160
34
103
146
22
18
69
108
68
140
93
117
0
143
183
89
114
186
23
137
46
115
59
118
```

![Visualization of Nearest Neighbor #2](nearest_neighbor_all_positions.png "Nearest Neighbor #2")

### Greedy cycle

The lowest cost is `74,252`.
```
0
117
143
183
89
186
23
137
176
80
79
63
94
124
152
97
1
101
2
120
129
55
49
102
148
9
62
144
14
178
106
165
90
81
196
40
119
185
52
57
92
179
145
78
31
56
113
175
171
16
25
44
75
86
26
100
53
154
180
135
70
127
123
162
133
151
51
118
59
65
116
43
184
35
84
112
4
190
10
177
30
54
48
160
34
146
22
18
108
69
159
181
42
5
115
41
193
139
68
46
0
```

![Visualization of Greedy Cycle Algorithm](greedy_cycle.png "Nearest All Positions Algorithm")

# Conclusions


