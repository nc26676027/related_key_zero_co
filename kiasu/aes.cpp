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

string mul_mat(int x,string y)
{
    if(x == 0x01)
    {
        return y;
    }
    else if(x == 0x02)
    {
        string s = "( IF (";
        s.append(y);
        s.append("&0hex80) = 0hex80 THEN ");
        s.append("BVXOR(((");
        s.append(y);
        s.append("<<1)[7:0]),0hex1b) ELSE ");
        s.append("((");
        s.append(y);
        s.append("<<1)[7:0]) ENDIF )");

        return s;
    }
    else if(x == 0x03)
    {
        string s = "( IF (";
        s.append(y);
        s.append("&0hex80) = 0hex80 THEN ");
        s.append("BVXOR( ");
        s.append(y);
        s.append(",BVXOR(((");
        s.append(y);
        s.append("<<1)[7:0]),0hex1b)) ELSE ");
        s.append("BVXOR( ");
        s.append(y);
        s.append(",(( ");
        s.append(y);
        s.append("<<1)[7:0])) ENDIF )");
        return s;
    }
}


//original MC layer matrix
int MCT[4][4]=
{
	2,1,1,3,
	3,2,1,1,
	1,3,2,1,
	1,1,3,2
};

int SR[16] = 	 { 0,  1,  2,  3,
              	   7,  4,  5,  6,
                  10, 11,  8,  9,
                  13, 14, 15, 12};

