#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>

using namespace std;


string tobits(int num, int bit_num)
{
	string res="";
	
	for(int pos=0;pos<bit_num;pos++)
	{
		res=to_string(num%2)+res;
		num/=2;
	}
	
	return res;
}



int h[4] = { 3, 0, 1, 2};

int h_inv[4] = {1, 2, 3, 0};

int rot[4] = { 1, 2, 3, 0 };

int RK[2] = { 0, 2};

int sbox[16] = {0xc, 0x0, 0xf, 0xa,
				0x2, 0xb, 0x9, 0x5,
				0x8, 0x3, 0xd, 0x7,
				0x1, 0xe, 0x6, 0x4};

int LAT[16][16] = {0};

//test
string branch(string a,string b)
{
	string s = "";
	s = s+"BVXOR( "+a+" , "+b+" )";
	return s;
}



int main(int argc,char * argv[])
{
    //input canshu
	int head_flag;
	int tail_flag;
	int key_flag;
	string filename;
	printf("input %d parameter \n" , argc);

	if(argc!=4)
	{
		printf("parameter number error!!");
		exit(0);
	}
	else
    {
        key_flag = atoi(argv[1]);
        head_flag = atoi(argv[2]);
        tail_flag = atoi(argv[3]);
		filename = "twine"+to_string(atoi(argv[3]))+".cvc";
    }
    printf("key_flag=%d , head_flag=%d , tail_flag=%d\n fileno = %d",key_flag,head_flag,tail_flag,tail_flag);
	
	//program main
	ofstream outcvc;
    int x_ROUND = 3;
	int y_ROUND = 4;
	int ROUND = x_ROUND+y_ROUND;



	//gen lat 
	for(int a=0;a<16;a++)
	{
		for(int b=0;b<16;b++)
		{
			for(int i=0;i<16;i++)
			{
				int a_i = a&i , b_si = sbox[i]&b , t = 0;
				while (a_i)
				{
					t = t ^ (a_i%2);
					a_i /= 2;
				}
				while (b_si)
				{
					t = t ^ (b_si%2);
					b_si /= 2;
				}
				if(t == 0)
				{
					LAT[a][b]++;
				}	
    
			}
			LAT[a][b] -= 8;
		}
	}

    
	//gen CVC
	outcvc.open(filename);
//variable claim

	outcvc<<"LAT : ARRAY BITVECTOR(8) OF BITVECTOR(1);"<<endl;
	for(int input_lc=0x0;input_lc<16;input_lc++)
	{
		for(int output_lc=0x0;output_lc<16;output_lc++)
		{
			outcvc<<hex<<"ASSERT( LAT[0bin"<<tobits(input_lc,4)<<tobits(output_lc,4)<<"] = 0bin"<<((LAT[input_lc][output_lc] == 0)?0:1)<<" );"<<endl;
		}
	}
	outcvc<<dec;

	//state variable claim
    for(int round=0;round<=x_ROUND;round++)
    {
		
        for(int pos=0;pos<4;pos++)
        {
            outcvc<<"x_Fin_"<<round<<"_"<<pos<<" , x_Xout_"<<round<<"_"<<pos;
            if(pos == 15)
            {
                outcvc<<" : BITVECTOR(4);"<<endl;
            }
            else
            {
                outcvc<<" , ";
            }
        }
		for(int s=0;s<2;s++)
		{
			outcvc<<"x_Sout_"<<round<<"_"<<s*2<<" , x_Sin_"<<round<<"_"<<s*2;
		
			if(s<1)
			{
				outcvc<<" , ";
			}
			else
			{
				outcvc<<" : BITVECTOR(4);"<<endl;
			}
		}
    }
	
	//backward state variable claim
	for(int round = y_ROUND;round>=0;round--)
	{
        for(int pos=0;pos<4;pos++)
        {
            outcvc<<"y_Fin_"<<round<<"_"<<pos<<" , y_Xout_"<<round<<"_"<<pos;
            if(pos == 3)
            {
                outcvc<<" : BITVECTOR(4);"<<endl;
            }
            else
            {
                outcvc<<" , ";
            }
        }
		for(int s=0;s<2;s++)
		{
			outcvc<<"y_Sout_"<<round<<"_"<<s*2<<" , y_Sin_"<<round<<"_"<<s*2;
		
			if(s<1)
			{
				outcvc<<" , ";
			}
			else
			{
				outcvc<<" : BITVECTOR(4);"<<endl;
			}
		}
		
	}
	for(int round=0;round<ROUND;round++)
	{
		//key schedule variable claim
		
		for(int pos=0;pos<4;pos++)
		{

			outcvc<<"Kin_"<<round<<"_"<<pos<<" , Rotin_"<<round<<"_"<<pos<<" , Kout_"<<round<<"_"<<pos;
			if(pos<3)
			{
				outcvc<<" , ";
			}
			else
			{
				outcvc<<" : BITVECTOR(4);"<<endl; 
			}						
		}
		for(int pos=0;pos<2;pos++)
		{
			outcvc<<"RKin_"<<round<<"_"<<RK[pos];
			if(pos<1)
			{
				outcvc<<" , ";
			}
			else
			{
				outcvc<<" : BITVECTOR(4);"<<endl; 
			}						
			
		}
		//key schedule Sbox
		outcvc<<"KSin_"<<round<<"_0 , KSout_"<<round<<"_0 : BITVECTOR(4);"<<endl;
		
	}
//ASSERT 
	for(int round=0;round<x_ROUND;round++)
	{	
		//forward state update
		for(int pos=0;pos<4;pos++)
		{
			
			if(pos%2 == 0)
			{
				string a = "x_Fin_"+to_string(round)+"_"+to_string(pos);
				string b = "x_Sin_"+to_string(round)+"_"+to_string(pos);
				outcvc<<"ASSERT( x_Xout_"<<round<<"_"<<pos<<" = "<<branch(a,b)<<" );"<<endl;
				outcvc<<"ASSERT( NOT( LAT[x_Sin_"<<round<<"_"<<pos<<"@x_Sout_"<<round<<"_"<<pos<<"] = 0bin0 ) );"<<endl;
				outcvc<<"ASSERT( "<<"x_Sout_"<<round<<"_"<<pos<<" = x_Fin_"<<round<<"_"<<pos+1<<" );"<<endl;
			}
			else
			{
				outcvc<<"ASSERT( x_Xout_"<<round<<"_"<<pos<<" = x_Fin_"<<round<<"_"<<pos<<" );"<<endl;
				
			}
			
			outcvc<<"ASSERT( x_Fin_"<<(round+1)<<"_"<<(h[pos])<<" = x_Xout_"<<round<<"_"<<pos<<" );"<<endl;
			
			if( pos%2 == 0 )
			{
				outcvc<<"ASSERT( RKin_"<<round<<"_"<<(RK[pos/2])<<" = x_Sin_"<<round<<"_"<<pos<<" );"<<endl;			
			}		
			
		}	
		
	}

	for(int round=y_ROUND-1;round>=0;round--)
	{	
		//backward state update
		for(int pos=0;pos<4;pos++)
		{
			if(round<y_ROUND-1)
			{
				outcvc<<"ASSERT( y_Xout_"<<round<<"_"<<h_inv[pos]<<" = y_Fin_"<<round+1<<"_"<<pos<<" );"<<endl;
			}

			if(pos%2 == 0)
			{
				string a = "y_Xout_"+to_string(round)+"_"+to_string(pos);
				string b = "y_Sin_"+to_string(round)+"_"+to_string(pos);
				outcvc<<"ASSERT( y_Fin_"<<round<<"_"<<pos<<" = "<<branch(a,b)<<" );"<<endl;
				outcvc<<"ASSERT( NOT( LAT[y_Sout_"<<round<<"_"<<pos<<"@y_Sin_"<<round<<"_"<<pos<<"] = 0bin0 ) );"<<endl;
				outcvc<<"ASSERT( "<<"y_Sout_"<<round<<"_"<<pos<<" = y_Xout_"<<round<<"_"<<pos+1<<" );"<<endl;
			}
			else
			{
				outcvc<<"ASSERT( y_Fin_"<<round<<"_"<<pos<<" = y_Xout_"<<round<<"_"<<pos<<" );"<<endl;
			}
			if( pos%2 == 0 )
			{
				outcvc<<"ASSERT( RKin_"<<x_ROUND+round<<"_"<<RK[pos/2]<<" = y_Sin_"<<round<<"_"<<pos<<" );"<<endl;			
			}		
			
		}		
	}

	//key schedule

	for(int round=0;round<ROUND;round++)
	{
		int index1 = 0;
		for(int pos=0;pos<4;pos++)
		{	
			if( pos == RK[index1] )
			{					
				string a = "RKin_"+to_string(round)+"_"+to_string(pos); 
				string b = "Kin_"+to_string(round)+"_"+to_string(pos);
				outcvc<<"ASSERT( Kout_"<<round<<"_"<<pos<<" = "<<branch(a,b)<<" );"<<endl;
				index1++;
			}
			else
			{
				outcvc<<"ASSERT( Kout_"<<round<<"_"<<pos<<" = Kin_"<<round<<"_"<<pos<<" );"<<endl;
			}
			
			if(pos == 0)
			{
				string a = "Kout_"+to_string(round)+"_"+to_string(pos); 
				string b = "KSin_"+to_string(round)+"_"+to_string(pos);
				outcvc<<"ASSERT( Rotin_"<<round<<"_"<<pos<<" = "<<branch(a,b)<<" );"<<endl;
				outcvc<<"ASSERT( NOT( LAT[KSin_"<<round<<"_"<<pos<<"@KSout_"<<round<<"_"<<pos<<"] = 0bin0 ) );"<<endl;
				outcvc<<"ASSERT( "<<"KSout_"<<round<<"_"<<pos<<" = Rotin_"<<round<<"_"<<pos+1<<" );"<<endl;
			}
			else
			{
				outcvc<<"ASSERT( Rotin_"<<round<<"_"<<pos<<" = Kout_"<<round<<"_"<<pos<<" );"<<endl;
			}
			if(round < ROUND-1)
			{
				outcvc<<"ASSERT( Kin_"<<round+1<<"_"<<rot[pos]<<" = Rotin_"<<round<<"_"<<pos<<" );"<<endl;
			}
			
		}		
	}
	
	for(int pos=0;pos<4;pos++)
	{
		outcvc<<"ASSERT( x_Fin_"<<x_ROUND<<"_"<<pos<<" = y_Fin_0_"<<pos<<" );"<<endl;
	}
	


	//assert active state
	for(int pos=0;pos<4;pos++)
	{

		if( pos < 4 )
		{
			if(  pos == head_flag  )
			{
				outcvc<<"ASSERT( NOT( x_Fin_0_"<<pos<<" = 0bin0000 ) );"<<endl;
			}
			else
			{
				outcvc<<"ASSERT( x_Fin_0_"<<pos<<" = 0bin0000 );"<<endl;
			}
			if(pos == tail_flag)
			{
				outcvc<<"ASSERT( NOT( y_Xout_"<<y_ROUND-1<<"_"<<pos<<" = 0bin0000 ) );"<<endl;
			}
			else
			{
				outcvc<<"ASSERT( y_Xout_"<<y_ROUND-1<<"_"<<pos<<" = 0bin0000 );"<<endl;
			}		
			
		}
		/*
		if( pos < 4 )
		{
			if( pos == key_flag )
			{
				outcvc<<"ASSERT( Kin_0_"<<pos<<" = 0bin0000 );"<<endl;
			}
			else if(pos == 0)
			{
				outcvc<<"ASSERT( Kin_0_"<<pos<<" = 0bin0001 );"<<endl;
			}
			else if(pos == 1)
			{
				outcvc<<"ASSERT( Kin_0_"<<pos<<" = 0bin0110 );"<<endl;

			}
			else if(pos == 2)
			{
				outcvc<<"ASSERT( Kin_0_"<<pos<<" = 0bin1010 );"<<endl;

			}
			outcvc<<"ASSERT( Kout_"<<ROUND-1<<"_"<<pos<<" = 0bin0000 );"<<endl;
		}
		*/


	}





	
	
	outcvc<<"QUERY(FALSE);"<<endl;
	outcvc<<"COUNTEREXAMPLE;"<<endl;
	
	outcvc.close();
	
	
	







    return 0;
}
