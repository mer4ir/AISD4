#include "balanced_set.h"
#include <vector>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <random>

static int lcg() {  
    static std::mt19937 gen(42);
    static std::uniform_int_distribution<int> dist(0, 999999); 
    return dist(gen);
}

static std::vector<int> generate_unique_random_numbers(size_t count) {
    std::vector<int> result;
    result.reserve(count);
    while (result.size() < count) {
        int value = lcg();
        if (std::find(result.begin(), result.end(), value) == result.end()) {
            result.push_back(value);
        }
    }
    return result;
}

static void run_balanced_set_experiments(std::ofstream& file) {
    std::cout << "=== Balancedset experiments ===\n" << std::endl;
    
    file << "=== Balancedset experiments ===\n\n";
    
    struct ExperimentConfig {
        size_t size;
        size_t fill_trials;
        size_t search_ops;
        size_t insert_erase_ops;
    };
    
    std::vector<ExperimentConfig> configs = {
        {1000,   100, 1000, 1000},
        {10000,  100, 1000, 1000},
        {100000, 100, 1000, 1000}
    };

    for (const auto& cfg : configs) {
        std::cout << "--- Container size: " << cfg.size << " ---" << std::endl;
        file << "--- Container size: " << cfg.size << " ---\n";

        double fill_time = 0.0;
        for (size_t trial = 0; trial < cfg.fill_trials; ++trial) {
            std::vector<int> random_numbers = generate_unique_random_numbers(cfg.size);
            BalancedSet set;
            auto start = std::chrono::high_resolution_clock::now();
            for (int v : random_numbers) {
                set.insert(v);
            }
            auto end = std::chrono::high_resolution_clock::now();
            fill_time += std::chrono::duration<double, std::milli>(end - start).count();
        }
        fill_time /= static_cast<double>(cfg.fill_trials);
        
        BalancedSet test_set;
        std::vector<int> numbers = generate_unique_random_numbers(cfg.size);
        for (int v : numbers) test_set.insert(v);

        double search_time = 0.0;
        for (size_t i = 0; i < cfg.search_ops; ++i) {
            int key = static_cast<int>(lcg() % 1000000);
            auto start = std::chrono::high_resolution_clock::now();
            test_set.contains(key);
            auto end = std::chrono::high_resolution_clock::now();
            search_time += std::chrono::duration<double, std::milli>(end - start).count();
        }
        search_time /= static_cast<double>(cfg.search_ops);

        double insert_erase_time = 0.0;
        for (size_t i = 0; i < cfg.insert_erase_ops; ++i) {
            int key = static_cast<int>(lcg() % 1000000);
            auto start = std::chrono::high_resolution_clock::now();
            test_set.insert(key);
            test_set.erase(key);
            auto end = std::chrono::high_resolution_clock::now();
            insert_erase_time += std::chrono::duration<double, std::milli>(end - start).count();
        }
        insert_erase_time /= static_cast<double>(cfg.insert_erase_ops);

        std::cout << std::fixed << std::setprecision(4);
        std::cout << "  Fill (avg over " << cfg.fill_trials << " trials): " 
                  << fill_time << " ms" << std::endl;
        std::cout << "  Search (avg over " << cfg.search_ops << " queries): " 
                  << search_time << " ms" << std::endl;
        std::cout << "  Insert/Erase (avg over " << cfg.insert_erase_ops << " ops): " 
                  << insert_erase_time << " ms" << std::endl;
        std::cout << std::endl;
        
        file << std::fixed << std::setprecision(4);
        file << "  Fill (avg over " << cfg.fill_trials << " trials): " 
             << fill_time << " ms\n";
        file << "  Search (avg over " << cfg.search_ops << " queries): " 
             << search_time << " ms\n";
        file << "  Insert/Erase (avg over " << cfg.insert_erase_ops << " ops): " 
             << insert_erase_time << " ms\n\n";
    }
}

