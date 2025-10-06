#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <ctime>
#include <vector>
#include <climits>

#define panicf(__format, ...) \
    do { \
        std::fprintf(stderr, "[CRITICAL ERROR] %s:%d: " __format "\n", \
                     __func__, __LINE__, ##__VA_ARGS__); \
        std::exit(1); \
    } while (0)

#define panic(msg) \
    do { \
        panicf("%s", msg); \
    } while (0)

#define infof(__format, ...) \
    do { \
        std::fprintf(stdout, "[INFO] %s: " __format "\n", \
                     __func__, ##__VA_ARGS__); \
    } while (0)

#define info(msg) \
    do { \
        infof("%s", msg); \
    } while (0)


char* read_file(const char filename[]) {
    FILE *fp = fopen(filename, "r");
    if(fp == NULL) {
        panic("Failed to open a file");
    }
    if (fseek(fp, 0, SEEK_END) != 0) {
        panic("Failed to seek in a file");
    }
    long size = ftell(fp);
    if (size == -1) {
        panic("Failed to get size of a file");
    }

    if (fseek(fp, 0, SEEK_SET) != 0) {
        panic("Failed to rewind");
    }
    char *ret = new char[size];
    long total_read = 0;
    while(total_read != size) {
        long bytes_read = fread(ret + total_read, 1, size - total_read, fp);
        if (bytes_read == 0) {
            if(bytes_read == 0) {
                if (feof(fp)) break;
                if(ferror(fp)) panic("Read error");
            }
        }
        total_read += bytes_read;
    }
    ret[total_read] = '\0';
    return ret;
}

struct Node {
    int id;
    int x;
    int y;
    int cost;
};

struct NodeSet {
    int size;
    Node *nodes;
};

NodeSet parse_dataset(char *in_file_contents) {
    std::string whole_file(in_file_contents);
    std::string number_separator = ";";
    std::string new_line = "\n";
    std::string rest(whole_file);
    int size = 0;
    while(rest.size() > 0) {
        // x
        size_t pos = rest.find(number_separator);
        rest = rest.substr(pos+1);

        pos = rest.find(number_separator);
        rest = rest.substr(pos+1);

        pos = rest.find(new_line);
        rest = rest.substr(pos+1);
        size++;
    }
    rest=whole_file;
    NodeSet ret;
    ret.nodes = new Node[size];
    ret.size = size;
    int index = 0;
    while(rest.size() > 0) {
        assert(index < size);
        Node new_node;
        new_node.id = index;
        size_t pos = rest.find(number_separator);
        int x = std::atoi(rest.substr(0, pos).c_str());
        rest = rest.substr(pos+1);

        pos = rest.find(number_separator);
        int y = std::atoi(rest.substr(0, pos).c_str());
        rest = rest.substr(pos+1);

        pos = rest.find(new_line);
        int cost = std::atoi(rest.substr(0, pos).c_str());
        rest = rest.substr(pos+1);
        new_node.x = x;
        new_node.y = y;
        new_node.cost = cost;
        ret.nodes[index] = new_node;
        index++;
    }
    return ret;
}

void print_node(Node node) {
    std::cout << "x = " << node.x << "; y = " << node.y << 
        "; cost = " << node.cost << std::endl;
}

NodeSet get_random_solution(NodeSet dataset) {
    NodeSet ret;
    int target_size = std::ceil((double)dataset.size/2);
    ret.nodes = new Node[target_size];
    bool *present = new bool[dataset.size]();
    int n_found = 0;
    while(n_found < target_size) {
        int rand = std::rand() % dataset.size;
        while(present[rand]) {
            rand = std::rand() % dataset.size;
        }
        present[rand] = true;
        ret.nodes[n_found] = dataset.nodes[rand];
        n_found++;
    }
    ret.size = target_size;
    delete[] present;
    return ret;
}

int euc_distance(Node &a, Node &b) {
    double diff_x = a.x - b.x;
    double diff_y = a.y - b.y;
    return (int)std::round(std::sqrt(diff_x*diff_x + diff_y*diff_y));
}

std::vector<std::vector<int>> compute_distance_matrix(const NodeSet &dataset) {
    std::vector<std::vector<int>> dist(dataset.size, std::vector<int>(dataset.size));
    for (int i = 0; i < dataset.size; i++) {
        for (int j = 0; j < dataset.size; j++) {
            if (i == j) dist[i][j] = 0;
            else dist[i][j] = euc_distance(dataset.nodes[i], dataset.nodes[j]);
        }
    }
    return dist;
}

int compute_total_cost(const std::vector<int> &path, const NodeSet &dataset, const std::vector<std::vector<int>> &dist) {
    int total = 0;
    int path_size = path.size();
    for (int i = 0; i < path_size; i++) {
        int next = (i + 1) % path_size;
        total += dist[path[i]][path[next]];
        total += dataset.nodes[path[i]].cost;
    }
    return total;
}

NodeSet get_nearest_neighbor_end_only(NodeSet dataset, std::vector<std::vector<int>> dist) {
    int target_size = std::ceil((double)dataset.size/2);
    NodeSet result;
    result.size = target_size;
    result.nodes = new Node[target_size];

    std::vector<bool> visited_nodes(dataset.size, false);
    int start = std::rand() % dataset.size;
    visited_nodes[start] = true;

    std::vector<int> path;
    path.push_back(start);

    while((int)path.size() < target_size) {
        int last = path.back();
        int nearest_node = -1;
        int nearest_distance = INT_MAX;

        for (int j = 0; j < dataset.size; j++) {
            if (!visited_nodes[j]) {
                int distance = dist[last][j];
                if (distance < nearest_distance) {
                    nearest_distance = distance;
                    nearest_node = j;
                }
            }
        }

        if (nearest_node == -1) break;
        visited_nodes[nearest_node] = true;
        path.push_back(nearest_node);
    }

    for (int i = 0; i < target_size; i++){   
        result.nodes[i] = dataset.nodes[path[i]];
    }
    
    return result;
}


int main() {
    std::srand(42);
    char * whole_file = read_file("./TSPA.csv");
    NodeSet dataset = parse_dataset(whole_file);
    auto dist = compute_distance_matrix(dataset);


    NodeSet nn_solution = get_nearest_neighbor_end_only(dataset, dist);

    std::cout << "Nearest Neighbor (end-only) solution:\n";
    for (int i = 0; i < nn_solution.size; i++) {
        print_node(nn_solution.nodes[i]);
    }

    std::vector<int> nn_path;
    for (int i = 0; i < nn_solution.size; i++) {
        nn_path.push_back(nn_solution.nodes[i].id);
    }
    
    int total_cost = compute_total_cost(nn_path, dataset, dist);
    std::cout << "\bNearest Neighbors (only end) total cost: " << total_cost << std::endl;


    NodeSet random_solution = get_random_solution(dataset);
    std::cout << "Random solution:\n";
    for(int i = 0; i < random_solution.size; i++) {
        print_node(random_solution.nodes[i]);
    }

    std::vector<int> random_path;
    for (int i = 0; i < random_solution.size; i++) {
        random_path.push_back(random_solution.nodes[i].id);
    }
    
    int random_total_cost = compute_total_cost(random_path, dataset, dist);
    std::cout << "\nRandom solution total cost: " << random_total_cost << std::endl;
    
    return 0;
}
