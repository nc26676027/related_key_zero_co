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

	if (x == 2)
	{
		string s = "( IF (";
		s.append(y);
		s.append("&0hex8 = 0hex8 THEN BVXOR((");
		s.append(y);
		s.append("<<1)[3:0] , 0hex3) ELSE (");
		s.append(y);
		s.append("<<1)[3:0] ENDIF )");
		return s;
	}
    else if(x == 4)
    {
		string s = "( IF (";
		s.append(y);
		s.append("&0hexc = 0hex8 OR ");
		s.append(y);
		s.append("&0hexc = 0hex4 ) THEN BVXOR((");
		s.append(y);
		s.append("<<2)[3:0],0hex3) ELSE (");
		s.append(y);
		s.append("<<2)[3:0] ENDIF )");
        return s;
    }
    else if(x == 9)
    {		
        string s = "( IF (";
        s.append(y);
        s.append("&0hexe = 0hex8 OR ");
        s.append(y);
        s.append("&0hexe = 0hex4 OR ");
        s.append(y);
        s.append("&0hexe = 0hex2 OR ");
        s.append(y);
        s.append("&0hexe = 0hexe ) THEN BVXOR(");
        s.append(y);
        s.append(" , BVXOR((");
        s.append(y);
        s.append("<<3)[3:0],0hex3 )) ELSE BVXOR( ");
        s.append(y);
        s.append(", (");
        s.append(y);
        s.append("<<3)[3:0] ) ENDIF )");
        return s;
    }
    else if(x == 13)
    {		
        string s = "BVXOR( ";
		s.append( mul_mat(4,y) );
		s.append(" , ");
		s.append( mul_mat(9,y) );
		s.append(" );");
        return s;
    }
}


//original MC layer matrix

int SR[16] = 	 { 0,  5, 10, 15,
              	   4,  9, 14,  3,
                   8, 13,  2,  6,
                  12,  1,  5, 11};



int P[16] = {1, 6, 11, 12, 5, 10, 15, 0, 9, 14, 3, 4, 13, 2, 7, 8};

