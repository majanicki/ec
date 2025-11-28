#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <vector>
#include <climits>
#include <cstring>
#include <fstream>
#include <raylib.h>
#include <algorithm>
#include <random>
#include <sstream>
#include <utility>
#include <chrono>

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

struct Node {
    int id;
    int x;
    int y;
    int cost;
};

typedef std::vector<Node> Solution;
typedef std::vector<Node> Dataset;

void print_node(Node node) {
    std::cout << "x = " << node.x << "; y = " << node.y <<
        "; cost = " << node.cost << std::endl;
}

char* read_file(const char filename[]) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) panic("Failed to open file");

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    char *buf = new char[size + 1]();  // zero-initialize
    size_t total = fread(buf, 1, size, fp);
    buf[total] = '\0';
    fclose(fp);
    return buf;
}



Dataset parse_dataset(const char *contents) {
    Dataset data;
    std::stringstream ss(contents);
    std::string line;
    int index = 0;

    while (std::getline(ss, line)) {
        std::stringstream ls(line);
        std::string sx, sy, sc;
        if (!std::getline(ls, sx, ';')) continue;
        if (!std::getline(ls, sy, ';')) continue;
        if (!std::getline(ls, sc)) continue;
        data.push_back({index++, std::stoi(sx), std::stoi(sy), std::stoi(sc)});
    }
    return data;
}


int euc_distance(const Node &a, const Node &b) {
    double diff_x = a.x - b.x;
    double diff_y = a.y - b.y;
    double distance = std::sqrt(diff_x*diff_x + diff_y*diff_y);
    int result = static_cast<int>(std::floor(distance + 0.5)); // dokładne odwzorowanie ZAOKR.DO.CAŁK
    return result;
}


// 2D vectors are weird
struct CostMatrix {
    int dim;
    int* data;
    int get(const Node &a, const Node &b) {
        assert(a.id < dim);
        assert(b.id < dim);
        assert(data != NULL);
        return data[b.id * dim + a.id];
    }
};

CostMatrix compute_distance_matrix(const Dataset &dataset) {
    CostMatrix dist;
    dist.dim = dataset.size();
    dist.data = new int[dist.dim * dist.dim];
    for (size_t i = 0; i < dataset.size(); i++) {
        Node a = dataset[i];
        for (size_t j = 0; j < dataset.size(); j++) {
            // lets rely on index
            Node b = dataset[j];
            int value;
            if (a.id == b.id) value = 0;
            else value = euc_distance(a, b);
            dist.data[b.id * dist.dim + a.id] = value;
        }
    }
    return dist;
}

int compute_total_cost(const Solution& solution, CostMatrix cost_matrix) {
    int total = 0;
    int path_size = solution.size();
    for (int i = 0; i < path_size; i++) {
        int next = (i + 1) % path_size;
        Node a = solution[i];
        Node b = solution[next];
        total += cost_matrix.get(a, b) + a.cost;
    }
    return total;
}

Vector2 node_to_canvas(const Node &node, int min_x, int max_x, int min_y, int max_y, int canvas_width, int canvas_height)
{

    float padd = canvas_width > canvas_height ? canvas_width : canvas_height;
    padd *= 0.05;
    float step_size_x = (float)(canvas_width - 2.0f * padd) / (max_x - min_x);
    float step_size_y = (float)(canvas_height - 2.0f * padd) / (max_y - min_y);

    float step_size = step_size_x < step_size_y ? step_size_x : step_size_y;
    // padd
    // step_size *= 0.9f;
    // int padding_x = (float)canvas_height * 0.1f;
    // int padding_y = (float)canvas_height * 0.1f;
    int canvas_x = step_size * (node.x - min_x) + padd;
    int canvas_y = canvas_height - (step_size * (node.y - min_y) + padd);
    Vector2 ret = (Vector2){(float)canvas_x, (float)canvas_y};
    return ret;
}

void visualize_solution(const Solution& solution, const Dataset& dataset, int canvas_width, int canvas_height) {
    int max_x = 0;
    int min_x = INT_MAX;
    int max_y = 0;
    int min_y = INT_MAX;
    int max_cost = 0;
    int min_cost = INT_MAX;
    for(size_t i = 0; i < dataset.size(); i++) {
        Node node = dataset[i];
        if(node.x < min_x) {
            min_x = node.x;
        }
        if(node.y < min_y) {
            min_y = node.y;
        }
        if(node.x > max_x) {
            max_x = node.x;
        }
        if(node.y > max_y) {
            max_y = node.y;
        }
        if(node.cost > max_cost) {
            max_cost = node.cost;
        }
        if(node.cost < min_cost) {
            min_cost = node.cost;
        }
    }
    for(size_t i = 0; i < solution.size(); i++) {
        Node node_a = solution[i];
        Vector2 pos_a = node_to_canvas(node_a, min_x, max_x, min_y, max_y, canvas_width, canvas_height);
        Node node_b = solution[(i + 1) % solution.size()];
        Vector2 pos_b = node_to_canvas(node_b, min_x, max_x, min_y, max_y, canvas_width, canvas_height);
        DrawLineEx(pos_a, pos_b, canvas_width * 0.001, BLACK);
    }
    for(size_t i = 0; i < dataset.size(); i++) {
        Node node = dataset[i];
        Vector2 pos = node_to_canvas(node, min_x, max_x, min_y, max_y, canvas_width, canvas_height);
        float red = 225.0f * ((float)(node.cost - min_cost) / (max_cost-min_cost) ) + 15.0f;
        unsigned char green = 255 - red;
        DrawCircle(pos.x, pos.y, canvas_height * 0.0065, (Color){(unsigned char)red, green, 0, 255});
    }
    Node start = solution[0];
    Vector2 pos = node_to_canvas(start, min_x, max_x, min_y, max_y, canvas_width, canvas_height);
    DrawCircle(pos.x, pos.y, canvas_height * 0.008, BLUE);
}



double measure_mean(const std::vector<Solution>& solutions, CostMatrix dist) {
    double score = 0.0;
    for (size_t i = 0;  i < solutions.size(); i++) {
        score += compute_total_cost(solutions[i], dist);
    }
    return score / (float)solutions.size();
}

int measure_max(const std::vector<Solution>& solutions, CostMatrix dist) {
    int max = 0;
    for (size_t i = 0; i < solutions.size(); i++) {
        int score = compute_total_cost(solutions[i], dist);
        if (score > max) {
            max = score;
        }
    }
    return max;
}

