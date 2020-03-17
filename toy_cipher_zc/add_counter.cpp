#include<stdio.h>
#include<iostream>
#include<fstream>
#include<vector>
#include<time.h>
#include<cmath>
#include<stdint.h>
#include<stdlib.h>

using namespace std;

#include"counter1_h/counterone.h"
#include"counter1_h/countertwo.h"
#include"counter1_h/counterthree.h"
#include"counter1_h/counterfour.h"
#include"counter1_h/counterfive.h"
#include"counter1_h/countersix.h"
#include"counter1_h/counterseven.h"
#include"counter1_h/countereight.h"


const unsigned long int size1 = pow(2 , (4*3));

int N1 = pow(2 , (4*3));
int N2 = pow(2 , (4*3));

vector<unsigned int> counter( size1 , 0 );

int main()
{
    printf("add all counter on toy_cipher.\n");
    
    ofstream outfile;
    outfile.open("counter.txt");
    for (int i = 0; i < size1; i++)
    {
        for (int j = 0; j < size1; j++)
        {
            counter[i][j] = counterone[i][j] + countertwo[i][j] + counterthree[i][j] + counterfour[i][j] + counterfive[i][j] + countersix[i][j] + counterseven[i][j] + countereight[i][j];
        }
    }

    for(int i = 0;i < size1;i++)
    {
        for(int j = 0;j < size1;j++)
        {
            
        }
        outfile<<"counter =  (   "<<counter[j]<<"   )"<<endl;
        
    }
    




    

    outfile.close();

    return 0;
}

