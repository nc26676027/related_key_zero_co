#include<stdio.h>
#include<iostream>
#include<fstream>
#include<vector>
#include<time.h>
#include<cmath>
#include<stdint.h>
#include<stdlib.h>

#include "../table_h/table0.h"
#include "../table_h/table1.h"
#include "../table_h/table2.h"
#include "../table_h/table3.h"


/*
#include "test_h/table0.h"
#include "test_h/table1.h"
#include "test_h/table2.h"
#include "test_h/table3.h"
#include "test_h/table4.h"
#include "test_h/table5.h"
#include "test_h/table6.h"
#include "test_h/table7.h"
#include "test_h/table8.h"
#include "test_h/table9.h"
#include "test_h/table10.h"
#include "test_h/table11.h"
#include "test_h/table12.h"
#include "test_h/table13.h"
#include "test_h/table14.h"
#include "test_h/table15.h"
*/




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

    unsigned int xor = 0;

    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            xor = xor ^ (alpha1 & i) ^ (alpha2 & j) ^(beta & unsigned(Encoding0[i][j]) );
        }
    }

    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            xor = xor ^ (alpha1 & i) ^ (alpha2 & j) ^(beta & unsigned(Encoding1[i][j]) );
        }
    }

    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            xor = xor ^ (alpha1 & i) ^ (alpha2 & j) ^(beta & unsigned(Encoding2[i][j]) );
        }
    }

    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            xor = xor ^ (alpha1 & i) ^ (alpha2 & j) ^(beta & unsigned(Encoding3[i][j]) );
        }
    }
    /*
    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            xor = xor ^ (alpha1 & i) ^ (alpha2 & j) ^(beta & unsigned(Encoding4[i][j]) );
        }
    }

    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            xor = xor ^ (alpha1 & i) ^ (alpha2 & j) ^(beta & unsigned(Encoding5[i][j]) );
        }
    }

    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            xor = xor ^ (alpha1 & i) ^ (alpha2 & j) ^(beta & unsigned(Encoding6[i][j]) );
        }
    }

    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            xor = xor ^ (alpha1 & i) ^ (alpha2 & j) ^(beta & unsigned(Encoding7[i][j]) );
        }
    }
    
    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            xor = xor ^ (alpha1 & i) ^ (alpha2 & j) ^(beta & unsigned(Encoding8[i][j]) );
        }
    }
    
    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            xor = xor ^ (alpha1 & i) ^ (alpha2 & j) ^(beta & unsigned(Encoding9[i][j]) );
        }
    }

    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            xor = xor ^ (alpha1 & i) ^ (alpha2 & j) ^(beta & unsigned(Encoding10[i][j]) );
        }
    }

    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            xor = xor ^ (alpha1 & i) ^ (alpha2 & j) ^(beta & unsigned(Encoding11[i][j]) );
        }
    }

    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            xor = xor ^ (alpha1 & i) ^ (alpha2 & j) ^(beta & unsigned(Encoding12[i][j]) );
        }
    }

    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            xor = xor ^ (alpha1 & i) ^ (alpha2 & j) ^(beta & unsigned(Encoding13[i][j]) );
        }
    }

    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            xor = xor ^ (alpha1 & i) ^ (alpha2 & j) ^(beta & unsigned(Encoding14[i][j]) );
        }
    }

    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            xor = xor ^ (alpha1 & i) ^ (alpha2 & j) ^(beta & unsigned(Encoding15[i][j]) );
        }
    }
    */
    bool judge = get_xored(xor);
    if( ! judge )
    {
        unsigned index = (alpha2 >> 4);
        counter[index]++;
    }
}


// Test d i s t i n g u i s h e r f o r twe ak able BC with one tweakey l i n e

int testTK1( int num )
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
            if( ( (i*2 + j) == 0 ) || ( (i*2 + j) == 3 ) )
            {
                alpha1[i][j] = 0xF;
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
                alpha1[i][j] = 0;
            }            
        
        }
        
    }
    

    for ( int i1 = num; i1 < num+1; i1++)
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
    string file = "counterone"+to_string(num)+".h";
    ofstream outfile;
    outfile.open(file);

    outfile<<"#include <stdint.h>"<<endl;
    outfile<<"#ifndef _COUNTERONE"<<num<<"_H"<<endl;
    outfile<<"#define _COUNTERONE"<<num<<"_H"<<endl;
    outfile<<endl; 
    string group_num = "counterone"+to_string(num);

    outfile<<"unsigned "<<group_num<<"["<<N1<<"]"<<" = "<<endl;
    outfile<<"  {"<<endl;   
    for (int j = 0; j < 16; j++)
    {
        for(int k = 0;k < 16; k++)
        {
            unsigned index = rol(num , 8) ^ rol(j , 4) ^ k;
            outfile<<counter[index];
        }
    }
    outfile<<"  };"<<endl;

    printf("[OK] ==============> Initial_Encoding%d has been print.\n" , num);

    outfile<<"#endif"<<endl;
    outfile.close();

}


int main(int argc,char * argv[])
{
    printf("Experimental verification of distinguisher on TK1.\n");
    

    //input canshu
	int origin;

	printf("input %d parameter \n" , argc);

	if(argc!=2)
	{
		printf("parameter number error!!");
		exit(0);
	}
	else
    {
        origin = atoi(argv[1]);
    }
    printf("begin num = %d  ",origin );



    testTK1(origin);


    return 0;
}
