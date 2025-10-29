#include <solverlib.cpp>

int main() {


    char       *whole_file_a = read_file("./TSPA.csv");
    Dataset    dataset_a = parse_dataset(whole_file_a);
    CostMatrix dist_a = compute_distance_matrix(dataset_a);
    size_t iterations = dataset_a.size();

    std::vector<Solution> solutions;
    for (size_t i = 0; i < iterations; ++i) {
        Solution solution = get_random_solution(dataset_a);
        solutions.push_back(get_local_search_candidate(solution, dataset_a, dist_a, INTRA_ROUTE_EDGE_EXCHANGE, 10));
        // solutions.push_back(get_local_search_steepest(solution, dataset_a, dist_a, INTRA_ROUTE_EDGE_EXCHANGE));
        // solutions.push_back(solution);
        std::cout << i << std::endl;
    }
    double min_cost  = measure_min(solutions, dist_a);
    double mean_cost = measure_mean(solutions, dist_a);
    double max_cost  = measure_max(solutions, dist_a);

    Solution best_solution = get_best_solution(solutions, dist_a);
    std::string lore = "";
    lore += "Score: " + std::to_string((int)mean_cost) +
            " (" + std::to_string((int)min_cost) + ", " + std::to_string((int)max_cost) + ")";

    SetTraceLogLevel(LOG_NONE);
    InitWindow(2000, 1000, "This is a title");
    // SetWindowState(FLAG_WINDOW_HIDDEN);
    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);
        visualize_solution(best_solution, dataset_a, 2000, 1000);
        DrawText(lore.c_str(), 10, 10, 70, BLACK);
        EndDrawing();
    }

}
