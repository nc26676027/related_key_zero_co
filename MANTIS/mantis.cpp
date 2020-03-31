#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>

using namespace std;



int sbox[16] = {0xc, 0xa, 0xd, 0x3,
				0xe, 0xb, 0xf, 0x7,
				0x8, 0x9, 0x1, 0x5,
				0x0, 0x2, 0x6, 0x4};

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
void P_make(int round){

	int tem[16];
	for (int i=0 ;i<16;i++){
		tem[i]=P_R[i];
	}
	for (int i=0 ; i<round-2; i++){
        for (int j=0;j<16;j++){
            tem[j]=P_R[P[j]];
        }

		for (int j=0;j<16;j++){
			P_R[j]=tem[j];
		}			
	}	
}

int return_index(int pos , int matrix[])
{
	int index;
	for(int i=0;i<16;i++)
	{
		if(pos == matrix[i])
		{
			index = i;
		}
	}
	return index;
}

string left_shift(string x , int left)
{
	string y = "";
	y = "( BVXOR( ("+x+"<<"+to_string(left)+")[3:0] , ("+x+">>"+to_string(left)+") ) )";
	return y;
}

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
		filename = "mantis"+to_string(atoi(argv[3]))+".cvc";
    }
    printf("key_flag=%d , head_flag=%d , tail_flag=%d\n fileno = %d",key_flag,head_flag,tail_flag,tail_flag);

    //peogram main
    ofstream outcvc;
    int x_ROUND = 4;
    int y_ROUND = 5;
    int ROUND = x_ROUND+y_ROUND;

	//P_make(3);
	
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
    for(int round=0;round<x_ROUND;round++)//up
    {
        for(int pos=0;pos<16;pos++)
        {
            
            outcvc<<"x_Fin_"<<round<<"_"<<pos<<" , x_SCout_"<<round<<"_"<<pos<<" , x_MCout_"<<round<<"_"<<pos<<" , x_Sout_"<<round<<"_"<<pos;
            if(pos < 15)
            {
                outcvc<<" , ";
            }
            else
            {
                outcvc<<" : BITVECTOR(4);"<<endl;
            }
            

            
        }
    }
	//down
	for(int round=y_ROUND-1;round>=0;round--)
	{
		for(int pos=0;pos<16;pos++)
		{
			outcvc<<"y_Fin_"<<round<<"_"<<pos<<" , y_SCout_"<<round<<"_"<<pos<<" , y_MCout_"<<round<<"_"<<pos<<" , y_Sout_"<<round<<"_"<<pos;
			if(pos<15)
			{
				outcvc<<" , ";
			}
			else
			{
				outcvc<<" : BITVECTOR(4);"<<endl;
			}
			
		}
	}
	//key 
	for(int round=0;round<x_ROUND;round++)
	{
		for(int pos=0;pos<16;pos++)
		{
		
			outcvc<<"x_Kin_"<<round<<"_"<<pos<<" , x_LPout_"<<round<<"_"<<pos<<" , x_RKin_"<<round<<"_"<<pos;
			if(pos<15)
			{
				outcvc<<" , ";
			}
			else
			{
				outcvc<<" : BITVECTOR(4);"<<endl;
			}			
	
		}

	}
	for(int round=0;round<y_ROUND;round++)
	{
		for(int pos=0;pos<16;pos++)
		{
		
			outcvc<<"y_Kin_"<<round<<"_"<<pos<<" , y_LPin_"<<round<<"_"<<pos<<" , y_RKin_"<<round<<"_"<<pos;
			if(pos<15)
			{
				outcvc<<" , ";
			}
			else
			{
				outcvc<<" : BITVECTOR(4);"<<endl;
			}			
	
		}

	}
	

	/*     *************************** ASSERT   ******************************  */
	//state update
	for(int round=0;round<x_ROUND-1;round++)//up
	{			
		//subCells
		for(int pos=0;pos<16;pos++)
		{
			outcvc<<"ASSERT( NOT( LAT[x_Fin_"<<round<<"_"<<pos<<"@x_Sout_"<<round<<"_"<<pos<<"] = 0bin0 ) );"<<endl;
		}	
		//add round tweak
		for(int pos=0;pos<16;pos++)
		{
			outcvc<<"ASSERT( x_Sout_"<<round<<"_"<<pos<<" = x_RKin_"<<round<<"_"<<pos<<" );"<<endl;
		}
		//shuffleCells
		for(int pos=0;pos<16;pos++)
		{
			outcvc<<"ASSERT( x_SCout_"<<round<<"_"<<pos<<" = x_Sout_"<<round<<"_"<<SC[pos]<<" );"<<endl;
		}
		//mix Col
	 	for(int col=0;col<4;col++)
		{
			 string a = "x_MCout_"+to_string(round)+"_"+to_string(col);
			 string b = "x_MCout_"+to_string(round)+"_"+to_string(col+4);
			 string c = "x_MCout_"+to_string(round)+"_"+to_string(col+8);
			 string d = "x_MCout_"+to_string(round)+"_"+to_string(col+12);

			 outcvc<<"ASSERT( x_SCout_"<<round<<"_"<<col<<" = BVXOR( "<<b<<" , BVXOR( "<<c<<" , "<<d<<" ) ) );"<<endl;
			 outcvc<<"ASSERT( x_SCout_"<<round<<"_"<<col+4<<" = BVXOR( "<<a<<" , BVXOR( "<<c<<" , "<<d<<" ) ) );"<<endl;
			 outcvc<<"ASSERT( x_SCout_"<<round<<"_"<<col+8<<" = BVXOR( "<<a<<" , BVXOR( "<<b<<" , "<<d<<" ) ) );"<<endl;
			 outcvc<<"ASSERT( x_SCout_"<<round<<"_"<<col+12<<" = BVXOR( "<<a<<" , BVXOR( "<<b<<" , "<<c<<" ) ) );"<<endl;			 			 
		}
		// next round
		if(round<x_ROUND-2)
		{
			for(int pos=0;pos<16;pos++)
			{
				outcvc<<"ASSERT( x_Fin_"<<round+1<<"_"<<pos<<" = x_MCout_"<<round<<"_"<<pos<<" );"<<endl;
			}	
		}	

	}
	//down
	for(int round=0;round<y_ROUND-1;round++)//down
	{	
		//mix Col
	 	for(int col=0;col<4;col++)
		{
			 string a = "y_Fin_"+to_string(round)+"_"+to_string(col);
			 string b = "y_Fin_"+to_string(round)+"_"+to_string(col+4);
			 string c = "y_Fin_"+to_string(round)+"_"+to_string(col+8);
			 string d = "y_Fin_"+to_string(round)+"_"+to_string(col+12);

			 outcvc<<"ASSERT( y_MCout_"<<round<<"_"<<col<<" = BVXOR( "<<b<<" , BVXOR( "<<c<<" , "<<d<<" ) ) );"<<endl;
			 outcvc<<"ASSERT( y_MCout_"<<round<<"_"<<col+4<<" = BVXOR( "<<a<<" , BVXOR( "<<c<<" , "<<d<<" ) ) );"<<endl;
			 outcvc<<"ASSERT( y_MCout_"<<round<<"_"<<col+8<<" = BVXOR( "<<a<<" , BVXOR( "<<b<<" , "<<d<<" ) ) );"<<endl;
			 outcvc<<"ASSERT( y_MCout_"<<round<<"_"<<col+12<<" = BVXOR( "<<a<<" , BVXOR( "<<b<<" , "<<c<<" ) ) );"<<endl;			 			 
		}		
		//shuffleCells
		for(int pos=0;pos<16;pos++)
		{
			outcvc<<"ASSERT( y_MCout_"<<round<<"_"<<pos<<" = y_SCout_"<<round<<"_"<<SC[pos]<<" );"<<endl;
		}		
		//add Round Tweak
		for(int pos=0;pos<16;pos++)
		{
			outcvc<<"ASSERT( y_SCout_"<<round<<"_"<<pos<<" = y_RKin_"<<round<<"_"<<pos<<" );"<<endl;
		}				
		//subCells
		for(int pos=0;pos<16;pos++)
		{
			outcvc<<"ASSERT( NOT( LAT[y_Sout_"<<round<<"_"<<pos<<"@y_SCout_"<<round<<"_"<<pos<<"] = 0bin0 ) );"<<endl;
		}
		//next round	
		if(round<y_ROUND-2)
		{
			for(int pos=0;pos<16;pos++)
			{
				outcvc<<"ASSERT( y_Fin_"<<round+1<<"_"<<pos<<" = y_Sout_"<<round<<"_"<<pos<<" );"<<endl;
			}	
		}

	}
