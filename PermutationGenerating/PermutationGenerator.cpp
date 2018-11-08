//
// Created by 李晓涵 on 2018/11/3.
//
#include "PermutationGenerator.h"
#include "ChangeCarryNumber.h"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <algorithm>
#include <cstring>
#include <time.h>
#include <ctime>
using namespace std;

#define LEFT false
#define RIGHT true

void log(string s);


void PermutationGenerator::generate_permutations(int rank, int nprocs, int method, string output_file){
    clock_t start, end;
    start = clock();
    
  switch (method){
    case PermutationGenerator::RECURSION:
      recursion(rank, nprocs, output_file);
      break;
    case PermutationGenerator::CLASSIC_LEXICOGRAPHIC:
      classic_lexi(rank, nprocs, output_file);
      break;
    case PermutationGenerator::SJT:
      SJT_method(rank, nprocs, output_file);
      break;
    case PermutationGenerator::MEDIATOR_LEXICOHRAPHIC:
      mediator_lexi(rank, nprocs, output_file);
      break;
    case PermutationGenerator::INCREMENTAL_CARRYING:
      inc_carrying(rank, nprocs, output_file);
      break;
    case PermutationGenerator::DECREMENTAL_CARRYING:
      dec_carrying(rank, nprocs, output_file);
      break;
    case PermutationGenerator::SWAPPING:
      swapping(rank, nprocs, output_file);
      break;
    case PermutationGenerator::HEAP:
      Heap_method(rank, nprocs, output_file);
      break;
    case PermutationGenerator::INTEGRATED:
      integrated_method(rank, nprocs, output_file);
      break;
    default:
      printf("Invalid method.\n");
  }
    
    end = clock();
    cout << output_file << ":\t\t"  << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;
    
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

void PermutationGenerator::recursion(int rank, int nprocs, string output_file){
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

void PermutationGenerator::classic_lexi(int rank, int nprocs, string output_file){
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

void PermutationGenerator::SJT_method(int rank, int nprocs, string output_file){
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

void mediator_generating(int rank, int nprocs, string output_file, int inc_or_dec, char algo, int n) {
    char* c = new char[n]; //构造初始排列
    for(int i = 0; i < n; i++) {
        c[i] = 'a'+i;
    }
    c[n] = '\n';

	long long total = fac[n];
	long long chunk = total/nprocs;
	long long start = rank*chunk;
	long long end = (rank == (nprocs-1))? total : (rank+1)*chunk;
	
	printf("%d of %d nprocs, %ld - %ld\n", rank, nprocs, start, end);
    ChangeCarryNumber x = ChangeCarryNumber(n, inc_or_dec, start);
    //x.fromPermutation(c, algo);
    delete []c;
    int fd = open(output_file.c_str(), O_RDWR | O_CREAT, 0777);
    
    //生成全排列
    //write(fd, c, n+1);
    for(long long i = start; i < end; i++) {
        char* next = x.toPermutation(algo);
        pwrite(fd, next, n+1, i*(n+1));
		++x;
    }
    close(fd);
}

void PermutationGenerator::mediator_lexi(int rank, int nprocs, string output_file){
    mediator_generating(rank, nprocs, output_file, INC, 'l', n);
}

void PermutationGenerator::inc_carrying(int rank, int nprocs, string output_file){
    mediator_generating(rank, nprocs, output_file, INC, 'i', n);
}

void PermutationGenerator::dec_carrying(int rank, int nprocs, string output_file){
    mediator_generating(rank, nprocs, output_file, DEC, 'd', n);
}

void PermutationGenerator::swapping(int rank, int nprocs, string output_file){
    mediator_generating(rank, nprocs, output_file, DEC, 'n', n);
}


void Heap_step(char *x, int n, int len_of_x, int fd) {
    int c = 0;
    while(true) {
        if(n > 2) Heap_step(x, n-1, len_of_x, fd);
        if(n <= c+1) break;
        else if ((n & 1) == 1) {
            swap(x[0], x[n-1]);
        } else {
            swap(x[c], x[n-1]);
        }
        write(fd, x, len_of_x+1);
        c++;
    }
}

void PermutationGenerator::Heap_method(int rank, int nprocs, string output_file){
    char *x = new char[n+1]; // one more for '\n'
    x[n] = '\n';
    for(int i = 0; i < n; ++i) {
        x[i] = C[i+1];
    }
    
    int fd = open(output_file.c_str(), O_RDWR | O_CREAT, 0777);
    write(fd, x, n+1); //original permutation
    
    //Heap_step(x, n, n, fd);
    
    int c[n];
    memset(c, 0, sizeof(c));
    int i = 0;
    while(i < n) {
        if(c[i] < i) {
            if((i & 1) == 0)
                swap(x[0], x[i]);
            else
                swap(x[c[i]], x[i]);
            write(fd, x, n+1);
            c[i]++;
            i = 0;
        } else {
            c[i] = 0;
            i += 1;
        }
    }
    
    delete []x;
    close(fd);
}

char starter[1814402][12];

void PermutationGenerator::integrated_method(int rank, int nprocs, string output_file) {
    long long f = fac[n-1]/2;
//    char starter[f][n+1];
    
    memset(starter, 0, sizeof(starter));
    for(long long i = 0; i < f; i++) {
        starter[i][n] = '\0';
    }
    for(int i = 0; i < n; i++) {
        starter[0][i] = C[i+1];
    }
    //cout << "starter[0]: " << starter[0] << endl;
    int cnt = 1;
    for(int k = n-3; k >= 1; k--) {
        int current_cnt = cnt;
        for(int j = 0; j < current_cnt; j++) {
            strcpy(starter[cnt], starter[j]);
            swap(starter[cnt][k], starter[cnt][k+1]);
            //cout << "starter[" << cnt << "]: " << starter[cnt] << endl;
            cnt++;
            for(int i = k+1; i < n-1; i++) {
                strcpy(starter[cnt], starter[cnt-1]);
                swap(starter[cnt][i], starter[cnt][i+1]);
                //cout << "starter[" << cnt << "]: " << starter[cnt] << endl;
                cnt++;
            }
        }
    }
    
    int fd = open(output_file.c_str(), O_RDWR | O_CREAT, 0777);
    //FILE *fp = freopen(output_file.c_str(), "w", stdout);
    char *x = new char[n+1];
    x[n] = '\n';
    for(int k = 0; k < cnt; k++) {
        for(int j = 0; j < n; j++) { //CP 初始位置
            //【CP操作】
            int i = j;
            int l = 0;
            do {
                //putchar(starter[k][i]);
                x[l++] = starter[k][i];
                i++;
                if(i >= n) i = 0;
            } while(i != j);
            //putchar('\n');
            write(fd, x, n+1);
            //【RoCP操作】
            i = j;
            l = 0;
            do {
                //putchar(starter[k][i]);
                x[l++] = starter[k][i];
                i--;
                if(i < 0) i = n-1;
            } while(i != j);
            //putchar('\n');
            write(fd, x, n+1);
        }
    }
    //fclose(stdout);
    //fflush(fp);
    //fp = freopen("CON", "w", stdout);
    //cout << "hello" << endl;
}


//void output(int a[]) {
//
//}
//
//void fast_generator(int n) {
//    //initialization
//    int P[n+1];  //P[1:n-1]
//    int IP[n]; //IP[1:n-1]
//    int T[n];  //T[1:n-1]
//    int D[n];  //D[2:n-1]
//    P[0] = P[n] = n+1;
//    for(int k = 1; k < n; k++) {
//        P[k] = k;
//        IP[k] = k;
//        D[k] = -1;
//    }
//    int i = n-1;
//    T[n-1] = -2;
//    T[2] = 0;
//    int s = n, v = -1, m = 1;
//
//    while(true) {
//        output(P);
//        if(s == m) {
//            m = n + 1 - m;
//            v = -v;
//        } else {
//            P[s] = P[s+v];
//            s = s + v;
//            P[s] = n;
//            continue;
//        }
//        if(i == 0) break;
//        swap(P[IP[i]], P[IP[i] + D[i]]);
//        swap(IP[i], IP[P[IP[i]]]);
//        bool a = (i < n-1);
//        bool b = (i < P[IP[i] + D[i]]);
//        if(a) {
//            D[i] = -D[i];
//            if(T[i] < 0) {
//                if(-T[i] != i-1) T[i-1] = T[i];
//                T[i] = i - 1;
//            }
//        }
//        if(!a && b) {
//            T[k] = -i;
//            i = k;
//        } else if (a && !b) {
//
//        } else if (a && b) {
//
//        }
//    }
//
//}
