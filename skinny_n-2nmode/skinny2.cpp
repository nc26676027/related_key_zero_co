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
	1,1,0,1,
	0,0,1,0,
	1,0,1,1,
	1,0,0,0
};



int SR[16] = 	 { 0,  1,  2,  3,
            	   5,  6,  7,  4,
            	  10, 11,  8,  9,
             	  15, 12, 13, 14};

int P[16] = {9, 15, 8, 13, 10, 14, 12, 11, 0, 1, 2, 3, 4, 5, 6, 7};

int P_R[16]={9, 15, 8, 13, 10, 14, 12, 11, 0, 1, 2, 3, 4, 5, 6, 7};


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

string bvxor( string a , string b , string c )
{
	string s ="";
	s = s+"(IF ("+a+"@"+b+"@"+c+") = 0bin000000 THEN 0bin00 ELSE (IF ("+a+"@"+b+"@"+c+") = 0bin010000 THEN 0bin01 ELSE (IF ("+a+"@"+b+"@"+c+") = 0bin000100 THEN 0bin01 ELSE(IF ("+a+"@"+b+"@"+c+") = 0bin000001 THEN 0bin01 ELSE 0bin10 ENDIF ) ENDIF ) ENDIF ) ENDIF )";
	return s;
}




string branch(string a,string b)
{
	string s = "";
	s = s+"(IF ("+a+" = 0bin00)AND("+b+" = 0bin00) THEN 0bin00 ELSE (IF (("+a+" = 0bin00)AND("+b+" = 0bin01))OR(("+a+" = 0bin01)AND("+b+" = 0bin00)) THEN 0bin01 ELSE 0bin10 ENDIF ) ENDIF)";
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
		filename = "skinny"+to_string(atoi(argv[3]))+".cvc";
    }
    printf("key_flag=%d , head_flag=%d , tail_flag=%d\n fileno = %d",key_flag,head_flag,tail_flag,tail_flag);

    //peogram main
    ofstream outcvc;
    int x_ROUND = 7;
    int y_ROUND = 8;
    int ROUND = x_ROUND+y_ROUND;

	P_make(ROUND);


    //gen CVC
    outcvc.open(filename);

    //state variable claim
    for(int round=0;round<x_ROUND;round++)//up
    {
        for(int pos=0;pos<16;pos++)
        {
            
            outcvc<<"x_Sin_"<<round<<"_"<<pos<<" , x_Sout_"<<round<<"_"<<pos<<" , x_SRout_"<<round<<"_"<<pos<<" , x_MCout_"<<round<<"_"<<pos;
            if(pos < 15)
            {
                outcvc<<" , ";
            }
            else
            {
                outcvc<<" : BITVECTOR(2);"<<endl;
            }            
        }
    }
	//down
	for(int round=0;round<y_ROUND;round++)
	{
		for(int pos=0;pos<16;pos++)
		{
			outcvc<<"y_Sin_"<<round<<"_"<<pos<<" , y_Sout_"<<round<<"_"<<pos<<" , y_SRout_"<<round<<"_"<<pos<<" , y_MCout_"<<round<<"_"<<pos;
			if(pos<15)
			{
				outcvc<<" , ";
			}
			else
			{
				outcvc<<" : BITVECTOR(2);"<<endl;
			}
			
		}
	}
	//key 
	for(int round=0;round<ROUND;round++)
	{
		for(int pos=0;pos<16;pos++)
		{
		
			outcvc<<"Kin_"<<round<<"_"<<pos<<" , LPout_"<<round<<"_"<<pos<<" , Kin2_"<<round<<"_"<<pos<<" , LPout2_"<<round<<"_"<<pos<<" , RKin_"<<round<<"_"<<pos<<" , RKin2_"<<round<<"_"<<pos;
			if(pos<15)
			{
				outcvc<<" , ";
			}
			else
			{
				outcvc<<" : BITVECTOR(2);"<<endl;
			}			
	
		}

	}



	/*     *************************** ASSERT   ******************************  */
	//state update
	for(int round=0;round<x_ROUND;round++)//up
	{
		for(int pos=0;pos<16;pos++)
		{
			outcvc<<"ASSERT( x_Sin_"<<round<<"_"<<pos<<" = x_Sout_"<<round<<"_"<<pos<<" );"<<endl;
			if( pos<8 )
			{
				outcvc<<"ASSERT( x_Sout_"<<round<<"_"<<pos<<" = RKin_"<<round<<"_"<<pos<<" );"<<endl;
				outcvc<<"ASSERT( x_Sout_"<<round<<"_"<<pos<<" = RKin2_"<<round<<"_"<<pos<<" );"<<endl;
			}

			
			outcvc<<"ASSERT( x_SRout_"<<round<<"_"<<SR[pos]<<" = x_Sout_"<<round<<"_"<<pos<<" );"<<endl;
			
		}
		//mix Col
	 	for(int col=0;col<4;col++)
		{
			string a = "x_MCout_"+to_string(round)+"_"+to_string(col);
			string b = "x_MCout_"+to_string(round)+"_"+to_string(col+4);
			string c = "x_MCout_"+to_string(round)+"_"+to_string(col+8);
			string d = "x_MCout_"+to_string(round)+"_"+to_string(col+12);

			outcvc<<"ASSERT( x_SRout_"<<round<<"_"<<col<<" = "<<bvxor(a, b, d)<<" );"<<endl;
			outcvc<<"ASSERT( x_SRout_"<<round<<"_"<<col+4<<" = "<<c<<" );"<<endl;
			outcvc<<"ASSERT( x_SRout_"<<round<<"_"<<col+8<<" = "<<bvxor(a, c, d)<<" );"<<endl;
			outcvc<<"ASSERT( x_SRout_"<<round<<"_"<<col+12<<" = "<<a<<" );"<<endl;
		}
		if( round<x_ROUND-1)
		{
			for(int pos=0;pos<16;pos++)
			{
				outcvc<<"ASSERT( x_Sin_"<<round+1<<"_"<<pos<<" = x_MCout_"<<round<<"_"<<pos<<" );"<<endl;
			}
		}
	}
	//down
	for(int round=0;round<y_ROUND;round++)
	{
		for(int pos=0;pos<16;pos++)
		{
			outcvc<<"ASSERT( y_Sin_"<<round<<"_"<<pos<<" = y_Sout_"<<round<<"_"<<pos<<" );"<<endl;
			if( pos<8 )
			{
				outcvc<<"ASSERT( RKin_"<<round+x_ROUND<<"_"<<pos<<" = y_Sout_"<<round<<"_"<<pos<<" );"<<endl;
				outcvc<<"ASSERT( RKin2_"<<round+x_ROUND<<"_"<<pos<<" = y_Sout_"<<round<<"_"<<pos<<" );"<<endl;
			}
			outcvc<<"ASSERT( y_SRout_"<<round<<"_"<<SR[pos]<<" = y_Sout_"<<round<<"_"<<pos<<" );"<<endl;
			
		}
		//Mix COL
	 	for(int col=0;col<4;col++)
		 {
			string a = "x_MCout_"+to_string(round)+"_"+to_string(col);
			string b = "x_MCout_"+to_string(round)+"_"+to_string(col+4);
			string c = "x_MCout_"+to_string(round)+"_"+to_string(col+8);
			string d = "x_MCout_"+to_string(round)+"_"+to_string(col+12);

			outcvc<<"ASSERT( x_SRout_"<<round<<"_"<<col<<" = "<<bvxor(a, b, d)<<" );"<<endl;
			outcvc<<"ASSERT( x_SRout_"<<round<<"_"<<col+4<<" = "<<c<<" );"<<endl;
			outcvc<<"ASSERT( x_SRout_"<<round<<"_"<<col+8<<" = "<<bvxor(a, c, d)<<" );"<<endl;
			outcvc<<"ASSERT( x_SRout_"<<round<<"_"<<col+12<<" = "<<a<<" );"<<endl;
		 }	
		 
		 	
		if( round<y_ROUND-1)
		{
			for(int pos=0;pos<16;pos++)
			{
				outcvc<<"ASSERT( y_Sin_"<<round+1<<"_"<<pos<<" = y_MCout_"<<round<<"_"<<pos<<" );"<<endl;
			}
		}
	}

	//connection up and down
	for(int pos=0;pos<16;pos++)
	{
		outcvc<<"ASSERT( x_MCout_"<<(x_ROUND-1)<<"_"<<pos<<" = y_Sin_0_"<<pos<<" );"<<endl;
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
				outcvc<<"ASSERT( Kin_"<<round+1<<"_"<<P[pos]<<" = LPout_"<<round<<"_"<<pos<<" );"<<endl;		
			}

			//  TK2
			if(pos<8)
			{
				string a = "Kin2_"+to_string(round)+"_"+to_string(pos);
				string b = "RKin2_"+to_string(round)+"_"+to_string(pos);

				outcvc<<"ASSERT( LPout2_"<<round<<"_"<<pos<<" = "<<branch(a,b)<<" );"<<endl;
			}
			else
			{
				outcvc<<"ASSERT( LPout2_"<<round<<"_"<<pos<<" = Kin2_"<<round<<"_"<<pos<<" );"<<endl;
				outcvc<<"ASSERT( RKin2_"<<round<<"_"<<pos<<" = Kin2_"<<round<<"_"<<pos<<" );"<<endl;
			}
			if(round<ROUND-1)
			{					
				outcvc<<"ASSERT( Kin2_"<<round+1<<"_"<<P[pos]<<" = LPout2_"<<round<<"_"<<pos<<" );"<<endl;		
		
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
				outcvc<<"ASSERT( x_Sin_0_"<<pos<<" = 0bin01 );"<<endl;
			}
			else
			{
				outcvc<<"ASSERT( x_Sin_0_"<<pos<<" = 0bin00 );"<<endl;
			}
			if(pos == tail_flag)
			{
				outcvc<<"ASSERT( y_SRout_"<<y_ROUND-1<<"_"<<pos<<" = 0bin01 );"<<endl;
			}
			else
			{
				outcvc<<"ASSERT( y_SRout_"<<y_ROUND-1<<"_"<<pos<<" = 0bin00 );"<<endl;
			}		
			
		}
		
		if( 1  /*pos == return_index(key_flag,P_R)*/)
		{
			outcvc<<"ASSERT( LPout_"<<ROUND-1<<"_"<<pos<<" = 0bin00 );"<<endl;
			outcvc<<"ASSERT( LPout2_"<<ROUND-1<<"_"<<pos<<" = 0bin00 );"<<endl;
		}
		

		if( pos == key_flag )
		{
			outcvc<<"ASSERT( Kin_0_"<<pos<<" = 0bin00 );"<<endl;
			outcvc<<"ASSERT( Kin2_0_"<<pos<<" = 0bin00 );"<<endl;		
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
