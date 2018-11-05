#include <iostream>
#include "PermutationGenerator.h"
#include "ChangeCarryNumber.h"
using namespace std;



int main() {

  PermutationGenerator pg(5);
  pg.generate_permutations(PermutationGenerator::CLASSIC_LEXICOGRAPHIC, "../test.txt");
    
    //pg.generate_permutations(PermutationGenerator::INCREMENTAL_CARRYING, "incremental.txt");
    //pg.generate_permutations(PermutationGenerator::DECREMENTAL_CARRYING, "decremental.txt");
    //pg.generate_permutations(PermutationGenerator::MEDIATOR_LEXICOHRAPHIC, "mediator_lexi.txt");
    pg.generate_permutations(PermutationGenerator::SWAPPING, "swapping.txt");
    
    return 0;
}
