//
//  ChangeCarryNumber.cpp
//  PermutationGenerationProj
//
//  Created by 陈禹东 on 11/4/18.
//  Copyright © 2018 陈禹东. All rights reserved.
//

#include "ChangeCarryNumber.h"
#include <iostream>
using namespace std;

//重载运算符
ChangeCarryNumber ChangeCarryNumber::operator+(ChangeCarryNumber& b) {
    int N = get_N();
    ChangeCarryNumber ans = ChangeCarryNumber(N, get_mode());
    int car = 0;  //进位1或0
    for(int i = 0; i < N-1; i++) {
        int sum = this->number[i] + b.number[i] + car;
        if(sum > carry[i]) {
            ans.number[i] = sum - carry[i+1];
            car = 1;
        } else {
            ans.number[i] = sum;
            car = 0;
        }
    }
    return ans;
}

ChangeCarryNumber ChangeCarryNumber::operator-(ChangeCarryNumber& b) {
    int N = get_N();
    ChangeCarryNumber ans = ChangeCarryNumber(N, get_mode());
    int borrow = 0;
    for(int i = 0; i < N-1; i++) {
        int dif = this->number[i] - b.number[i] - borrow;
        if(dif < 0) {
            ans.number[i] = dif + carry[i+1];
            borrow = 1;
        } else {
            ans.number[i] = dif;
            borrow = 0;
        }
    }
    return ans;
}

ChangeCarryNumber& ChangeCarryNumber::operator++() {
    this->number[0]++;
    int i = 0;
    while(this->number[i] > carry[i]) {
        this->number[i] = 0;
        this->number[i+1]++;
        i++;
    }
    return *this;
}

ChangeCarryNumber& ChangeCarryNumber::operator=(const ChangeCarryNumber& b) {
    this->set_N(b.N);
    this->set_mode(b.mode);
    int N = get_N();
    for(int i = 0; i < N-1; i++) {
        this->number[i] = b.number[i];
        this->carry[i] = b.carry[i];
    }
    return *this;
 }


//重要函数
void ChangeCarryNumber::fromPermutation(char p[], char algo) { //从排列生成中介数，并更新本对象的number
    int N = get_N();
    for(int i = 0; i < N; i++) { //对于排列p的每一位，注意：排列"83674521"的p[0]='8'
        int num = p[i] - '0'; //p[i]对应的数字，如p[i]='8'，则num=8
        if(num != 1) {
            int cnt = 0;
            for(int j = i+1; j < N; j++) { //寻找它右边的数
                if(p[i] > p[j]) cnt++;
            }
            switch (algo) {
                case 'l': //【字典序法'l'】
                    number[N-2-i] = cnt;
                    break;
                case 'i': //【递增进位制数法'i'】
                    number[num-2] = cnt;
                    break;
                case 'd': //【递减进位制数法'd'】
                    number[N-num] = cnt;
                    break;
                case 'n': //【邻位对换法'n'】
                    //to be continued...
                    break;
                    
                default:
                    cout << "Wrong Input (algo) in fromPermutation()!" << endl;
                    break;
            }
        }
    }
}

char* ChangeCarryNumber::toPermutation(char algo) { //从中介数生成排列
    int N = get_N();
    char* p = new char[N];
    memset(p, 0, sizeof(p)+1); //初始全部置为'\0'，且p[N]作为字符串终止符也置为'\0'
    switch (algo) {
        case 'l': //【字典序法'l'】
            // to be continued..
            break;
        case 'i': //【递增进位制数法'i'】
            for(int i = N-2; i >= 0; i--) { //对中介数的每一位，从aN到a2
                int emptyCnt = 0;
                for(int j = N-1; j >= 0; j--) { //扫描当前排列p
                    if(p[j] == '\0') emptyCnt++;
                    if(emptyCnt == number[i] + 1) {
                        p[j] = C[i+2];
                        break;
                    }
                }
            }
            for(int j = N-1; j >= 0; j--) { //填上没有中介数对应的'1'
                if(p[j] == '\0') {
                    p[j] = C[1];
                    break;
                }
            }
            break;
        case 'd': //【递减进位制数法'd'】
            for(int i = 0; i < N-1; i++) { //对中介数的每一位，从aN到a2
                int emptyCnt = 0;
                for(int j = N-1; j >= 0; j--) { //扫描当前排列p
                    if(p[j] == '\0') emptyCnt++;
                    if(emptyCnt == number[i] + 1) {
                        p[j] = C[N-i];
                        break;
                    }
                }
            }
            for(int j = N-1; j >= 0; j--) { //填上没有中介数对应的'1'
                if(p[j] == '\0') {
                    p[j] = C[1];
                    break;
                }
            }
            break;
        case 'n': //【邻位对换法'n'】
            //to be continued...
            break;
            
        default:
            cout << "Wrong Input (algo) in toPermutation()!" << endl;
            break;
    }
    return p;
}
