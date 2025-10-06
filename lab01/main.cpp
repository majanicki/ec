#include <iostream>
#include <cstdio>
#include <cstdlib>

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
    int x;
    int y;
    int cost;
};

struct NodeSet {
    int size;
    Node *nodes;
};

NodeSet parse_dataset(char *in_file_contents) {
    std::string whole_file()
}

int main() {
    char * whole_file = read_file("./TSPA.csv");
    std::cout << whole_file;

}