int measure_min(const std::vector<Solution>& solutions, CostMatrix dist) {
    int min = INT_MAX;
    for (size_t i = 0; i < solutions.size(); i++) {
        int score = compute_total_cost(solutions[i], dist);
        if (score < min) {
            min = score;
        }
    }
    return min;
}

Solution get_best_solution(const std::vector<Solution>& solutions, CostMatrix dist) {
    int min = INT_MAX;
    Solution best;
    for (size_t i = 0; i < solutions.size(); i++) {
        int score = compute_total_cost(solutions[i], dist);
        if (score < min) {
            min = score;
            best = solutions[i];
        }
    }
    return best;
}

void save_solution_to_txt(Solution solution, const std::string& filename) {
    std::ofstream file(filename);            // create and open file
    if (!file) {
        panicf("Failed to open a file %s\n", filename.c_str());
    }
    for (size_t i = 0; i < solution.size(); i++) {
        file << solution[i].id << ", ";
    }
    file << solution[0].id << "\n";
    infof("Saved %s", filename.c_str());
    file.close();
}

void save_solution_to_png(Solution solution, const Dataset& dataset, const std::string &filename, std::string lore = "") {
    RenderTexture2D render_texture = LoadRenderTexture(4000, 2000);

    BeginTextureMode(render_texture);
    ClearBackground(WHITE);
    visualize_solution(solution, dataset, 4000, 2000);
    if (!lore.empty()) {
        DrawText(lore.c_str(), 10, 10, 70, BLACK);
    }
    EndTextureMode();

    Image final_image = LoadImageFromTexture(render_texture.texture);
    ImageFlipVertical(&final_image);
    ExportImage(final_image, filename.c_str());

    UnloadImage(final_image);
    UnloadRenderTexture(render_texture);
    infof("Saved %s", filename.c_str());
}
std::mt19937 rng(42);  // seed = 42
// no reference since we want to copy the dataset
Solution get_random_solution(Dataset dataset) {
    size_t target_size = std::ceil((double)dataset.size()/2);
    Solution ret;
    while(ret.size() < target_size) {
        std::uniform_int_distribution<int> dist(0, dataset.size() - 1);
        int rand = dist(rng);
        ret.push_back(dataset[rand]);

        if((size_t)rand == (dataset.size() - 1)) {
            dataset.pop_back();
        } else {
            dataset[rand] = dataset[dataset.size() - 1];
            dataset.pop_back();
        }
    }
    return ret;
}

void print_stats(const std::vector<Solution>& solutions, const Dataset &dataset,
                 CostMatrix dist, const std::string& name, std::string lore = "") {
    double min_cost  = measure_min(solutions, dist);
    double mean_cost = measure_mean(solutions, dist);
    double max_cost  = measure_max(solutions, dist);

    std::cout << name << " min cost: "  << min_cost << std::endl;
    std::cout << name << " mean cost: " << mean_cost << std::endl;
    std::cout << name << " max cost: "  << max_cost << std::endl;

    Solution best_solution = get_best_solution(solutions, dist);

    lore += "Score: " + std::to_string((int)mean_cost) +
            " (" + std::to_string((int)min_cost) + ", " + std::to_string((int)max_cost) + ")";


    save_solution_to_txt(best_solution, name + ".txt");
    save_solution_to_png(best_solution, dataset, name + ".png", lore);
}

Solution get_nearest_neighbor_end_only(const Dataset& dataset, CostMatrix dist, int start) {
    int target_size = std::ceil((double)dataset.size()/2);
    Solution result;

    std::vector<bool> visited_nodes(dataset.size(), false);
    result.push_back(dataset[start]);
    visited_nodes[start] = true;


    while((int)result.size() < target_size) {
        Node last = result.back();
        Node nearest_node;
        bool found = false;
        int nearest_distance = INT_MAX;

        for (size_t j = 0; j < dataset.size(); j++) {
            if (!visited_nodes[j]) {
                Node candidate = dataset[j];
                int distance = dist.get(last, candidate) + candidate.cost;
                if (distance < nearest_distance) {
                    nearest_distance = distance;
                    nearest_node = candidate;
                    found = true;
                }
            }
        }

        if (!found) break;
        visited_nodes[nearest_node.id] = true;
        result.push_back(nearest_node);
    }

    return result;
}
// implementation from first labs
// incorrect
Solution get_nearest_neighbor_every_position_old(const Dataset& dataset, CostMatrix dist, int start) {
    int target_size = (dataset.size() + 1) / 2;
    Solution result;
    std::vector<bool> used(dataset.size(), false);

    result.push_back(dataset[start]);
    used[start] = true;

    while ((int)result.size() < target_size) {
        int best_new_index = -1;
        int insert_after   = -1;
        int min_dist = INT_MAX;

        for (size_t i = 0; i < result.size(); i++) {
            Node current_node = result[i];
            for (size_t j = 0; j < dataset.size(); j++) {
                if (used[j]) continue;

                int cost = dist.get(current_node, dataset[j]) + dataset[j].cost;
                if (cost < min_dist) {
                    min_dist = cost;
                    insert_after = i;
                    best_new_index = j;
                }
            }
        }

        assert(best_new_index != -1);
        assert(insert_after != -1);

        result.insert(result.begin() + insert_after + 1, dataset[best_new_index]);
        used[best_new_index] = true;
    }

    return result;
}

// new and better implementation
Solution get_nearest_neighbor_every_position_new(const Dataset& dataset, CostMatrix dist, int start) {
    int target_size = (dataset.size() + 1) / 2;
    Solution result;
    std::vector<bool> used(dataset.size(), false);

    result.push_back(dataset[start]);
    used[start] = true;

    while ((int)result.size() < target_size) {
        int best_new_index = -1;
        int insert_after   = -1;
        int min_dist = INT_MAX;

        for (size_t j = 0; j < dataset.size(); j++) {
            if (used[j]) continue;
            for (int i = -1; i < (int)result.size(); i++) {
                // before beginning case
                int cost;
                if(i == -1) {
                    Node beginning = result[0];
                    cost = dist.get(dataset[j], beginning) + dataset[j].cost;
                } else if(i == (int)result.size()-1) {
                    Node ending = result[result.size() - 1];
                    cost = dist.get(ending, dataset[j]) + dataset[j].cost;
                } else {
                    Node current_node = result[i];
                    Node next_node = result[i+1];
                    int curr_cost = dist.get(current_node, next_node);
                    int new_cost = dist.get(current_node, dataset[j]) + dist.get(dataset[j], next_node) + dataset[j].cost;
                    cost = new_cost - curr_cost;
                }
                if (cost < min_dist) {
                    min_dist = cost;
                    insert_after = i;
                    best_new_index = j;
                }
            }
        }

        result.insert(result.begin() + insert_after + 1, dataset[best_new_index]);
        used[best_new_index] = true;
    }

    return result;
}

