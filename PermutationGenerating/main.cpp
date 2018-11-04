#include <iostream>
#include "PermutationGenerator.h"
#include "ChangeCarryNumber.h"
using namespace std;



int main() {

    PermutationGenerator pg(4);
    pg.generate_permutations(PermutationGenerator::RECURSION, "aaaa");
    
    //pg.generate_permutations(PermutationGenerator::INCREMENTAL_CARRYING, "bbbb");
    
    pg.generate_permutations(PermutationGenerator::DECREMENTAL_CARRYING, "bbbb");
//    int N = 8;
//    ChangeCarryNumber x = ChangeCarryNumber(N, DEC);
//    char c[8] = {'8', '3', '6', '7', '4', '5', '2', '1'};
//    x.fromPermutation(c, 'd');
//    x.print();
//    char* ans = x.toPermutation('d');
//    cout << ans << endl;
    //    ++x;
    //    x.print();
//    ChangeCarryNumber y = ChangeCarryNumber(N, DEC, 4002);
//    y.print();
//    ChangeCarryNumber z = ChangeCarryNumber(N, DEC);
//    z = x - y;
//    z.print();
    ChangeCarryNumber x = ChangeCarryNumber(8, DEC, 1012120);
    ChangeCarryNumber y = ChangeCarryNumber(8, DEC, 11002);
    (x-y).print();
    
    
    return 0;
}
