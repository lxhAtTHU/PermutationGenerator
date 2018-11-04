#include <iostream>
#include "PermutationGenerator.h"
#include "ChangeCarryNumber.h"
using namespace std;



int main() {

    PermutationGenerator pg(4);
    pg.generate_permutations(PermutationGenerator::RECURSION, "aaaa");
    
    //pg.generate_permutations(PermutationGenerator::INCREMENTAL_CARRYING, "bbbb");
    //pg.generate_permutations(PermutationGenerator::DECREMENTAL_CARRYING, "bbbb");
    pg.generate_permutations(PermutationGenerator::MEDIATOR_LEXICOHRAPHIC, "bbbb");
    
//    int N = 8;
//    ChangeCarryNumber x = ChangeCarryNumber(N, INC, 7244221);
//    cout << x.toPermutation('l') << endl;
    
    return 0;
}
