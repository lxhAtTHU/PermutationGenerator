#include <iostream>
#include "PermutationGenerator.h"
using namespace std;



int main() {

  PermutationGenerator pg(3);
  pg.generate_permutations(PermutationGenerator::RECURSION, "aaaa");

  return 0;
}