#include "partitioner.h"
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cmath>

namespace qpp {
namespace {
struct DSU {
    std::vector<int> parent;
    int add() { int id = parent.size(); parent.push_back(id); return id; }
    int find(int x) { return parent[x] == x ? x : parent[x] = find(parent[x]); }
    void unite(int a, int b) { a = find(a); b = find(b); if (a != b) parent[b] = a; }
};

std::vector<std::complex<double>> tensor_product(const std::vector<std::complex<double>>& a,
                                                 const std::vector<std::complex<double>>& b) {
    std::vector<std::complex<double>> res(a.size()*b.size());
    for (std::size_t i=0;i<a.size();++i)
        for (std::size_t j=0;j<b.size();++j)
            res[i*b.size()+j] = a[i]*b[j];
    return res;
}
} // namespace

std::vector<Partition> analyze_separable_regions(const std::vector<std::vector<std::string>>& ops) {
    std::unordered_map<QubitRef,int,QubitRefHash> idx;
    DSU dsu;
    std::vector<QubitRef> qubits;
    auto get_id = [&](const QubitRef& q){
        auto it = idx.find(q);
        if(it!=idx.end()) return it->second;
        int id = dsu.add();
        idx[q]=id;
        qubits.push_back(q);
        return id;
    };

    for(const auto& op: ops){
        if(op.empty()) continue;
        if(op[0]=="QALLOC" && op.size()==3){
            std::string name = op[1];
            std::size_t n = std::stoul(op[2]);
            for(std::size_t i=0;i<n;++i){
                get_id({name,i});
            }
        } else if((op[0]=="CNOT"||op[0]=="CZ"||op[0]=="SWAP") && op.size()==5){
            int a=get_id({op[1],std::stoul(op[2])});
            int b=get_id({op[3],std::stoul(op[4])});
            dsu.unite(a,b);
        } else if(op[0]=="CCX" && op.size()==7){
            int a=get_id({op[1],std::stoul(op[2])});
            int b=get_id({op[3],std::stoul(op[4])});
            int c=get_id({op[5],std::stoul(op[6])});
            dsu.unite(a,b);
            dsu.unite(a,c);
        }
    }

    std::unordered_map<int,Partition> parts;
    std::unordered_map<int,std::size_t> first_idx;
    for(std::size_t i=0;i<qubits.size();++i){
        int root = dsu.find(i);
        parts[root].push_back(qubits[i]);
        if(!first_idx.count(root)) first_idx[root] = i;
    }
    std::vector<std::pair<std::size_t,Partition>> ordered;
    for(auto& kv : parts) ordered.push_back({first_idx[kv.first], std::move(kv.second)});
    std::sort(ordered.begin(), ordered.end(), [](const auto& a, const auto& b){return a.first < b.first;});
    std::vector<Partition> out;
    for(auto& kv : ordered) out.push_back(std::move(kv.second));
    return out;
}

Wavefunction execute_partitions(const std::vector<std::vector<std::string>>& ops) {
    auto parts = analyze_separable_regions(ops);
    std::unordered_map<QubitRef,std::pair<int,std::size_t>,QubitRefHash> loc;
    std::vector<Wavefunction> wfs;
    for(std::size_t p=0;p<parts.size();++p){
        wfs.emplace_back(parts[p].size());
        for(std::size_t i=0;i<parts[p].size();++i){
            loc[parts[p][i]]={static_cast<int>(p),i};
        }
    }
    for(const auto& op: ops){
        if(op.empty()) continue;
        if(op[0]=="H"||op[0]=="X"||op[0]=="Y"||op[0]=="Z"||op[0]=="S"||op[0]=="T"){
            auto q = loc.at({op[1],std::stoul(op[2])});
            auto& wf = wfs[q.first];
            std::size_t idx=q.second;
            if(op[0]=="H") wf.apply_h(idx);
            else if(op[0]=="X") wf.apply_x(idx);
            else if(op[0]=="Y") wf.apply_y(idx);
            else if(op[0]=="Z") wf.apply_z(idx);
            else if(op[0]=="S") wf.apply_s(idx);
            else if(op[0]=="T") wf.apply_t(idx);
        } else if((op[0]=="CNOT"||op[0]=="CZ"||op[0]=="SWAP") && op.size()==5){
            auto qa = loc.at({op[1],std::stoul(op[2])});
            auto qb = loc.at({op[3],std::stoul(op[4])});
            if(qa.first!=qb.first) continue; // cross partition not supported
            auto& wf = wfs[qa.first];
            std::size_t a=qa.second, b=qb.second;
            if(op[0]=="CNOT") wf.apply_cnot(a,b);
            else if(op[0]=="CZ") wf.apply_cz(a,b);
            else if(op[0]=="SWAP") wf.apply_swap(a,b);
        } else if(op[0]=="CCX" && op.size()==7){
            auto qa=loc.at({op[1],std::stoul(op[2])});
            auto qb=loc.at({op[3],std::stoul(op[4])});
            auto qc=loc.at({op[5],std::stoul(op[6])});
            if(qa.first!=qb.first||qa.first!=qc.first) continue;
            auto& wf=wfs[qa.first];
            wf.apply_ccnot(qa.second,qb.second,qc.second);
        }
    }

    Wavefunction result(0);
    result.state = {1.0};
    for(const auto& wf : wfs){
        result.state = tensor_product(result.state,wf.state);
    }
    result.num_qubits = std::log2(result.state.size());
    return result;
}

} // namespace qpp