Solution get_greedy_cycle(const Dataset& dataset, CostMatrix dist, int start)
{
    int target_size = std::ceil((double)dataset.size() / 2);
    Solution result;
    std::vector<bool> visited_nodes(dataset.size(), false);

    // choose random start node
    Node start_node = dataset[start];
    visited_nodes[start] = true;

    // find nearest neighbor to the starting node to form initial cycle
    int nearest_idx = -1;
    int nearest_dist = INT_MAX;
    for (size_t i = 0; i < dataset.size(); i++) {
        if (visited_nodes[i]) continue;
        int d = dist.get(start_node, dataset[i]) + dataset[i].cost;
        if (d < nearest_dist) {
            nearest_dist = d;
            nearest_idx = i;
        }
    }

    visited_nodes[nearest_idx] = true;
    result.push_back(start_node);
    result.push_back(dataset[nearest_idx]);

    while ((int)result.size() < target_size) {
        int best_node = -1;
        int best_pos = -1;
        int best_delta = INT_MAX;

        for (size_t j = 0; j < dataset.size(); j++) {
            if (visited_nodes[j]) continue; // for every node that has not been visited yet
            Node candidate = dataset[j];

            for (size_t pos = 0; pos < result.size(); pos++) {
                // try inserting it between every two nodes in the current cycle
                Node a = result[pos];
                Node b = result[(pos + 1) % result.size()]; // go to the beginning if at the end

                int delta = dist.get(a, candidate) + dist.get(candidate, b) - dist.get(a, b) + candidate.cost;
                if (delta < best_delta) {
                    best_delta = delta;
                    best_node = j;
                    best_pos = pos + 1;
                }
            }
        }

        if (best_pos == -1) break;
        visited_nodes[best_node] = true;
        result.insert(result.begin() + best_pos, dataset[best_node]);
    }

    return result;
}

Solution get_nearest_neighbor_regret_weighted_sum(const Dataset& dataset, CostMatrix dist, int start, float weight_cost = 0.5f, float weight_regret = 0.5f) {
    int target_size = (dataset.size() + 1) / 2;
    Solution result;
    std::vector<bool> used(dataset.size(), false);

    result.push_back(dataset[start]);
    used[start] = true;

    while ((int)result.size() < target_size) {
        int final_insert_location = -2;
        int best_weighted_sum = INT_MIN;
        int insert_id = -1;
        for (size_t j = 0; j < dataset.size(); j++) {
            if (used[j]) continue;
            int best_place_cost = INT_MAX;
            int second_best_place_cost = INT_MAX;
            int insert_location;
            for (int i = -1; i < (int)result.size(); i++) {
                // before beginning case
                int cost;
                if(i == -1) {
                    Node beginning = result[0];
                    cost = dist.get(dataset[j], beginning) + dataset[j].cost;
                } else if(i == (int)result.size()-1) {
                    Node ending = result[result.size() - 1];
                    cost = dist.get(ending, dataset[j]) + dataset[j].cost;
                } else {
                    Node current_node = result[i];
                    Node next_node = result[i+1];
                    int curr_cost = dist.get(current_node, next_node);
                    int new_cost = dist.get(current_node, dataset[j]) + dist.get(dataset[j], next_node) + dataset[j].cost;
                    cost = new_cost - curr_cost;
                }
                if(best_place_cost > cost) {
                    second_best_place_cost = best_place_cost;
                    best_place_cost = cost;
                    insert_location = i;
                } else if (second_best_place_cost > cost) {
                    second_best_place_cost = cost;
                }
            }
            int regret = second_best_place_cost - best_place_cost;
            assert(regret >= 0);
            int weighted_sum = -(float)best_place_cost * weight_cost + (float)regret * weight_regret;
            if(best_weighted_sum < weighted_sum) {
                best_weighted_sum = weighted_sum;
                final_insert_location = insert_location;
                insert_id = j;
            }
        }

        assert(insert_id != -1);
        assert(final_insert_location != -2);
        result.insert(result.begin() + final_insert_location + 1, dataset[insert_id]);
        used[insert_id] = true;
    }

    return result;
}

Solution get_nearest_neighbor_regret(const Dataset& dataset, CostMatrix dist, int start) {
    return get_nearest_neighbor_regret_weighted_sum(dataset, dist, start, 0.0f, 1.0f);
}

Solution get_greedy_cycle_regret_weighted_sum(const Dataset& dataset, CostMatrix dist, int start, float weight_cost = 0.5f, float weight_regret = 0.5f) {
    int target_size = (dataset.size() + 1) / 2;
    Solution result;
    std::vector<bool> used(dataset.size(), false);

    result.push_back(dataset[start]);
    used[start] = true;

    while ((int)result.size() < target_size) {
        int final_insert_location = -1;
        int best_weighted_sum = INT_MIN;
        int insert_id = -1;
        for (size_t j = 0; j < dataset.size(); j++) {
            if (used[j]) continue;
            int best_place_cost = INT_MAX;
            int second_best_place_cost = INT_MAX;
            int insert_location;
            for (int i = 0; i < (int)result.size(); i++) {
                Node current_node = result[i];
                Node next_node = result[(i+1)%result.size()];
                int curr_cost = dist.get(current_node, next_node);
                int new_cost = dist.get(current_node, dataset[j]) + dist.get(dataset[j], next_node) + dataset[j].cost;
                int cost = new_cost - curr_cost;
                if(best_place_cost > cost) {
                    second_best_place_cost = best_place_cost;
                    best_place_cost = cost;
                    insert_location = i;
                } else if (second_best_place_cost > cost) {
                    second_best_place_cost = cost;
                }
            }
            int regret = second_best_place_cost - best_place_cost;
            assert(regret >= 0);
            int weighted_sum = -(float)best_place_cost * weight_cost + (float)regret * weight_regret;
            if(best_weighted_sum < weighted_sum) {
                best_weighted_sum = weighted_sum;
                final_insert_location = insert_location;
                insert_id = j;
            }
        }

        assert(insert_id != -1);
        assert(final_insert_location != -1);
        result.insert(result.begin() + final_insert_location + 1, dataset[insert_id]);
        used[insert_id] = true;
    }

    return result;
}

