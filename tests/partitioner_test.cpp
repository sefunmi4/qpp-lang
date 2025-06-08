#include "../runtime/partitioner.h"
#include <cassert>
#include <cmath>
#include <iostream>

int main(){
    using namespace qpp;
    std::vector<std::vector<std::string>> ops = {
        {"QALLOC","a","1"},
        {"QALLOC","b","1"},
        {"H","a","0"},
        {"X","b","0"}
    };
    auto parts = analyze_separable_regions(ops);
    assert(parts.size()==2);
    auto wf = execute_partitions(ops);
    double f = 1.0/std::sqrt(2.0);
    assert(wf.state.size()==4);
    assert(std::abs(wf.state[1]-std::complex<double>(f,0.0))<1e-9);
    assert(std::abs(wf.state[3]-std::complex<double>(f,0.0))<1e-9);
    std::cout << "Partitioner test passed." << std::endl;
    return 0;
}
