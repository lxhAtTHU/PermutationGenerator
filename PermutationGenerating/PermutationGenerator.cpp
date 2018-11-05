//
// Created by 李晓涵 on 2018/11/3.
//
#include "PermutationGenerator.h"
#include "ChangeCarryNumber.h"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include <time.h>
using namespace std;

#define LEFT false
#define RIGHT true

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

void recursion_step(char *x, bool *flag, int n, int depth, int fd){
  if(depth == n){
    for(int i=0; i<n; i++){
      if(flag[i] == true)
        continue;
      x[depth-1] = 'a' + i;
      write(fd, x, n+1);
    }
    return;
  }

  for(int i=0; i<n; i++){
    if(flag[i] == true)
      continue;
    x[depth-1] = 'a' + i;
    flag[i] = true;
    recursion_step(x, flag, n, depth+1, fd);
    flag[i] = false;
  }
}

void PermutationGenerator::recursion(string output_file){
  char *x = new char[n+1]; // one more for '\n'
  x[n] = '\n';
  bool *flag = new bool[n];
  memset(flag, 0, n* sizeof(bool));
  int fd = open(output_file.c_str(), O_RDWR | O_CREAT, 0777);
  recursion_step(x, flag, n, 1, fd);
  delete []x;
  delete []flag;
  close(fd);
}

void PermutationGenerator::classic_lexi(string output_file){
  char *x = new char[n+1]; // one more for '\n'
  x[n] = '\n';
  int fd = open(output_file.c_str(), O_RDWR | O_CREAT, 0777);
  for(int i=0; i<n; i++)
    x[i] = 'a' + i;
  write(fd, x, n+1);

  while(true) {
    int dec_index = -1;
    int t = n;
    while (t-- > 1) {
      if(x[t-1] < x[t]) {
        dec_index = t - 1;
        break;
      }
    }
    if(dec_index == -1)
      break;

    // find another object to swap
    int sw_index = dec_index+1;
    for(int i=dec_index+1; i<n; i++){
      if(x[dec_index] < x[i]){
        sw_index = x[i] < x[sw_index] ? i : sw_index;
      }
    }
    swap(x[dec_index], x[sw_index]);
    // reverse
    reverse(x+dec_index+1, x+n);
    write(fd, x, n+1);
  }
  delete []x;
  close(fd);
}

int moveable(char *x, bool *dirc, int n, int i){
  if(i == 0 && dirc[i] == LEFT)
    return -1;
  if(i == n-1 && dirc[i] == RIGHT)
    return -1;

  if(dirc[i] == LEFT && x[i-1] < x[i])
    return i-1;
  if(dirc[i] == RIGHT && x[i] > x[i+1])
    return i+1;

  return -1;
}

void PermutationGenerator::SJT_method(string output_file){
  char *x = new char[n+1]; // one more for '\n'
  x[n] = '\n';
  bool *dirc = new bool[n];
  int fd = open(output_file.c_str(), O_RDWR | O_CREAT, 0777);
  for(int i=0; i<n; i++) {
    x[i] = 'a' + i;
    dirc[i] = LEFT;
  }
  write(fd, x, n+1);

  while(true) {
    int max_moveable_index = -1, sw_index = -1;
    for (int i=0; i<n; i++) {
      int t = moveable(x, dirc, n, i);
      if (t != -1 && x[i] > x[max_moveable_index]) {
        max_moveable_index = i;
        sw_index = t;
      }
    }
    if (max_moveable_index == -1)
      break;

    swap(x[max_moveable_index], x[sw_index]);
    swap(dirc[max_moveable_index], dirc[sw_index]);
    for(int i=0; i<n; i++){
      if(x[i] > x[sw_index])
        dirc[i] = !dirc[i];
    }
    write(fd, x, n+1);
  }

  delete []x;
  delete []dirc;
  close(fd);
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