Solution get_greedy_cycle_regret(const Dataset& dataset, CostMatrix dist, int start) {
    return get_greedy_cycle_regret_weighted_sum(dataset, dist, start, 0.0f, 1.0f);
}


enum MoveKind{
    INTER_ROUTE,
    INTRA_ROUTE_EDGE_EXCHANGE,
    INTRA_ROUTE_NODE_EXCHANGE,
};

struct Move {
    int solution_index;
    int dataset_index;

    int swap_index_a;
    int swap_index_b;

    MoveKind kind;
    bool valid = false;
};

int get_move_delta(const Move &move, const Solution& solution, const Dataset &dataset, CostMatrix dist) {
    switch(move.kind) {
        case INTER_ROUTE: {
            Node candidate = dataset[move.dataset_index];
            Node next_node = solution[(move.solution_index+1) % solution.size()];
            Node swap_out_node = solution[move.solution_index];
            Node prev_node = solution[(move.solution_index == 0) ? solution.size() - 1 : move.solution_index - 1];
            int to_cost   = dist.get(prev_node, swap_out_node);
            int from_cost = dist.get(swap_out_node, next_node);
            int old_cost = to_cost + from_cost + swap_out_node.cost;

            int new_to_cost = dist.get(prev_node, candidate);
            int new_from_cost = dist.get(candidate, next_node);
            int new_cost = new_to_cost + new_from_cost + candidate.cost;
            return new_cost - old_cost;
        }
        case INTRA_ROUTE_NODE_EXCHANGE:{
            Node next_node_a = solution[(move.swap_index_a+1) % solution.size()];
            Node node_a = solution[move.swap_index_a];
            Node prev_node_a = solution[(move.swap_index_a == 0) ? solution.size() - 1 : move.swap_index_a - 1];

            Node next_node_b = solution[(move.swap_index_b+1) % solution.size()];
            Node node_b = solution[move.swap_index_b];
            Node prev_node_b = solution[(move.swap_index_b == 0) ? solution.size() - 1 : move.swap_index_b - 1];

            int old_cost = 0;
            int new_cost = 0;
            if(next_node_a.id == node_b.id) {
                old_cost = dist.get(prev_node_a, node_a) + dist.get(node_a, node_b) + dist.get(node_b, next_node_b);
                new_cost = dist.get(prev_node_a, node_b) + dist.get(node_b, node_a) + dist.get(node_a, next_node_b);

            } else if (next_node_b.id == node_a.id) {
                old_cost = dist.get(prev_node_b, node_b) + dist.get(node_b, node_a) + dist.get(node_a, next_node_a);
                new_cost = dist.get(prev_node_b, node_a) + dist.get(node_a, node_b) + dist.get(node_b, next_node_a);

            } else {
                old_cost = dist.get(prev_node_a, node_a) + dist.get(node_a, next_node_a) +
                    dist.get(prev_node_b, node_b) + dist.get(node_b, next_node_b);
                new_cost = dist.get(prev_node_a, node_b) + dist.get(node_b, next_node_a) +
                    dist.get(prev_node_b, node_a) + dist.get(node_a, next_node_b);
            }
            return new_cost-old_cost;
        }
        case INTRA_ROUTE_EDGE_EXCHANGE:{
            // swapping (a -> c) and (b -> d) with (a -> b) and (c -> d)
            int a = move.swap_index_a;
            int b = move.swap_index_b;
            int c = (a + 1) % (int)solution.size();
            int d = (b + 1) % (int)solution.size();

            Node node_a = solution[a];
            Node node_b = solution[b];
            Node node_c = solution[c];
            Node node_d = solution[d];

            int old_cost = dist.get(node_a, node_c) + dist.get(node_b, node_d);
            int new_cost = dist.get(node_a, node_b) + dist.get(node_c, node_d);

            return new_cost-old_cost;
        }
        default:
            panicf("impossible");
    }
}

static inline Move move_inter_route(int solution_index, int dataset_index) {
    Move move;
    move.kind = INTER_ROUTE;
    move.solution_index = solution_index;
    move.dataset_index = dataset_index;
    move.valid = true;
    return move;
}

static inline Move move_intra_route_node_exchange(int index_node_a, int index_node_b) {
    Move move;
    move.kind = INTRA_ROUTE_NODE_EXCHANGE;
    move.swap_index_a = index_node_a;
    move.swap_index_b = index_node_b;
    move.valid = true;
    return move;
}

static inline Move move_intra_route_edge_exchange(int index_edge_a, int index_edge_b) {
    Move move;
    move.kind = INTRA_ROUTE_EDGE_EXCHANGE;
    move.swap_index_a = index_edge_a;
    move.swap_index_b = index_edge_b;
    move.valid = true;
    return move;
}

Move get_best_random_move(const Solution& solution, const Dataset &dataset, CostMatrix dist,
        const std::vector<bool>&used, MoveKind intra_route_move_kind) {

    int inter_solution_index = 0;
    int inter_dataset_index = 0;

    int intra_node_a_index = 0;
    int intra_node_b_index = 1;
    int range_start = 0;
    int range_end = 1;
    while(true) {
        if(range_end - range_start < 0) {
            break;
        }
        std::uniform_int_distribution<int> rng_dist(range_start, range_end);
        int move_kind = rng_dist(rng);
        switch(move_kind) {
            case 0:{
                if((size_t)inter_dataset_index >= dataset.size()) {
                    inter_solution_index++;
                    if((size_t)inter_solution_index >= solution.size()) {
                        range_start = 1;
                        break;
                    }
                    inter_dataset_index = 0;
                }
                if(used[inter_dataset_index]) {
                    inter_dataset_index++;
                    break;
                }

                Move move = move_inter_route(inter_solution_index, inter_dataset_index);
                int delta = get_move_delta(move, solution, dataset, dist);
                if(delta < 0) {
                    return move;
                }
                inter_dataset_index++;
                break;
            }
            case 1: {
                if((size_t)intra_node_b_index >= solution.size()) {
                    intra_node_a_index++;
                    intra_node_b_index = intra_node_a_index+1;
                    if((size_t)intra_node_a_index >= solution.size() || (size_t)intra_node_b_index >= solution.size()) {
                        range_end = 0;
                        break;
                    }
                }
                Move move;
                if(intra_route_move_kind == INTRA_ROUTE_EDGE_EXCHANGE){
                    move = move_intra_route_edge_exchange(intra_node_a_index, intra_node_b_index);
                } else if(intra_route_move_kind == INTRA_ROUTE_NODE_EXCHANGE) {
                    move = move_intra_route_node_exchange(intra_node_a_index, intra_node_b_index);
                }
                int delta = get_move_delta(move, solution, dataset, dist);
                if(delta < 0) {
                    return move;
                }
                intra_node_b_index++;
                break;
            }

        }
    }
    Move m;
    m.valid = false;
    return m;
}

