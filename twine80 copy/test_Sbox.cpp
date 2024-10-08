#include<stdio.h>
#include<iostream>
#include<vector>
#include<time.h>
#include<cmath>

using namespace std;



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
    srand( time(NULL));

    vector<vector<int>> key1(2 , vector<int>(2, 0));
    for( int row = 0; row < 2 ; row++)
    {
        for( int col = 0; col < 2; col++)
        {
            key1[row][col] = rand() & 0xF;
        }
    }
    int sbox[16] = {0xc, 0x6, 0x9, 0x0,
                    0x1, 0xa, 0x2, 0xb,
                    0x3, 0x8, 0x5, 0xd,
                    0x4, 0xe, 0x7, 0xf};

    int x_Rounds = 4;
    int y_Rounds = 5;
    int Round = x_Rounds+y_Rounds;
    printf("  Number of rounds : %d\n" , Round);

    vector<int> counter(16 , 0);

    for ( int i1 = 0; i1 < 16; i1++)
    {

        for ( int i2 = 0; i2 < 16; i2++)
        {
            int x = i1;
            int y = i2;
            int z = y ^ sbox[x];
            counter[z] ++;
                
              
            
        }
    }

    for (int i = 0; i < 16; i++)
    {
        printf("    %X : frequency of apprearance = %5d, mod 2 = %d \n", i , counter[i] , counter[i] % 2);
    }

    cout << endl;
}


int main(void)
{
    printf("Experimental verification of distinguisher on TK1.\n");
    testTK1();


    return 0;
}
