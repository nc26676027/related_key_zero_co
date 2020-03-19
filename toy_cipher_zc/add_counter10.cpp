#include<stdio.h>
#include<iostream>
#include<fstream>
#include<vector>
#include<time.h>
#include<cmath>
#include<stdint.h>
#include<stdlib.h>



#include "../counter10_h/counterone10.h"
#include "../counter10_h/countertwo10.h"
#include "../counter10_h/counterthree10.h"
#include "../counter10_h/counterfour10.h"
#include "../counter10_h/counterfive10.h"
#include "../counter10_h/countersix10.h"
#include "../counter10_h/counterseven10.h"
#include "../counter10_h/countereight10.h"

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
    outfile.open("counter10.h");
    for (int i = 0; i < size1; i++)
    {
        for (int j = 0; j < size2; j++)
        {
            counter[i][j] = counterone10[i][j] + countertwo10[i][j] + counterthree10[i][j] + counterfour10[i][j] + counterfive10[i][j] + countersix10[i][j] + counterseven10[i][j] + countereight10[i][j];
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

