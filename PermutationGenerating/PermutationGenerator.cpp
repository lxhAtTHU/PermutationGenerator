//
// Created by 李晓涵 on 2018/11/3.
//
#include "PermutationGenerator.h"
#include "ChangeCarryNumber.h"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
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

string getTime()
{
    time_t timep;
    time (&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&timep) );
    return tmp;
}

void log(string s) {
    cout << "[" << getTime() << "] " << s << endl;
}

void mediator_generating(string output_file, int inc_or_dec, char algo, int n) {
    char* c = new char[n]; //构造初始排列
    for(int i = 0; i < n; i++) {
        c[i] = C[i+1];
    }
    c[n] = '\n';
    ChangeCarryNumber x = ChangeCarryNumber(n, inc_or_dec);
    x.fromPermutation(c, algo);
    int fd = open(output_file.c_str(), O_RDWR | O_CREAT, 0777);

    time_t start, stop;
    log("Outputing to file " + output_file);
    start = time(NULL);
    
    //生成全排列
    write(fd, c, n+1);
    for(long long i = 1; i < fac[n]; i++) {
        ++x;
        char* next = x.toPermutation(algo);
        next[n] = '\n';
        write(fd, next, n+1);
    }
    
    log("Finished!");
    stop = time(NULL);
    cout << "Use Time: " << (stop-start) << "ms" << endl;
    close(fd);
}

void PermutationGenerator::mediator_lexi(string output_file){
    mediator_generating(output_file, INC, 'l', n);
}

void PermutationGenerator::inc_carrying(string output_file){
    mediator_generating(output_file, INC, 'i', n);
}

void PermutationGenerator::dec_carrying(string output_file){
    mediator_generating(output_file, DEC, 'd', n);
}

void PermutationGenerator::swapping(string output_file){
    mediator_generating(output_file, DEC, 'n', n);
}
