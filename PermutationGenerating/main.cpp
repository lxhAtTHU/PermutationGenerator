#include <iostream>
#include "PermutationGenerator.h"
#include "ChangeCarryNumber.h"
using namespace std;



int main() {

    PermutationGenerator pg(3);
    pg.generate_permutations(PermutationGenerator::RECURSION, "aaaa");
    
    int N = 8;
    ChangeCarryNumber x = ChangeCarryNumber(N, DEC);
    char c[8] = {'8', '3', '6', '7', '4', '5', '2', '1'};
    x.fromPermutation(c, 'd');
    x.print();
    //    ++x;
    //    x.print();
    ChangeCarryNumber y = ChangeCarryNumber(N, DEC, 4002);
    y.print();
    ChangeCarryNumber z = ChangeCarryNumber(N, DEC);
    z = x - y;
    z.print();
    return 0;
    
    return 0;
}
