#include<stdio.h>
#include<iostream>
#include<vector>
#include<time.h>
#include<cmath>
#include <stdint.h>
#include <stdlib.h>

using namespace std;

const unsigned long int size = pow(2 , (4*8));

int N = pow(2 , (4*4));

static uint8_t encrypt_Array[size] = {0};





unsigned rol(unsigned val, int size)
{
  unsigned res = val << size;

  return res;
}



static void Print_Array_u8(FILE* table, \
                            char * Arrayname, \
                            int row, \
                            int column, \
                            uint8_t *Array ){
    int i, j;
    
	fprintf(table,"uint8_t %s[%d][%d] = {", Arrayname, row, column);
	for (i = 0; i < row; i++)
	{
		fprintf(table,"{");
        for (j = 0; j < column; j++)
		{
			fprintf(table, "%d" , Array[i * row + j]);
			if (j < column - 1)
			{
				fprintf(table, ",");
			}
		}
		fprintf(table, "}");
        
		if (i < row - 1)
		{
			fprintf(table,",");
		}
	}
	fprintf(table,"};\n");
}


unsigned long int nible_to_int(vector<vector<int>> in , vector<vector<int>> tk1 , int row , int col )
{
    int res = 0;
    for( int i = 0; i < row; i++ )
    {
        for (int j = 0; j < col; j++)
        {
            res = res ^ rol( in[row][col] , 4 * (i * row + j) );
        }

    }

    for( int i = 0; i < row; i++ )
    {
        for (int j = 0; j < col; j++)
        {
            res = res ^ rol( tk1[row][col] , 4 * (i * row + j) + 12 );
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

int testTK1(void)
{
    //generate all keys at random

    int x_Rounds = 3;
    int y_Rounds = 5;
    int Round = x_Rounds+y_Rounds;
    printf("  Number of rounds : %d\n" , Round);



    for ( int i1 = 0; i1 < 16; i1++)
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

                                    unsigned long int P = nible_to_int(in , tk1 , 2 , 2);

                                    //encryption
                                    for (int r = 0; r < Round - 1 ; r++)
                                    {
                                        in = subByte (in , tk1);
                                        in = shiftNible(in);

                                        tk1 = keySchedule(tk1);

                                    }
                                    in = subByte (in , tk1);

                                    encrypt_Array[P] = in[0][0];
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    FILE *tableFile;

    /*---------------------------打印加密表---------------------------*/
	tableFile = fopen("table.h", "w+");

	if(tableFile != NULL)
	{
        fprintf(tableFile,"#include <stdint.h>\n");
        fprintf(tableFile,"#ifndef _TABLE_H\n");
        fprintf(tableFile,"#define _TABLE_H\n");
        fprintf(tableFile,"\n");
        
        Print_Array_u8( tableFile, "Encoding", N, N, encrypt_Array );
        printf("[OK] ==============> Initial_Encoding has been print.\n");

        fprintf(tableFile,"#endif\n");
		fclose(tableFile);
	}

}


int main(void)
{
    printf("Experimental encrypt table on toy_cipher.\n");
    testTK1();

    return 0;
}
