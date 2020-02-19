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

    int RK[8] = { 1,  3,  4,  6, 13, 14, 15, 16 };

    int index = 0;
    vector<vector<int>> out = in;
    for (int i = 0;i < 4; i++)
    {
        for( int j = 0; j < 4; j++)
        {
            if( (j % 2) == 1 )
            {
               out[i][j] = sbox[ in[i][j-1] ^ rk[ RK[index]/4 ][ RK[index]%4 ] ] ^ in[i][j];
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
    int h[20] = {5,  0,  1,  4,
                 7, 12,  3,  8,
                13,  6,  9,  2,
                15, 10, 11, 14};

    vector<vector<int>> out = in;

    for (int i = 0;i < 4; i++)
    {
        for( int j = 0; j < 4; j++)
        {
            out[ h[4*i+j] / 4 ][ h[4*i+j] % 4 ] = in[i][j];
        }
    }

    return out;
}

//key update
vector<vector<int>> keySchedule(vector<vector<int>> in , int round)
{
    vector<int> sbox = {0xc, 0x0, 0xf, 0xa,
				        0x2, 0xb, 0x9, 0x5,
				        0x8, 0x3, 0xd, 0x7,
				        0x1, 0xe, 0x6, 0x4};

    int CON[35] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
                    0x03, 0x06, 0x0c, 0x18, 0x30, 0x23,
                    0x05, 0x0a, 0x14, 0x28, 0x13, 0x26,
                    0x0f, 0x1e, 0x3c, 0x3b, 0x35, 0x29,
                    0x11, 0x22, 0x07, 0x0e, 0x1c, 0x38,
                    0x33, 0x25, 0x09, 0x12, 0x24 };

    int Rot[20] = {19, 16, 17, 18,  0,
                    1,  2,  3,  4,  5,
                    6,  7,  8,  9, 10,
                   11, 12, 13, 14, 15,};
    //permutation
    vector<vector<int>> rot(5, vector<int>(4, 0));
    vector<vector<int>> out(5, vector<int>(4, 0));
    for ( int i = 0; i < 20; i++)
    {
        if( i == 1 )
        {
            rot[0][1] = sbox[ in[0][0] ] ^ in[0][1];
        }
        else if ( i == 4 )
        {
            rot[1][0] = sbox[ in[4][0] ] ^ in[1][0];
        }
        else
        {
            rot[ i / 4 ][ i % 4 ] = in[ i / 4 ][ i % 4 ];
        }

        if( i == 7 )
        {
            rot[1][3] = rot[1][3] ^ ( 0xf0 & CON[round] );
        }
        else if ( i == 19)
        {
            rot[4][3] = rot[4][3] ^ ( 0x0f & CON[round] );
        }


    }
    for (int i = 0; i < 20; i++)
    {
        out[ Rot[i] / 4][ Rot[i] % 4 ] = rot[ i / 4 ][ i % 4 ];
    }

    return out;
}



// Test d i s t i n g u i s h e r f o r twe ak able BC with one tweakey l i n e
/*
int testTK1(void)
{
    //generate all keys at random
    srand( time(NULL));
    vector<vector<int>> key1(5 , vector<int>(4, 0));
    for( int row = 0; row < 5 ; row++)
    {
        for( int col = 0; col < 4; col++)
        {
            key1[row][col] = rand() & 0xF;
        }
    }

    int x_Rounds = 2;
    int y_Rounds = 8;
    int Round = x_Rounds+y_Rounds;
    printf("  Number of rounds : %d\n" , Round);

    vector<int> counter(16 , 0);
    for ( int i1 = 0; i1 < 16; i1++)
    {
        for ( int i2 = 0; i2 < 16; i2++)
        {
            vector<vector<int>> in(4, vector<int>(4, 0));
            vector<vector<int>> tk1 = key1;


            in[3][2] = i1;

            tk1[3][1] = i2;


            //encryption

            for (int r = 0; r < Round - 1 ; r++)
            {
                in = subByte (in , tk1);
                in = shiftNible(in);

                tk1 = keySchedule(tk1);

            }
            in = subByte (in , tk1);

            counter[in[2][0]]++;



        }
    }
    for (int i = 0; i < 16; i++)
    {
        printf("    %X : frequency of apprearance = %5d, mod 2 = %d \n", i , counter[i] , counter[i] % 2);
    }
    cout << endl;
}
*/

int test(int plaintext[] , int key_to_set[] )
{
    vector<vector<int>> key(5 , vector<int>(4, 0));
    vector<vector<int>> text(4 , vector<int>(4, 0));
    for( int row = 0; row < 5 ; row++)
    {
        for( int col = 0; col < 4; col++)
        {
            key[row][col] = key_to_set[ row*4 + col];
        }
    }
    for( int row = 0; row < 4 ; row++)
    {
        for( int col = 0; col < 4; col++)
        {
            text[row][col] = plaintext[ row*4 + col ];
        }
    }

    for( int round = 0; round < 36 ; round++)
    {
        text = subByte(text , key);
        text = shiftNible( text );
        if( round < 35)
        {
            key = keySchedule( key , round );
        }
    }

    for (int i = 0; i < 16; i++)
    {
        printf("  %X  " ,  text[ i / 4 ][ i % 4 ]);
    }
    cout << endl;

}


int main(void)
{
    printf("Experimental verification of distinguisher on TK1.\n");

    int plain[16] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7,
                     0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf};

    int key[20] = { 0x0, 0x0, 0x1, 0x1, 0x2, 0x2, 0x3, 0x3,
                    0x4, 0x4, 0x5, 0x5, 0x6, 0x6, 0x7, 0x7,
                    0x8, 0x8, 0x9, 0x9};

    test(plain , key);





    return 0;
}
