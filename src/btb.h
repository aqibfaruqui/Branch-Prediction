#pragma once
#include <cstdint>
#include <unordered_map>

class BranchTargetBuffer 
{
    public:
        // Constructor
        BranchTargetBuffer(size_t size);       
        
        // Destructor
        ~BranchTargetBuffer();

        // Branch prediction & update BTB entry methods
        uint32_t predict(uint32_t pc);
        void update(uint32_t source, uint32_t target, bool cond, bool taken);

        // States for 2-bit counter
        enum State {
            STRONGLY_NOT_TAKEN = 0,
            WEAKLY_NOT_TAKEN = 1,
            WEAKLY_TAKEN = 2,
            STRONGLY_TAKEN = 3
        };


    private:
        struct Node {
            uint32_t source;        // Branch instruction address
            uint32_t target;        // Branch target
            bool conditional;       // Conditional/Unconditional branch
            State state;            // 2-bit prediction state
            Node* prev;             // Prev entry in LRU doubly linked list
            Node* next;             // Next entry in LRU doubly linked list

            // Constructor
            Node(uint32_t key, uint32_t val, bool cond);
        };

        Node* head;                 // Dummy node before MRU node
        Node* tail;                 // Dummy node after LRU node
        size_t capacity;
        std::unordered_map<uint32_t, Node*> cache;

        // Cache update methods
        void add(Node* node);
        void remove(Node* node);
};