static void run_vector_experiments(std::ofstream& file) {
    std::cout << "=== std::vector experiments ===\n" << std::endl;
    
    file << "=== std::vector experiments ===\n\n";

    struct ExperimentConfig {
        size_t size;
        size_t fill_trials;
        size_t search_ops;
        size_t insert_erase_ops;
    };
    
    std::vector<ExperimentConfig> configs = {
        {1000,   100, 1000, 1000},
        {10000,  100, 1000, 1000},
        {100000, 100, 1000, 1000}
    };

    for (const auto& cfg : configs) {
        std::cout << "--- Container size: " << cfg.size << " ---" << std::endl;
        file << "--- Container size: " << cfg.size << " ---\n";
        
        double fill_time = 0.0;
        for (size_t trial = 0; trial < cfg.fill_trials; ++trial) {
            std::vector<int> random_numbers = generate_unique_random_numbers(cfg.size);
            std::vector<int> vec;
            auto start = std::chrono::high_resolution_clock::now();
            for (int v : random_numbers) {
                if (std::find(vec.begin(), vec.end(), v) == vec.end()) {
                    vec.push_back(v);
                }
            }
            auto end = std::chrono::high_resolution_clock::now();
            fill_time += std::chrono::duration<double, std::milli>(end - start).count();
        }
        fill_time /= static_cast<double>(cfg.fill_trials);
        std::vector<int> test_vec = generate_unique_random_numbers(cfg.size);

        double search_time = 0.0;
        for (size_t i = 0; i < cfg.search_ops; ++i) {
            int key = static_cast<int>(lcg() % 1000000);
            auto start = std::chrono::high_resolution_clock::now();
            (void)std::find(test_vec.begin(), test_vec.end(), key);
            auto end = std::chrono::high_resolution_clock::now();
            search_time += std::chrono::duration<double, std::milli>(end - start).count();
        }
        search_time /= static_cast<double>(cfg.search_ops);

        double insert_erase_time = 0.0;
        for (size_t i = 0; i < cfg.insert_erase_ops; ++i) {
            int key = static_cast<int>(lcg() % 1000000);
            auto start = std::chrono::high_resolution_clock::now();
            
            if (std::find(test_vec.begin(), test_vec.end(), key) == test_vec.end()) {
                test_vec.push_back(key);
            }
            auto it = std::find(test_vec.begin(), test_vec.end(), key);
            if (it != test_vec.end()) {
                test_vec.erase(it);
            }
            
            auto end = std::chrono::high_resolution_clock::now();
            insert_erase_time += std::chrono::duration<double, std::milli>(end - start).count();
        }
        insert_erase_time /= static_cast<double>(cfg.insert_erase_ops);

        std::cout << std::fixed << std::setprecision(4);
        std::cout << "  Fill (avg over " << cfg.fill_trials << " trials): " 
                  << fill_time << " ms" << std::endl;
        std::cout << "  Search (avg over " << cfg.search_ops << " queries): " 
                  << search_time << " ms" << std::endl;
        std::cout << "  Insert/Erase (avg over " << cfg.insert_erase_ops << " ops): " 
                  << insert_erase_time << " ms" << std::endl;
        std::cout << std::endl;
        
        file << std::fixed << std::setprecision(4);
        file << "  Fill (avg over " << cfg.fill_trials << " trials): " 
             << fill_time << " ms\n";
        file << "  Search (avg over " << cfg.search_ops << " queries): " 
             << search_time << " ms\n";
        file << "  Insert/Erase (avg over " << cfg.insert_erase_ops << " ops): " 
             << insert_erase_time << " ms\n\n";
    }
}

int main() {
    std::ofstream results_file("results.txt");
    if (!results_file.is_open()) {
        std::cerr << "Error: Could not open results.txt for writing!" << std::endl;
        return 1;
    }
    
    run_balanced_set_experiments(results_file);
    run_vector_experiments(results_file);
    
    results_file.close();
    
    return 0;
}