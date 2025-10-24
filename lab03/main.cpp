#include <solverlib.cpp>

// 1. Greedy + Intra Nodes + Random solution
// 2. Greedy + Intra Nodes + Greedy solution
// 3. Greedy + Intra Edges + Random solution
// 4. Greedy + Intra Edges + Greedy solution
// 5. Steepest + Intra Nodes + Random solution
// 6. Steepest + Intra Nodes + Greedy solution
// 7. Steepest + Intra Edges + Random solution
// 8. Steepest + Intra Edges + Greedy solution

void benchmark_solutions(const std::vector<Node> dataset, CostMatrix dist, const std::string& suffix) {
    std::vector<Solution> solutions_greedy_nodes_randominit;
    std::vector<Solution> solutions_greedy_nodes_greedyinit;
    std::vector<Solution> solutions_greedy_edges_randominit;
    std::vector<Solution> solutions_greedy_edges_greedyinit;

    std::vector<Solution> solutions_steepest_nodes_randominit;
    std::vector<Solution> solutions_steepest_nodes_greedyinit;
    std::vector<Solution> solutions_steepest_edges_randominit;
    std::vector<Solution> solutions_steepest_edges_greedyinit;

    // greedy init solutions
    for (size_t i = 0; i < dataset.size(); i++)
    {
        Solution initial_solution = get_nearest_neighbor_every_position_new(dataset, dist, i);
        solutions_steepest_nodes_greedyinit.push_back(get_local_search_steepest(initial_solution, dataset, dist, INTRA_ROUTE_NODE_EXCHANGE));
        solutions_steepest_edges_greedyinit.push_back(get_local_search_steepest(initial_solution, dataset, dist, INTRA_ROUTE_EDGE_EXCHANGE));
        solutions_greedy_nodes_greedyinit.push_back(get_local_search_greedy(initial_solution, dataset, dist, INTRA_ROUTE_NODE_EXCHANGE));
        solutions_greedy_edges_greedyinit.push_back(get_local_search_greedy(initial_solution, dataset, dist, INTRA_ROUTE_EDGE_EXCHANGE));
    }

    // random init solutions
    for (size_t i = 0; i < dataset.size(); i++)
    {
        Solution initial_solution = get_random_solution(dataset);
        solutions_steepest_nodes_randominit.push_back(get_local_search_steepest(initial_solution, dataset, dist, INTRA_ROUTE_NODE_EXCHANGE));
        solutions_steepest_edges_randominit.push_back(get_local_search_steepest(initial_solution, dataset, dist, INTRA_ROUTE_EDGE_EXCHANGE));
        solutions_greedy_nodes_randominit.push_back(get_local_search_greedy(initial_solution, dataset, dist, INTRA_ROUTE_NODE_EXCHANGE));
        solutions_greedy_edges_randominit.push_back(get_local_search_greedy(initial_solution, dataset, dist, INTRA_ROUTE_EDGE_EXCHANGE));
    }

    print_stats(solutions_greedy_nodes_randominit, dataset, dist, "greedy_intra_nodes_randominit" + suffix);
    print_stats(solutions_greedy_nodes_greedyinit, dataset, dist, "greedy_intra_nodes_greedyinit" + suffix);
    print_stats(solutions_greedy_edges_randominit, dataset, dist, "greedy_intra_edges_randominit" + suffix);
    print_stats(solutions_greedy_edges_greedyinit, dataset, dist, "greedy_intra_edges_greedyinit" + suffix);

    print_stats(solutions_steepest_nodes_randominit, dataset, dist, "steepest_intra_nodes_randominit" + suffix);
    print_stats(solutions_steepest_nodes_greedyinit, dataset, dist, "steepest_intra_nodes_greedyinit" + suffix);
    print_stats(solutions_steepest_edges_randominit, dataset, dist, "steepest_intra_edges_randominit" + suffix);
    print_stats(solutions_steepest_edges_greedyinit, dataset, dist, "steepest_intra_edges_greedyinit" + suffix);
}

int main() {
    char *whole_file = read_file("./TSPA.csv");
    Dataset dataset = parse_dataset(whole_file);
    CostMatrix dist = compute_distance_matrix(dataset);

    SetTraceLogLevel(LOG_NONE);
    InitWindow(1, 1, "This is a title");
    SetWindowState(FLAG_WINDOW_HIDDEN);

    benchmark_solutions(dataset, dist, "_a");
}
