#include<stdio.h>
#include<iostream>
#include<vector>
#include<time.h>

using namespace std;
/*
unsigned rol(unsigned val, int size)
{
  unsigned res = val << size;
  res |= val >> (4 - size);
  res &= 0x0000000f;
  return res;
}

//循环右移
unsigned ror(unsigned val, int size)
{
  unsigned res = val >> size;
  res |= val << (4 - size);
  res &= 0x0000000f;
  return res;
}


//original MC layer matrix

int sbox[16] = {0xc, 0x0, 0xf, 0xa,
				0x2, 0xb, 0x9, 0x5,
				0x8, 0x3, 0xd, 0x7,
				0x1, 0xe, 0x6, 0x4};

//original linear layer
int h[16] = {5,  0,  1,  4,
             7, 12,  3,  8,
            13,  6,  9,  2,
            15, 10, 11, 14};

int rot[20] = {19, 16, 17, 18,  0,
				1,  2,  3,  4,  5,
				6,  7,  8,  9, 10,
			   11, 12, 13, 14, 15,};

int RK[8] = { 1,  3,  4,  6, 13, 14, 15, 16};


int SR[16] = { 0,  1,  2,  3,
               5,  6,  7,  4,
              10, 11,  8,  9,
              15, 12, 13, 14};

*/

vector<vector<int>> subByte(vector<vector<int>> in , vector<vector<int>> rk )
{
    vector<int> sbox = {0xc, 0x0, 0xf, 0xa,
				        0x2, 0xb, 0x9, 0x5,
				        0x8, 0x3, 0xd, 0x7,
				        0x1, 0xe, 0x6, 0x4};

    int RK[8] = { 1,  3};

    int index = 0;
    vector<vector<int>> out = in;
    for (int i = 0;i < 2; i++)
    {
        for( int j = 0; j < 2; j++)
        {
            if( (j % 2) == 1 )
            {
               out[i][j] = in[i][j-1] ^ rk[ RK[index]/2 ][ RK[index]%2 ] ^ in[i][j];
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

    int Rot[5] = {4, 0, 1, 2, 3};
    //permutation
    vector<vector<int>> rot(3, vector<int>(2, 0));
    vector<vector<int>> out(3, vector<int>(2, 0));
    for ( int i = 0; i < 5; i++)
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
    for (int i = 0; i < 5; i++)
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

    vector<vector<int>> alpha1(2 , vector<int>(2, 0));
    for( int row = 0; row < 2 ; row++)
    {
        for( int col = 0; col < 2; col++)
        {
            if( (row*2+col) == 2 )
            {
                alpha1[row][col] = rand() & 0xF;
            }
            else
            {
                alpha1[row][col] = 0x0;
            }   
            
        }
    }
    vector<vector<int>> alpha2(3 , vector<int>(2, 0));
    for( int row = 0; row < 3 ; row++)
    {
        for( int col = 0; col < 2; col++)
        {
            if( (row*2+col) == 3 )
            {
                alpha2[row][col] = 0x0;
            }
            else
            {
                alpha2[row][col] = rand() & 0xF;
            }   
            
        }
    }
    vector<vector<int>> beta(2 , vector<int>(2, 0));
    for( int row = 0; row < 2 ; row++)
    {
        for( int col = 0; col < 2; col++)
        {
            if( (row*2+col) == 0 )
            {
                beta[row][col] = rand() & 0xF;
            }
            else
            {
                beta[row][col] = 0x0;
            }   
            
        }
    }


    int x_Rounds = 5;
    int y_Rounds = 4;
    int Round = x_Rounds+y_Rounds;
    printf("  Number of rounds : %d\n" , Round);

    vector<int> counter(16 , 0);
    int XOR = 0;
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
                                    for ( int i9 = 0; i9 < 16; i9++)
                                    {
                                        vector<vector<int>> in(2, vector<int>(2, 0));
                                        vector<vector<int>> tk1(3, vector<int>(2, 0));

                                        in[0][0] = i1;
                                        in[0][1] = i2;
                                        in[1][0] = i3;
                                        in[1][1] = i4;
                                        tk1[0][0] = i5;
                                        tk1[0][1] = i6;
                                        tk1[1][0] = i7;
                                        tk1[1][1] = i8;
                                        tk1[2][0] = i9;
                                        
                                        vector<vector<int>> K = tk1;
                                        vector<vector<int>> P = in;

                                        //encryption
                                        for (int r = 0; r < Round - 1 ; r++)
                                        {
                                            in = subByte (in , tk1);
                                            in = shiftNible(in);

                                            tk1 = keySchedule(tk1);

                                        }
                                        in = subByte (in , tk1);

                                        XOR = ( K[0][0] & alpha2[0][0] ) ^ ( K[0][1] & alpha2[0][1] ) ^ ( K[1][0] & alpha2[1][0] ) ^ ( K[2][0] & alpha2[2][0] );
                                        for( int i = 0; i < 2; i++)
                                        {
                                            for( int j =0; j < 2; j++)
                                            {
                                                XOR = XOR ^ ( P[i][j] & alpha1[i][j] ); 
                                            }
                                        }
                                        for( int i = 0; i < 2; i++)
                                        {
                                            for( int j =0; j < 2; j++)
                                            {
                                                XOR = XOR ^ ( in[i][j] & beta[i][j] ); 
                                            }
                                        }                                                 

                                    }
                                }
                            }
                        }
                    }
                }
            }             
        }
    }

    for (int i = 0; i < 16; i++)
    {
        printf("    %X : XOR's value", XOR );
    }
    cout << endl;
}


int main(void)
{
    printf("Experimental verification of distinguisher on TK1.\n");
    testTK1();


    return 0;
}
