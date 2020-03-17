#include<stdio.h>
#include<iostream>
#include<fstream>
#include<vector>
#include<time.h>
#include<cmath>
#include<stdint.h>
#include<stdlib.h>

#include "../table0_h/table0.h"
#include "../table0_h/table1.h"


using namespace std;


const unsigned long int size1 = pow(2 , (4*2));
const unsigned long int size2 = pow(2 , (4*3));


int N1 = pow(2 , (4*2));
int N2 = pow(2 , (4*3));

vector<vector<unsigned>> counter( size1 , vector<unsigned> (size2 , 0) );

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


unsigned int nible_to_int1(vector<vector<int>> in )
{
    unsigned int res = 0;

    for( int i = 0; i < 2; i++ )
    {
        for (int j = 0; j < 2; j++)
        {
            if ( i*2 + j == 3 )
            {
                res = res ^ rol( in[i][j] & 0xF , 4 );               
            } 
            else if ( i*2 + j == 0 )   
            {
                 res = res ^ rol( in[i][j] & 0xF , 8 );
            }                   
            
        }

    }
    return res;
}

unsigned int nible_to_int2(vector<vector<int>> in )
{
    unsigned int res = 0;

    for( int i = 0; i < 2; i++ )
    {
        for (int j = 0; j < 2; j++)
        {
            if( i*2 + j != 3 )
            {
                res = res ^ rol( in[i][j] & 0xF , 8 - ( 4*(i*2 + j) ) );
            }
            
        }

    }
    return res;
}



int encrypt_all( unsigned alpha1 , unsigned alpha2 )
{

    unsigned int XOR = 0;
    unsigned a1 = alpha1 & 0xff;


    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            XOR = 0;
            unsigned p0 = i ^ rol(0x0 , 8); 
            XOR = XOR ^ (alpha1 & p0) ^ (alpha2 & j);
            bool judge = get_xored(XOR);
            
            if( ! judge )
            {
                counter[a1][alpha2] += unsigned(Encoding0[i][j]);
            }
        }
    }

    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            XOR = 0;
            unsigned p1 = i ^ rol(0x1 , 8); 
            XOR = XOR ^ (alpha1 & p1) ^ (alpha2 & j);

            bool judge = get_xored(XOR);
            
            if( ! judge )
            {                
                counter[a1][alpha2] += unsigned(Encoding1[i][j]);
            }
        }
    }


}


// Test d i s t i n g u i s h e r f o r twe ak able BC with one tweakey l i n e

int testTK1(int num)
{
	


    //generate all keys at random
    srand( time(NULL));

    vector<vector<int>> alpha1(2 , vector<int>(2, 0));
    vector<vector<int>> alpha2(2 , vector<int>(2, 0));
    vector<vector<int>> beta(2 , vector<int>(2, 0));


    
    
    for ( int i1 = num; i1 < num+1; i1++)
    {
        for ( int i2 = 0; i2 < 16; i2++)
        {            
            for ( int i3 = 0; i3 < 16; i3++)
            {
                for ( int i4 = 0; i4 < 16; i4++)
                {
                    for ( int i5 = 0; i5 < 16; i5++)
                    {            
                        for ( int i6 = 0; i6 < 16; i6++)
                        {
                            alpha1[0][0] = i1;
                            alpha1[1][1] = i2;

                            beta[0][0] = i3;

                            alpha2[0][0] = i4;
                            alpha2[0][1] = i5;
                            alpha2[1][0] = i6;

                            unsigned a1 = nible_to_int1(alpha1);
                            unsigned a2 = nible_to_int2(alpha2);
                            a1 = a1 ^ ( beta[0][0] & 0xF );

                            encrypt_all( a1 , a2 );


                        }
                    }
                }
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

}


int main(int argc,char * argv[])
{
    printf("Experimental encrypt table on toy_cipher.\n");
    

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
