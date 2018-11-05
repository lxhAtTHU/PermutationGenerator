#include <iostream>
#include "PermutationGenerator.h"
using namespace std;



int main() {

  PermutationGenerator pg(5);
  pg.generate_permutations(PermutationGenerator::CLASSIC_LEXICOGRAPHIC, "../test.txt");

  return 0;
}