bool act_on_move(const Move &move, Solution &solution, const Dataset &dataset, std::vector<bool> &used) {
    if(!move.valid) {
        return false;
    }
    switch(move.kind) {
        case INTER_ROUTE:{
            used[solution[move.solution_index].id] = false;
            used[move.dataset_index] = true;
            solution[move.solution_index] = dataset[move.dataset_index];
            return true;
        }

        case INTRA_ROUTE_NODE_EXCHANGE:{
            Node tmp = solution[move.swap_index_b];
            solution[move.swap_index_b] = solution[move.swap_index_a];
            solution[move.swap_index_a] = tmp;
            return true;
        }

        case INTRA_ROUTE_EDGE_EXCHANGE: {
            int n = (int)solution.size();
            int a = move.swap_index_a;
            int b = move.swap_index_b;

            if (a == b) return false;

            if (a < b) { // standard case
                std::reverse(solution.begin() + a + 1, solution.begin() + b + 1);
            } else { // wraparound case
                std::vector<Node> segment;

                segment.insert(segment.end(), solution.begin() + a + 1, solution.end());
                segment.insert(segment.end(), solution.begin(), solution.begin() + b + 1);

                std::reverse(segment.begin(), segment.end());

                int idx = a + 1;
                for (size_t i = 0; i < segment.size(); ++i) {
                    solution[idx % n] = segment[i];
                    idx++;
                }
            }
            return true;
        }
    }
    return false;
}


Solution get_local_search_greedy(Solution solution, const Dataset &dataset, CostMatrix dist, MoveKind intra_route_move_kind) {

    std::vector<bool> used(dataset.size(), false);

    for(size_t i = 0; i < solution.size(); i++) {
        used[solution[i].id] = true;
    }

    bool okay = true;
    while(okay) {
        Move next_move = get_best_random_move(solution, dataset, dist, used, intra_route_move_kind);
        okay = act_on_move(next_move, solution, dataset, used);
    }
    return solution;
}


Move get_best_move_steepest(const Solution& solution, const Dataset &dataset, CostMatrix dist,
                            const std::vector<bool>& used, MoveKind intra_route_move_kind) {
    Move best_move;
    best_move.valid = false;
    int best_delta = 0;

    for (int i = 0; i < (int)solution.size(); i++) {
        for (int j = 0; j < (int)dataset.size(); j++) {
            if (used[j]) continue;
            Move move = move_inter_route(i, j);
            int delta = get_move_delta(move, solution, dataset, dist);
            if (delta < best_delta) {
                best_delta = delta;
                best_move = move;
            }
        }
    }

    if (intra_route_move_kind == INTRA_ROUTE_NODE_EXCHANGE) {
        for (int a = 0; a < (int)solution.size()-1; a++) {
            for (int b = a+1; b < (int)solution.size(); b++) {
                Move move = move_intra_route_node_exchange(a, b);
                int delta = get_move_delta(move, solution, dataset, dist);
                if (delta < best_delta) {
                    best_delta = delta;
                    best_move = move;
                }
            }
        }
    } else if (intra_route_move_kind == INTRA_ROUTE_EDGE_EXCHANGE) {
        for (int a = 0; a < (int)solution.size()-1; a++) {
            for (int b = a+1; b < (int)solution.size(); b++) {
                Move move = move_intra_route_edge_exchange(a, b);
                int delta = get_move_delta(move, solution, dataset, dist);
                if (delta < best_delta) {
                    best_delta = delta;
                    best_move = move;
                }
            }
        }
    }

    if (best_delta < 0) {
        return best_move;
    } else {
        best_move.valid = false;
        return best_move;
    }

    return best_move;
}


Solution get_local_search_steepest(Solution solution, const Dataset &dataset, CostMatrix dist, MoveKind intra_route_move_kind) {
    std::vector<bool> used(dataset.size(), false);
    for (size_t i = 0; i < solution.size(); i++) {
        used[solution[i].id] = true;
    }

    bool okay = true;
    while(okay) {
        Move best_move = get_best_move_steepest(solution, dataset, dist, used, intra_route_move_kind);
        okay = act_on_move(best_move, solution, dataset, used);
    }

    return solution;
}

bool solution_valid(const Solution& solution, const Dataset &dataset) {
    std::vector<bool> used(dataset.size(), false);
    for(size_t i = 0; i < solution.size(); i++) {
        Node n = solution[i];
        if(used[n.id]) return false;
        used[n.id] = true;
    }
    return true;
}

std::vector<std::vector<int>> precompute_nearest_neighbors(const Dataset &dataset, CostMatrix &dist, int n_candidates) {
    size_t n = dataset.size();
    std::vector<std::vector<int>> neighbors(n);

    for (size_t i = 0; i < n; i++) {
        std::vector<std::pair<int,int>> queue;
        for (size_t j = 0; j < n; j++) {
            if (i == j) continue;
            int cost = dist.get(dataset[i], dataset[j]) + dataset[j].cost;
            queue.emplace_back(cost, j);
        }
        std::nth_element(queue.begin(), queue.begin() + std::min(n_candidates, (int)queue.size()), queue.end(),
                         [](auto &a, auto &b){ return a.first < b.first; });
        int count = std::min(n_candidates, (int)queue.size());
        neighbors[i].resize(count);
        for (int k = 0; k < count; k++) neighbors[i][k] = queue[k].second;
    }
    return neighbors;
}