int inv_SR[16] = { 0,  1,  2,  3,
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
		filename = "skinny"+to_string(atoi(argv[3]))+".cvc";
    }
    printf("key_flag=%d , head_flag=%d , tail_flag=%d\n fileno = %d",key_flag,head_flag,tail_flag,tail_flag);

    //peogram main
    ofstream outcvc;
    int x_ROUND = 2;
    int y_ROUND = 2;
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
                outcvc<<" : BITVECTOR(8);"<<endl;
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
				outcvc<<" : BITVECTOR(8);"<<endl;
			}
			
		}
	}
	//key 
	for(int round=0;round<ROUND;round++)
	{
		for(int pos=0;pos<8;pos++)
		{
		
			outcvc<<"Kin_"<<round<<"_"<<pos<<" , RKin_"<<round<<"_"<<pos;
			if(pos<7)
			{
				outcvc<<" , ";
			}
			else
			{
				outcvc<<" : BITVECTOR(8);"<<endl;
			}			
	
		}

	}



	/*     *************************** ASSERT   ******************************  */
	//state update
	for(int round=0;round<x_ROUND;round++)//up
	{
		for(int pos=0;pos<16;pos++)
		{
			outcvc<<"ASSERT( IF x_Sin_"<<round<<"_"<<pos<<" = 0bin00000000 THEN x_Sout_"<<round<<"_"<<pos<<" = 0bin00000000 ELSE NOT ( x_Sout_"<<round<<"_"<<pos<<" = 0bin00000000 ) ENDIF );"<<endl;
			if( pos<8 )
			{
				outcvc<<"ASSERT( x_MCout_"<<round<<"_"<<pos<<" = RKin_"<<round<<"_"<<pos<<" );"<<endl;
			}

			
			outcvc<<"ASSERT( x_SRout_"<<round<<"_"<<SR[pos]<<" = x_Sout_"<<round<<"_"<<pos<<" );"<<endl;
			
		}
		//mix Col
	 	for(int col=0;col<4;col++)
		{
			string a = "x_MCout_"+to_string(round)+"_"+to_string(col);
			string b = "x_MCout_"+to_string(round)+"_"+to_string(4+col);
			string c = "x_MCout_"+to_string(round)+"_"+to_string(8+col);
			string d = "x_MCout_"+to_string(round)+"_"+to_string(12+col);
			outcvc<<"ASSERT( x_SRout_"<<round<<"_"<<col<<" = BVXOR( "<<mul_mat(2,a)<<" , BVXOR( "<<b<<" , BVXOR( "<<c<<" , "<<mul_mat(3,d)<<" ) ) ) );"<<endl;
			outcvc<<"ASSERT( x_SRout_"<<round<<"_"<<col+4<<" = BVXOR( "<<mul_mat(3,a)<<" , BVXOR( "<<c<<" , BVXOR( "<<d<<" , "<<mul_mat(2,b)<<" ) ) ) );"<<endl;
			outcvc<<"ASSERT( x_SRout_"<<round<<"_"<<col+8<<" = BVXOR( "<<mul_mat(3,b)<<" , BVXOR( "<<a<<" , BVXOR( "<<d<<" , "<<mul_mat(2,c)<<" ) ) ) );"<<endl;
			outcvc<<"ASSERT( x_SRout_"<<round<<"_"<<col+12<<" = BVXOR( "<<mul_mat(3,c)<<" , BVXOR( "<<a<<" , BVXOR( "<<b<<" , "<<mul_mat(2,d)<<" ) ) ) );"<<endl;


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
			outcvc<<"ASSERT( IF y_Sin_"<<round<<"_"<<pos<<" = 0bin00000000 THEN y_Sout_"<<round<<"_"<<pos<<" = 0bin00000000 ELSE NOT ( y_Sout_"<<round<<"_"<<pos<<" = 0bin00000000 ) ENDIF );"<<endl;
			if( pos<8 )
			{
				outcvc<<"ASSERT( RKin_"<<round+x_ROUND<<"_"<<pos<<" = y_MCout_"<<round<<"_"<<pos<<" );"<<endl;
			}
			outcvc<<"ASSERT( y_Sout_"<<round<<"_"<<inv_SR[pos]<<" = y_SRout_"<<round<<"_"<<pos<<" );"<<endl;
			
		}

		 
        //MixCol
	 	for(int col=0;col<4;col++)
		{
			string a = "y_MCout_"+to_string(round)+"_"+to_string(col);
			string b = "y_MCout_"+to_string(round)+"_"+to_string(4+col);
			string c = "y_MCout_"+to_string(round)+"_"+to_string(8+col);
			string d = "y_MCout_"+to_string(round)+"_"+to_string(12+col);
			outcvc<<"ASSERT( y_SRout_"<<round<<"_"<<col<<" = BVXOR( "<<mul_mat(2,a)<<" , BVXOR( "<<b<<" , BVXOR( "<<c<<" , "<<mul_mat(3,d)<<" ) ) ) );"<<endl;
			outcvc<<"ASSERT( y_SRout_"<<round<<"_"<<col+4<<" = BVXOR( "<<mul_mat(3,a)<<" , BVXOR( "<<c<<" , BVXOR( "<<d<<" , "<<mul_mat(2,b)<<" ) ) ) );"<<endl;
			outcvc<<"ASSERT( y_SRout_"<<round<<"_"<<col+8<<" = BVXOR( "<<mul_mat(3,b)<<" , BVXOR( "<<a<<" , BVXOR( "<<d<<" , "<<mul_mat(2,c)<<" ) ) ) );"<<endl;
			outcvc<<"ASSERT( y_SRout_"<<round<<"_"<<col+12<<" = BVXOR( "<<mul_mat(3,c)<<" , BVXOR( "<<a<<" , BVXOR( "<<b<<" , "<<mul_mat(2,d)<<" ) ) ) );"<<endl;


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
	for(int round=0;round<ROUND-1;round++)
	{
		for(int pos=0;pos<8;pos++)
		{

			string a = "Kin_"+to_string(round)+"_"+to_string(pos);
			string b = "RKin_"+to_string(round)+"_"+to_string(pos);

			outcvc<<"ASSERT( Kin_"<<round+1<<"_"<<pos<<" = "<<branch(a,b)<<" );"<<endl;
	
		
		}
	}


	//assert active state
	for(int pos=0;pos<16;pos++)
	{
		if(pos<16)
		{
			if( pos != head_flag )
			{
				outcvc<<"ASSERT( x_Sin_0_"<<pos<<" = 0bin00000000 ) );"<<endl;
			}

			if(pos == tail_flag)
			{
				outcvc<<"ASSERT( NOT( y_MCout_"<<y_ROUND-1<<"_"<<pos<<" = 0bin00000000 ) );"<<endl;
			}
			else
			{
				outcvc<<"ASSERT( y_MCout_"<<y_ROUND-1<<"_"<<pos<<" = 0bin00000000 );"<<endl;
			}		
			
		}
		
		if(1  /*pos == return_index(key_flag,P_R)*/)
		{
			outcvc<<"ASSERT( Kin_"<<ROUND<<"_"<<pos<<" = 0bin00000000 );"<<endl;

		}
		

		if( pos == key_flag )
		{
			outcvc<<"ASSERT( Kin_0_"<<pos<<" = 0bin00000000 );"<<endl;

		}
	}


	outcvc<<"QUERY(FALSE);"<<endl;
	outcvc<<"COUNTEREXAMPLE;"<<endl;
	
	outcvc.close();
	
    return 0;
}
