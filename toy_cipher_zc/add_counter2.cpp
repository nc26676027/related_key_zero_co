#include<stdio.h>
#include<iostream>
#include<fstream>
#include<vector>
#include<time.h>
#include<cmath>
#include<stdint.h>
#include<stdlib.h>


#include"../counter2_h/counterone2.h"
#include"../counter2_h/countertwo2.h"
#include"../counter2_h/counterthree2.h"
#include"../counter2_h/counterfour2.h"
#include"../counter2_h/counterfive2.h"
#include"../counter2_h/countersix2.h"
#include"../counter2_h/counterseven2.h"
#include"../counter2_h/countereight2.h"

using namespace std;

const unsigned long int size1 = pow(2 , (4*2));
const unsigned long int size2 = pow(2 , (4*3));
const unsigned long int size3 = pow(2 , 31);
int count = 0;

int N1 = pow(2 , (4*2));
int N2 = pow(2 , (4*3));

vector<vector<unsigned>> counter( size1 , vector<unsigned> (size2 , 0));

int main()
{
    printf("add all counter on toy_cipher.\n");
    
    ofstream outfile;
    outfile.open("counter2.h");
    for (int i = 0; i < size1; i++)
    {
        for (int j = 0; j < size2; j++)
        {
            counter[i][j] = counterone2[i][j] + countertwo2[i][j] + counterthree2[i][j] + counterfour2[i][j] + counterfive2[i][j] + countersix2[i][j] + counterseven2[i][j] + countereight2[i][j];
            if ( counter[i][j] != size3)
            {
                printf("the counter is not zero-correlation is %d , %d : %u\n" , i , j , counter[i][j]);
                count++;
            }
        }

    }
    printf("the total number of none zero-correlation is %d\n" , count);

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
                outfile<<" , ";
            }
        }
        outfile<<"}"<<endl;
        
        if (i < N1 - 1)
        {
            outfile<<" , ";
        }
    }
    outfile<<"};"<<endl;


    outfile<<"#endif"<<endl;
    outfile.close();



    return 0;
}

