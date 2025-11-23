#include "raylib.h"
#include <cerrno>
#include <solverlib.cpp>
#include <chrono>
#include <iomanip>

void benchmark_solutions(const std::vector<Node> &dataset, CostMatrix dist, const std::string &suffix)
{
    {
        std::vector<Solution> solutions;
        std::vector<double> iteration_times;

        for (size_t i = 0; i < 20; i++)
        {
            std::cout << "MSLS: " << i << std::endl;
            auto start = std::chrono::high_resolution_clock::now();
            Solution final_solution = get_multiple_start_local_search(dataset, dist);
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
        double avg_time = total_time / iteration_times.size();
        std::cout << "MSLS | Avg time: " << std::fixed << std::setprecision(2) << avg_time
                  << " ms (" << min_time << ", " << max_time << ") \n";

        std::cout << "MSLS";

        std::string lore = std::string("MSLS");

        std::string filename = "msls" + suffix;
        print_stats(solutions, dataset, dist, filename, lore);
    }
    {
        std::vector<Solution> solutions;
        std::vector<double> iteration_times;
        std::vector<int> ls_counts;

        for (size_t i = 0; i < 20; i++)
        {
            std::cout << "ILS: " << i << std::endl;
            auto start = std::chrono::high_resolution_clock::now();
            auto res = get_iterated_local_search(dataset, dist);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> duration = end - start;
            iteration_times.push_back(duration.count());
            solutions.push_back(res.solution);
            ls_counts.push_back(res.ls_runs);
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
        double avg_time = total_time / iteration_times.size();
        std::cout << "ILS | Avg time: " << std::fixed << std::setprecision(2) << avg_time
                  << " ms (" << min_time << ", " << max_time << ") \n";

        int total_ls = 0, min_ls = ls_counts[0], max_ls = ls_counts[0];
        for (int count : ls_counts)
        {
            total_ls += count;
            if (count < min_ls)
                min_ls = count;
            if (count > max_ls)
                max_ls = count;
        }
        double avg_ls = (double)total_ls / ls_counts.size();
        std::cout << "ILS | Avg LS runs: " << std::fixed << std::setprecision(2) << avg_ls
                  << " (" << min_ls << ", " << max_ls << ") \n";

        std::cout << "ILS";

        std::string lore = std::string("ILS");

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
