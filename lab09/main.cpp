#include "raylib.h"
#include <cerrno>
#include <solverlib.cpp>
#include <chrono>
#include <iomanip>


int main()
{
    SetTraceLogLevel(LOG_NONE);
    InitWindow(1, 1, "This is a title");
    SetWindowState(FLAG_WINDOW_HIDDEN);

    char *whole_file_a = read_file("./TSPA.csv");
    Dataset dataset_a = parse_dataset(whole_file_a);
    CostMatrix dist_a = compute_distance_matrix(dataset_a);
    get_hybrid_evolution(dataset_a, dist_a);

}
