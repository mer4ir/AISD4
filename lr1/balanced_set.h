#ifndef BALANCED_SET_H
#define BALANCED_SET_H

#include <algorithm>
#include <vector>
#include <cstddef>

struct Node {
    int key;
    Node* left;
    Node* right;
    int height;

    explicit Node(int value);
};

class BalancedSet {
private:
    Node* root_;
    size_t size_;

    static int get_height(const Node* node);
    static void update_height(Node* node);
    static int get_balance_factor(const Node* node);
    static Node* rotate_right(Node* y);
    static Node* rotate_left(Node* x);
    static Node* balance_node(Node* node);
    Node* insert_node(Node* node, int key, bool& inserted);
    static bool contains_node(const Node* node, int key);
    static Node* find_min_node(Node* node);
    Node* erase_node(Node* node, int key, bool& erased);
    static bool is_strictly_balanced_helper(const Node* node);
    static Node* copy_tree(Node* other_node);
    static void clear_tree(Node* node);
    static void print_inorder(const Node* node);

public:
    BalancedSet();
    BalancedSet(const BalancedSet& other);
    ~BalancedSet();
    BalancedSet& operator=(const BalancedSet& other);

    bool insert(int key);
    bool contains(int key) const;
    bool erase(int key);
    void print() const;
    bool strictly_balanced() const;
    size_t size() const;
};

std::vector<int> get_unique_elements(const std::vector<int>& input);

#endif