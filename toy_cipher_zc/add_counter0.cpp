#include<stdio.h>
#include<iostream>
#include<fstream>
#include<vector>
#include<time.h>
#include<cmath>
#include<stdint.h>
#include<stdlib.h>

using namespace std;

#include"counterone0.h"
#include"countertwo0.h"
#include"counterthree0.h"
#include"counterfour0.h"
#include"counterfive0.h"
#include"countersix0.h"
#include"counterseven0.h"
#include"countereight0.h"


const unsigned long int size1 = pow(2 , (4*2));
const unsigned long int size2 = pow(2 , (4*3));


int N1 = pow(2 , (4*2));
int N2 = pow(2 , (4*3));

vector<vector<unsigned>> counter( size1 , vector<unsigned> (size2 , 0));

int main()
{
    printf("add all counter on toy_cipher.\n");
    
    ofstream outfile;
    outfile.open("counter0.txt");
    for (int i = 0; i < size1; i++)
    {
        for (int j = 0; j < size2; j++)
        {
            counter[i][j] = counterone0[i][j] + countertwo0[i][j] + counterthree0[i][j] + counterfour0[i][j] + counterfive0[i][j] + countersix0[i][j] + counterseven0[i][j] + countereight0[i][j];
        }
    }
    string group_num = "last_counter";
    outfile<<"unsigned "<<group_num<<"["<<N1<<"]"<<"["<<N2<<"]"<<" = {";
    for (int i = 0; i < N1; i++)
    {
        outfile<<"{";
        for (int j = 0; j < N2; j++)
        {
            outfile<<unsigned(counter[i][j]);
            if (j < N2 - 1)
            {
                outfile<<"     ,     ";
            }
            if ( j % 6 == 0)
            {
                outfile<<"    "<<endl;
            }
        }
        outfile<<"}"<<endl;
        
        if (i < N1 - 1)
        {
            //outfile<<" , ";
            outfile<<"***********************"<<endl;
            outfile<<"***********************"<<endl;
            outfile<<"***********************"<<endl;

        }
    }
    outfile<<"};"<<endl;
    outfile<<"***********************"<<endl;
    




    

    outfile.close();

    return 0;
}

