#include "HashTable.h"
#include <cstdlib>

template<typename T>
HashTable<T>::Node::Node(int k, const T& v) : key(k), value(v), next(nullptr) {}

template<typename T>
int HashTable<T>::hashFunction(int key) const {
    return key % capacity;
}

template<typename T>
HashTable<T>::HashTable(int size) : capacity(size) {
    table = new Node*[capacity];
    for (int i = 0; i < capacity; ++i) {
        table[i] = nullptr;
    }
}

template<typename T>
HashTable<T>::HashTable(int size, T minVal, T maxVal) : capacity(size) {
    table = new Node*[capacity];
    for (int i = 0; i < capacity; ++i) {
        table[i] = nullptr;
    }
    
    for (int i = 0; i < capacity; ++i) {
        int key = i;
        T value = minVal + (rand() % (maxVal - minVal + 1));
        insert(key, value);
    }
}

template<typename T>
HashTable<T>::HashTable(const HashTable& other) : capacity(other.capacity) {
    table = new Node*[capacity];
    for (int i = 0; i < capacity; ++i) {
        table[i] = nullptr;
    }
    
    for (int i = 0; i < capacity; ++i) {
        Node* current = other.table[i];
        while (current) {
            insert(current->key, current->value);
            current = current->next;
        }
    }
}

template<typename T>
HashTable<T>::~HashTable() {
    for (int i = 0; i < capacity; ++i) {
        Node* current = table[i];
        while (current) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
    }
    delete[] table;
}

template<typename T>
HashTable<T>& HashTable<T>::operator=(const HashTable& other) {
    if (this != &other) {
        for (int i = 0; i < capacity; ++i) {
            Node* current = table[i];
            while (current) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] table;
        
        capacity = other.capacity;
        table = new Node*[capacity];
        for (int i = 0; i < capacity; ++i) {
            table[i] = nullptr;
        }
        
        for (int i = 0; i < capacity; ++i) {
            Node* current = other.table[i];
            while (current) {
                insert(current->key, current->value);
                current = current->next;
            }
        }
    }
    return *this;
}


template<typename T>
void HashTable<T>::print() {
    for (int i = 0; i < capacity; ++i) {
        std::cout << "Bucket " << i << ": ";
        Node* current = table[i];
        
        if (!current) {
            std::cout << "empty";
        }
        
        while (current) {
            std::cout << "[" << current->key << " -> " << current->value << "] ";
            current = current->next;
        }
        std::cout << std::endl;
    }
}

template<typename T>
bool HashTable<T>::insert(int key, const T& value) {
    int index = hashFunction(key);
    Node* current = table[index];
    while (current) {
        if (current->key == key) {
            return false;
        }
        current = current->next;
    }
    
    Node* newNode = new Node(key, value);
    newNode->next = table[index];
    table[index] = newNode;
    return true;
}

template<typename T>
void HashTable<T>::insert_or_assign(int key, T& value) {
    int index = hashFunction(key);
    Node* current = table[index];
    while (current) {
        if (current->key == key) {
            current->value = value;
            return;
        }
        current = current->next;
    }
    
    Node* newNode = new Node(key, value);
    newNode->next = table[index];
    table[index] = newNode;
}

template<typename T>
bool HashTable<T>::contains(T& value) {
    for (int i = 0; i < capacity; ++i) {
        Node* current = table[i];
        while (current) {
            if (current->value == value) {
                return true;
            }
            current = current->next;
        }
    }
    return false;
}

template<typename T>
T* HashTable<T>::search(int key) {
    int index = hashFunction(key);
    Node* current = table[index];
    while (current) {
        if (current->key == key) {
            return &(current->value);
        }
        current = current->next;
    }
    return nullptr;
}

template<typename T>
bool HashTable<T>::erase(int key) {
    int index = hashFunction(key);
    Node* current = table[index];
    Node* prev = nullptr;
    
    while (current) {
        if (current->key == key) {
            if (prev) {
                prev->next = current->next;
            } else {
                table[index] = current->next;
            }
            delete current;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}

template<typename T>
int HashTable<T>::count(int key) {
    int index = hashFunction(key);
    int cnt = 0;
    Node* current = table[index];
    while (current) {
        cnt++;
        current = current->next;
    }
    return cnt;
}

unsigned char pearsonHash(const char* str) {
    unsigned char hash = 0;
    const unsigned char lookupTable[256] = {
        109, 106, 30, 158, 120, 147, 85, 174, 29, 151, 81, 235, 25, 60, 183, 126,
        218, 216, 69, 74, 129, 36, 104, 135, 11, 119, 157, 55, 190, 31, 236, 42,
        14, 185, 207, 188, 163, 204, 97, 70, 105, 238, 27, 88, 94, 166, 41, 229,
        86, 128, 138, 197, 148, 187, 152, 203, 75, 224, 20, 246, 72, 16, 66, 145,
        124, 108, 68, 23, 177, 91, 223, 192, 213, 77, 48, 56, 15, 251, 61, 17,
        169, 112, 121, 107, 114, 89, 51, 98, 205, 49, 111, 189, 116, 47, 234, 53,
        201, 215, 139, 245, 212, 132, 19, 9, 233, 83, 34, 103, 39, 18, 5, 249,
        143, 43, 10, 130, 168, 191, 90, 193, 131, 73, 181, 242, 172, 159, 253, 250,
        142, 220, 133, 230, 118, 171, 45, 150, 149, 123, 100, 184, 176, 160, 101, 67,
        241, 154, 186, 44, 35, 78, 71, 134, 194, 1, 62, 219, 7, 195, 12, 202,
        244, 199, 26, 175, 140, 21, 79, 122, 209, 82, 247, 141, 37, 58, 211, 222,
        0, 4, 40, 8, 63, 64, 99, 252, 96, 153, 231, 65, 6, 161, 46, 165,
        200, 137, 76, 113, 155, 110, 239, 180, 226, 52, 167, 206, 221, 117, 80, 146,
        84, 196, 115, 32, 87, 38, 144, 127, 237, 57, 92, 59, 217, 208, 210, 102,
        136, 125, 93, 13, 22, 3, 178, 54, 2, 243, 162, 170, 254, 28, 173, 95,
        182, 164, 74, 179, 228, 248, 225, 198, 24, 255, 42, 50, 189, 156, 227, 235
    };
    
    while (*str) {
        hash = lookupTable[hash ^ (*str)];
        str++;
    }
    return hash;
}

bool compareStoredHash(unsigned char storedHash, const char* inputString) {
    unsigned char newHash = pearsonHash(inputString);
    return storedHash == newHash;
}

template class HashTable<int>;
template class HashTable<char>;