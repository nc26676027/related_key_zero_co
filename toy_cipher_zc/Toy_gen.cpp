#include<stdio.h>
#include<iostream>
#include <fstream>
#include<vector>
#include<time.h>
#include<cmath>
#include <stdint.h>
#include <stdlib.h>

using namespace std;

const unsigned long int size1 = pow(2 , (4*3));
const unsigned long int size2 = pow(2 , (4*4));

int N1 = pow(2 , (4*3));
int N2 = pow(2 , (4*4));

vector<vector<uint8_t>> encrypt_Array( size1 , vector<uint8_t> (size2 , 0) );

unsigned long long int rol(int val, int size)
{

  unsigned long long int res = val << size;

  return res;
}
/*
static void Print_Array_u8(ofstream &f, int num, int row, int column, vector<vector<uint8_t>> Array)
{
    int i, j;
    string group_num;
    group_num = "Encoding"+to_string(num);

    f<<"uint8_t "<<group_num<<"["<<row<<"]"<<"["<<column<<"]"<<" = {";
	for (i = 0; i < row; i++)
	{
        f<<"{";
        for (j = 0; j < column; j++)
		{
            f<<Array[i][j];
			if (j < column - 1)
			{
                f<<" , ";
			}
		}
        f<<"}"<<endl;
        
		if (i < row - 1)
		{
            f<<" , ";
		}
	}
    f<<"};"<<endl;
}
*/


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


vector<vector<int>> subByte(vector<vector<int>> in , vector<vector<int>> rk )
{
    vector<int> sbox = {0xc, 0x0, 0xf, 0xa,
				        0x2, 0xb, 0x9, 0x5,
				        0x8, 0x3, 0xd, 0x7,
				        0x1, 0xe, 0x6, 0x4};

    int RK[2] = { 0, 2 };

    int index = 0;
    vector<vector<int>> out = in;
    for (int i = 0;i < 2; i++)
    {
        for( int j = 0; j < 2; j++)
        {
            if( (j % 2) == 1 )
            {
               out[i][j] = sbox[ in[i][j-1] ^ rk[ RK[index]/2 ][ RK[index]%2 ] ] ^ in[i][j];
               index ++;
            }
            else
            {
                out[i][j] = in[i][j];
            }

        }
    }
    return out;
}



//linear layer
vector<vector<int>> shiftNible(vector<vector<int>> in )
{
    int h[4] = {3, 0, 1, 2};

    vector<vector<int>> out = in;

    for (int i = 0;i < 4; i++)
    {
        out[ h[i] / 2 ][ h[i] % 2 ] = in[i/2][i%2];
    }

    return out;
}

//key update
vector<vector<int>> keySchedule(vector<vector<int>> in )
{
    vector<int> sbox = {0xc, 0x0, 0xf, 0xa,
				        0x2, 0xb, 0x9, 0x5,
				        0x8, 0x3, 0xd, 0x7,
				        0x1, 0xe, 0x6, 0x4};

    int Rot[4] = { 1, 2, 3, 0 };
    //permutation
    vector<vector<int>> rot(2, vector<int>(2, 0));
    vector<vector<int>> out(2, vector<int>(2, 0));
    for ( int i = 0; i < 4; i++)
    {
        if( i == 1 )
        {
            rot[0][1] = sbox[ in[0][0] ] ^ in[0][1];
        }
        else
        {
            rot[ i / 2 ][ i % 2 ] = in[ i / 2 ][ i % 2 ];
        }

    }
    for (int i = 0; i < 4; i++)
    {
        out[ Rot[i] / 2][ Rot[i] % 2 ] = rot[ i / 2 ][ i % 2 ];
    }

    return out;
}



// Test d i s t i n g u i s h e r f o r twe ak able BC with one tweakey l i n e

int testTK1(int num)
{
    //generate all keys at random

    int x_Rounds = 3;
    int y_Rounds = 5;
    int Round = x_Rounds+y_Rounds;
    printf("  Number of rounds : %d\n" , Round);



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
                            for ( int i7 = 0; i7 < 16; i7++)
                            {
                                for ( int i8 = 0; i8 < 16; i8++)
                                {
                                    vector<vector<int>> in(2, vector<int>(2, 0));
                                    vector<vector<int>> tk1(2, vector<int>(2, 0));

                                    in[0][0] = i1;
                                    in[0][1] = i2;
                                    in[1][0] = i3;
                                    in[1][1] = i4;

                                    tk1[0][0] = i5;
                                    tk1[0][1] = i6;
                                    tk1[1][0] = i7;
                                    tk1[1][1] = i8;
                                    
                                    printf("p1*******************\n");
                                    unsigned int P = nible_to_int(in);
                                    unsigned int T = nible_to_int(tk1);
                                    printf("P2*+++++++++++++++++\n");
                                    //encryption
                                    for (int r = 0; r < Round - 1 ; r++)
                                    {
                                        in = subByte (in , tk1);
                                        in = shiftNible(in);

                                        tk1 = keySchedule(tk1);

                                    }
                                    in = subByte (in , tk1);
                                    printf("P3*+++++++++++++++++\n");
                                  
                                    encrypt_Array[P][T] = in[0][0];
                                    printf("P4*+++++++++++++++++\n");

                                }
                            }
                        }
                    }
                }
            }
        }
    }

    printf("p5++++++++++++++++++++++++++\n");
    /*---------------------------打印加密表---------------------------*/
    string file = "table"+to_string(num)+".h";
    ofstream outfile;
    outfile.open(file);

    outfile<<"#include <stdint.h>"<<endl;
    outfile<<"#ifndef _TABLE_H"<<endl;
    outfile<<"#define _TABLE_H"<<endl;
    outfile<<endl; 
    string group_num = "Encoding"+to_string(num);

    outfile<<"uint8_t "<<group_num<<"["<<N1<<"]"<<"["<<N2<<"]"<<" = {";
    for (int i = 0; i < N1; i++)
    {
        outfile<<"{";
        for (int j = 0; j < N2; j++)
        {
            outfile<<encrypt_Array[i][j];
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

    printf("[OK] ==============> Initial_Encoding%d has been print.\n" , num);

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
