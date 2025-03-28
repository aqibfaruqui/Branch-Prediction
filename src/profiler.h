#pragma once
#include <cstdint>
#include <vector>
#include <string>

struct BranchEncounter {
    char type;              // Types: {B, R, D, M, L}
    uint32_t source;        // Branch instruction address
    uint32_t target;        // Branch target address - only deterministic for invariant branches, 0 otherwise
    char direction;         // (F)orward/(B)ackward for invariant branches, (N) for variant branches
    bool conditional;
    bool taken;
};

// Extracts branch encounter metadata from profile line-by-line
std::vector<BranchEncounter> parseProfile(const std::string& fp);