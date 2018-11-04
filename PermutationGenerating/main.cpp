#include <iostream>
#include "PermutationGenerator.h"
#include "ChangeCarryNumber.h"
using namespace std;



int main() {

    PermutationGenerator pg(4);
    pg.generate_permutations(PermutationGenerator::RECURSION, "aaaa");
    
    //pg.generate_permutations(PermutationGenerator::INCREMENTAL_CARRYING, "bbbb");
    //pg.generate_permutations(PermutationGenerator::DECREMENTAL_CARRYING, "bbbb");
    //pg.generate_permutations(PermutationGenerator::MEDIATOR_LEXICOHRAPHIC, "bbbb");
    pg.generate_permutations(PermutationGenerator::SWAPPING, "bbbb");
    
    int N = 8;
    ChangeCarryNumber x = ChangeCarryNumber(N, DEC);
    char p[8] = {'h', 'c', 'f', 'g', 'd', 'e', 'b', 'a'};
    x.fromPermutation(p, 'n');
    x.print();
    cout << x.toPermutation('n') << endl;
    
    return 0;
}
