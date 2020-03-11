#include<stdio.h>
#include<iostream>
#include<fstream>
#include<vector>
#include<time.h>
#include<cmath>
#include<stdint.h>
#include<stdlib.h>

#include "../table_h/table2.h"
#include "../table_h/table3.h"

using namespace std;


const unsigned long int size1 = pow(2 , (4*3));

int N1 = pow(2 , (4*3));
int N2 = pow(2 , (4*4));

vector<unsigned int> counter( size1 , 0 );

bool get_xored(unsigned int in)
{
    unsigned int count = 0;
    unsigned int v = in;
    while (v)
    {
        count += (v % 2);
        v = v / 2;
    }
    if( (count % 2)  == 0 )
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

unsigned long long int rol(int val, int size)
{

  unsigned long long int res = val << size;

  return res;
}

unsigned long long int ror(int val, int size)
{

  unsigned long long int res = val >> size;

  return res;
}


unsigned int nible_to_int(vector<vector<int>> in )
{
    unsigned int res = 0;

    for( int i = 0; i < 2; i++ )
    {
        for (int j = 0; j < 2; j++)
        {
            res = res ^ rol( in[i][j] & 0xF , 12 - ( 4*(i*2 + j) ) );
        }

    }
    return res;
}



int encrypt_all( unsigned alpha1 , unsigned alpha2 , unsigned beta )
{

    unsigned int XOR = 0;

    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            unsigned p0 = i ^ rol(0x2 , 12 ); 
            XOR = XOR ^ (alpha1 & p0) ^ (alpha2 & j) ^(beta & rol(unsigned(Encoding2[i][j]) , 12) );
        }
    }

    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            unsigned p1 = i ^ rol(0x3 , 12 ); 
            XOR = XOR ^ (alpha1 & p1) ^ (alpha2 & j) ^(beta & rol(unsigned(Encoding3[i][j]) , 12) );
        }
    }

    bool judge = get_xored(XOR);
    if( ! judge )
    {
        unsigned index = ror(alpha2 , 4);
        counter[index]++;
    }
}


// Test d i s t i n g u i s h e r f o r twe ak able BC with one tweakey l i n e

int testTK1()
{
	


    //generate all keys at random
    srand( time(NULL));

    vector<vector<int>> alpha1(2 , vector<int>(2, 0));
    vector<vector<int>> alpha2(2 , vector<int>(2, 0));
    vector<vector<int>> beta(2 , vector<int>(2, 0));

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            if( ( i*2 + j) == 0  )
            {
                alpha1[i][j] = 0xF;
            }
            else if( (i*2 + j) == 3 )
            {
                alpha1[i][j] = 0xa;
            }
            else
            {
                alpha1[i][j] = 0;
            }            
        
        }
        
    }

    alpha2[1][1] = 0;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            if( (i*2 + j) == 0 )
            {
                beta[i][j] = 0xa;
            }
            else
            {
                beta[i][j] = 0;
            }            
        
        }
        
    }
    

    for ( int i1 = 0; i1 < 16; i1++)
    {
        for ( int i2 = 0; i2 < 16; i2++)
        {
            for ( int i3 = 0; i3 < 16; i3++)
            {
                alpha2[0][0] = i1;
                alpha2[0][1] = i2;
                alpha2[1][0] = i3;

                unsigned a1 = nible_to_int(alpha1);
                unsigned a2 = nible_to_int(alpha2);
                unsigned b = nible_to_int(beta);

                encrypt_all(a1 , a2 , b);

            }
        }
    }

    /*---------------------------打印计数器---------------------------*/
    string file = "countertwo.h";
    ofstream outfile;
    outfile.open(file);

    outfile<<"#include <stdint.h>"<<endl;
    outfile<<"#ifndef _COUNTERTWO_H"<<endl;
    outfile<<"#define _COUNTERTWO_H"<<endl;
    outfile<<endl; 
    string group_num = "countertwo";

    outfile<<"unsigned "<<group_num<<"["<<N1<<"]"<<" = "<<endl;
    outfile<<"  {"<<endl;
    for (int i = 0; i < N1; i++)
    { 

        outfile<<counter[i];            
        if ( i < N1-1 )
        {
            outfile<<" , ";
        }                

    }
    outfile<<"  };"<<endl;


    outfile<<"#endif"<<endl;
    outfile.close();

}


int main()
{
    printf("Experimental verification of distinguisher on TK1.\n");
    
    testTK1();


    return 0;
}
