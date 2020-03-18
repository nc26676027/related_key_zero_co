#include<stdio.h>
#include<iostream>
#include<fstream>
#include<vector>
#include<time.h>
#include<cmath>
#include<stdint.h>
#include<stdlib.h>

using namespace std;

#include"counter3_h/counterone3.h"
#include"counter3_h/countertwo3.h"
#include"counter3_h/counterthree3.h"
#include"counter3_h/counterfour3.h"
#include"counter3_h/counterfive3.h"
#include"counter3_h/countersix3.h"
#include"counter3_h/counterseven3.h"
#include"counter3_h/countereight3.h"


const unsigned long int size1 = pow(2 , (4*2));
const unsigned long int size2 = pow(2 , (4*3));


int N1 = pow(2 , (4*2));
int N2 = pow(2 , (4*3));

vector<vector<unsigned>> counter( size1 , vector<unsigned> (size2 , 0));

int main()
{
    printf("add all counter on toy_cipher.\n");
    
    ofstream outfile;
    outfile.open("counter3.txt");
    for (int i = 0; i < size1; i++)
    {
        for (int j = 0; j < size2; j++)
        {
            counter[i][j] = counterone3[i][j] + countertwo3[i][j] + counterthree3[i][j] + counterfour3[i][j] + counterfive3[i][j] + countersix3[i][j] + counterseven3[i][j] + countereight3[i][j];
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

