#include <solverlib.cpp>


int main() {
    char * whole_file = read_file("./TSPA.csv");
    Dataset dataset = parse_dataset(whole_file);
    CostMatrix dist = compute_distance_matrix(dataset);
    std::vector<Solution> solutions;
    for(int i = 0; i < 200; i++) {
        solutions.push_back(get_nearest_neighbor_every_position_new(dataset, dist, i));
    }
    Solution best = get_best_solution(solutions, dist);
    double mean = measure_mean(solutions, dist);
    double min = measure_min(solutions, dist);
    double max = measure_max(solutions, dist);
    std::string lore = "min: " + std::to_string((int)min) +
        ", mean: " + std::to_string((int)mean) +
        ", max: "  + std::to_string((int)max);
    InitWindow(1200, 600, "This is a title");
    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);
        DrawText(lore.c_str(), 10, 10, 20, BLACK);
        visualize_solution(best, dataset, 1200, 600);
        EndDrawing();
    }
}