Move get_best_move_candidate(const Solution& solution, const Dataset &dataset, CostMatrix dist,
                                         const std::vector<bool>& used, MoveKind intra_route_move_kind,
                                         const std::vector<std::vector<int>>& inter_neighbors) {
    Move best_move;
    best_move.valid = false;
    int best_delta = 0;

    for (size_t i = 0; i < solution.size(); i++) {
        Node candidate = solution[i];
        for (int j : inter_neighbors[candidate.id]) {
            if (used[j]) continue;
            Move move = move_inter_route(i, j);
            int delta = get_move_delta(move, solution, dataset, dist);
            if (delta < best_delta) {
                best_move = move;
                best_delta = delta;
            }
        }
    }

    int shift = (intra_route_move_kind == INTRA_ROUTE_NODE_EXCHANGE) ? 2 :
                (intra_route_move_kind == INTRA_ROUTE_EDGE_EXCHANGE) ? 1 : 0;

    for (size_t i = 0; i < solution.size() - shift; i++) {
        for (size_t j = i + shift; j < solution.size(); j++) {
            Move move;
            if (intra_route_move_kind == INTRA_ROUTE_NODE_EXCHANGE)
                move = move_intra_route_node_exchange(i, j);
            else if (intra_route_move_kind == INTRA_ROUTE_EDGE_EXCHANGE)
                move = move_intra_route_edge_exchange(i, j);
            else
                continue;

            int delta = get_move_delta(move, solution, dataset, dist);
            if (delta < best_delta) {
                best_move = move;
                best_delta = delta;
            }
        }
    }

    return best_move;
}

Solution get_local_search_candidate(Solution solution, const Dataset &dataset, CostMatrix dist,
                                                MoveKind intra_route_move_kind, int n_candidates) {
    std::vector<bool> used(dataset.size(), false);
    for (size_t i = 0; i < solution.size(); i++) used[solution[i].id] = true;

    auto inter_neighbors = precompute_nearest_neighbors(dataset, dist, n_candidates);

    bool okay = true;
    while (okay) {
        Move best_move = get_best_move_candidate(solution, dataset, dist, used, intra_route_move_kind, inter_neighbors);
        okay = act_on_move(best_move, solution, dataset, used);
    }

    assert(solution_valid(solution, dataset));
    return solution;
}

struct DirEdge {
    int u_id;
    int v_id;
};

enum EdgeCheck {
    EDGES_MISSING,   // at least one removed edge no longer exists in current solution
    EDGES_REVERSED,  // all removed edges exist, but reversed in direction
    EDGES_SAME       // all removed edges exist in the same direction
};

struct MoveId {
    MoveKind kind;
    int out_node_id;
    int in_node_id;
    int node_a_id;
    int node_b_id;
    bool valid = false;
};

struct LMEntry {
    MoveId move;
    int delta;
    std::vector<DirEdge> removed_edges;
};


std::vector<int> build_pos(const std::vector<Node>& sol) {
    std::vector<int> pos;
    int max_id = 0;
    for (auto& x : sol) if (x.id > max_id) max_id = x.id;
    pos.assign(max_id + 1, -1);
    for (int i = 0; i < (int)sol.size(); i++) pos[sol[i].id] = i;
    return pos;
}

EdgeCheck check_removed_edges(const std::vector<Node>& sol, const std::vector<int>& pos, const std::vector<DirEdge>& removed) {
    bool all_same = true;
    bool all_rev = true;
    for (auto& e : removed) {
        if (pos[e.u_id] == -1 || pos[e.v_id] == -1) return EDGES_MISSING;
        
        int i = pos[e.u_id];
        int j = (i + 1) % (int)sol.size();
        bool same = (sol[j].id == e.v_id);
        
        i = pos[e.v_id];
        j = (i + 1) % (int)sol.size();
        bool rev = (sol[j].id == e.u_id);
        
        if (!(same || rev)) return EDGES_MISSING;
        if (!same) all_same = false;
        if (!rev)  all_rev  = false;
    }
    if (all_same) return EDGES_SAME;
    if (all_rev)  return EDGES_REVERSED;
    return EDGES_MISSING;
}

MoveId to_move_id_from_indices(const Move& m, const std::vector<Node>& sol, const Dataset& dataset) {
    MoveId r;
    r.valid = m.valid;
    r.kind = m.kind;
    if (m.kind == INTER_ROUTE) {
        r.out_node_id = sol[m.solution_index].id;
        r.in_node_id = dataset[m.dataset_index].id;
    } else if (m.kind == INTRA_ROUTE_NODE_EXCHANGE) {
        r.node_a_id = sol[m.swap_index_a].id;
        r.node_b_id = sol[m.swap_index_b].id;
    } else if (m.kind == INTRA_ROUTE_EDGE_EXCHANGE) {
        r.node_a_id = sol[m.swap_index_a].id;
        r.node_b_id = sol[m.swap_index_b].id;
    }
    return r;
}

Move to_indices_from_move_id(const MoveId& m, const std::vector<Node>& sol, const std::vector<int>& pos, const std::vector<bool>& used) {
    Move r;
    r.valid = false;
    if (!m.valid) return r;
    r.kind = m.kind;
    if (m.kind == INTER_ROUTE) {
        if (m.in_node_id < 0 || m.in_node_id >= (int)used.size()) return r;
        if (used[m.in_node_id]) return r;
        int si = pos[m.out_node_id];
        if (si < 0 || si >= (int)sol.size()) return r;
        int dj = m.in_node_id;
        r = move_inter_route(si, dj);
    } else if (m.kind == INTRA_ROUTE_NODE_EXCHANGE) {
        int a = pos[m.node_a_id];
        int b = pos[m.node_b_id];
        if (a < 0 || b < 0 || a == b) return r;
        r = move_intra_route_node_exchange(a, b);
    } else if (m.kind == INTRA_ROUTE_EDGE_EXCHANGE) {
        int a = pos[m.node_a_id];
        int b = pos[m.node_b_id];
        if (a < 0 || b < 0 || a == b) return r;
        r = move_intra_route_edge_exchange(a, b);
    }
    r.valid = true;
    return r;
}