//***************************************************************************************
	//connection up and down
	//variavle cleim
	for(int pos=0;pos<16;pos++)
	{
		outcvc<<"Sout_"<<pos<<" , MCout_"<<pos<<" , inv_Sout_"<<pos;
		if(pos<15)
		{
			outcvc<<" , ";
		}
		else
		{
			outcvc<<" : BITVECTOR(4);"<<endl;
		}	
	}
	//ASSERT center
	if(1)
	{
		for(int pos=0;pos<16;pos++)
		{
			outcvc<<"ASSERT( NOT( LAT[x_MCout_"<<x_ROUND-2<<"_"<<pos<<"@Sout_"<<pos<<"] = 0bin0 ) );"<<endl;
		}
		for(int col=0;col<4;col++)
		{
				string a = "MCout_"+to_string(col);
				string b = "MCout_"+to_string(col+4);
				string c = "MCout_"+to_string(col+8);
				string d = "MCout_"+to_string(col+12);

				outcvc<<"ASSERT( Sout_"<<col<<" = BVXOR( "<<b<<" , BVXOR( "<<c<<" , "<<d<<" ) ) );"<<endl;
				outcvc<<"ASSERT( Sout_"<<col+4<<" = BVXOR( "<<a<<" , BVXOR( "<<c<<" , "<<d<<" ) ) );"<<endl;
				outcvc<<"ASSERT( Sout_"<<col+8<<" = BVXOR( "<<a<<" , BVXOR( "<<b<<" , "<<d<<" ) ) );"<<endl;
				outcvc<<"ASSERT( Sout_"<<col+12<<" = BVXOR( "<<a<<" , BVXOR( "<<b<<" , "<<c<<" ) ) );"<<endl;			 			 
		}
		for(int pos=0;pos<16;pos++)
		{
			outcvc<<"ASSERT( NOT( LAT[MCout_"<<pos<<"@inv_Sout_"<<pos<<"] = 0bin0 ) );"<<endl;
			outcvc<<"ASSERT( y_Fin_0_"<<pos<<" = inv_Sout_"<<pos<<" );"<<endl;					
		}
		//key connection
		//tk1
		for(int pos=0;pos<16;pos++)
		{
			string a = "x_LPout_"+to_string(x_ROUND-2)+"_"+to_string(pos);
			string b = "x_RKin_"+to_string(x_ROUND-2)+"_"+to_string(pos);
			outcvc<<"ASSERT( y_Kin_0_"<<pos<<" = "<<branch(a,b)<<" );"<<endl;
		}
	}

