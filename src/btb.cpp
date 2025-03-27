#include "btb.h"

/* Cache Entry Constructor */
BranchTargetBuffer::Node::Node(uint32_t k, uint32_t v, bool c)
    : pc(k), target(v), conditional(c), prev(nullptr), next(nullptr) {}

/* Cache Constructor */
BranchTargetBuffer::BranchTargetBuffer(size_t size) : capacity(size) {
    head = new Node(0, 0, false);
    tail = new Node(0, 0, false);
    head->next = tail;
    tail->prev = head;
}

/* Cache Desctructor */
BranchTargetBuffer::~BranchTargetBuffer() {
    Node* curr = head->next;
    while (curr != tail) {
        Node* tmp = curr;
        curr = curr->next;
        delete tmp;
    }
    delete head;
    delete tail;
}

/* Inserts node between head and head->next (at MRU position) */
void BranchTargetBuffer::add(Node* node) {
    node->next = head->next;
    node->prev = head;
    head->next->prev = node;
    head->next = node;
}

/* Removes node and reestablishes neighbour links */
void BranchTargetBuffer::remove(Node* node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

/* Returns branch target address if branch instruction in BTB */
uint32_t BranchTargetBuffer::predict(uint32_t pc) {
    auto it = cache.find(pc);
    if (it != cache.end()) {
        Node* node = it->second;
        remove(node);                   
        add(node);                      
        return node->target;
    }
    return 0;
}

/* Adds new entry/Moves existing entry in LRU cache */
void BranchTargetBuffer::update(uint32_t pc, uint32_t target, bool cond) {
    auto it = cache.find(pc);
    if (it != cache.end()) {
        Node* node = it->second;
        node->target = target;
        remove(node);
        add(node);
        return;
    }
    
    if (cache.size() >= capacity) {
        Node* lru = tail->prev;
        cache.erase(lru->pc);
        remove(lru);
        delete lru;
    }
    
    Node* newNode = new Node(pc, target, cond);
    add(newNode);
    cache[pc] = newNode;
}