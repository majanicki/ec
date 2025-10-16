#include <iostream>
#include <vector>
#include <solverlib.cpp>


void benchmark_solutions(const std::vector<Node> dataset, CostMatrix dist, const std::string& suffix) {
    std::vector<Solution> solutions_random;
    std::vector<Solution> solutions_nearest_neighbor_end_only;
    std::vector<Solution> solutions_nearest_neighbor_every_position;
    std::vector<Solution> solutions_greedy_cycle;
    for (size_t i = 0; i < dataset.size(); i++) {
        Solution random_solution = get_random_solution(dataset);
        solutions_random.push_back(random_solution);

        Solution nn_end_only_solution = get_nearest_neighbor_end_only(dataset, dist, i);
        solutions_nearest_neighbor_end_only.push_back(nn_end_only_solution);

        Solution nn_all_solution = get_nearest_neighbor_every_position(dataset, dist, i);
        solutions_nearest_neighbor_every_position.push_back(nn_all_solution);

        Solution greedy_cycle_solution = get_greedy_cycle(dataset, dist, i);
        solutions_greedy_cycle.push_back(greedy_cycle_solution);
    }
    print_stats(solutions_random, dataset, dist, "random"+suffix);
    std::cout << std::endl;
    print_stats(solutions_nearest_neighbor_end_only, dataset, dist, "nearest_neighbor_end_only"+suffix);
    std::cout << std::endl;
    print_stats(solutions_nearest_neighbor_every_position, dataset, dist, "nearest_neighbor_all_positions"+suffix);
    std::cout << std::endl;
    print_stats(solutions_greedy_cycle, dataset, dist, "greedy_cycle"+suffix);

}

int main() {
    std::srand(42);
    char * whole_file = read_file("./TSPA.csv");
    std::vector<Node> dataset = parse_dataset(whole_file);
    auto dist = compute_distance_matrix(dataset);

    SetTraceLogLevel(LOG_NONE);
    InitWindow(1, 1, "This is a title");
    SetWindowState(FLAG_WINDOW_HIDDEN);

    benchmark_solutions(dataset, dist, "_a");

    whole_file = read_file("./TSPB.csv");
    std::vector<Node> dataset_b = parse_dataset(whole_file);
    dist = compute_distance_matrix(dataset_b);

    benchmark_solutions(dataset_b, dist, "_b");
    return 0;
}
