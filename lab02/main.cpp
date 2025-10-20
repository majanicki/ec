#include <solverlib.cpp>


void benchmark_solutions(const std::vector<Node> dataset, CostMatrix dist, const std::string& suffix) {
    std::vector<Solution> solutions_nearest_neighbor_regret;
    std::vector<Solution> solutions_greedy_cycle_regret;
    std::vector<Solution> solutions_nearest_neighbor_regret_weighted;
    std::vector<Solution> solutions_greedy_cycle_regret_weighted;
    for (size_t i = 0; i < dataset.size(); i++) {
        Solution nn_regret_solution = get_nearest_neighbor_regret(dataset, dist, i);
        solutions_nearest_neighbor_regret.push_back(nn_regret_solution);

        Solution greedy_cycle_regret_solution = get_greedy_cycle_regret(dataset, dist, i);
        solutions_greedy_cycle_regret.push_back(greedy_cycle_regret_solution);

        Solution nn_regret_weighted_solution = get_nearest_neighbor_regret_weighted_sum(dataset, dist, i);
        solutions_nearest_neighbor_regret_weighted.push_back(nn_regret_weighted_solution);

        Solution greedy_cycle_regret_weighted_solution = get_greedy_cycle_regret_weighted_sum(dataset, dist, i);
        solutions_greedy_cycle_regret_weighted.push_back(greedy_cycle_regret_weighted_solution);
    }

    print_stats(solutions_nearest_neighbor_regret, dataset, dist, "nearest_neighbor_regret"+suffix);
    std::cout << std::endl;
    print_stats(solutions_greedy_cycle_regret, dataset, dist, "greedy_cycle_regret"+suffix);
    std::cout << std::endl;
    print_stats(solutions_nearest_neighbor_regret_weighted, dataset, dist, "nearest_neighbor_regret_weighted"+suffix);
    std::cout << std::endl;
    print_stats(solutions_greedy_cycle_regret_weighted, dataset, dist, "greedy_cycle_regret_weighted"+suffix);
}

void benchmark_only_weighted(const std::vector<Node> dataset, CostMatrix dist, const std::string& suffix, float weight_cost = 0.5f, float weight_regret = 0.5f) {
    std::vector<Solution> solutions_nearest_neighbor_regret_weighted;
    std::vector<Solution> solutions_greedy_cycle_regret_weighted;
    for (size_t i = 0; i < dataset.size(); i++) {
        Solution nn_regret_weighted_solution = get_nearest_neighbor_regret_weighted_sum(dataset, dist, i, weight_cost, weight_regret);
        solutions_nearest_neighbor_regret_weighted.push_back(nn_regret_weighted_solution);

        Solution greedy_cycle_regret_weighted_solution = get_greedy_cycle_regret_weighted_sum(dataset, dist, i, weight_cost, weight_regret);
        solutions_greedy_cycle_regret_weighted.push_back(greedy_cycle_regret_weighted_solution);
    }

    print_stats(solutions_nearest_neighbor_regret_weighted, dataset, dist, "nearest_neighbor_regret_weighted"+suffix);
    std::cout << std::endl;
    print_stats(solutions_greedy_cycle_regret_weighted, dataset, dist, "greedy_cycle_regret_weighted"+suffix);
}

int main() {
    char * whole_file = read_file("./TSPA.csv");
    Dataset dataset = parse_dataset(whole_file);
    CostMatrix dist = compute_distance_matrix(dataset);

    SetTraceLogLevel(LOG_NONE);
    InitWindow(1, 1, "This is a title");
    SetWindowState(FLAG_WINDOW_HIDDEN);

    benchmark_solutions(dataset, dist, "_a");

    char * whole_file_b = read_file("./TSPB.csv");
    Dataset dataset_b = parse_dataset(whole_file_b);
    CostMatrix dist_b = compute_distance_matrix(dataset_b);
    benchmark_solutions(dataset_b, dist_b, "_b");

    std::cout << "\n\n--- WEIGHTED 0.3 COST 0.7 REGRET ---\n\n" << std::endl;
    benchmark_only_weighted(dataset, dist, "_a_03_07", 0.3f, 0.7f);
    benchmark_only_weighted(dataset_b, dist_b, "_b_03_07", 0.3f, 0.7f);

    std::cout << "\n\n--- WEIGHTED 0.7 COST 0.3 REGRET ---\n\n" << std::endl;
    benchmark_only_weighted(dataset, dist, "_a_07_03", 0.7f, 0.3f);
    benchmark_only_weighted(dataset_b, dist_b, "_b_07_03", 0.7f, 0.3f);
}
