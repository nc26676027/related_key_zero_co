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




//original MC layer matrix
int MCT[4][4]=
{
	1,0,0,0,
	0,1,0,0,
	1,0,1,0,
	1,1,0,1
};



int P[16] = {15, 12, 13, 14, 10, 9, 8, 11, 6, 5, 4, 7, 1, 2, 3, 0};


void P_make(int round){

	int tem[16];

	for (int i=0 ;i<16;i++)
		tem[i]=P_R[i];

	for (int i=0 ; i<round-2; i++){
        for (int j=0;j<16;j++){
            tem[j]=P_R[P[j]];
        }

		for (int j=0;j<16;j++)
			P_R[j]=tem[j];
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
		filename = "craft"+to_string(atoi(argv[3]))+".cvc";
    }
    printf("key_flag=%d , head_flag=%d , tail_flag=%d\n fileno = %d",key_flag,head_flag,tail_flag,tail_flag);

    //peogram main
    ofstream outcvc;
    int x_ROUND = 3;
    int y_ROUND = 3;
    int ROUND = x_ROUND+y_ROUND;

	//P_make(ROUND);


    //gen CVC
    outcvc.open(filename);

    //state variable claim
	outcvc<<"LAT : ARRAY BITVECTOR(8) OF BITVECTOR(1);"<<endl;
	for(int input_lc=0x0;input_lc<16;input_lc++)
	{
		for(int output_lc=0x0;output_lc<16;output_lc++)
		{
			outcvc<<hex<<"ASSERT( LAT[0bin"<<tobits(input_lc,4)<<tobits(output_lc,4)<<"] = 0bin"<<((LAT[input_lc][output_lc] == 0)?0:1)<<" );"<<endl;
		}
	}
	outcvc<<dec;
    for(int round=0;round<x_ROUND;round++)//up
    {
        for(int pos=0;pos<16;pos++)
        {
            
            outcvc<<"x_Fin_"<<round<<"_"<<pos<<" , x_Sout_"<<round<<"_"<<pos<<" , x_SRout_"<<round<<"_"<<pos<<" , x_MCout_"<<round<<"_"<<pos;
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
	for(int round=0;round<y_ROUND;round++)
	{
		for(int pos=0;pos<16;pos++)
		{
			outcvc<<"y_Fin_"<<round<<"_"<<pos<<" , y_Sout_"<<round<<"_"<<pos<<" , y_SRout_"<<round<<"_"<<pos<<" , y_MCout_"<<round<<"_"<<pos;
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
	for(int round=0;round<ROUND;round++)
	{
		for(int pos=0;pos<16;pos++)
		{
		
			outcvc<<"Kin_"<<round<<"_"<<pos<<" , LPout_"<<round<<"_"<<pos<<" , RKin_"<<round<<"_"<<pos;
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
	for(int round=0;round<x_ROUND;round++)//up
	{
		//mix Col
	 	for(int col=0;col<4;col++)
		{
			outcvc<<"ASSERT( x_Fin_"<<round<<"_"<<col<<" = x_MCout_"<<round<<"_"<<col<<" );"<<endl;
			outcvc<<"ASSERT( x_Fin_"<<round<<"_"<<col+4<<" = x_MCout_"<<round<<"_"<<col+4<<" );"<<endl;
			outcvc<<"ASSERT( x_Fin_"<<round<<"_"<<col+8<<" = BVXOR( x_MCout_"<<round<<"_"<<col<<" , x_MCout_"<<round<<"_"<<col+8<<" ) );"<<endl;
			outcvc<<"ASSERT( x_Fin_"<<round<<"_"<<col+12<<" = BVXOR( x_MCout_"<<round<<"_"<<col<<" , BVXOR( x_MCout_"<<round<<"_"<<col+4<<" , x_MCout_"<<round<<"_"<<col+12<<" ) ) );"<<endl;
		}
		//ADD_ROUND-TWEAK and permute
		for(int pos=0;pos<16;pos++)
		{
			outcvc<<"ASSERT( x_MCout_"<<round<<"_"<<pos<<" = RKin_"<<round<<"_"<<pos<<" );"<<endl;

			outcvc<<"ASSERT( x_SRout_"<<round<<"_"<<P[pos]<<" = x_MCout_"<<round<<"_"<<pos<<" );"<<endl;
		}

		if( round<x_ROUND-1)
		{
			for(int pos=0;pos<16;pos++)
			{
				outcvc<<"ASSERT( NOT( LAT[x_SRout_"<<round<<"_"<<pos<<"@x_Sout_"<<round<<"_"<<pos<<"] = 0bin0 ) );"<<endl;
			}
		}
	}
	//down
	for(int round=0;round<y_ROUND;round++)
	{
		//mix Col
	 	for(int col=0;col<4;col++)
		{
			outcvc<<"ASSERT( y_Fin_"<<round<<"_"<<col<<" = y_MCout_"<<round<<"_"<<col<<" );"<<endl;
			outcvc<<"ASSERT( y_Fin_"<<round<<"_"<<col+4<<" = y_MCout_"<<round<<"_"<<col+4<<" );"<<endl;
			outcvc<<"ASSERT( y_Fin_"<<round<<"_"<<col+8<<" = BVXOR( y_MCout_"<<round<<"_"<<col<<" , y_MCout_"<<round<<"_"<<col+8<<" ) );"<<endl;
			outcvc<<"ASSERT( y_Fin_"<<round<<"_"<<col+12<<" = BVXOR( y_MCout_"<<round<<"_"<<col<<" , BVXOR( y_MCout_"<<round<<"_"<<col+4<<" , y_MCout_"<<round<<"_"<<col+12<<" ) ) );"<<endl;
		}
		//ADD_ROUND-TWEAK and permute
		for(int pos=0;pos<16;pos++)
		{
			outcvc<<"ASSERT( y_MCout_"<<round<<"_"<<pos<<" = RKin_"<<x_ROUND+round<<"_"<<pos<<" );"<<endl;

			outcvc<<"ASSERT( y_SRout_"<<round<<"_"<<P[pos]<<" = y_MCout_"<<round<<"_"<<pos<<" );"<<endl;
		}

		if( round<y_ROUND-1)
		{
			for(int pos=0;pos<16;pos++)
			{
				outcvc<<"ASSERT( NOT( LAT[y_SRout_"<<round<<"_"<<pos<<"@y_Sout_"<<round<<"_"<<pos<<"] = 0bin0 ) );"<<endl;
			}
		}
	}

	//connection up and down
	for(int pos=0;pos<16;pos++)
	{
		outcvc<<"ASSERT( x_Sout_"<<(x_ROUND-1)<<"_"<<pos<<" = y_Fin_0_"<<pos<<" );"<<endl;
	}
	
	//key update up
	for(int round=0;round<ROUND;round++)
	{
		for(int pos=0;pos<16;pos++)
		{
			//	TK1
			if(pos<8)
			{
				string a = "Kin_"+to_string(round)+"_"+to_string(pos);
				string b = "RKin_"+to_string(round)+"_"+to_string(pos);

				outcvc<<"ASSERT( LPout_"<<round<<"_"<<pos<<" = "<<branch(a,b)<<" );"<<endl;
			}
			else
			{
				outcvc<<"ASSERT( LPout_"<<round<<"_"<<pos<<" = Kin_"<<round<<"_"<<pos<<" );"<<endl;
				outcvc<<"ASSERT( RKin_"<<round<<"_"<<pos<<" = Kin_"<<round<<"_"<<pos<<" );"<<endl;
			}
			if(round<ROUND-1)
			{
				outcvc<<"ASSERT( Kin_"<<round+1<<"_"<<pos<<" = LPout_"<<round<<"_"<<P[pos]<<" );"<<endl;		
			}


		}
	}


	//assert active state
	for(int pos=0;pos<16;pos++)
	{
		if(pos<16)
		{
			if( pos == head_flag )
			{
				outcvc<<"ASSERT( NOT( x_Sin_0_"<<pos<<" = 0bin0000 ) );"<<endl;
			}
			else
			{
				outcvc<<"ASSERT( x_Sin_0_"<<pos<<" = 0bin0000 );"<<endl;
			}
			if(pos == tail_flag)
			{
				outcvc<<"ASSERT( NOT( y_SRout_"<<y_ROUND-1<<"_"<<pos<<" = 0bin0000 ) );"<<endl;
			}
			else
			{
				outcvc<<"ASSERT( y_SRout_"<<y_ROUND-1<<"_"<<pos<<" = 0bin0000 );"<<endl;
			}		
			
		}
		
		if(1  /*pos == return_index(key_flag,P_R)*/)
		{
			outcvc<<"ASSERT( LPout_"<<ROUND-1<<"_"<<pos<<" = 0bin0000 );"<<endl;


		}
		

		if( pos == key_flag )
		{
			outcvc<<"ASSERT( Kin_0_"<<pos<<" = 0bin0000 );"<<endl;


		}
	}
/*
	//other tweak 
	for(int pos=0;pos<16;pos++)
	{
		if(pos != return_index(key_flag,P_R))
		{
			outcvc<<"ASSERT( LPout_"<<ROUND-1<<"_"<<pos<<" = Kin_0_"<<P_R[pos]<<" );"<<endl;
			outcvc<<"ASSERT( LPout2_"<<ROUND-1<<"_"<<pos<<" = Kin2_0_"<<P_R[pos]<<" );"<<endl;
		}
	}
*/


	outcvc<<"QUERY(FALSE);"<<endl;
	outcvc<<"COUNTEREXAMPLE;"<<endl;
	
	outcvc.close();
	
    return 0;
}
