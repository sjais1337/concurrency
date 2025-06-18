#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <thread>
#include <mutex>
#include <filesystem>
#include <algorithm>
#include <chrono>

const std::string DICT_PATH = "./10000_most_common"; 
const size_t TARGET_SIZE_KB = 500;
const size_t NUM_FILES = 16;
const size_t WORDS_PER_CHUNK = 100;
const size_t TARGET_SIZE_BYTES = TARGET_SIZE_KB * 1024;

std::mutex print_mutex;

std::vector<std::string> load_words(const std::string& path) {
    std::ifstream file(path);
    std::vector<std::string> words;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && std::all_of(line.begin(), line.end(), ::isalpha)) {
            words.push_back(line);
        }
    }
    return words;
}

void generate_file(const std::vector<std::string>& words, const std::string& filename) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::lock_guard<std::mutex> lock(print_mutex);
        std::cerr << "Failed to open file: " << filename << "\n";
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, words.size() - 1);

    size_t total_bytes = 0;

    while (total_bytes < TARGET_SIZE_BYTES) {
        std::string chunk;
        size_t word_count = 0;

        for (size_t i = 0; i < WORDS_PER_CHUNK; ++i) {
            chunk += words[dist(gen)];
            ++word_count;

            if (word_count % 50 == 0) {
                chunk += "\n";  // New line after every 50 words
            } else {
                chunk += " ";
            }
        }

        out << chunk;
        total_bytes += chunk.size();

        if (total_bytes % (10 * 1024 ) < chunk.size()) {
            std::lock_guard<std::mutex> lock(print_mutex);
            std::cout << filename << ": " << (total_bytes / (1024)) << " KB written...\n";
        }
    }

    out.close();
    std::lock_guard<std::mutex> lock(print_mutex);
    std::cout << "Finished: " << filename << "\n";
}

int main() {
    std::vector<std::string> words = load_words(DICT_PATH);
    if (words.empty()) {
        std::cerr << "Dictionary file not found or empty.\n";
        return 1;
    }

    std::vector<std::thread> threads;

    std::cout << "Generating " << NUM_FILES << " files of " << TARGET_SIZE_KB << "KB each...\n";

    auto time_start = std::chrono::high_resolution_clock::now();

    for (size_t i = 1; i <= NUM_FILES; ++i) {
        std::string filename = "small/" + std::to_string(i) + ".txt";
        threads.emplace_back(generate_file, std::cref(words), filename);
    }

    for (auto& t : threads) {
        t.join();
    }

    auto time_end = std::chrono::high_resolution_clock::now();

    auto time_difference = time_end - time_start; 

    std::cout << "All files generated successfully in " << time_difference.count() << " seconds.";
    return 0;
}

