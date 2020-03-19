#include<stdio.h>
#include<iostream>
#include<fstream>
#include<vector>
#include<time.h>
#include<cmath>
#include<stdint.h>
#include<stdlib.h>



#include "../counter9_h/counterone9.h"
#include "../counter9_h/countertwo9.h"
#include "../counter9_h/counterthree9.h"
#include "../counter9_h/counterfour9.h"
#include "../counter9_h/counterfive9.h"
#include "../counter9_h/countersix9.h"
#include "../counter9_h/counterseven9.h"
#include "../counter9_h/countereight9.h"

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
    outfile.open("counter9.h");
    for (int i = 0; i < size1; i++)
    {
        for (int j = 0; j < size2; j++)
        {
            counter[i][j] = counterone9[i][j] + countertwo9[i][j] + counterthree9[i][j] + counterfour9[i][j] + counterfive9[i][j] + countersix9[i][j] + counterseven9[i][j] + countereight9[i][j];
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

