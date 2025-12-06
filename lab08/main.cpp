#include "raylib.h"
#include <cerrno>
#include <solverlib.cpp>
#include <chrono>
#include <iomanip>

void run_global_convexity_experiment(const Dataset &dataset, const CostMatrix &dist, const std::string &instance_name)
{
    SolutionAndIteration ils = get_iterated_local_search(dataset, dist);
    Solution ils_best = ils.solution;

    int n_optima = 1000;
    std::vector<Solution> locals = generate_local_optima(dataset, dist, n_optima);

    std::vector<double> costs(n_optima);
    for (int i = 0; i < n_optima; i++)
    {
        costs[i] = compute_total_cost(locals[i], dist);
    }

    int best_idx = 0;
    double best_cost = costs[0];
    for (int i = 1; i < n_optima; i++)
    {
        if (costs[i] < best_cost)
        {
            best_cost = costs[i];
            best_idx = i;
        }
    }
    Solution best_local = locals[best_idx];

    std::vector<double> avg_edges(n_optima, 0.0);
    std::vector<double> avg_nodes(n_optima, 0.0);

    for (int i = 0; i < n_optima; i++)
    {
        for (int j = i + 1; j < n_optima; j++)
        {
            int e_sim = calculate_similarity_common_edges(locals[i], locals[j]);
            int n_sim = calculate_similarity_common_nodes(locals[i], locals[j]);
            avg_edges[i] += e_sim;
            avg_edges[j] += e_sim;
            avg_nodes[i] += n_sim;
            avg_nodes[j] += n_sim;
        }
    }

    for (int i = 0; i < n_optima; i++)
    {
        avg_edges[i] /= (n_optima - 1);
        avg_nodes[i] /= (n_optima - 1);
    }

    std::vector<double> costs_best_local;
    std::vector<double> sim_edges_best_local;
    std::vector<double> sim_nodes_best_local;

    costs_best_local.reserve(n_optima - 1);
    sim_edges_best_local.reserve(n_optima - 1);
    sim_nodes_best_local.reserve(n_optima - 1);

    for (int i = 0; i < n_optima; i++)
    {
        if (i == best_idx)
            continue;
        costs_best_local.push_back(costs[i]);
        sim_edges_best_local.push_back(calculate_similarity_common_edges(locals[i], best_local));
        sim_nodes_best_local.push_back(calculate_similarity_common_nodes(locals[i], best_local));
    }

    std::vector<double> sim_edges_ils(n_optima);
    std::vector<double> sim_nodes_ils(n_optima);
    for (int i = 0; i < n_optima; i++)
    {
        sim_edges_ils[i] = calculate_similarity_common_edges(locals[i], ils_best);
        sim_nodes_ils[i] = calculate_similarity_common_nodes(locals[i], ils_best);
    }

    double corr_edges_avg = calculate_correlation(costs, avg_edges);
    double corr_nodes_avg = calculate_correlation(costs, avg_nodes);
    double corr_edges_best_local = calculate_correlation(costs_best_local, sim_edges_best_local);
    double corr_nodes_best_local = calculate_correlation(costs_best_local, sim_nodes_best_local);
    double corr_edges_ils = calculate_correlation(costs, sim_edges_ils);
    double corr_nodes_ils = calculate_correlation(costs, sim_nodes_ils);

    std::cout << "Instance " << instance_name << " edges avg corr: " << corr_edges_avg << std::endl;
    std::cout << "Instance " << instance_name << " nodes avg corr: " << corr_nodes_avg << std::endl;
    std::cout << "Instance " << instance_name << " edges best_local corr: " << corr_edges_best_local << std::endl;
    std::cout << "Instance " << instance_name << " nodes best_local corr: " << corr_nodes_best_local << std::endl;
    std::cout << "Instance " << instance_name << " edges ils corr: " << corr_edges_ils << std::endl;
    std::cout << "Instance " << instance_name << " nodes ils corr: " << corr_nodes_ils << std::endl;

    std::string prefix = instance_name + "_";

    write_csv_xy(prefix + "edges_avg.csv", costs, avg_edges);
    write_csv_xy(prefix + "nodes_avg.csv", costs, avg_nodes);

    write_csv_xy(prefix + "edges_best_local.csv", costs_best_local, sim_edges_best_local);
    write_csv_xy(prefix + "nodes_best_local.csv", costs_best_local, sim_nodes_best_local);

    write_csv_xy(prefix + "edges_ils.csv", costs, sim_edges_ils);
    write_csv_xy(prefix + "nodes_ils.csv", costs, sim_nodes_ils);
}

int main()
{
    SetTraceLogLevel(LOG_NONE);
    InitWindow(1, 1, "This is a title");
    SetWindowState(FLAG_WINDOW_HIDDEN);

    char *whole_file_a = read_file("./TSPA.csv");
    Dataset dataset_a = parse_dataset(whole_file_a);
    CostMatrix dist_a = compute_distance_matrix(dataset_a);
    run_global_convexity_experiment(dataset_a, dist_a, "TSPA");

    char *whole_file_b = read_file("./TSPB.csv");
    Dataset dataset_b = parse_dataset(whole_file_b);
    CostMatrix dist_b = compute_distance_matrix(dataset_b);
    run_global_convexity_experiment(dataset_b, dist_b, "TSPB");
}
