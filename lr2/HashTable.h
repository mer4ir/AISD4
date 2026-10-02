#ifndef HASHTABLE_H
#define HASHTABLE_H

template<typename T>
class HashTable {
private:
    struct Node {
        int key;
        T value;
        Node* next;
        Node(int k, const T& v);
    };
    
    Node** table;
    int capacity;
    
    int hashFunction(int key) const;
    
public:
    HashTable(int size);
    HashTable(int size, T minVal, T maxVal);
    HashTable(const HashTable& other);
    ~HashTable();
    
    HashTable& operator=(const HashTable& other);
    void print();
    bool insert(int key, const T& value);
    void insert_or_assign(int key, T& value);
    bool contains(T& value);
    T* search(int key);
    bool erase(int key);
    int count(int key);
};

unsigned char pearsonHash(const char* str);
bool compareStoredHash(unsigned char storedHash, const char* inputString);

#endif