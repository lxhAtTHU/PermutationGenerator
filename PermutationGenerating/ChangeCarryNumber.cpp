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
    while(this->number[i] == carry[i+1]) {
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
        int num = p[i] - C[0]; //p[i]对应的数字，如p[i]='8'，则num=8
        if(num != 1) {
            int cnt = 0;
            for(int j = i+1; j < N; j++) { //寻找它右边比它小的数有几个
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
                    break;
                default:
                    cout << "Wrong Input (algo) in fromPermutation()!" << endl;
                    break;
            }
        }
    }
    if(algo == 'n') {
        bool d[N+1]; //记录每一个数字k的方向d[k]，false为左，true为右
        memset(d, 0, sizeof(d));
        int pos[N+1]; //记录每一个数字k对应的下标pos[k]，如pos[2]代表'b'对应的下标方便之后计算中介数
        for(int i = 0; i < N; i++) { //扫描排列p的每一个位置i
            pos[p[i]-C[0]] = i; //下标pos[k]越小，说明该数字k越靠左
        }
        //计算中介数中的b2
        if(pos[2] < pos[1]) number[N-2] = 1;
        else number[N-2] = 0;
        for(int i = 2; i < N; i++) {
            int k = i+1; //当前考虑数字k
            int bk_1 = number[N-(k-1)]; //中介数中的b_(k-1)
            if((k & 1) == 1) { //(k&1)==1表示k是奇数
                if((bk_1 & 1) == 1) d[k] = 1; //如果b_(k-1)为奇，则k的方向向右。默认向左
            } else {
                int bk_2 = number[N-(k-2)]; //中介数中的b_(k-2)
                if((bk_1 + bk_2 & 1) == 1) d[k] = 1; //如果b_(k-1)+b_(k-2)为奇，则k的方向向右
            }
            int cnt = 0;
            if(d[k]) { //数字k的方向朝右，则看它左边比它小的数有几个
                for(int j = 0; j < pos[k]; j++) {
                    if(p[j]-C[0] < k) cnt++;
                }
            } else {
                for(int j = pos[k]+1; j < N; j++) {
                    if(p[j]-C[0] < k) cnt++;
                }
            }
            number[N-k] = cnt;
        }
    }
}

char* ChangeCarryNumber::toPermutation(char algo) { //从中介数生成排列
    int N = get_N();
    char* p = new char[N];
    memset(p, 0, sizeof(p)+1); //初始全部置为'\0'，且p[N]作为字符串终止符也置为'\0'
    switch (algo) {
        case 'l': //【字典序法'l'】
        {
            bool u[N]; //记录每一个数是否已存在于排列中，u[0]==true代表'a'已填入排列p
            memset(u, 0, sizeof(u));
            for(int i = 0; i < N-1; i++) { //依次填充排列p的每一位
                int a = number[N-2-i]; //按字典序法的定义，p[i]的右边有a个数比它小
                int cnt = 0;
                for(int j = 0; j < N; j++) { //扫描所有数字，将第a+1个没有用过的数字填入p[i]
                    if(!u[j]) cnt++;
                    if(cnt == a+1) {
                        p[i] = C[j+1];
                        u[j] = true;
                        break;
                    }
                }
            }
            for(int j = 0; j < N; j++) {
                if(!u[j]) {
                    p[N-1] = C[j+1];
                    break;
                }
            }
        }
            break;
        case 'i': //【递增进位制数法'i'】
        {
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
        }
            break;
        case 'd': //【递减进位制数法'd'】
        {
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
        }
            break;
        case 'n': //【邻位对换法'n'】
        {
            for(int k = N; k > 1; k--) { //对于从N到2的每一个数k
                int bk = number[N-k];
                int bk_1 = number[N-(k-1)];
                int emptyCnt = 0;
                if((k & 1) == 1) { //若k为奇，则看b_(k-1)
                    if((bk_1 & 1) == 1) { //若为奇，则k的方向向右，将其填到 向右第b_k + 1个空
                        for(int j = 0; j < N; j++) {
                            if(p[j] == '\0') emptyCnt++;
                            if(emptyCnt == bk + 1) {
                                p[j] = C[k];
                                break;
                            }
                        }
                    } else { //否则，将其填到 向左第b_k + 1个
                        for(int j = N-1; j >= 0; j--) {
                            if(p[j] == '\0') emptyCnt++;
                            if(emptyCnt == bk + 1) {
                                p[j] = C[k];
                                break;
                            }
                        }
                    }
                } else { //若k为偶，则看b_(k-1)+b_(k-2)
                    int bk_2 = number[N-(k-2)];
                    if((bk_1 + bk_2 & 1) == 1) {
                        for(int j = 0; j < N; j++) {
                            if(p[j] == '\0') emptyCnt++;
                            if(emptyCnt == bk + 1) {
                                p[j] = C[k];
                                break;
                            }
                        }
                    } else {
                        for(int j = N-1; j >= 0; j--) {
                            if(p[j] == '\0') emptyCnt++;
                            if(emptyCnt == bk + 1) {
                                p[j] = C[k];
                                break;
                            }
                        }
                    }
                }
            }
            for(int i = 0; i < N; i++) {
                if(p[i] == '\0') {
                    p[i] = C[1];
                    break;
                }
            }
        }
            break;
            
        default:
            cout << "Wrong Input (algo) in toPermutation()!" << endl;
            break;
    }
    return p;
}
