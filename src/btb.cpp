#include "btb.h"

/* Cache Entry Constructor */
BranchTargetBuffer::Node::Node(uint32_t k, uint32_t v, bool c)
    : source(k), target(v), conditional(c), state(WEAKLY_TAKEN), prev(nullptr), next(nullptr) {}

/* Cache Constructor */
BranchTargetBuffer::BranchTargetBuffer(size_t size) : capacity(size) {
    head = std::make_shared<Node>(0, 0, false);
    tail = std::make_shared<Node>(0, 0, false);
    head->next = tail;
    tail->prev = head;
}

/* Cache Destructor */
BranchTargetBuffer::~BranchTargetBuffer() {
    auto curr = head->next;
    while (curr != tail) {
        auto tmp = curr;
        curr = curr->next;
        tmp->prev.reset();
        tmp->next.reset();
    }
    head.reset();
    tail.reset();
}

/* Inserts node between head and head->next (at MRU position) */
void BranchTargetBuffer::add(std::shared_ptr<Node> node) {
    node->next = head->next;
    node->prev = head;
    head->next->prev = node;
    head->next = node;
}

/* Removes node and reestablishes neighbour links */
void BranchTargetBuffer::remove(std::shared_ptr<Node> node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

/* Returns branch target address if branch instruction in BTB */
uint32_t BranchTargetBuffer::predict(uint32_t pc) {
    auto it = cache.find(pc);
    if (it != cache.end()) {
        auto node = it->second;
        remove(node);                   
        add(node);                      
        return (node->state >= WEAKLY_TAKEN) ? node->target : 0;       // Predict taken if in WEAKLY_TAKEN or STRONGLY_TAKEN
    }
    return 0;
}

/* Adds new entry/Moves existing entry in LRU cache */
void BranchTargetBuffer::update(uint32_t source, uint32_t target, bool cond, bool taken) {
    auto it = cache.find(source);
    std::shared_ptr<Node> node;

    if (it != cache.end()) {
        node = it->second;
        remove(node);               // Remove existing entry to update position on add(node)
    } else {
        if (cache.size() >= capacity) {
            auto lru = tail->prev;
            cache.erase(lru->source);
            remove(lru);            // Remove LRU if capacity exceeded to make space for add(node)
        }
        node = std::make_shared<Node>(source, target, cond);
        cache[source] = node;
    }
    
    // Update 2-bit counter
    if (taken) {
        node->state = (node->state == STRONGLY_TAKEN) ? STRONGLY_TAKEN : static_cast<State>(node->state + 1);
    } else {
        node->state = (node->state == STRONGLY_NOT_TAKEN) ? STRONGLY_NOT_TAKEN : static_cast<State>(node->state - 1);
    }
    
    add(node);  
}