int P_R[16] = {1, 6, 11, 12, 5, 10, 15, 0, 9, 14, 3, 4, 13, 2, 7, 8};


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
		filename = "aes"+to_string(atoi(argv[3]))+".cvc";
    }
    printf("key_flag=%d , head_flag=%d , tail_flag=%d\n fileno = %d",key_flag,head_flag,tail_flag,tail_flag);

    //peogram main
    ofstream outcvc;
    int x_ROUND = 3;
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
                outcvc<<" : BITVECTOR(4);"<<endl;
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
				outcvc<<" : BITVECTOR(4);"<<endl;
			}
			
		}
	}
	//key 
	for(int round=0;round<ROUND+1;round++)
	{
		for(int pos=0;pos<8;pos++)
		{
		
			outcvc<<"Kin_"<<round<<"_"<<pos<<" , RKin_"<<round<<"_"<<pos<<" , LPin_"<<round<<"_"<<pos<<"Kin2_"<<round<<"_"<<pos<<" , RKin2_"<<round<<"_"<<pos<<" , LPin2_"<<round<<"_"<<pos;
			if(pos<7)
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
		for(int pos=0;pos<16;pos++)
		{
			outcvc<<"ASSERT( x_Sin_"<<round<<"_"<<"pos"<<" = RKin_"<<round<<"_"<<pos<<" );"<<endl;
			outcvc<<"ASSERT( x_Sin_"<<round<<"_"<<"pos"<<" = RKin2_"<<round<<"_"<<pos<<" );"<<endl;

			outcvc<<"ASSERT( IF x_Sin_"<<round<<"_"<<pos<<" = 0bin0000 THEN x_Sout_"<<round<<"_"<<pos<<" = 0bin0000 ELSE NOT ( x_Sout_"<<round<<"_"<<pos<<" = 0bin0000 ) ENDIF );"<<endl;

			
			outcvc<<"ASSERT( x_SRout_"<<round<<"_"<<pos<<" = x_Sout_"<<round<<"_"<<SR[pos]<<" );"<<endl;
			
		}
		//mix Col
	 	for(int col=0;col<4;col++)
		{
			string a = "x_MCout_"+to_string(round)+"_"+to_string(col);
			string b = "x_MCout_"+to_string(round)+"_"+to_string(4+col);
			string c = "x_MCout_"+to_string(round)+"_"+to_string(8+col);
			string d = "x_MCout_"+to_string(round)+"_"+to_string(12+col);
			outcvc<<"ASSERT( x_SRout_"<<round<<"_"<<col<<" = BVXOR( "<<a<<" , BVXOR( "<<mul_mat(4,b)<<" , BVXOR( "<<mul_mat(9,c)<<" , "<<mul_mat(13,d)<<" ) ) ) );"<<endl;
			outcvc<<"ASSERT( x_SRout_"<<round<<"_"<<col+4<<" = BVXOR( "<<mul_mat(4,a)<<" , BVXOR( "<<b<<" , BVXOR( "<<mul_mat(13,c)<<" , "<<mul_mat(9,d)<<" ) ) ) );"<<endl;
			outcvc<<"ASSERT( x_SRout_"<<round<<"_"<<col+8<<" = BVXOR( "<<mul_mat(9,a)<<" , BVXOR( "<<mul_mat(13,b)<<" , BVXOR( "<<c<<" , "<<mul_mat(4,d)<<" ) ) ) );"<<endl;
			outcvc<<"ASSERT( x_SRout_"<<round<<"_"<<col+12<<" = BVXOR( "<<mul_mat(13,a)<<" , BVXOR( "<<mul_mat(9,b)<<" , BVXOR( "<<mul_mat(4,c)<<" , "<<d<<" ) ) ) );"<<endl;


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
			outcvc<<"ASSERT( y_Sin_"<<round<<"_"<<"pos"<<" = RKin_"<<x_ROUND+round<<"_"<<pos<<" );"<<endl;
			outcvc<<"ASSERT( y_Sin_"<<round<<"_"<<"pos"<<" = RKin2_"<<x_ROUND+round<<"_"<<pos<<" );"<<endl;

			outcvc<<"ASSERT( IF y_Sin_"<<round<<"_"<<pos<<" = 0bin0000 THEN y_Sout_"<<round<<"_"<<pos<<" = 0bin0000 ELSE NOT ( y_Sout_"<<round<<"_"<<pos<<" = 0bin0000 ) ENDIF );"<<endl;

			outcvc<<"ASSERT( y_Sout_"<<round<<"_"<<SR[pos]<<" = y_SRout_"<<round<<"_"<<pos<<" );"<<endl;
			
		}

		 
        //MixCol
	 	for(int col=0;col<4;col++)
		{
			string a = "y_MCout_"+to_string(round)+"_"+to_string(col);
			string b = "y_MCout_"+to_string(round)+"_"+to_string(4+col);
			string c = "y_MCout_"+to_string(round)+"_"+to_string(8+col);
			string d = "y_MCout_"+to_string(round)+"_"+to_string(12+col);
			outcvc<<"ASSERT( y_SRout_"<<round<<"_"<<col<<" = BVXOR( "<<a<<" , BVXOR( "<<mul_mat(4,b)<<" , BVXOR( "<<mul_mat(9,c)<<" , "<<mul_mat(13,d)<<" ) ) ) );"<<endl;
			outcvc<<"ASSERT( y_SRout_"<<round<<"_"<<col+4<<" = BVXOR( "<<mul_mat(4,a)<<" , BVXOR( "<<b<<" , BVXOR( "<<mul_mat(13,c)<<" , "<<mul_mat(9,d)<<" ) ) ) );"<<endl;
			outcvc<<"ASSERT( y_SRout_"<<round<<"_"<<col+8<<" = BVXOR( "<<mul_mat(9,a)<<" , BVXOR( "<<mul_mat(13,b)<<" , BVXOR( "<<c<<" , "<<mul_mat(4,d)<<" ) ) ) );"<<endl;
			outcvc<<"ASSERT( y_SRout_"<<round<<"_"<<col+12<<" = BVXOR( "<<mul_mat(13,a)<<" , BVXOR( "<<mul_mat(9,b)<<" , BVXOR( "<<mul_mat(4,c)<<" , "<<d<<" ) ) ) );"<<endl;


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
			//TK1
			string a = "Kin_"+to_string(round)+"_"+to_string(pos);
			string b = "RKin_"+to_string(round)+"_"+to_string(pos);

			outcvc<<"ASSERT( LPin_"<<round<<"_"<<pos<<" = "<<branch(a,b)<<" );"<<endl;

			outcvc<<"ASSERT( Kin_"<<round+1<<"_"<<pos<<" = LPin_"<<round<<"_"<<P[pos]<<" );"<<endl;

			//TK2
			string a2 = "Kin2_"+to_string(round)+"_"+to_string(pos);
			string b2 = "RKin2_"+to_string(round)+"_"+to_string(pos);
			string c2 = "LPin2_"+to_string(round)+"_"+to_string(P[pos]);

			outcvc<<"ASSERT( LPin2_"<<round<<"_"<<pos<<" = "<<branch(a2,b2)<<" );"<<endl;

			outcvc<<"ASSERT( Kin2_"<<round+1<<"_"<<pos<<" = "<<mul_mat(2,c2)<<" );"<<endl;

	
		
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
		if(pos<16)
		{
			if(1  /*pos == return_index(key_flag,P_R)*/)
			{
				outcvc<<"ASSERT( Kin_"<<ROUND<<"_"<<pos<<" = 0bin0000 );"<<endl;

			}
			

			if( pos == key_flag )
			{
				outcvc<<"ASSERT( Kin_0_"<<pos<<" = 0bin0000 );"<<endl;
				outcvc<<"ASSERT( Kin2_0_"<<pos<<" = 0bin0000 );"<<endl;


			}
		}

	}


	outcvc<<"QUERY(FALSE);"<<endl;
	outcvc<<"COUNTEREXAMPLE;"<<endl;
	
	outcvc.close();
	
    return 0;
}
