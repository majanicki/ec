#include <solverlib.cpp>
#include <iostream>
#include <chrono>
#include <iomanip>

// 1. Greedy + Intra Nodes + Random solution
// 2. Greedy + Intra Nodes + Greedy solution
// 3. Greedy + Intra Edges + Random solution
// 4. Greedy + Intra Edges + Greedy solution
// 5. Steepest + Intra Nodes + Random solution
// 6. Steepest + Intra Nodes + Greedy solution
// 7. Steepest + Intra Edges + Random solution
// 8. Steepest + Intra Edges + Greedy solution


void benchmark_solutions(const std::vector<Node> &dataset, CostMatrix dist, const std::string &suffix) {
    struct Approach {
        std::string local_search;
        MoveKind intra_kind;
        std::string init_type;
        std::string filename_prefix;
        std::string label;
    };

    std::vector<Approach> approaches = {
        {"Greedy", INTRA_ROUTE_NODE_EXCHANGE, "Random", "greedy_intra_nodes_randominit", "Greedy + Intra Nodes + Random Start"},
        {"Greedy", INTRA_ROUTE_NODE_EXCHANGE, "Greedy", "greedy_intra_nodes_greedyinit", "Greedy + Intra Nodes + Greedy Start"},
        {"Greedy", INTRA_ROUTE_EDGE_EXCHANGE, "Random", "greedy_intra_edges_randominit", "Greedy + Intra Edges + Random Start"},
        {"Greedy", INTRA_ROUTE_EDGE_EXCHANGE, "Greedy", "greedy_intra_edges_greedyinit", "Greedy + Intra Edges + Greedy Start"},
        {"Steepest", INTRA_ROUTE_NODE_EXCHANGE, "Random", "steepest_intra_nodes_randominit", "Steepest + Intra Nodes + Random Start"},
        {"Steepest", INTRA_ROUTE_NODE_EXCHANGE, "Greedy", "steepest_intra_nodes_greedyinit", "Steepest + Intra Nodes + Greedy Start"},
        {"Steepest", INTRA_ROUTE_EDGE_EXCHANGE, "Random", "steepest_intra_edges_randominit", "Steepest + Intra Edges + Random Start"},
        {"Steepest", INTRA_ROUTE_EDGE_EXCHANGE, "Greedy", "steepest_intra_edges_greedyinit", "Steepest + Intra Edges + Greedy Start"},
    };

    for (const auto &approach : approaches) {
        std::vector<Solution> solutions;
        std::vector<double> iteration_times;
        size_t iterations = dataset.size();

        for (size_t i = 0; i < iterations; ++i) {
            Solution initial_solution;

            if (approach.init_type == "Random") {
                initial_solution = get_random_solution(dataset);
            } else {
                initial_solution = get_nearest_neighbor_every_position_new(dataset, dist, i);
            }

            auto start = std::chrono::high_resolution_clock::now();
            Solution final_solution;
            if (approach.local_search == "Greedy") {
                final_solution = get_local_search_greedy(initial_solution, dataset, dist, approach.intra_kind);
            } else {
                final_solution = get_local_search_steepest(initial_solution, dataset, dist, approach.intra_kind);
            }
            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double, std::milli> elapsed = end - start;
            iteration_times.push_back(elapsed.count());
            solutions.push_back(final_solution);
        }

        double total_time = 0.0, min_time = iteration_times[0], max_time = iteration_times[0];
        for (double t : iteration_times) {
            total_time += t;
            if (t < min_time) min_time = t;
            if (t > max_time) max_time = t;
        }
        double avg_time = total_time / iterations;

        std::cout << approach.label << " | Avg time: " << std::fixed << std::setprecision(2) << avg_time
                  << " ms (" << min_time << ", " << max_time << ") \n";

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << avg_time;
        std::string avg_time_str = oss.str();
        std::string lore = approach.label + " | Avg time: " + avg_time_str + " ms | ";

        std::string filename = approach.filename_prefix + suffix;
        print_stats(solutions, dataset, dist, filename, lore);
    }
}

int main() {
    SetTraceLogLevel(LOG_NONE);
    InitWindow(1, 1, "This is a title");
    SetWindowState(FLAG_WINDOW_HIDDEN);

    char *whole_file = read_file("./TSPA.csv");
    Dataset dataset = parse_dataset(whole_file);
    CostMatrix dist = compute_distance_matrix(dataset);


    benchmark_solutions(dataset, dist, "_a");
}