std::vector<DirEdge> removed_by_move_indices(const Move& m, const std::vector<Node>& sol) {
    std::vector<DirEdge> rem;
    int n = (int)sol.size();
    if (m.kind == INTER_ROUTE) {
        int i = m.solution_index;
        int ip = (i == 0) ? n - 1 : i - 1;
        int in = (i + 1) % n;
        rem.push_back({sol[ip].id, sol[i].id});
        rem.push_back({sol[i].id, sol[in].id});
    } else if (m.kind == INTRA_ROUTE_NODE_EXCHANGE) {
        int a = m.swap_index_a;
        int b = m.swap_index_b;
        int ap = (a == 0) ? n - 1 : a - 1;
        int an = (a + 1) % n;
        int bp = (b == 0) ? n - 1 : b - 1;
        int bn = (b + 1) % n;
        if (an == b) {
            rem.push_back({sol[ap].id, sol[a].id});
            rem.push_back({sol[b].id, sol[bn].id});
            rem.push_back({sol[a].id, sol[b].id});
        } else if (bn == a) {
            rem.push_back({sol[b].id, sol[a].id});
            rem.push_back({sol[ap].id, sol[a].id});
            rem.push_back({sol[b].id, sol[bn].id});
        } else {
            rem.push_back({sol[ap].id, sol[a].id});
            rem.push_back({sol[a].id, sol[an].id});
            rem.push_back({sol[bp].id, sol[b].id});
            rem.push_back({sol[b].id, sol[bn].id});
        }
    } else if (m.kind == INTRA_ROUTE_EDGE_EXCHANGE) {
        int a = m.swap_index_a;
        int b = m.swap_index_b;
        int c = (a + 1) % n;
        int d = (b + 1) % n;
        rem.push_back({sol[a].id, sol[c].id});
        rem.push_back({sol[b].id, sol[d].id});
    }
    return rem;
}

void store_move_variants(
    const Move& m,
    const std::vector<Node>& sol,
    std::vector<LMEntry>& lm,
    const Dataset& dataset,
    int delta
) {
    LMEntry e;
    e.move = to_move_id_from_indices(m, sol, dataset);
    e.delta = delta;
    e.removed_edges = removed_by_move_indices(m, sol);
    lm.push_back(e);

    LMEntry inverted = e;
    for (auto& ed : inverted.removed_edges)
        std::swap(ed.u_id, ed.v_id);
    lm.push_back(inverted);
}

bool browse_LM_and_apply(
    std::vector<LMEntry>& lm,
    std::vector<Node>& solution,
    std::vector<bool>& used,
    const Dataset& dataset,
    const CostMatrix& dist
) {
    if (lm.empty()) return false;
    auto pos = build_pos(solution);

    for (size_t i = 0; i < lm.size(); ) {
        auto& entry = lm[i];
        auto chk = check_removed_edges(solution, pos, entry.removed_edges);

        if (chk == EDGES_MISSING) {
            lm[i] = std::move(lm.back());
            lm.pop_back();
            continue;
        }

        if (chk == EDGES_REVERSED) { ++i; continue; }

        Move midx = to_indices_from_move_id(entry.move, solution, pos, used);
        if (!midx.valid) {
            lm[i] = std::move(lm.back());
            lm.pop_back();
            continue;
        }

        int delta_now = get_move_delta(midx, solution, dataset, dist);
        if (delta_now >= 0) {
            lm[i] = std::move(lm.back());
            lm.pop_back();
            continue;
        }

        if (act_on_move(midx, solution, dataset, used)) {
            lm[i] = std::move(lm.back());
            lm.pop_back();
            return true;
        }

        ++i;
    }
    return false;
}

Move get_best_move_and_fill_LM(
    const Solution& solution,
    const Dataset& dataset,
    const CostMatrix& dist,
    const std::vector<bool>& used,
    MoveKind intra_route_move_kind,
    std::vector<LMEntry>& lm
) {
    Move best_move;
    best_move.valid = false;
    int best_delta = 0;

    for (int i = 0; i < (int)solution.size(); i++) {
        for (int j = 0; j < (int)dataset.size(); j++) {
            if (used[j]) continue;
            Move m = move_inter_route(i, j);
            int d = get_move_delta(m, solution, dataset, dist);
            if (d < 0) store_move_variants(m, solution, lm, dataset, d);
            if (d < best_delta) { best_delta = d; best_move = m; }
        }
    }

    if (intra_route_move_kind == INTRA_ROUTE_NODE_EXCHANGE) {
        for (int a = 0; a < (int)solution.size() - 1; a++)
            for (int b = a + 1; b < (int)solution.size(); b++) {
                Move m = move_intra_route_node_exchange(a, b);
                int d = get_move_delta(m, solution, dataset, dist);
                if (d < 0) store_move_variants(m, solution, lm, dataset, d);
                if (d < best_delta) { best_delta = d; best_move = m; }
            }
    } else if (intra_route_move_kind == INTRA_ROUTE_EDGE_EXCHANGE) {
        for (int a = 0; a < (int)solution.size() - 1; a++)
            for (int b = a + 1; b < (int)solution.size(); b++) {
                Move m = move_intra_route_edge_exchange(a, b);
                int d = get_move_delta(m, solution, dataset, dist);
                if (d < 0) store_move_variants(m, solution, lm, dataset, d);
                if (d < best_delta) { best_delta = d; best_move = m; }
            }
    }

    if (best_delta < 0) return best_move;
    best_move.valid = false;
    return best_move;
}

Solution get_local_search_steepest_with_LM(
    Solution solution,
    const Dataset& dataset,
    const CostMatrix& dist,
    MoveKind intra_route_move_kind
) {
    std::vector<bool> used(dataset.size(), false);
    for (auto& x : solution) used[x.id] = true;
    std::vector<LMEntry> lm;
    bool progressed = true;

    while (progressed) {
        progressed = false;
        if (browse_LM_and_apply(lm, solution, used, dataset, dist)) {
            progressed = true;
            continue;
        }

        Move best = get_best_move_and_fill_LM(solution, dataset, dist, used, intra_route_move_kind, lm);
        if (!best.valid) break;
        if (act_on_move(best, solution, dataset, used)) progressed = true;
    }
    return solution;
}

Solution get_multiple_start_local_search(Dataset dataset, CostMatrix dist) {
    Solution best;
    int best_score = INT_MAX;
    for (int i = 0; i < 200; i++) {
        Solution seed = get_random_solution(dataset);
        Solution candidate = get_local_search_steepest(seed, dataset, dist, INTRA_ROUTE_EDGE_EXCHANGE);
        int score = compute_total_cost(candidate, dist);
        if(best_score > score) {
            best_score = score;
            best = candidate;
        }
    }
    return best;
}


