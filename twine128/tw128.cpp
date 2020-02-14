#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>

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


int h[16] = {
			5 , 0 , 1 , 4 ,
            7 , 12 , 3 , 8 ,
            13 , 6 , 9 , 2 ,
            15 , 10 , 11 , 14
            };

int h_inv[16] = 
	    {
	    1 , 2 , 11 , 6 , 3 ,
	    0 , 9 , 4 , 7 , 10 ,
	    13 , 14 , 5 , 8 , 15 , 
	    12
	    };
int rot[32] = 
	    {
	    4 , 5 , 6 , 7 , 
		8 , 9 , 10 , 11 , 
		12 , 13 , 14 , 15 , 
		16 , 17 , 18 , 19 ,
		20 , 21 , 22 , 23 ,
		24 , 25 , 26 , 27 ,
		28 , 29 , 30 , 31 ,
		1 , 2 , 3 , 0
	    };
int RK[8] = 
	    {
	    2 , 3 , 12 , 15 , 17 , 18 , 28 , 31
	    };

string branch(string a,string b)
{
	string s = "";
	s = s+"(IF ("+a+" = 0bin00)AND("+b+" = 0bin00) THEN 0bin00 ELSE (IF (("+a+" = 0bin00)AND("+b+" = 0bin01))OR(("+a+" = 0bin01)AND("+b+" = 0bin00)) THEN 0bin01 ELSE 0bin10 ENDIF ) ENDIF)";
	return s;
}



