#include <iostream>
#include <fstream>

int main() {
    std::ifstream in("../output/sorted_counts.txt");
    unsigned long total = 3423933;
    unsigned long threshold = total * 0.9;
    unsigned long count_sum = 0;
    int count;
    int instructions_needed = 0;

    while (in >> count) {
        count_sum += count;
        instructions_needed++;
        if (count_sum >= threshold) break;
    }

    std::cout << "Instructions needed to reach 90% threshold: " << instructions_needed << std::endl;
    std::cout << "Proportion of program in which 90% of its time is spent: " << ((double)instructions_needed / 11339) * 100.0 << "%" << std::endl;
    return 0;
}