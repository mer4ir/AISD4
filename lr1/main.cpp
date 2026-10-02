#include "balanced_set.h"
#include <iostream>
#include <vector>

static void demonstrate_class() {
    std::cout << "\n=== Balancedset class ===\n" << std::endl;
    
    BalancedSet set;
    
    std::cout << "Inserting: 10, 5, 15, 3, 7, 12, 17" << std::endl;
    set.insert(10);
    set.insert(5);
    set.insert(15);
    set.insert(3);
    set.insert(7);
    set.insert(12);
    set.insert(17);
    
    std::cout << "Tree (inorder): ";
    set.print();
    
    std::cout << "\nContains 7? " << (set.contains(7) ? "Yes" : "No") << std::endl;
    std::cout << "Contains 8? " << (set.contains(8) ? "Yes" : "No") << std::endl;
    
    std::cout << "\nErasing 7" << std::endl;
    bool erased = set.erase(7);
    std::cout << "Result: " << (erased ? "Success" : "Not found") << std::endl;
    
    std::cout << "Tree after erase: ";
    set.print();
    
    std::cout << "\nStrictly balanced? " << (set.strictly_balanced() ? "Yes" : "No") << std::endl;
    std::cout << "Size: " << set.size() << std::endl;
    
    BalancedSet set_copy(set);
    std::cout << "\nCopy: ";
    set_copy.print();
    
    BalancedSet set2;
    set2 = set;
    std::cout << "After assignment: ";
    set2.print();
}

static void task_variant_4() {
    std::cout << "\n=== Task ===\n" << std::endl;
    
    std::vector<int> input = {3, 2, 2, 4, 2};
    
    std::cout << "Input vector:  ";
    for (int v : input) std::cout << v << " ";
    std::cout << std::endl;
    
    std::vector<int> output = get_unique_elements(input);
    
    std::cout << "Output vector: ";
    for (int v : output) std::cout << v << " ";
    std::cout << std::endl;
    
    std::cout << "Expected: 3 4" << std::endl;
}

int main() {    
    demonstrate_class();
    task_variant_4();

    return 0;
}