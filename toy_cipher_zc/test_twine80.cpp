#include<stdio.h>
#include<iostream>
#include<vector>
#include<time.h>
#include<cmath>

using namespace std;

void encrypt (uint32_t* plaintext, uint32_t* ciphertext) {
	char S[] = { 0x9e37b8e9, 0xaf48c9fa, 0x8d26a7d8, … }; /* Sbox */
	ciphertext = S[plaintext];
}


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






// Test d i s t i n g u i s h e r f o r twe ak able BC with one tweakey l i n e

int testTK1( int num )
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
                alpha1[row][col] = 0x7;
            }
            else
            {
                alpha1[row][col] = 0x0;
            }

        }
    }
    vector<vector<int>> alpha2(2 , vector<int>(2, 0));
    for( int row = 0; row < 2 ; row++)
    {
        for( int col = 0; col < 2; col++)
        {
            if( (row*2+col) == 3 )
            {
                alpha2[row][col] = 0x7;
            }
            else if((row*2+col) == 0)
            {
                alpha2[row][col] = 0x6;
            }
            else if((row*2+col) == 1)
            {
                alpha2[row][col] = 0x0;
            }
            else if((row*2+col) == 2)
            {
                alpha2[row][col] = 0xc;
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
                beta[row][col] = 0x1;
            }
            else
            {
                beta[row][col] = 0x0;
            }

        }
    }


    int x_Rounds = 5;
    int y_Rounds = 5;
    int Round = x_Rounds+y_Rounds;
    printf("  Number of rounds : %d\n" , Round);

    unsigned long int N = pow(2 , (4*8));
    unsigned long int counter = 0;

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

                                    int XOR = 0;

                                    for( int i = 0; i < 2; i++)
                                    {
                                        for( int j =0; j < 2; j++)
                                        {
                                            XOR = XOR ^ ( K[i][j] & alpha2[i][j] );
                                        }
                                    }
                                    
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

                                    bool judge = get_xored(XOR);
                                    if( ! judge )
                                    {
                                        counter++;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }


    printf("N:%lu\n", N );
    printf("counter:%lu\n", counter);
    printf("result = %lu\n", N/counter);

    cout << endl;
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
