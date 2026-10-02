#include "balanced_set.h"
#include <iostream>
#include <utility>

Node::Node(int value) : key(value), left(nullptr), right(nullptr), height(1) {}

int BalancedSet::get_height(const Node* node) {
    return node ? node->height : 0;
}

void BalancedSet::update_height(Node* node) {
    if (node) {
        node->height = 1 + std::max(get_height(node->left), get_height(node->right));
    }
}

int BalancedSet::get_balance_factor(const Node* node) {
    return node ? get_height(node->left) - get_height(node->right) : 0;
}

Node* BalancedSet::rotate_right(Node* y) {
    Node* x = y->left;
    Node* t2 = x->right;

    x->right = y;
    y->left = t2;

    update_height(y);
    update_height(x);

    return x;
}

Node* BalancedSet::rotate_left(Node* x) {
    Node* y = x->right;
    Node* t2 = y->left;

    y->left = x;
    x->right = t2;

    update_height(x);
    update_height(y);

    return y;
}

Node* BalancedSet::balance_node(Node* node) {
    if (!node) return nullptr;

    update_height(node);
    int balance = get_balance_factor(node);

    if (balance > 1 && get_balance_factor(node->left) >= 0) {
        return rotate_right(node);
    }
    if (balance > 1 && get_balance_factor(node->left) < 0) {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }
    if (balance < -1 && get_balance_factor(node->right) <= 0) {
        return rotate_left(node);
    }
    if (balance < -1 && get_balance_factor(node->right) > 0) {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }
    return node;
}

Node* BalancedSet::insert_node(Node* node, int key, bool& inserted) {
    if (!node) {
        inserted = true;
        return new Node(key);
    }

    if (key < node->key) {
        node->left = insert_node(node->left, key, inserted);
    } else if (key > node->key) {
        node->right = insert_node(node->right, key, inserted);
    } else {
        inserted = false;
        return node;
    }

    return balance_node(node);
}

bool BalancedSet::contains_node(const Node* node, int key) {
    if (!node) return false;
    if (key < node->key) return contains_node(node->left, key);
    if (key > node->key) return contains_node(node->right, key);
    return true;
}

Node* BalancedSet::find_min_node(Node* node) {
    if (!node || !node->left) return node;
    return find_min_node(node->left);
}

Node* BalancedSet::erase_node(Node* node, int key, bool& erased) {
    if (!node) {
        erased = false;
        return nullptr;
    }

    if (key < node->key) {
        node->left = erase_node(node->left, key, erased);
    } else if (key > node->key) {
        node->right = erase_node(node->right, key, erased);
    } else {
        erased = true;
        if (!node->left || !node->right) {
            Node* temp = node->left ? node->left : node->right;
            delete node;
            return temp;
        } else {
            Node* successor = find_min_node(node->right);
            node->key = successor->key;
            node->right = erase_node(node->right, successor->key, erased);
        }
    }

    return balance_node(node);
}

bool BalancedSet::is_strictly_balanced_helper(const Node* node) {
    if (!node) return true;
    int left_height = get_height(node->left);
    int right_height = get_height(node->right);
    if (std::abs(left_height - right_height) > 1) return false;
    return is_strictly_balanced_helper(node->left) && is_strictly_balanced_helper(node->right);
}

Node* BalancedSet::copy_tree(Node* other_node) {
    if (!other_node) return nullptr;
    Node* new_node = new Node(other_node->key);
    new_node->left = copy_tree(other_node->left);
    new_node->right = copy_tree(other_node->right);
    new_node->height = other_node->height;
    return new_node;
}

void BalancedSet::clear_tree(Node* node) {
    if (node) {
        clear_tree(node->left);
        clear_tree(node->right);
        delete node;
    }
}

void BalancedSet::print_inorder(const Node* node) {
    if (node) {
        print_inorder(node->left);
        std::cout << node->key << " ";
        print_inorder(node->right);
    }
}

BalancedSet::BalancedSet() : root_(nullptr), size_(0) {}

BalancedSet::BalancedSet(const BalancedSet& other) : root_(copy_tree(other.root_)), size_(other.size_) {}

BalancedSet::~BalancedSet() {
    clear_tree(root_);
}

BalancedSet& BalancedSet::operator=(const BalancedSet& other) {
    if (this != &other) {
        BalancedSet temp(other);
        std::swap(root_, temp.root_);
        std::swap(size_, temp.size_);
    }
    return *this;
}

bool BalancedSet::insert(int key) {
    bool inserted = false;
    root_ = insert_node(root_, key, inserted);
    if (inserted) {
        ++size_;
    }
    return inserted;
}

bool BalancedSet::contains(int key) const {
    return contains_node(root_, key);
}

bool BalancedSet::erase(int key) {
    bool erased = false;
    root_ = erase_node(root_, key, erased);
    if (erased) {
        --size_;
    }
    return erased;
}

void BalancedSet::print() const {
    print_inorder(root_);
    std::cout << std::endl;
}

bool BalancedSet::strictly_balanced() const {
    return is_strictly_balanced_helper(root_);
}

size_t BalancedSet::size() const {
    return size_;
}

std::vector<int> get_unique_elements(const std::vector<int>& input) {
    BalancedSet seen_once;
    BalancedSet seen_multiple;
    std::vector<int> result;

    for (int value : input) {
        if (seen_once.contains(value)) {
            seen_once.erase(value);
            seen_multiple.insert(value);
        } else if (!seen_multiple.contains(value)) {
            seen_once.insert(value);
        }
    }

    for (int value : input) {
        if (seen_once.contains(value)) {
            result.push_back(value);
            seen_once.erase(value);
        }
    }

    return result;
}