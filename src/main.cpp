#include "btb.h"
#include "profiler.h"
#include <iostream>

int main() 
{
    const size_t BTB_SIZE = 32;             // Cache size
    BranchTargetBuffer btb(BTB_SIZE);
    std::vector<BranchEncounter> branches = parseProfile("../data/block_profile");

    size_t hits = 0;
    size_t total_branches = 0;
    size_t forward = 0;
    size_t backward = 0;
    for (const auto& branch : branches) {
        if (branch.type != 'B') continue;   // Only consider caching invariant branches
        
        total_branches++;
        uint32_t predicted = btb.predict(branch.source);        // Check BTB for entry
        if (predicted != 0) hits++;                             // Branch instruction address in BTB = Cache hit
        
        if (branch.direction == 'B') backward++;
        if (branch.direction == 'F') forward++;

        // Only cache backward taken branches
        if (branch.taken && branch.direction == 'B') {
            btb.update(branch.source, branch.target, branch.conditional);
        }
    }
    
    std::cout << "Branch Target Buffer Statistics:\n"
              << "Size: " << BTB_SIZE << " entries\n"
              << "Hits: " << hits << "\n"
              << "Misses: " << (total_branches - hits) << "\n"
              << "Forward: " << (forward) << "\n"
              << "Backward: " << (backward) << "\n"
              << "Hit Rate: " << (100.0 * hits / total_branches) << "%\n";
    
    return 0;
}