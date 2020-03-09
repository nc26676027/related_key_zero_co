#include<stdio.h>
#include<iostream>
#include<vector>
#include<time.h>
#include<cmath>
#include <stdint.h>
#include <stdlib.h>
#include "test_h\table0.h"
#include "test_h\table1.h"
#include "test_h\table2.h"
#include "test_h\table3.h"
#include "test_h\table4.h"
#include "test_h\table5.h"
#include "test_h\table6.h"
#include "test_h\table7.h"
#include "test_h\table8.h"
#include "test_h\table9.h"
#include "test_h\table10.h"
#include "test_h\table11.h"
#include "test_h\table12.h"
#include "test_h\table13.h"
#include "test_h\table14.h"
#include "test_h\table15.h"


using namespace std;

void encrypt (uint32_t* plaintext, uint32_t* ciphertext) {
	char S[] = { 0x9e37b8e9, 0xaf48c9fa, 0x8d26a7d8, … }; /* Sbox */
	ciphertext = S[plaintext];
}



int main(void)
{
    printf("Experimental encrypt table on toy_cipher.\n");
    //testTK1();
    uint8_t test_num = 68;
    cout<<test_num<<endl;
    cout<<unsigned(test_num)<<endl;

    return 0;
}
