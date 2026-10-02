#include "HashTable.h"
#include <iostream>

int main() {
    char input1[256];
    char input2[256];
    
    std::cout << "Enter first string: ";
    std::cin.getline(input1, 256);
    
    unsigned char hash = pearsonHash(input1);
    
    std::cout << "Enter second string: ";
    std::cin.getline(input2, 256);
    
    bool result = compareStoredHash(hash, input2);
    
    std::cout << "Hash of first string: " << (int)hash << std::endl;
    std::cout << "Strings are equal: " << (result ? "true" : "false") << std::endl;
    
    return 0;
}