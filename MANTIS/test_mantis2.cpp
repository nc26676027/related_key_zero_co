#include<stdio.h>
#include<iostream>
#include<vector>
#include<time.h>

using namespace std;




//original MC layer matrix
int MCT[4][4]=
{
	0,1,1,1,
	1,0,1,1,
	1,1,0,1,
	1,1,1,0
};
int SC[16] = {0, 11,  6, 13,
			 10,  1, 12,  7,
			  5, 14,  3,  8,
			 15,  4,  9,  2};

int P[16] = {6, 5, 14, 15, 0, 1, 2, 3, 7, 12, 13, 4, 8, 9, 10, 11};

int P_R[16] = {6, 5, 14, 15, 0, 1, 2, 3, 7, 12, 13, 4, 8, 9, 10, 11};

int W[7] = {0, 1, 3, 4, 8, 11, 13};








vector<vector<int>> subByte(vector<vector<int>> in )
{
    vector<int> sbox = {0xc, 0xa, 0xd, 0x3,
				        0xe, 0xb, 0xf, 0x7,
				        0x8, 0x9, 0x1, 0x5,
				        0x0, 0x2, 0x6, 0x4 };
    vector<vector<int>> out = in;
    for (int i = 0;i < 4; i++)
    {
        for( int j = 0; j < 4; j++)
        {
            out[i][j] = sbox[in[i][j] ];
        }
    }
    return out;
}
vector<vector<int>> subByte(vector<vector<int>> in , vector<vector<int>> rk )
{
    vector<int> sbox = {0xc, 0xa, 0xd, 0x3,
				        0xe, 0xb, 0xf, 0x7,
				        0x8, 0x9, 0x1, 0x5,
				        0x0, 0x2, 0x6, 0x4 };
    vector<vector<int>> out = in;
    for (int i = 0;i < 4; i++)
    {
        for( int j = 0; j < 4; j++)
        {
            out[i][j] = sbox[in[i][j] ^ rk[i][j]];
        }
    }
    return out;
}




vector<vector<int>> pCells(vector<vector<int>> in )
{
    vector<int> p = {0,  5, 15, 10,
			        13,  8,  2,  7,
			        11, 14,  4,  1,
			         6,  3,  9, 12};
    vector<vector<int>> out(4, vector<int>(4, 0));
    for ( int i = 0; i < 16; i++)
    {
        out[i / 4][ ( i % 4 )] = in[p[i] / 4 ][ p[i] % 4];        
    }
    return out;   
}

vector<vector<int>> pCells(vector<vector<int>> in , vector<vector<int>> rk)
{
    vector<int> p = {0, 11,  6, 13,
			        10,  1, 12,  7,
			         5, 14,  3,  8,
			        15,  4,  9,  2 };
    vector<vector<int>> xor_out = in;
    for (int i = 0;i < 4; i++)
    {
        for( int j = 0; j < 4; j++)
        {
            xor_out[i][j] = in[i][j] ^ rk[i][j];
        }
    }
    vector<vector<int>> out(4, vector<int>(4, 0));
    for ( int i = 0; i < 16; i++)
    {
        out[i / 4][ ( i % 4 )] = xor_out[p[i] / 4 ][ p[i] % 4];        
    }
    return out;       
}

// MixColumns
vector<vector<int>> mixColumn(vector<vector<int>> in )
{
    vector<vector<int>> out = in;
    for (int j = 0;j < 4; j++)
    {
        out[0][j] = in[1][j] ^ in[2][j] ^ in[3][j];
        out[1][j] = in[0][j] ^ in[2][j] ^ in[3][j];
        out[2][j] = in[0][j] ^ in[1][j] ^ in[3][j];
        out[3][j] = in[0][j] ^ in[1][j] ^ in[2][j];
    }
    return out;      
}

//Tweakey update
vector<vector<int>> keySchedule(vector<vector<int>> in )
{
    //permutation
    vector<int> ks = { 6, 5, 14, 15, 0, 1, 2, 3, 7, 12, 13, 4, 8, 9, 10, 11 };
    vector<vector<int>> out(4, vector<int>(4, 0));
    for ( int i = 0; i < 16; i++)
    {
        out[i / 4][ ( i % 4 )] = in[ks[i] / 4 ][ ks[i] % 4];        
    }
    return out;
}
//Tweakey update
vector<vector<int>> inv_keySchedule(vector<vector<int>> in )
{
    //permutation
    vector<int> ks = { 4, 5, 6, 7, 11, 1, 0, 8, 12, 13, 14, 15, 9, 10, 2, 3 };
    vector<vector<int>> out(4, vector<int>(4, 0));
    for ( int i = 0; i < 16; i++)
    {
        out[i / 4][ ( i % 4 )] = in[ks[i] / 4 ][ ks[i] % 4];        
    }
    return out;
}


// Test d i s t i n g u i s h e r f o r twe ak able BC with one tweakey l i n e

int testTK1()
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

    int x_Rounds = 2;
    int y_Rounds = 5;
    printf("  Number of rounds : %d\n" , x_Rounds+y_Rounds);

    vector<int> counter(16 , 0);
    for ( int i1 = 0; i1 < 16; i1++)
    {
        for ( int i2 = 0; i2 < 16; i2++)
        {

                                    vector<vector<int>> in(4, vector<int>(4, 0));
                                    vector<vector<int>> tk = key;
                                    in[2][2] = i1;

                        
                                    tk[3][1] = i2;

                                    //encryption
                                    in = subByte (in);
                                    for (int r = 0; r < (x_Rounds -1); r++)
                                    {
                                        tk = keySchedule(tk);
                                        in = pCells(in, tk);
                                        in = mixColumn(in);
                                        in = subByte(in);                                            
                                    }
                                    in = mixColumn(in);
                                    in = subByte (in);
                                    for (int r = 0;r < (y_Rounds-1); r++ )
                                    {
                                        in = mixColumn(in);
                                        in = pCells(in);
                                        in = subByte(in, tk);
                                        tk = inv_keySchedule(tk);
                                    }
                                    counter[in[2][2]]++;
                                

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
