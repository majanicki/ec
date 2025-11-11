#include "raylib.h"
#include <cerrno>
#include <solverlib.cpp>
#include <chrono>
#include <iomanip>

void benchmark_solutions(const std::vector<Node> &dataset, CostMatrix dist, const std::string &suffix)
{
    struct Approach
    {
        MoveKind intra_kind;
        std::string filename_prefix;
        std::string label;
    };

    std::vector<Approach> approaches = {
        {INTRA_ROUTE_NODE_EXCHANGE, "steepest_LM_intra_nodes", "Steepest with LM + Intra Nodes"},
        {INTRA_ROUTE_EDGE_EXCHANGE, "steepest_LM_intra_edges", "Steepest with LM + Intra Edges"},
    };

    for (const auto &approach : approaches)
    {
        std::vector<Solution> solutions;
        std::vector<double> iteration_times;
        size_t iterations = dataset.size();

        for (size_t i = 0; i < iterations; i++)
        {
            Solution initial_solution = get_random_solution(dataset);

            auto start = std::chrono::high_resolution_clock::now();
            Solution final_solution = get_local_search_steepest_with_LM(initial_solution, dataset, dist, approach.intra_kind);

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> duration = end - start;
            iteration_times.push_back(duration.count());
            solutions.push_back(final_solution);
        }

        double total_time = 0.0, min_time = iteration_times[0], max_time = iteration_times[0];
        for (double t : iteration_times)
        {
            total_time += t;
            if (t < min_time)
                min_time = t;
            if (t > max_time)
                max_time = t;
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
