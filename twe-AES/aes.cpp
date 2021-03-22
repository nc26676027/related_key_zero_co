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
		filename = "aes"+to_string(atoi(argv[3]))+".cvc";
    }
    printf("key_flag=%d , head_flag=%d , tail_flag=%d\n fileno = %d",key_flag,head_flag,tail_flag,tail_flag);

    //peogram main
    ofstream outcvc;
    int ROUND = 3;

    //gen CVC
    outcvc.open(filename);

    //state variable claim
    for(int round=0;round<ROUND;round++)//up
    {
        for(int pos=0;pos<16;pos++)
        {
            
            outcvc<<"Sin_"<<round<<"_"<<pos<<" , Sout_"<<round<<"_"<<pos<<" , SRout_"<<round<<"_"<<pos<<" , MCout_"<<round<<"_"<<pos;
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

	//key 
	for(int round=0;round<ROUND+1;round++)
	{
		outcvc<<"Kin_"<<round<<" , RKin_"<<round<<" : BITVECTOR(4);"<<endl;

	}



	/*     *************************** ASSERT   ******************************  */
	//state update
	for(int round=0;round<ROUND;round++)//up
	{
		for(int pos=0;pos<16;pos++)
		{
			outcvc<<"ASSERT( IF Sin_"<<round<<"_"<<pos<<" = 0bin00000000 THEN Sout_"<<round<<"_"<<pos<<" = 0bin00000000 ELSE NOT ( Sout_"<<round<<"_"<<pos<<" = 0bin00000000 ) ENDIF );"<<endl;
			if( (pos < 4) && (round%2 == 1 ) )
			{
				outcvc<<"ASSERT( MCout_"<<round<<"_"<<pos<<"[0:0] = RKin_"<<round<<"_"<<pos<<" );"<<endl;
			}
			
			outcvc<<"ASSERT( SRout_"<<round<<"_"<<SR[pos]<<" = Sout_"<<round<<"_"<<pos<<" );"<<endl;
			
		}
		//mix Col
	 	for(int col=0;col<4;col++)
		{
			string a = "MCout_"+to_string(round)+"_"+to_string(col);
			string b = "MCout_"+to_string(round)+"_"+to_string(4+col);
			string c = "MCout_"+to_string(round)+"_"+to_string(8+col);
			string d = "MCout_"+to_string(round)+"_"+to_string(12+col);
			outcvc<<"ASSERT( SRout_"<<round<<"_"<<col<<" = BVXOR( "<<mul_mat(2,a)<<" , BVXOR( "<<b<<" , BVXOR( "<<c<<" , "<<mul_mat(3,d)<<" ) ) ) );"<<endl;
			outcvc<<"ASSERT( SRout_"<<round<<"_"<<col+4<<" = BVXOR( "<<mul_mat(3,a)<<" , BVXOR( "<<c<<" , BVXOR( "<<d<<" , "<<mul_mat(2,b)<<" ) ) ) );"<<endl;
			outcvc<<"ASSERT( SRout_"<<round<<"_"<<col+8<<" = BVXOR( "<<mul_mat(3,b)<<" , BVXOR( "<<a<<" , BVXOR( "<<d<<" , "<<mul_mat(2,c)<<" ) ) ) );"<<endl;
			outcvc<<"ASSERT( SRout_"<<round<<"_"<<col+12<<" = BVXOR( "<<mul_mat(3,c)<<" , BVXOR( "<<a<<" , BVXOR( "<<b<<" , "<<mul_mat(2,d)<<" ) ) ) );"<<endl;

		}
		if( round<ROUND-1)
		{
			for(int pos=0;pos<16;pos++)
			{
				outcvc<<"ASSERT( x_Sin_"<<round+1<<"_"<<pos<<" = x_MCout_"<<round<<"_"<<pos<<" );"<<endl;
			}
		}
	}

	//assert active state
	for(int pos=0;pos<16;pos++)
	{
		if(pos<16)
		{
			if( pos != head_flag )
			{
				outcvc<<"ASSERT( x_Sin_0_"<<pos<<" = 0bin00000000 );"<<endl;
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
		if(pos<8)
		{
			if(1  /*pos == return_index(key_flag,P_R)*/)
			{
				outcvc<<"ASSERT( Kin_"<<ROUND<<"_"<<pos<<" = 0bin00000000 );"<<endl;

			}
			

			if( pos == key_flag )
			{
				outcvc<<"ASSERT( Kin_0_"<<pos<<" = 0bin00000000 );"<<endl;

			}
		}

	}


	outcvc<<"QUERY(FALSE);"<<endl;
	outcvc<<"COUNTEREXAMPLE;"<<endl;
	
	outcvc.close();
	
    return 0;
}
