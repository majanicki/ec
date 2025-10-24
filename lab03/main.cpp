#include <solverlib.cpp>

int main()
{
    char *whole_file = read_file("./TSPA.csv");
    Dataset dataset = parse_dataset(whole_file);
    CostMatrix dist = compute_distance_matrix(dataset);
    std::vector<Solution> solutions;
    std::vector<Solution> seed_solutions;
    for (int i = 0; i < 200; i++)
    {
        Solution initial_solution = get_nearest_neighbor_every_position_new(dataset, dist, i);
        solutions.push_back(get_local_search_steepest(initial_solution, dataset, dist));
        seed_solutions.push_back(initial_solution);
    }
    Solution best_solution = get_best_solution(solutions, dist);
    double min_cost = measure_min(solutions, dist);
    double mean_cost = measure_mean(solutions, dist);
    double max_cost = measure_max(solutions, dist);

    std::string lore = "New method min: " + std::to_string((int)min_cost) +
                       ", mean: " + std::to_string((int)mean_cost) +
                       ", max: " + std::to_string((int)max_cost);

    min_cost = measure_min(seed_solutions, dist);
    mean_cost = measure_mean(seed_solutions, dist);
    max_cost = measure_max(seed_solutions, dist);

    std::string lore_seed = "Seed min: " + std::to_string((int)min_cost) +
                            ", mean: " + std::to_string((int)mean_cost) +
                            ", max: " + std::to_string((int)max_cost);
    InitWindow(1000, 500, "Test");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
        visualize_solution(best_solution, dataset, 1000, 500);
        DrawText(lore.c_str(), 10, 10, 7, BLACK);
        DrawText(lore_seed.c_str(), 10, 20, 7, BLACK);
        EndDrawing();
    }
}
