#include<stdio.h>
#include<iostream>
#include<vector>
#include<time.h>

using namespace std;

unsigned rol(unsigned val, int size)
{
  unsigned res = val << size;
  res |= val >> (4 - size);
  res &= 0x0000000f; 
  return res;
}

/*循环右移*/
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


int SR[16] = 	 { 0,  1,  2,  3,
            	   5,  6,  7,  4,
            	  10, 11,  8,  9,
             	  15, 12, 13, 14};

int P[16] = {9, 15, 8, 13, 10, 14, 12, 11, 0, 1, 2, 3, 4, 5, 6, 7};


vector<vector<int>> subByte(vector<vector<int>> in )
{
    vector<int> sbox = {0xc, 0x6, 0x9, 0x0,
				        0x1, 0xa, 0x2, 0xb,
				        0x3, 0x8, 0x5, 0xd,
				        0x4, 0xe, 0x7, 0xf};
    vector<vector<int>> out = in;
    for (int i = 0;i < 4; i++)
    {
        for( int j = 0; j < 4; j++)
        {
            out[i][j] = sbox[ in[i][j] ];
        }
    }
    return out;
}
/*
vector<vector<int>> addConstants(vector<vector<int>> in , int round )
{    
    vector<int> Consts = {0x01,0x03,0x07,0x0F,0x1F,0x3E,0x3D,0x3B,0x37,0x2F,0x1E,0x3C,0x39,0x33,0x27,0x0E};
    vector<vector<int>> out = in;
    int c0 = 0x0f & Consts[round];
    int c1 = 0xf0 & Consts[round];
    int c2 = 0x2;
    out[0][0] = in[0][0] ^ c0;
    out[1][0] = in[1][0] ^ c1;
    out[2][0] = in[2][0] ^ c2;        

    return out;
}
*/



vector<vector<int>> shiftRow(vector<vector<int>> in , vector<vector<int>> rk1, vector<vector<int>> rk2)
{
    vector<vector<int>> add = in;   
    vector<vector<int>> out = in;


    for (int i = 0;i < 4; i++)
    {
        for( int j = 0; j < 4; j++)
        {
            if( i < 2 )
            {
                add[i][j] = in[i][j] ^ rk1[i][j] ^ rk2[i][j];
            }
            else
            {
                add[i][j] = in[i][j];
            }            
        } 
    }

    for (int i = 0;i < 4; i++)
    {
        for( int j = 0; j < 4; j++)
        {
            out[i][(i+j)%4] = add[i][j];
        } 
    }
    return out;     
}


// MixColumns
vector<vector<int>> mixColumn(vector<vector<int>> in )
{
    vector<vector<int>> out = in;
    for (int j = 0;j < 4; j++)
    {
        out[0][j] = in[0][j] ^ in[2][j] ^ in[3][j];
        out[1][j] = in[0][j];
        out[2][j] = in[1][j] ^ in[2][j];
        out[3][j] = in[0][j] ^ in[2][j];
    }
    return out;      
}

//Tweakey update
vector<vector<int>> keySchedule1(vector<vector<int>> in )
{
    //permutation
    vector<int> ks = {9, 15, 8, 13, 10, 14, 12, 11, 0, 1, 2, 3, 4, 5, 6, 7};
    vector<vector<int>> out(4, vector<int>(4, 0));
    for ( int i = 0; i < 16; i++)
    {
        out[i / 4][ ( i % 4 )] = in[ks[i] / 4 ][ ks[i] % 4];        
    }
    return out;
}
//Tweakey update
vector<vector<int>> keySchedule2(vector<vector<int>> in )
{
    //permutation
    vector<int> ks = {9, 15, 8, 13, 10, 14, 12, 11, 0, 1, 2, 3, 4, 5, 6, 7};
    vector<vector<int>> out(4, vector<int>(4, 0));
    vector<vector<int>> lfsr(4, vector<int>(4, 0));

    for ( int i = 0; i < 16; i++)
    {
        lfsr[i / 4][ ( i % 4 )] = in[ks[i] / 4 ][ ks[i] % 4];        
    }
    
    for ( int i = 0; i < 4; i++)
    {
        for ( int j = 0; j < 4; j++)
        {
            if ( i < 2 )
            {
                out[i][j] = rol(lfsr[i][j] , 1) ^ ( ror( ( lfsr[i][j] & 0x00000004 ) , 2 ) );
            }
            else
            {
                out[i][j] = lfsr[i][j];
            }            
        }
    }
    return out;
}


// Test d i s t i n g u i s h e r f o r twe ak able BC with one tweakey l i n e

int testTK2(void)
{
    //generate all keys at random
    srand( time(NULL));
    vector<vector<int>> key1(4 , vector<int>(4, 0));
    vector<vector<int>> key2(4 , vector<int>(4, 0));
    for( int row = 0; row < 4 ; row++)
    {
        for( int col = 0; col < 4; col++)
        {
            key1[row][col] = rand() & 0xF;
            key2[row][col] = rand() & 0xF;
        }
    }

    int x_Rounds = 2;
    int y_Rounds = 7;
    printf("  Number of rounds : %d\n" , x_Rounds+y_Rounds);

    vector<int> counter(16 , 0);
    for ( int i1 = 0; i1 < 16; i1++)
    {
        for ( int i2 = 0; i2 < 16; i2++)
        {
            for ( int i3 = 0; i3 < 16; i3++)
            {
                for ( int i4 = 0; i4 < 16; i4++)
                {
                    for (int i5 = 0; i5 < 16; i5++)
                    {
                        for (int i6 = 0; i6 < 16; i6++)
                        {

                            vector<vector<int>> in(4, vector<int>(4, 0));
                            vector<vector<int>> tk1 = key1;
                            vector<vector<int>> tk2 = key2;
                            
                            in[0][1] = i1;
                            in[0][3] = i2;
                            in[2][0] = i3;
                            in[2][3] = i4;
                
                            tk1[3][0] = i5;
                            tk2[3][0] = i6;


                            //encryption
                            
                            for (int r = 0; r < (x_Rounds); r++)
                            {
                                in = subByte (in);
                                in = shiftRow(in, tk1, tk2);
                                
                                tk1 = keySchedule1(tk1);
                                tk2 = keySchedule2(tk2);    
                                                                
                                in = mixColumn(in);                                                                                   
                            }

                            for (int r = 0;r < (y_Rounds-1); r++ )
                            {
                                in = subByte (in);
                                in = shiftRow(in, tk1, tk2);
                                
                                tk1 = keySchedule1(tk1);
                                tk2 = keySchedule2(tk2);    
                                                                
                                in = mixColumn(in);     
                            }
                            in = subByte (in);
                            in = shiftRow(in, tk1, tk2);


                            counter[in[2][3]]++;
                                
                                
                            
                        }
                    }

                }
            }
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
    testTK2();


    return 0;
}
