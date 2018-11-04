//
// Created by 李晓涵 on 2018/11/3.
//
#include "PermutationGenerator.h"
#include "ChangeCarryNumber.h"
#include <iostream>
using namespace std;

void PermutationGenerator::generate_permutations(int method, string output_file){
  switch (method){
    case PermutationGenerator::RECURSION:
      recursion(output_file);
      break;
    case PermutationGenerator::CLASSIC_LEXICOGRAPHIC:
      classic_lexi(output_file);
      break;
    case PermutationGenerator::SJT:
      SJT_method(output_file);
      break;
    case PermutationGenerator::MEDIATOR_LEXICOHRAPHIC:
      mediator_lexi(output_file);
      break;
    case PermutationGenerator::INCREMENTAL_CARRYING:
      inc_carrying(output_file);
      break;
    case PermutationGenerator::DECREMENTAL_CARRYING:
      dec_carrying(output_file);
      break;
    case PermutationGenerator::SWAPPING:
      swapping(output_file);
      break;
    default:
      printf("Invalid method.\n");
  }
  return;
}

void PermutationGenerator::recursion(string output_file){

}

void PermutationGenerator::classic_lexi(string output_file){

}

void PermutationGenerator::SJT_method(string output_file){

}

void PermutationGenerator::mediator_lexi(string output_file){

}

void PermutationGenerator::inc_carrying(string output_file){
    char* c = new char[n];
    for(int i = 0; i < n; i++) {
        c[i] = C[i+1];
    }
    c[n] = '\0';
    ChangeCarryNumber x = ChangeCarryNumber(n, INC);
    x.fromPermutation(c, 'i');
    cout << c << endl;
    for(int i = 1; i < fac[n]; i++) {
        ++x;
        char* next = x.toPermutation('i');
        cout << next << endl;
    }
}

void PermutationGenerator::dec_carrying(string output_file){
    char* c = new char[n];
    for(int i = 0; i < n; i++) {
        c[i] = C[i+1];
    }
    c[n] = '\0';
    ChangeCarryNumber x = ChangeCarryNumber(n, DEC);
    x.fromPermutation(c, 'd');
    x.print();
    cout << c << endl;
    for(int i = 1; i < 10; i++) {
        ++x;
        char* next = x.toPermutation('i');
        x.print();
        cout << next << endl;
    }
}

void PermutationGenerator::swapping(string output_file){

}