Solution perturbate(Solution solution, const Dataset &dataset) {
    std::vector<bool> used(dataset.size(), false);
    for (size_t i = 0; i < solution.size(); i++) used[solution[i].id] = true;

    for(int i = 0; i < 10; i++) {
        std::uniform_int_distribution<int> rng_dist(1, 10);
        int move_kind = rng_dist(rng);
        std::uniform_int_distribution<int> rng_sol(0, solution.size() - 1);
        std::uniform_int_distribution<int> rng_dataset(0, dataset.size() - 1);
        if(move_kind <= 5) {
            int sol_index = rng_sol(rng);
            int dataset_index = rng_dataset(rng);
            for(; used[dataset_index]; dataset_index = (dataset_index + 1) % dataset.size());
            Move move = move_inter_route(sol_index, dataset_index);
            act_on_move(move, solution, dataset, used);
        } else {
            int sol_index1 = rng_sol(rng);
            int sol_index2 = rng_sol(rng);
            if(sol_index1 == sol_index2) {
                sol_index2 = (sol_index2 + 2) % solution.size();
            }
            Move move = move_intra_route_edge_exchange(sol_index1, sol_index2);
            act_on_move(move, solution, dataset, used);
        }
    }
    return solution;
}

struct SolutionAndIteration {
    Solution solution;
    int ls_runs;
};


SolutionAndIteration get_iterated_local_search(const Dataset &dataset, const CostMatrix &dist)
{
    int ls_runs = 0;
    auto start_time = std::chrono::high_resolution_clock::now();
    Solution seed = get_random_solution(dataset);
    Solution best = get_local_search_steepest(seed, dataset, dist, INTRA_ROUTE_EDGE_EXCHANGE);
    int best_score = compute_total_cost(best, dist);
    ls_runs++;

    while (true) {
        auto current_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = current_time - start_time;
        if (elapsed.count() >= 2.943)
            break;

        ls_runs++;
        Solution pert = perturbate(best, dataset);
        Solution candidate = get_local_search_steepest(pert, dataset, dist, INTRA_ROUTE_EDGE_EXCHANGE);
        int score = compute_total_cost(candidate, dist);
        if(score < best_score)
        {
            best_score = score;
            best = candidate;
        }
    }

    return { best, ls_runs };
}

Solution destroy(Solution solution) {
    int to_remove = solution.size() * 0.30f;
    for (int n_removed = 0; n_removed < to_remove; ) {
        std::uniform_int_distribution<int> rng_sol(0, solution.size() - 1);
        std::uniform_int_distribution<int> rng_path_length(5, 8);
        int path_start = rng_sol(rng);
        int path_length = rng_path_length(rng);
        int n_to_remove_wraparound = (path_start + path_length) - solution.size();
        int n_to_remove_normal = path_length;
        if (n_to_remove_wraparound > 0) {
             path_length = path_length - n_to_remove_wraparound;
        }
        for(int i = 0; i < n_to_remove_normal; i++) {
            solution.erase(solution.begin() + path_start);
        }
        for(int i = 0; i < n_to_remove_wraparound; i++) {
            solution.erase(solution.begin() + 0);
        }
        n_removed += path_length;
    }
    return solution;
}

Solution rebuild(Solution solution, const Dataset &dataset, CostMatrix dist) {
    int target_size = (dataset.size() + 1) / 2;
    std::vector<bool> used(dataset.size(), false);
    for (size_t i = 0; i < solution.size(); i++) {
      used[solution[i].id] = true;
    }
    float weight_cost = 0.5f;
    float weight_regret = 1.0f - weight_cost;
    while ((int)solution.size() < target_size) {
        int final_insert_location = -1;
        int best_weighted_sum = INT_MIN;
        int insert_id = -1;
        for (size_t j = 0; j < dataset.size(); j++) {
            if (used[j]) continue;
            int best_place_cost = INT_MAX;
            int second_best_place_cost = INT_MAX;
            int insert_location;
            for (int i = 0; i < (int)solution.size(); i++) {
                Node current_node = solution[i];
                Node next_node = solution[(i+1)%solution.size()];
                int curr_cost = dist.get(current_node, next_node);
                int new_cost = dist.get(current_node, dataset[j]) + dist.get(dataset[j], next_node) + dataset[j].cost;
                int cost = new_cost - curr_cost;
                if(best_place_cost > cost) {
                    second_best_place_cost = best_place_cost;
                    best_place_cost = cost;
                    insert_location = i;
                } else if (second_best_place_cost > cost) {
                    second_best_place_cost = cost;
                }
            }
            int regret = second_best_place_cost - best_place_cost;
            assert(regret >= 0);
            int weighted_sum = -(float)best_place_cost * weight_cost + (float)regret * weight_regret;
            if(best_weighted_sum < weighted_sum) {
                best_weighted_sum = weighted_sum;
                final_insert_location = insert_location;
                insert_id = j;
            }
        }

        assert(insert_id != -1);
        assert(final_insert_location != -1);
        solution.insert(solution.begin() + final_insert_location + 1, dataset[insert_id]);
        used[insert_id] = true;
    }

    return solution;
}

SolutionAndIteration get_large_neighborhood_search_base(const Dataset& dataset, CostMatrix dist, bool do_local_search) {
    int ls_runs = 0;
    auto start_time = std::chrono::high_resolution_clock::now();
    Solution best = get_random_solution(dataset);
    int best_score = compute_total_cost(best, dist);
    ls_runs++;

    while (true) {
        auto current_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = current_time - start_time;
        if (elapsed.count() >= 2.943)
            break;
        Solution y = destroy(best);
        if(do_local_search) {
            y = get_local_search_steepest(y, dataset, dist, INTRA_ROUTE_EDGE_EXCHANGE);
        }
        y = rebuild(y, dataset, dist);
        int cost = compute_total_cost(y, dist);
        if(cost < best_score) {
            best_score = cost;
            best = y;
        }
        ls_runs++;
    }
    std::cout << best.size() << std::endl;
    return {best, ls_runs};
}

SolutionAndIteration get_large_neighborhood_search(const Dataset& dataset, CostMatrix dist) {
    return get_large_neighborhood_search_base(dataset, dist, false);
}

SolutionAndIteration get_large_neighborhood_search_with_local_search(const Dataset& dataset, CostMatrix dist) {
    return get_large_neighborhood_search_base(dataset, dist, true);
}
