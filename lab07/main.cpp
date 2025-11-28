#include "raylib.h"
#include <cerrno>
#include <solverlib.cpp>
#include <chrono>
#include <iomanip>

void benchmark_solutions(const std::vector<Node> &dataset, CostMatrix dist, const std::string &suffix)
{
    {
        std::vector<Solution> solutions;
        std::vector<int> ls_counts;

        for (size_t i = 0; i < 20; i++) {
            std::cout << "Large Neighborhood Search: " << i << std::endl;
            auto res = get_large_neighborhood_search(dataset, dist);
            solutions.push_back(res.solution);
            ls_counts.push_back(res.ls_runs);
        }

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
        std::cout << "LNS | Avg LS runs: " << std::fixed << std::setprecision(2) << avg_ls
                  << " (" << min_ls << ", " << max_ls << ") \n";
        std::string lore = std::string("LNS");
        std::string filename = "lns" + suffix;
        print_stats(solutions, dataset, dist, filename, lore);
    }
    {
        std::vector<Solution> solutions;
        std::vector<int> ls_counts;

        for (size_t i = 0; i < 20; i++) {
            std::cout << "Large Neighborhood Search with Local Search: " << i << std::endl;
            auto res = get_large_neighborhood_search_with_local_search(dataset, dist);
            solutions.push_back(res.solution);
            ls_counts.push_back(res.ls_runs);
        }

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
        std::cout << "LNS with LS| Avg LS runs: " << std::fixed << std::setprecision(2) << avg_ls
                  << " (" << min_ls << ", " << max_ls << ") \n";
        std::string lore = std::string("LNS with LS");
        std::string filename = "lns_w_ls" + suffix;
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
