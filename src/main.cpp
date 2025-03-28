#include "btb.h"
#include "profiler.h"
#include <iostream>

int main(int argc, char* argv[]) 
{
    const size_t BTB_SIZE = (argc > 1) ? std::stoul(argv[1]) : 32;      // Default cache size to 32 given no input
    BranchTargetBuffer btb(BTB_SIZE);
    std::vector<BranchEncounter> branches = parseProfile("data/block_profile");

    size_t hits = 0;
    size_t total_branches = 0;
    size_t total_executed = 0;
    size_t mispredictions = 0;

    for (const auto& branch : branches) {
        total_executed++;                   // Count all instructions

        if (branch.type != 'B') {
            // Misprediction 1: Variant branches never cached
            mispredictions++;
            continue;   // Not caching variant branches
        }
        total_branches++;
        uint32_t predicted = btb.predict(branch.source);        // Check BTB for entry

        // Branch instruction address in BTB = Cache hit
        if (predicted != 0) {               
            hits++;             
            // Misprediction 2: Predicted taken but actually not taken
            if (!branch.taken) {
                mispredictions++;
            }
        } 
        else {
            // Misprediction 3: Predicted not taken but actually taken
            if (branch.taken) {
                mispredictions++;
            }
        }

        // Cache all invariant branches
        btb.update(branch.source, branch.target, branch.conditional);
    }

    // 2 cycle penalty for mispredicted branches
    size_t total_fetched = total_executed + (2 * mispredictions);
    double fetch_to_execute = (total_fetched / static_cast<double>(total_executed)) * 100.0;

    // CSV formatting for graphing overhead in several BTB sizes
    if (argc > 1) {
        std::cout << BTB_SIZE << ","
                  << fetch_to_execute << "\n";
    } else {
        // Statistics for simulating single BTB
        std::cout << "Branch Target Buffer Statistics:\n"
                << "Size: " << BTB_SIZE << " entries\n"
                << "Hits: " << hits << "\n"
                << "Misses: " << (total_branches - hits) << "\n"
                << "Hit Rate: " << (100.0 * hits / total_branches) << "%\n";
    }    
    return 0;
}