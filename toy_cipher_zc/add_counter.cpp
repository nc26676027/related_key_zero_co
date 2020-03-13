#include<stdio.h>
#include<iostream>
#include<fstream>
#include<vector>
#include<time.h>
#include<cmath>
#include<stdint.h>
#include<stdlib.h>

using namespace std;

#include"counter0_h/counterone.h"
#include"counter0_h/countertwo.h"
#include"counter0_h/counterthree.h"
#include"counter0_h/counterfour.h"
#include"counter0_h/counterfive.h"
#include"counter0_h/countersix.h"
#include"counter0_h/counterseven.h"
#include"counter0_h/countereight.h"


const unsigned long int size1 = pow(2 , (4*3));

int N1 = pow(2 , (4*3));
int N2 = pow(2 , (4*4));

vector<unsigned int> counter( size1 , 0 );

int main()
{
    printf("add all counter on toy_cipher.\n");
    
    ofstream outfile;
    outfile.open("counter0.txt");
    for (int i = 0; i < size1; i++)
    {
        counter[i] = counterone[i] + countertwo[i] + counterthree[i] + counterfour[i] + counterfive[i] + countersix[i] + counterseven[i] + countereight[i];
    }

    for(int j = 0;j < size1;j++)
    {
        outfile<<"counter =  (   "<<counter[j]<<"   )"<<endl;
        
    }
    




    

    outfile.close();

    return 0;
}

