#include<stdio.h>
#include<iostream>
#include<vector>
#include<time.h>

using namespace std;


int pi[16] = { 6,  7,  8,  9,
			  10, 11, 12, 13,
			  14, 15,  1,  0,
			   4,  2,  3,  5};

int RK[6] = {14, 10, 8, 6, 2, 0};

vector<vector<int>> subByte(vector<vector<int>> in , vector<vector<int>> rk )
{
    vector<int> sbox = {0xc, 0x0, 0xf, 0xa,
				        0x2, 0xb, 0x9, 0x5,
				        0x8, 0x3, 0xd, 0x7,
				        0x1, 0xe, 0x6, 0x4};

    int RK[6] = {14, 10, 8, 6, 2, 0};

    int index = 5;

    vector<vector<int>> out = in;


    for (int i = 0;i < 4; i++)
    {
        for( int j = 0; j < 4; j++)
        {
            if( (i*4 + j) == RK[index] )
            {
               out[i][j+1] = sbox[ rk[ index/4 ][ index%4 ] ^ in[i][j] ] ^ in[i][j+1];
               index--;
            }
            else if( ( (i*4 + j) == 4 ) || ( (i*4 + j) == 12 ) )
            {
                out[i][j+1] = sbox[ in[i][j] ] ^ in[i][j+1];
            }
            else
            {
                out[i][j-1] = in[i][j-1];
            }

        }
    }
    return out;
}



//linear layer
vector<vector<int>> shiftNible(vector<vector<int>> in )
{
    int h[16] = {5,  0,  1,  4,
                 7, 12,  3,  8,
                13,  6,  9,  2,
                15, 10, 11, 14};

    vector<vector<int>> out = in;

    for (int i = 0;i < 16; i++)
    {
        out[ h[i] / 4 ][ h[i] % 4 ] = in[i/4][i%4];
    }

    return out;
}

//key update
vector<vector<int>> keySchedule(vector<vector<int>> in )
{

    int Rot[16] = {  6,  7,  8,  9,
                    10, 11, 12, 13,
                    14, 15,  1,  0,
                     4,  2,  3,  5};
    //permutation
    vector<vector<int>> out(4, vector<int>(4, 0));

    for (int i = 0; i < 16; i++)
    {
        out[ i / 4 ][ i % 4 ] = in[ Rot[i] / 4 ][ Rot[i] % 4 ];
    }

    return out;
}



// Test d i s t i n g u i s h e r f o r twe ak able BC with one tweakey l i n e

int testTK1(void)
{
    //generate all keys at random
    srand( time(NULL));
    vector<vector<int>> key(4 , vector<int>(4, 0));
    for( int row = 0; row < 4 ; row++)
    {
        for( int col = 0; col < 4; col++)
        {
            key[row][col] = rand() & 0xF;
        }
    }

    int x_Rounds = 3;
    int y_Rounds = 8;
    int Round = x_Rounds+y_Rounds;
    printf("  Number of rounds : %d\n" , Round);

    vector<int> counter(16 , 0);
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
                        vector<vector<int>> in(4, vector<int>(4, 0));
                        vector<vector<int>> tk1 = key;


                        in[0][2] = i1;
                        in[1][3] = i2;
                        in[2][0] = i3;
                        in[3][0] = i4;

                        tk1[1][2] = i5;


                        //encryption

                        for (int r = 0; r < Round - 1 ; r++)
                        {
                            in = subByte (in , tk1);
                            in = shiftNible(in);

                            tk1 = keySchedule(tk1);

                        }
                        in = subByte (in , tk1);

                        counter[in[0][2]]++;


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
    testTK1();


    return 0;
}
