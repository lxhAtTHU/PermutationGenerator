//
// Created by 李晓涵 on 2018/11/3.
//
#include <string>
using namespace std;

#ifndef PERMUTATIONGENERATING_PERMUTATIONGENERATOR_H
#define PERMUTATIONGENERATING_PERMUTATIONGENERATOR_H

class PermutationGenerator{

private:

    int n;

    void recursion(int rank, int nprocs, string output_file);
    void classic_lexi(int rank, int nprocs, string output_file);
    void SJT_method(int rank, int nprocs, string output_file);
    void mediator_lexi(int rank, int nprocs, string output_file);
    void inc_carrying(int rank, int nprocs, string output_file);
    void dec_carrying(int rank, int nprocs, string output_file);
    void swapping(int rank, int nprocs, string output_file);
    void Heap_method(int rank, int nprocs, string output_file);
    void integrated_method(int rank, int nprocs, string output_file);


public:

    enum method_type {RECURSION=1, CLASSIC_LEXICOGRAPHIC, SJT,
                          MEDIATOR_LEXICOHRAPHIC, INCREMENTAL_CARRYING, DECREMENTAL_CARRYING, SWAPPING, HEAP, INTEGRATED};

    PermutationGenerator(int _n){
      n = _n;
    }

    void generate_permutations(int rank, int nprocs, int method, string output_file);


};

#endif //PERMUTATIONGENERATING_PERMUTATIONGENERATOR_H
