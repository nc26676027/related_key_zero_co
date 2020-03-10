#include<stdio.h>
#include<iostream>
#include<fstream>
#include<vector>
#include<time.h>
#include<cmath>
#include<stdint.h>
#include<stdlib.h>

using namespace std;

#include"counter0_h/counterone0"
#include"counter0_h/countertwo0"
#include"counter0_h/counterthree0"
#include"counter0_h/counterfour0"
#include"counter0_h/counterfive0"
#include"counter0_h/countersix0"
#include"counter0_h/counterseven0"
#include"counter0_h/countereight0"


const unsigned long int size1 = pow(2 , (4*2));

int N1 = pow(2 , (4*3));
int N2 = pow(2 , (4*2));

vector<unsigned int> counter( size1 , 0 );

int main()
{
    printf("add all counter on toy_cipher.\n");
    
    ofstream outfile;
    outfile.open("counter0.txt");
    for (int i = 0; i < size1; i++)
    {
        counter[i] = counterone0[i] + countertwo0[i] + counterthree0[i] + counterfour0[i] + counterfive0[i] + countersix0[i] + counterseven0[i] + countereight0[i];
    }

    for(int j = 0;j < size1;j++)
    {
        outfile<<"counter =  (   "<<counter[j]<<"   )"<<endl;
        
    }
    




    

    outfile.close();

    return 0;
}

