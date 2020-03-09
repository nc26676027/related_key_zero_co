#include<stdio.h>
#include<iostream>
#include<fstream>
#include<vector>
#include<time.h>
#include<cmath>
#include<stdint.h>
#include<stdlib.h>

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
            xor = xor ^ (alpha1 & i) ^ (alpha2 & j) ^(beta & Encoding0[i][j]);
        }
    }

    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            xor = xor ^ (alpha1 & i) ^ (alpha2 & j) ^(beta & Encoding1[i][j]);
        }
    }

    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            xor = xor ^ (alpha1 & i) ^ (alpha2 & j) ^(beta & Encoding2[i][j]);
        }
    }

    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            xor = xor ^ (alpha1 & i) ^ (alpha2 & j) ^(beta & Encoding3[i][j]);
        }
    }

    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            xor = xor ^ (alpha1 & i) ^ (alpha2 & j) ^(beta & Encoding4[i][j]);
        }
    }

    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            xor = xor ^ (alpha1 & i) ^ (alpha2 & j) ^(beta & Encoding5[i][j]);
        }
    }

    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            xor = xor ^ (alpha1 & i) ^ (alpha2 & j) ^(beta & Encoding6[i][j]);
        }
    }

    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            xor = xor ^ (alpha1 & i) ^ (alpha2 & j) ^(beta & Encoding7[i][j]);
        }
    }

    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            xor = xor ^ (alpha1 & i) ^ (alpha2 & j) ^(beta & Encoding8[i][j]);
        }
    }

    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            xor = xor ^ (alpha1 & i) ^ (alpha2 & j) ^(beta & Encoding9[i][j]);
        }
    }

    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            xor = xor ^ (alpha1 & i) ^ (alpha2 & j) ^(beta & Encoding10[i][j]);
        }
    }

    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            xor = xor ^ (alpha1 & i) ^ (alpha2 & j) ^(beta & Encoding11[i][j]);
        }
    }

    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            xor = xor ^ (alpha1 & i) ^ (alpha2 & j) ^(beta & Encoding12[i][j]);
        }
    }

    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            xor = xor ^ (alpha1 & i) ^ (alpha2 & j) ^(beta & Encoding13[i][j]);
        }
    }

    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            xor = xor ^ (alpha1 & i) ^ (alpha2 & j) ^(beta & Encoding14[i][j]);
        }
    }

    for(int i = 0;i < N1;i++)
    {
        for(int j = 0;j < N2;j++)
        {
            xor = xor ^ (alpha1 & i) ^ (alpha2 & j) ^(beta & Encoding15[i][j]);
        }
    }

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
                alpha1[i][j] = 0xF & rand();
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
                beta[i][j] = 0xF & rand();
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

    ofstream outfile;

    outfile.open("counter.txt");

    for(int i = num;i < num+1;i++)
    {
        outfile<<"the counter of number:"<<num<<" is {"<<endl;
        for (int j = 0; j < 16; j++)
        {
            outfile<<"{ ";
            for(int k = 0;k < 16; k++)
            {
                unsigned index = rol(num , 8) ^ rol(j , 4) ^ k;
                outfile<<counter[index];
                if(k < 15)
                {
                    outfile<<" , ";
                }
            }
            outfile<<"};"<<endl;

        }
        outfile<<" };"<<endl;
    }

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
