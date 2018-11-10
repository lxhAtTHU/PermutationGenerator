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
#include <string>
#include <time.h>
#include <ctime>
#include <mpi.h>
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
	double _t = (double)(end-start)/CLOCKS_PER_SEC;
	double t = 0;
	MPI_Reduce(&_t, &t, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	if(rank == 0){
		printf("%s avg time: %.2fs\n", output_file.c_str(), t/nprocs);
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

void PermutationGenerator::recursion(int rank, int nprocs, string output_file){
 	char *x = new char[n+1]; // one more for '\n'
  	bool *flag = new bool[n];
	memset(x, 0, n+1);
	x[n] = '\n';
  	memset(flag, 0, n* sizeof(bool));
	
	string _output_file = output_file + to_string(rank);
    int fd = open(_output_file.c_str(), O_RDWR | O_CREAT, 0777);
  	
	// task partition
	if(n < 9)	
  		recursion_step(x, flag, n, 1, fd);
	else {
		int total_task = 1;
		for(int i=0; i<n; i++){
			total_task *= (n-i);
			if(total_task >= nprocs){
				int base = total_task/nprocs;
				int rest = total_task%nprocs;
				if(rank < rest){
					// task_num = base + 1
					for(int task=(base+1)*rank; task<(base+1)*(rank+1); task++){
						memset(x, 0, n+1);
						x[n] = '\n';
					  	memset(flag, 0, n* sizeof(bool));
						int t = task;
						for(int j=0; j<=i; j++){
							int step = 1;
							for(int k=j+1; k<=i; k++){
								step *= (n-k);
							}
							int order = t/step+1;
							for(int f_i=0; f_i<n; f_i++){
								if(flag[f_i] == false)
									order --;
								if(order == 0){
									x[j] = 'a' + f_i;
									flag[f_i] = true;
									break;
								}
							}
							t %= step;
						}
						printf("%d: %s\n", task, x);
						recursion_step(x, flag, n, i+2, fd);
					}
				} else {
					// task_num = base
					for(int task=rest+base*rank; task<rest+base*(rank+1); task++){
						memset(x, 0, n+1);
						x[n] = '\n';
					  	memset(flag, 0, n* sizeof(bool));
						int t = task;
						for(int j=0; j<=i; j++){
							int step = 1;
							for(int k=j+1; k<=i; k++){
								step *= (n-k);
							}
							int order = t/step+1;
							for(int f_i=0; f_i<n; f_i++){
								if(flag[f_i] == false)
									order --;
								if(order == 0){
									x[j] = 'a' + f_i;
									flag[f_i] = true;
									break;
								}
							}
							t %= step;
						}
						printf("%d: %s\n", task, x);
						recursion_step(x, flag, n, i+2, fd);	
					}
				}
				break;
			}
		}
	}

	//int fd = open(output_file.c_str(), O_RDWR | O_CREAT, 0777);
  	//recursion_step(x, flag, n, 1, fd);
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
	long long total = fac[n];
	long long chunk = total/nprocs;
	long long start = rank*chunk;
	long long end = (rank == (nprocs-1))? total : (rank+1)*chunk;
	
	string _output_file = output_file + to_string(rank);
	cout << _output_file << endl;
    ChangeCarryNumber x = ChangeCarryNumber(n, inc_or_dec, start);
    int fd = open(_output_file.c_str(), O_RDWR | O_CREAT, 0777);
    
    //生成全排列
    for(long long i = start; i < end; i++) {
        char* next = x.toPermutation(algo);
        //pwrite(fd, next, n+1, i*(n+1));
		write(fd, next, n+1);
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
    int cnt = 1;
    for(int k = n-3; k >= 1; k--) {
        int current_cnt = cnt;
        for(int j = 0; j < current_cnt; j++) {
            strcpy(starter[cnt], starter[j]);
            swap(starter[cnt][k], starter[cnt][k+1]);
            cnt++;
            for(int i = k+1; i < n-1; i++) {
                strcpy(starter[cnt], starter[cnt-1]);
                swap(starter[cnt][i], starter[cnt][i+1]);
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
			strcpy(x, starter[k]);
			x[n] = '\n';
			// 求一个串循环左移j位，相当于前j位先逆序，后n-j位再逆序，然后整体求逆序
			// 利用这个性质，先得到RoCP的值
			reverse(x, x+j);
			reverse(x+j, x+n);
			write(fd, x, n+1);
			// 再得到循环移动的值
			reverse(x, x+n);
			write(fd, x, n+1);
        }
    }
	delete []x;
}

