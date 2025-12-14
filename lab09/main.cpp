#include "raylib.h"
#include <cerrno>
#include <solverlib.cpp>

int main() {
    char *whole_file_a = read_file("./TSPA.csv");
    Dataset dataset_a = parse_dataset(whole_file_a);
    CostMatrix dist_a = compute_distance_matrix(dataset_a);

    Solution sol = get_hybrid_evolution(dataset_a, dist_a);
    save_solution_to_txt(sol, "test_sol.txt");
    assert(solution_valid(sol, dataset_a) && sol.size() == 100);

    int cost = compute_total_cost(sol, dist_a);
    std::string lore = std::to_string(cost);
    InitWindow(1000, 500, "This is a title");
    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);
        DrawText(lore.c_str(), 10, 10, 70, BLACK);
        visualize_solution(sol, dataset_a, 1000, 500);
        EndDrawing();
    }
}
