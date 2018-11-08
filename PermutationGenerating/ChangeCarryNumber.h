//
//  ChangeCarryNumber.h
//  PermutationGenerationProj
//
//  Created by 陈禹东 on 11/4/18.
//  Copyright © 2018 陈禹东. All rights reserved.
//
#include <iostream>
#include <memory>
#include <cstring>
#include <cstdio>
using namespace std;

#ifndef ChangeCarryNumber_h
#define ChangeCarryNumber_h

#define INC 100
#define DEC 101
#define MAX_LEN 20

const char C[MAX_LEN] = {
    '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
    'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's',
}; //便于生成排列
const long long fac[MAX_LEN] = {
    1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880,
    3628800, 39916800, 479001600, 6227020800, 87178291200,
    1307674368000, 20922789888000, 355687428096000,
    6402373705728000, 121645100408832000 //最后一个是19!
}; //便于生成n!个排列

class ChangeCarryNumber
{
private:
    int N; //对应排列的长度，如生成8位全排列，则N=8，递增进位制数为a8a7...a2，是一个7位数
    int mode; //mode=INC或DEC，决定是递增进位制还是递减进位制
    void init(int _N, int _mode) {
        N = _N;
        mode = _mode;
        memset(number, 0, sizeof(number));
        if(mode == INC) {
            for(int i = 0; i < N; i++) carry[i] = i + 1;
        } else if(mode == DEC) {
            for(int i = 0; i < N; i++) carry[i] = N + 1 - i;
        }
    }
public:
    int number[MAX_LEN]; //进位制数，如递增进位制数a8a7...a2，由number[6]~number[0]存储
    int carry[MAX_LEN]; //每一位对应的进位数，如carry[1]为9表示最低位number[0]逢9进1
    char p[MAX_LEN];
	ChangeCarryNumber(int _N, int _mode) {
        init(_N, _mode);
    }
    ChangeCarryNumber(int _N, int _mode, long long ccn) { //通过十进制数（long long）产生中介数对象
        init(_N, _mode);
        int i = 0;
        while(ccn > 0) {
            number[i] = ccn % (long long)carry[i+1];
            ccn /= carry[i+1];
            i++;
        }
		printf("test\n");
    }	
 
    //重要函数
    void fromPermutation(char p[], char algo); //从排列生成中介数，并更新本对象的number
    char* toPermutation(char algo); //从中介数生成排列
    int nextNumber(); //返回下一个中介数（int）
    
    //重载加减运算
    ChangeCarryNumber operator+(ChangeCarryNumber& b);
    ChangeCarryNumber operator-(ChangeCarryNumber& b);
    ChangeCarryNumber& operator++();
    ChangeCarryNumber& operator=(const ChangeCarryNumber& b);
    
    //辅助函数
    const int get_N() { return N; }
    const int get_mode() { return mode; }
    void set_N(const int _N) { N = _N; }
    void set_mode(const int _mode) { mode = _mode; }
    void print() {
        for(int i = N-2; i > 0; i--) {
            cout << number[i];
        }
        cout << number[0] << endl;
    }
};

#endif /* ChangeCarryNumber_h */