//***************************************************************************************
	//key update ASSERT
	for(int round=0;round<x_ROUND-1;round++)//up
	{
		//TK1
		for(int pos=0;pos<16;pos++)
		{		
			//diffusion layer	
			outcvc<<"ASSERT( x_LPout_"<<round<<"_"<<pos<<" = x_Kin_"<<round<<"_"<<P[pos]<<" );"<<endl;
			//branch round tweak
			if(round<x_ROUND-2)
			{
				string a = "x_LPout_"+to_string(round)+"_"+to_string(pos);
				string b = "x_RKin_"+to_string(round)+"_"+to_string(pos);
				outcvc<<"ASSERT( x_Kin_"<<round+1<<"_"<<pos<<" = "<<branch(a,b)<<" );"<<endl;
			}
		}		
	}
	for(int round=0;round<y_ROUND-1;round++)//down
	{
		//TK1
		for(int pos=0;pos<16;pos++)
		{
			//branch round tweak 		
			string a = "y_Kin_"+to_string(round)+"_"+to_string(pos);
			string b = "y_RKin_"+to_string(round)+"_"+to_string(pos);
			outcvc<<"ASSERT( y_LPin_"<<round<<"_"<<pos<<" = "<<branch(a,b)<<" );"<<endl;
			//difffusion layer and next round
			if(round<y_ROUND-2)
			{
				outcvc<<"ASSERT( y_LPin_"<<round<<"_"<<pos<<" = y_Kin_"<<round+1<<"_"<<P[pos]<<" );"<<endl;
			}
			
		}	
	
	}
	//last round key
	if(1)
	{
		//tk1
		for(int pos=0;pos<16;pos++)
		{
			string a = "y_Kin_"+to_string(y_ROUND-2)+"_"+to_string(pos);
			string b = "y_RKin_"+to_string(y_ROUND-2)+"_"+to_string(pos);
			outcvc<<"ASSERT( y_LPin_"<<y_ROUND-2<<"_"<<pos<<" = "<<branch(a,b)<<" );"<<endl;
		}

	}

//***************************************************************************************




	int index = 0; 
	int head[6] = {2, 7, 8, 12, 13, 15};
	//int head[1] = { 10 };
	//assert active state
	for(int pos=0;pos<16;pos++)
	{
		//if( (pos == head[0])||(pos == head[1])||(pos == head[2])||(pos == head[3])||(pos == head[4])||(pos == head[5]) )
		if( pos != head[index] )
		{
			outcvc<<"ASSERT( x_Fin_0_"<<pos<<" = 0bin0000 );"<<endl;
		}
		else
		{
			index++;
		}

		if(pos == 10)
		{
			outcvc<<"ASSERT( NOT( y_Sout_"<<y_ROUND-2<<"_"<<pos<<" = 0bin0000 ) );"<<endl;
		}
		else
		{
			outcvc<<"ASSERT( y_Sout_"<<y_ROUND-2<<"_"<<pos<<" = 0bin0000 );"<<endl;
		}
	}
	
	for(int pos=0;pos<16;pos++)
	{
		if( pos == 12 )
		{
			outcvc<<"ASSERT( x_Kin_0_"<<pos<<" = 0bin0000 );"<<endl;
		}

		outcvc<<"ASSERT( y_LPin_"<<y_ROUND-2<<"_"<<pos<<" = 0bin0000 );"<<endl;
		
		
		
		
	}



	outcvc<<"QUERY(FALSE);"<<endl;
	outcvc<<"COUNTEREXAMPLE;"<<endl;
	
	outcvc.close();
	
    return 0;
}
