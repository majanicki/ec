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
#include <random>
#include <algorithm>
#include <sstream>

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
    if (!IsWindowReady()) {
        InitWindow(1, 1, "This is a title");
        SetWindowState(FLAG_WINDOW_HIDDEN);
    }
    
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

    if (IsWindowReady()) {
        CloseWindow();
    }
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
                 CostMatrix dist, const std::string& name) {
    double min_cost  = measure_min(solutions, dist);
    double mean_cost = measure_mean(solutions, dist);
    double max_cost  = measure_max(solutions, dist);

    std::cout << name << " min cost: "  << min_cost << std::endl;
    std::cout << name << " mean cost: " << mean_cost << std::endl;
    std::cout << name << " max cost: "  << max_cost << std::endl;

    Solution best_solution = get_best_solution(solutions, dist);

    std::string lore = "min: " + std::to_string((int)min_cost) +
                       ", mean: " + std::to_string((int)mean_cost) +
                       ", max: " + std::to_string((int)max_cost);

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

Move get_best_random_move(const Solution& solution, const Dataset &dataset, CostMatrix dist,
        const std::vector<bool>&used, MoveKind inter_route_move_kind) {

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
                Node candidate = dataset[inter_dataset_index];
                Node next_node = solution[(inter_solution_index+1) % solution.size()];
                Node swap_out_node = solution[inter_solution_index];
                Node prev_node = solution[(inter_solution_index == 0) ? solution.size() - 1 : inter_solution_index - 1];
                int to_cost   = dist.get(prev_node, swap_out_node);
                int from_cost = dist.get(swap_out_node, next_node);
                int old_cost = to_cost + from_cost + swap_out_node.cost;

                int new_to_cost = dist.get(prev_node, candidate);
                int new_from_cost = dist.get(candidate, next_node);
                int new_cost = new_to_cost + new_from_cost + candidate.cost;

                if(new_cost < old_cost) {
                    Move move;
                    move.kind = INTER_ROUTE;
                    move.solution_index = inter_solution_index;
                    move.dataset_index = inter_dataset_index;
                    move.valid = true;
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
                Node next_node_a = solution[(intra_node_a_index+1) % solution.size()];
                Node node_a = solution[intra_node_a_index];
                Node prev_node_a = solution[(intra_node_a_index == 0) ? solution.size() - 1 : intra_node_a_index - 1];

                Node next_node_b = solution[(intra_node_b_index+1) % solution.size()];
                Node node_b = solution[intra_node_b_index];
                Node prev_node_b = solution[(intra_node_b_index == 0) ? solution.size() - 1 : intra_node_b_index - 1];

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

                if(new_cost < old_cost) {
                    Move move;
                    move.kind = INTRA_ROUTE_NODE_EXCHANGE;
                    move.swap_index_a = intra_node_a_index;
                    move.swap_index_b = intra_node_b_index;
                    move.valid = true;
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
        case INTER_ROUTE:
            used[solution[move.solution_index].id] = false;
            used[move.dataset_index] = true;
            solution[move.solution_index] = dataset[move.dataset_index];
            return true;
            break;
        case INTRA_ROUTE_NODE_EXCHANGE:
            Node tmp = solution[move.swap_index_b];
            solution[move.swap_index_b] = solution[move.swap_index_a];
            solution[move.swap_index_a] = tmp;
            return true;
            break;
    }
    return false;

}


Solution get_local_search_greedy(Solution solution, const Dataset &dataset, CostMatrix dist) {

    std::vector<bool> used(dataset.size(), false);

    for(size_t i = 0; i < solution.size(); i++) {
        used[solution[i].id] = true;
    }

    bool okay = true;
    while(okay) {
        Move next_move = get_best_random_move(solution, dataset, dist, used, INTRA_ROUTE_EDGE_EXCHANGE);
        okay = act_on_move(next_move, solution, dataset, used);
    }
    return solution;
}
