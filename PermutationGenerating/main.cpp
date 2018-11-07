#include <iostream>
#include "PermutationGenerator.h"
#include "ChangeCarryNumber.h"
using namespace std;



int main() {

    for (int i = 5; i < 12; i++) {
        cout << "================ N = " << i << " =================" << endl;
        PermutationGenerator pg(i);
        pg.generate_permutations(PermutationGenerator::HEAP, "../heap.txt");
        //return 0;
//        pg.generate_permutations(PermutationGenerator::CLASSIC_LEXICOGRAPHIC, "../classic_lexi.txt");
//        pg.generate_permutations(PermutationGenerator::RECURSION, "../recursion.txt");
//        pg.generate_permutations(PermutationGenerator::SJT, "../sjt.txt");
//
//        pg.generate_permutations(PermutationGenerator::MEDIATOR_LEXICOHRAPHIC, "../mediator_lexi.txt");
//        pg.generate_permutations(PermutationGenerator::INCREMENTAL_CARRYING, "../incremental.txt");
//        pg.generate_permutations(PermutationGenerator::DECREMENTAL_CARRYING, "../decremental.txt");
//        pg.generate_permutations(PermutationGenerator::SWAPPING, "../swapping.txt");
    }
    return 0;
}
