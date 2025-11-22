#include "raylib.h"
#include <cerrno>
#include <solverlib.cpp>
#include <chrono>
#include <iomanip>

void benchmark_solutions(const std::vector<Node> &dataset, CostMatrix dist, const std::string &suffix)
{


    {
        std::vector<Solution> solutions;

        for (size_t i = 0; i < 20; i++)
        {
            std::cout << "MSLS: " << i << std::endl;
            Solution final_solution = get_multiple_start_local_search(dataset, dist);
            solutions.push_back(final_solution);
        }

        std::cout << "MSLS";

        std::string lore = "MSLS | ";

        std::string filename = "msls" + suffix;
        print_stats(solutions, dataset, dist, filename, lore);
    }
    {
        std::vector<Solution> solutions;

        for (size_t i = 0; i < 20; i++)
        {
            std::cout << "ILS: " << i << std::endl;
            Solution final_solution = get_iterated_local_search(dataset, dist);
            solutions.push_back(final_solution);
        }

        std::cout << "ILS";

        std::string lore = "ILS | ";

        std::string filename = "ils" + suffix;
        print_stats(solutions, dataset, dist, filename, lore);
    }
}

int main()
{
    SetTraceLogLevel(LOG_NONE);
    InitWindow(1, 1, "This is a title");
    SetWindowState(FLAG_WINDOW_HIDDEN);

    char *whole_file_a = read_file("./TSPA.csv");
    Dataset dataset_a = parse_dataset(whole_file_a);
    CostMatrix dist_a = compute_distance_matrix(dataset_a);
    benchmark_solutions(dataset_a, dist_a, "_a");

    char *whole_file_b = read_file("./TSPB.csv");
    Dataset dataset_b = parse_dataset(whole_file_b);
    CostMatrix dist_b = compute_distance_matrix(dataset_b);
    benchmark_solutions(dataset_b, dist_b, "_b");
}