int main()
{
	ofstream outcvc;
    int x_ROUND = 9;
	int y_ROUND = 8;
	int ROUND = x_ROUND+y_ROUND;
	int total_Ps_length = 8;
	string Ps_th = "0hex10";

    
	//gen CVC
	outcvc.open("twine.cvc");
//variable claim
	//head state count
	for(int pos=0;pos<16;pos++)
	{
		outcvc<<"Ps_"<<pos;
		if(pos < 15)
		{
			outcvc<<" , ";
		}
		else
		{
			outcvc<<" : BITVECTOR(1);"<<endl;
		}
	}
	outcvc<<"total_Ps : BITVECTOR("<<total_Ps_length<<" );"<<endl;
	//state variable claim
    for(int round=0;round<=x_ROUND;round++)
    {
		
        for(int pos=0;pos<16;pos++)
        {
            outcvc<<"x_Fin_"<<round<<"_"<<pos<<" , x_Xout_"<<round<<"_"<<pos;
            if(pos == 15)
            {
                outcvc<<" : BITVECTOR(2);"<<endl;
            }
            else
            {
                outcvc<<" , ";
            }
        }
		for(int s=0;s<8;s++)
		{
			outcvc<<"x_Sout_"<<round<<"_"<<s*2<<" , x_Sin_"<<round<<"_"<<s*2;
		
			if(s<7)
			{
				outcvc<<" , ";
			}
			else
			{
				outcvc<<" : BITVECTOR(2);"<<endl;
			}
		}
    }
	
	//backward state variable claim
	for(int round = y_ROUND;round>=0;round--)
	{
        for(int pos=0;pos<16;pos++)
        {
            outcvc<<"y_Fin_"<<round<<"_"<<pos<<" , y_Xout_"<<round<<"_"<<pos;
            if(pos == 15)
            {
                outcvc<<" : BITVECTOR(2);"<<endl;
            }
            else
            {
                outcvc<<" , ";
            }
        }
		for(int s=0;s<8;s++)
		{
			outcvc<<"y_Sout_"<<round<<"_"<<s*2<<" , y_Sin_"<<round<<"_"<<s*2;
		
			if(s<7)
			{
				outcvc<<" , ";
			}
			else
			{
				outcvc<<" : BITVECTOR(2);"<<endl;
			}
		}
		
	}
	for(int round=0;round<=ROUND;round++)
	{
		//key schedule variable claim
		
		for(int pos=0;pos<32;pos++)
		{
			if(round == 0)
			{
				outcvc<<"MK_0_"<<pos<<" : BITVECTOR(2);"<<endl;
			}
			outcvc<<"Kin_"<<round<<"_"<<pos<<" , Rotin_"<<round<<"_"<<pos<<" , Kout_"<<round<<"_"<<pos;
			if(pos<31)
			{
				outcvc<<" , ";
			}
			else
			{
				outcvc<<" : BITVECTOR(2);"<<endl; 
			}						
		}
		for(int pos=0;pos<8;pos++)
		{
			outcvc<<"RKin_"<<round<<"_"<<RK[pos];
			if(pos<19)
			{
				outcvc<<" , ";
			}
			else
			{
				outcvc<<" : BITVECTOR(2);"<<endl; 
			}						
			
		}
		//key schedule Sbox
		outcvc<<"KSin_"<<round<<"_0 , KSin_"<<round<<"_16 , KSin_"<<round<<"_30 : BITVECTOR(2);"<<endl;
		
	}
//ASSERT 
	for(int round=0;round<x_ROUND;round++)
	{	
		//forward state update
		for(int pos=0;pos<16;pos++)
		{
			
			if(pos%2 == 0)
			{
				string a = "x_Fin_"+to_string(round)+"_"+to_string(pos);
				string b = "x_Sin_"+to_string(round)+"_"+to_string(pos);
				outcvc<<"ASSERT( x_Xout_"<<round<<"_"<<pos<<" = "<<branch(a,b)<<" );"<<endl;
				outcvc<<"ASSERT( "<<b<<" = x_Fin_"<<round<<"_"<<pos+1<<" );"<<endl;
			}
			else
			{
				outcvc<<"ASSERT( x_Xout_"<<round<<"_"<<pos<<" = x_Fin_"<<round<<"_"<<pos<<" );"<<endl;
				
			}
			
			outcvc<<"ASSERT( x_Fin_"<<(round+1)<<"_"<<(h[pos])<<" = x_Xout_"<<round<<"_"<<pos<<" );"<<endl;
			
			if((pos%2 == 0)&&(pos<16))
			{
				outcvc<<"ASSERT( RKin_"<<round<<"_"<<(RK[pos/2])<<" = x_Sin_"<<round<<"_"<<pos<<" );"<<endl;			
			}		
			
		}	
		
	}
	for(int pos=0;pos<16;pos++)
	{
		outcvc<<"ASSERT( IF x_Fin_0"<<"_"<<pos<<" = 0bin00 THEN Ps_"<<pos<<" = 0bin1 ELSE Ps_"<<pos<<" = 0bin0 ENDIF );"<<endl;
	}
	for(int round=y_ROUND-1;round>=0;round--)
	{	
		//backward state update
		for(int pos=0;pos<16;pos++)
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
				outcvc<<"ASSERT( "<<b<<" = y_Xout_"<<round<<"_"<<pos+1<<" );"<<endl;
			}
			else
			{
				outcvc<<"ASSERT( y_Fin_"<<round<<"_"<<pos<<" = y_Xout_"<<round<<"_"<<pos<<" );"<<endl;
			}
			if((pos%2 == 0)&&(pos<16))
			{
				outcvc<<"ASSERT( RKin_"<<x_ROUND+round<<"_"<<RK[pos/2]<<" = y_Sin_"<<round<<"_"<<pos<<" );"<<endl;			
			}		
			
		}		
	}

	//key schedule
	int last_i = 0;
	for(int pos=0;pos<32;pos++)
	{
		if(pos == RK[last_i])
		{
			last_i = last_i+1;
			outcvc<<"ASSERT( Kout_"<<ROUND-2<<"_"<<pos<<" = RKin_"<<ROUND-1<<"_"<<pos<<" );"<<endl;
		}
	}
	for(int round=ROUND-2;round>=0;round--)
	{
		int i = 0;
		int j = 0;
		for(int pos=0;pos<32;pos++)
		{	
			outcvc<<"ASSERT( Rotin_"<<round<<"_"<<rot[pos]<<" = Kout_"<<round<<"_"<<pos<<" );"<<endl;
			
			if((pos == 0)||(pos == 16)||(pos == 30))
			{
				string a = "Rotin_"+to_string(round)+"_"+to_string(pos); 
				string b = "KSin_"+to_string(round)+"_"+to_string(pos);
				outcvc<<"ASSERT( Kin_"<<round<<"_"<<pos<<" = "<<branch(a,b)<<" );"<<endl;
				if(pos == 0)
				{
					outcvc<<"ASSERT( "<<b<<" = Rotin_"<<round<<"_"<<pos+1<<" );"<<endl;
				}
				else if(pos == 16)
				{
					outcvc<<"ASSERT( "<<b<<" = Rotin_"<<round<<"_4 );"<<endl;
				}
				else
				{
					outcvc<<"ASSERT( "<<b<<" = Rotin_"<<round<<"_23 );"<<endl;					
				}
			}
			else
			{
				outcvc<<"ASSERT( Kin_"<<round<<"_"<<pos<<" = Rotin_"<<round<<"_"<<pos<<" );"<<endl;
			}
			
			if((pos == RK[i])&&(round>0))
			{	
				i = i+1;
				string a = "RKin_"+to_string(round)+"_"+to_string(pos); 
				string b = "Kin_"+to_string(round)+"_"+to_string(pos);
				outcvc<<"ASSERT( Kout_"<<round-1<<"_"<<pos<<" = "<<branch(a,b)<<" );"<<endl;
			}
			else if(round>0)
			{
				outcvc<<"ASSERT( Kout_"<<round-1<<"_"<<pos<<" = Kin_"<<round<<"_"<<pos<<" );"<<endl;
			}
			if((pos == RK[j])&&(round == 0))
			{
				j = j+1;
				string a = "RKin_0_"+to_string(pos);
				string b = "Kin_0_"+to_string(pos);
				outcvc<<"ASSERT( MK_0_"<<pos<<" = "<<branch(a,b)<<" );"<<endl;
			}
			else if(round == 0)
			{
				outcvc<<"ASSERT( MK_0_"<<pos<<" = Kin_0_"<<pos<<" );"<<endl;
			}
		}		
	}
	
	for(int pos=0;pos<16;pos++)
	{
		outcvc<<"ASSERT( x_Fin_"<<x_ROUND<<"_"<<pos<<" = y_Fin_0_"<<pos<<" );"<<endl;
	}
	
	
	string str_zero="0bin";
	for(int length=0;length<total_Ps_length-1;length++)
	{
		str_zero+="0";
	}
	string str_ps="";
	for(int pos=0;pos<16;pos++)
	{
		str_ps+=(str_zero+"@Ps_"+to_string(pos));
		if(pos==15)
		{
			outcvc<<"ASSERT( total_Ps = BVPLUS( "<<total_Ps_length<<" , "<<str_ps<<" ) );"<<endl;
		}
		else
		{
			str_ps+=" , ";
		}
	}
	outcvc<<"ASSERT( BVLE( total_Ps , "<<Ps_th<<" ) );"<<endl;
	outcvc<<"ASSERT(  (MK_0_"<<RK[0]<<" = 0bin01) OR (MK_0_"<<RK[1]<<" = 0bin01) OR (MK_0_"<<RK[1]<<" = 0bin01) OR (MK_0_"<<RK[2]<<" = 0bin01) OR (MK_0_"<<RK[3]<<" = 0bin01) OR (MK_0_"<<RK[4]<<" = 0bin01) OR (MK_0_"<<RK[5]<<" = 0bin01) OR (MK_0_"<<RK[6]<<" = 0bin01) OR (MK_0_"<<RK[7]<<" = 0bin01) );"<<endl;
	outcvc<<"ASSERT( NOT( x_Fin_0_0@x_Fin_0_1@x_Fin_0_2@x_Fin_0_3@x_Fin_0_4@x_Fin_0_5@x_Fin_0_6@x_Fin_0_7@x_Fin_0_8@x_Fin_0_9@x_Fin_0_10@x_Fin_0_11@x_Fin_0_12@x_Fin_0_13@x_Fin_0_14@x_Fin_0_15 = 0bin00000000000000000000000000000000 ) );"<<endl;
	outcvc<<"ASSERT( NOT( y_Xout_"<<y_ROUND-1<<"_0@y_Xout_"<<y_ROUND-1<<"_1@y_Xout_"<<y_ROUND-1<<"_2@y_Xout_"<<y_ROUND-1<<"_3@y_Xout_"<<y_ROUND-1<<"_4@y_Xout_"<<y_ROUND-1<<"_5@y_Xout_"<<y_ROUND-1<<"_6@y_Xout_"<<y_ROUND-1<<"_7@y_Xout_"<<y_ROUND-1<<"_8@y_Xout_"<<y_ROUND-1<<"_9@y_Xout_"<<y_ROUND-1<<"_10@y_Xout_"<<y_ROUND-1<<"_11@y_Xout_"<<y_ROUND-1<<"_12@y_Xout_"<<y_ROUND-1<<"_13@y_Xout_"<<y_ROUND-1<<"_14@y_Xout_"<<y_ROUND-1<<"_15 = 0bin00000000000000000000000000000000 ) );"<<endl;
	
	outcvc<<"QUERY(FALSE);"<<endl;
	outcvc<<"COUNTEREXAMPLE;"<<endl;
	
	outcvc.close();
	
	
	







    return 0;
}
