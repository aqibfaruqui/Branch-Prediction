#include "profiler.h"
#include <fstream>
#include <sstream>
#include <iostream>

/* Process branch encounter's type, source/target addresses, and conditional/taken statuses */
// Example branch encounters:
//      Invariant: B from 00008008 to 00008040 F * taken
//        Variant: M from 00026488               ? not taken
BranchEncounter parseLine(const std::string& line) 
{
    BranchEncounter branch;
    std::istringstream input(line);
    input >> branch.type;           
    input.ignore(6);                        // Skip " from " in "B from 00008008 ...""

    if (branch.type == 'B') {
        input >> std::hex >> branch.source;     // Parse branch instruction PC
        input.ignore(4);                        // Skip " to " in "00008008 to 00008040 ..."
        input >> std::hex >> branch.target;     // Parse branch target address (invariant only)
        char dir;
        input >> branch.direction;              // Parse forward/backward branch direction
    
    // Variant branch target addresses are undetermined
    } else {
        input >> std::hex >> branch.source;
        branch.target = 0;
        branch.direction = 'N';
    }

    // ? = Conditional, * = Unconditional
    char cond;
    input >> cond;
    branch.conditional = (cond == '?');
    
    // All branch encounters labelled 'taken' or 'not taken'
    std::string t;
    input >> t;
    branch.taken = (t == "taken");
    
    return branch;
}

/* Process profile of branch encounters into vector of each branch's metadata */
std::vector<BranchEncounter> parseProfile(const std::string& fp) {
    std::vector<BranchEncounter> branches;
    std::ifstream file(fp);
    std::string line;
    
    while (std::getline(file, line)) {
        branches.push_back(parseLine(line));
    }
    
    return branches;
}