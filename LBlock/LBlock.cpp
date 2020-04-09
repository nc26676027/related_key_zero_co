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

int p[8] = {2, 0, 3, 1, 6, 4, 7, 5};

int rot[8] = {6, 7, 0, 1, 2, 3, 4, 5};

int krot[20] = {13, 14, 15, 16,
                17, 18, 19,  0,
                 1,  2,  3,  4,
                 5,  6,  7,  8,
                 9, 10, 11, 12};




int LAT0[16][16] = {
    { 8 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 } ,
    { 0 , 0 , 0 , 0 , -2 , 2 , 2 , -2 , -4 , -4 , 0 , 0 , 2 , -2 , 2 , -2 } ,
    { 0 , 0 , 0 , -4 , -2 , -2 , 2 , -2 , 2 , 2 , -2 , 2 , 0 , 0 , 0 , -4 } ,
    { 0 , -4 , 0 , 0 , 0 , 0 , 0 , 4 , 2 , -2 , 2 , 2 , 2 , 2 , 2 , -2 } ,
    { 0 , 0 , 0 , 0 , 4 , 0 , 4 , 0 , 0 , 0 , 0 , 0 , 4 , 0 , -4 , 0 } ,
    { 0 , 0 , 0 , 0 , -2 , -2 , 2 , 2 , 0 , 0 , 4 , -4 , -2 , -2 , -2 , -2 } ,
    { 0 , 0 , 0 , -4 , 2 , -2 , -2 , -2 , -2 , -2 , 2 , -2 , 0 , 4 , 0 , 0 } ,
    { 0 , 4 , 0 , 0 , 0 , -4 , 0 , 0 , 2 , -2 , 2 , 2 , 2 , -2 , 2 , 2 } ,
    { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -4 , 4 , 4 , 4 , 0 , 0 , 0 , 0 } ,
    { 0 , 0 , -4 , -4 , -2 , 2 , -2 , 2 , 0 , 0 , 0 , 0 , 2 , -2 , -2 , 2 } ,
    { 0 , 0 , 4 , 0 , -2 , -2 , -2 , 2 , -2 , -2 , -2 , 2 , 0 , 0 , -4 , 0 } ,
    { 0 , 4 , 0 , 0 , 0 , 0 , 0 , 4 , -2 , 2 , -2 , -2 , 2 , 2 , 2 , -2 } ,
    { 0 , 0 , 0 , 0 , -4 , 0 , 4 , 0 , 0 , 0 , 0 , 0 , 0 , 4 , 0 , 4 } ,
    { 0 , 0 , -4 , 4 , -2 , -2 , -2 , -2 , 0 , 0 , 0 , 0 , 2 , 2 , -2 , -2 } ,
    { 0 , 0 , -4 , 0 , 2 , -2 , 2 , 2 , -2 , -2 , -2 , 2 , -4 , 0 , 0 , 0 } ,
    { 0 , 4 , 0 , 0 , 0 , 4 , 0 , 0 , 2 , -2 , 2 , 2 , -2 , 2 , -2 , -2 } 
};
int LAT1[16][16] = {
    { 8 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 } ,
    { 0 , 0 , 0 , 0 , -2 , 2 , 2 , -2 , 4 , 0 , 4 , 0 , 2 , 2 , -2 , -2 } ,
    { 0 , 0 , 0 , 4 , -2 , 2 , -2 , -2 , -2 , 2 , -2 , -2 , 0 , 0 , 0 , -4 } ,
    { 0 , 0 , -4 , 0 , 0 , 0 , 0 , -4 , 2 , 2 , -2 , 2 , -2 , -2 , -2 , 2 } ,
    { 0 , 0 , 0 , 0 , -4 , -4 , 0 , 0 , 0 , 0 , 0 , 0 , -4 , 4 , 0 , 0 } ,
    { 0 , 0 , 0 , 0 , -2 , 2 , -2 , 2 , 0 , -4 , 0 , 4 , -2 , -2 , -2 , -2 } ,
    { 0 , 0 , 0 , 4 , 2 , -2 , -2 , -2 , 2 , -2 , 2 , 2 , 0 , 0 , 4 , 0 } ,
    { 0 , 0 , 4 , 0 , 0 , 0 , 4 , 0 , 2 , 2 , -2 , 2 , -2 , -2 , 2 , -2 } ,
    { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -4 , 4 , 4 , 4 , 0 , 0 , 0 , 0 } ,
    { 0 , 4 , 0 , 4 , -2 , -2 , 2 , 2 , 0 , 0 , 0 , 0 , 2 , -2 , -2 , 2 } ,
    { 0 , -4 , 0 , 0 , -2 , -2 , -2 , 2 , 2 , 2 , 2 , -2 , 0 , -4 , 0 , 0 } ,
    { 0 , 0 , 4 , 0 , 0 , 0 , 0 , -4 , -2 , -2 , 2 , -2 , -2 , -2 , -2 , 2 } ,
    { 0 , 0 , 0 , 0 , 4 , -4 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -4 , -4 } ,
    { 0 , 4 , 0 , -4 , -2 , -2 , -2 , -2 , 0 , 0 , 0 , 0 , 2 , -2 , 2 , -2 } ,
    { 0 , 4 , 0 , 0 , 2 , 2 , -2 , 2 , 2 , 2 , 2 , -2 , -4 , 0 , 0 , 0 } ,
    { 0 , 0 , 4 , 0 , 0 , 0 , -4 , 0 , 2 , 2 , -2 , 2 , 2 , 2 , -2 , 2 } 
};
int LAT2[16][16] = {
    { 8 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 } ,
    { 0 , -2 , 0 , 2 , 4 , 2 , 4 , -2 , 0 , 2 , 0 , -2 , 0 , 2 , 0 , -2 } ,
    { 0 , -2 , 0 , -2 , -2 , 0 , -2 , 0 , 0 , 2 , 4 , -2 , 2 , 0 , -2 , -4 } ,
    { 0 , 0 , -4 , 0 , 2 , -2 , -2 , -2 , 0 , 0 , 0 , -4 , 2 , -2 , 2 , 2 } ,
    { 0 , -4 , 0 , 0 , 0 , -4 , 0 , 0 , 0 , -4 , 0 , 0 , 0 , 4 , 0 , 0 } ,
    { 0 , -2 , 0 , -2 , 0 , -2 , 0 , -2 , 0 , 2 , 0 , 2 , -4 , -2 , 4 , -2 } ,
    { 0 , 2 , 0 , -2 , 2 , 0 , 2 , 4 , 0 , -2 , 4 , -2 , -2 , 0 , 2 , 0 } ,
    { 0 , 0 , 4 , 4 , 2 , -2 , -2 , 2 , 0 , 0 , 0 , 0 , 2 , -2 , 2 , -2 } ,
    { 0 , 0 , 0 , 0 , -4 , 0 , 4 , 0 , 0 , 0 , 0 , 0 , 4 , 0 , 4 , 0 } ,
    { 0 , -2 , 0 , 2 , 0 , 2 , 0 , -2 , 4 , -2 , 4 , 2 , 0 , -2 , 0 , 2 } ,
    { 0 , -2 , 0 , -2 , 2 , 0 , 2 , 0 , -4 , -2 , 0 , 2 , 2 , -4 , -2 , 0 } ,
    { 0 , 0 , 4 , 0 , -2 , -2 , 2 , -2 , 0 , 0 , 0 , -4 , -2 , -2 , -2 , 2 } ,
    { 0 , 4 , 0 , 0 , 0 , 0 , 0 , -4 , 0 , -4 , 0 , 0 , 0 , 0 , 0 , -4 } ,
    { 0 , -2 , 0 , -2 , 0 , 2 , 0 , 2 , 4 , -2 , -4 , -2 , 0 , -2 , 0 , -2 } ,
    { 0 , 2 , 0 , -2 , 2 , -4 , 2 , 0 , 4 , 2 , 0 , 2 , 2 , 0 , -2 , 0 } ,
    { 0 , 0 , 4 , -4 , 2 , 2 , -2 , -2 , 0 , 0 , 0 , 0 , 2 , 2 , 2 , 2 } 
};
int LAT3[16][16] = {
    { 8 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 } ,
    { 0 , 0 , 4 , 4 , 0 , 0 , 0 , 0 , 2 , -2 , -2 , 2 , -2 , 2 , -2 , 2 } ,
    { 0 , 0 , -2 , -2 , 0 , 4 , 2 , -2 , 2 , 2 , 0 , 0 , -2 , 2 , 0 , 4 } ,
    { 0 , -4 , 2 , -2 , 0 , 0 , 2 , 2 , 0 , 0 , 2 , 2 , 0 , 4 , 2 , -2 } ,
    { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -4 , 0 , -4 , 0 , -4 , 0 , 4 , 0 } ,
    { 0 , 0 , 0 , 0 , 0 , 0 , 4 , -4 , -2 , -2 , -2 , -2 , 2 , 2 , -2 , -2 } ,
    { 0 , 0 , -2 , -2 , 0 , -4 , 2 , -2 , 2 , -2 , 0 , 4 , -2 , -2 , 0 , 0 } ,
    { 0 , -4 , -2 , 2 , 0 , 0 , -2 , -2 , 0 , 4 , -2 , 2 , 0 , 0 , -2 , -2 } ,
    { 0 , 0 , -4 , 4 , 0 , 0 , 4 , 4 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 } ,
    { 0 , 0 , 0 , 0 , 4 , 4 , 0 , 0 , 2 , -2 , -2 , 2 , 2 , -2 , 2 , -2 } ,
    { 0 , 0 , 2 , 2 , -4 , 0 , 2 , -2 , 2 , 2 , 0 , 0 , 2 , -2 , 4 , 0 } ,
    { 0 , 4 , -2 , 2 , 0 , 0 , -2 , -2 , 0 , 0 , 2 , 2 , 0 , 4 , 2 , -2 } ,
    { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 4 , 0 , 0 , -4 , -4 , 0 , 0 , -4 } ,
    { 0 , 0 , 0 , 0 , -4 , 4 , 0 , 0 , -2 , -2 , 2 , 2 , -2 , -2 , -2 , -2 } ,
    { 0 , 0 , -2 , -2 , -4 , 0 , -2 , 2 , 2 , -2 , -4 , 0 , 2 , 2 , 0 , 0 } ,
    { 0 , -4 , -2 , 2 , 0 , 0 , -2 , -2 , 0 , -4 , 2 , -2 , 0 , 0 , 2 , 2 } 
};
int LAT4[16][16] = {
    { 8 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 } ,
    { 0 , 0 , -2 , 2 , -4 , -4 , 2 , -2 , 0 , 0 , 2 , -2 , 0 , 0 , 2 , -2 } ,
    { 0 , 0 , -2 , -2 , 2 , 2 , 0 , 0 , 0 , -4 , 2 , -2 , -2 , 2 , 0 , -4 } ,
    { 0 , -4 , 0 , 0 , 2 , -2 , 2 , 2 , 0 , 0 , 0 , 4 , 2 , 2 , 2 , -2 } ,
    { 0 , 0 , 4 , 0 , 0 , 0 , 4 , 0 , 0 , 0 , 4 , 0 , 0 , 0 , -4 , 0 } ,
    { 0 , 0 , -2 , -2 , 0 , 0 , -2 , -2 , 0 , 0 , 2 , 2 , 4 , -4 , -2 , -2 } ,
    { 0 , 0 , 2 , -2 , -2 , -2 , 0 , 4 , 0 , -4 , -2 , -2 , 2 , -2 , 0 , 0 } ,
    { 0 , 4 , 0 , -4 , 2 , -2 , 2 , -2 , 0 , 0 , 0 , 0 , 2 , 2 , 2 , 2 } ,
    { 0 , 0 , 0 , 0 , -4 , 4 , 0 , 0 , 0 , 0 , 0 , 0 , 4 , 4 , 0 , 0 } ,
    { 0 , 0 , -2 , 2 , 0 , 0 , 2 , -2 , -4 , -4 , -2 , 2 , 0 , 0 , -2 , 2 } ,
    { 0 , 0 , -2 , -2 , -2 , -2 , 0 , 0 , 4 , 0 , -2 , 2 , -2 , 2 , -4 , 0 } ,
    { 0 , 4 , 0 , 0 , -2 , 2 , 2 , 2 , 0 , 0 , 0 , 4 , -2 , -2 , 2 , -2 } ,
    { 0 , 0 , -4 , 0 , 0 , 0 , 0 , 4 , 0 , 0 , 4 , 0 , 0 , 0 , 0 , 4 } ,
    { 0 , 0 , -2 , -2 , 0 , 0 , 2 , 2 , -4 , 4 , -2 , -2 , 0 , 0 , -2 , -2 } ,
    { 0 , 0 , 2 , -2 , -2 , -2 , -4 , 0 , -4 , 0 , 2 , 2 , -2 , 2 , 0 , 0 } ,
    { 0 , 4 , 0 , 4 , 2 , -2 , -2 , 2 , 0 , 0 , 0 , 0 , 2 , 2 , -2 , -2 } 
};
int LAT5[16][16] = {
    { 8 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 } ,
    { 0 , 0 , -2 , 2 , 0 , 0 , 2 , -2 , 4 , 4 , 2 , -2 , 0 , 0 , 2 , -2 } ,
    { 0 , 0 , -2 , -2 , 0 , 4 , 2 , -2 , -2 , -2 , 0 , 0 , 2 , -2 , 0 , -4 } ,
    { 0 , -4 , 0 , 0 , 0 , 0 , 0 , -4 , 2 , -2 , -2 , -2 , 2 , 2 , -2 , 2 } ,
    { 0 , 0 , -4 , 0 , 0 , 0 , -4 , 0 , 0 , 0 , -4 , 0 , 0 , 0 , 4 , 0 } ,
    { 0 , 0 , -2 , -2 , 0 , 0 , 2 , 2 , 0 , 0 , -2 , -2 , -4 , 4 , -2 , -2 } ,
    { 0 , 0 , 2 , -2 , 0 , 4 , -2 , -2 , 2 , 2 , 0 , 4 , -2 , 2 , 0 , 0 } ,
    { 0 , 4 , 0 , 4 , 0 , 0 , 0 , 0 , 2 , -2 , -2 , 2 , 2 , 2 , -2 , -2 } ,
    { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -4 , 4 , 0 , 0 , 4 , 4 , 0 , 0 } ,
    { 0 , 0 , -2 , 2 , 4 , 4 , -2 , 2 , 0 , 0 , 2 , -2 , 0 , 0 , -2 , 2 } ,
    { 0 , 0 , -2 , -2 , -4 , 0 , -2 , 2 , 2 , 2 , 0 , 0 , 2 , -2 , -4 , 0 } ,
    { 0 , 4 , 0 , 0 , 0 , 0 , 0 , -4 , -2 , 2 , -2 , -2 , -2 , -2 , -2 , 2 } ,
    { 0 , 0 , 4 , 0 , 0 , 0 , -4 , 0 , 0 , 0 , 0 , -4 , 0 , 0 , 0 , -4 } ,
    { 0 , 0 , -2 , -2 , 4 , -4 , -2 , -2 , 0 , 0 , 2 , 2 , 0 , 0 , -2 , -2 } ,
    { 0 , 0 , 2 , -2 , 4 , 0 , 2 , 2 , 2 , 2 , -4 , 0 , 2 , -2 , 0 , 0 } ,
    { 0 , 4 , 0 , -4 , 0 , 0 , 0 , 0 , 2 , -2 , 2 , -2 , 2 , 2 , 2 , 2 } 
};
int LAT6[16][16] = {
    { 8 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 } ,
    { 0 , 0 , 0 , 0 , 2 , -2 , -2 , 2 , 4 , 0 , 4 , 0 , -2 , -2 , 2 , 2 } ,
    { 0 , 0 , 0 , 4 , 2 , -2 , 2 , 2 , -2 , 2 , -2 , -2 , 0 , 0 , 0 , 4 } ,
    { 0 , 0 , -4 , 0 , 0 , 0 , 0 , 4 , 2 , 2 , -2 , 2 , 2 , 2 , 2 , -2 } ,
    { 0 , 0 , 0 , 0 , -4 , -4 , 0 , 0 , 0 , 0 , 0 , 0 , -4 , 4 , 0 , 0 } ,
    { 0 , 0 , 0 , 0 , -2 , 2 , -2 , 2 , 0 , 4 , 0 , -4 , -2 , -2 , -2 , -2 } ,
    { 0 , 0 , 0 , -4 , 2 , -2 , -2 , -2 , -2 , 2 , -2 , -2 , 0 , 0 , 4 , 0 } ,
    { 0 , 0 , -4 , 0 , 0 , 0 , 4 , 0 , -2 , -2 , 2 , -2 , -2 , -2 , 2 , -2 } ,
    { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -4 , 4 , 4 , 4 , 0 , 0 , 0 , 0 } ,
    { 0 , 4 , 0 , 4 , 2 , 2 , -2 , -2 , 0 , 0 , 0 , 0 , -2 , 2 , 2 , -2 } ,
    { 0 , -4 , 0 , 0 , 2 , 2 , 2 , -2 , 2 , 2 , 2 , -2 , 0 , 4 , 0 , 0 } ,
    { 0 , 0 , 4 , 0 , 0 , 0 , 0 , 4 , -2 , -2 , 2 , -2 , 2 , 2 , 2 , -2 } ,
    { 0 , 0 , 0 , 0 , 4 , -4 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -4 , -4 } ,
    { 0 , -4 , 0 , 4 , -2 , -2 , -2 , -2 , 0 , 0 , 0 , 0 , 2 , -2 , 2 , -2 } ,
    { 0 , -4 , 0 , 0 , 2 , 2 , -2 , 2 , -2 , -2 , -2 , 2 , -4 , 0 , 0 , 0 } ,
    { 0 , 0 , -4 , 0 , 0 , 0 , -4 , 0 , -2 , -2 , 2 , -2 , 2 , 2 , -2 , 2 } 
};

int LAT7[16][16] = {
    { 8 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 } ,
    { 0 , 0 , 0 , 0 , -2 , 2 , 2 , -2 , -4 , 0 , -4 , 0 , 2 , 2 , -2 , -2 } ,
    { 0 , 0 , 0 , -4 , -2 , 2 , -2 , -2 , 2 , -2 , 2 , 2 , 0 , 0 , 0 , -4 } ,
    { 0 , 0 , -4 , 0 , 0 , 0 , 0 , 4 , 2 , 2 , -2 , 2 , 2 , 2 , 2 , -2 } ,
    { 0 , 0 , 0 , 0 , 4 , 4 , 0 , 0 , 0 , 0 , 0 , 0 , 4 , -4 , 0 , 0 } ,
    { 0 , 0 , 0 , 0 , -2 , 2 , -2 , 2 , 0 , 4 , 0 , -4 , -2 , -2 , -2 , -2 } ,
    { 0 , 0 , 0 , -4 , 2 , -2 , -2 , -2 , -2 , 2 , -2 , -2 , 0 , 0 , 4 , 0 } ,
    { 0 , 0 , 4 , 0 , 0 , 0 , -4 , 0 , 2 , 2 , -2 , 2 , 2 , 2 , -2 , 2 } ,
    { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -4 , 4 , 4 , 4 , 0 , 0 , 0 , 0 } ,
    { 0 , -4 , 0 , -4 , -2 , -2 , 2 , 2 , 0 , 0 , 0 , 0 , 2 , -2 , -2 , 2 } ,
    { 0 , 4 , 0 , 0 , -2 , -2 , -2 , 2 , -2 , -2 , -2 , 2 , 0 , -4 , 0 , 0 } ,
    { 0 , 0 , 4 , 0 , 0 , 0 , 0 , 4 , -2 , -2 , 2 , -2 , 2 , 2 , 2 , -2 } ,
    { 0 , 0 , 0 , 0 , -4 , 4 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 4 , 4 } ,
    { 0 , -4 , 0 , 4 , -2 , -2 , -2 , -2 , 0 , 0 , 0 , 0 , 2 , -2 , 2 , -2 } ,
    { 0 , -4 , 0 , 0 , 2 , 2 , -2 , 2 , -2 , -2 , -2 , 2 , -4 , 0 , 0 , 0 } ,
    { 0 , 0 , 4 , 0 , 0 , 0 , 4 , 0 , 2 , 2 , -2 , 2 , -2 , -2 , 2 , -2 } 
};






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
		filename = "lblock"+to_string(atoi(argv[3]))+".cvc";
    }
    printf("key_flag=%d , head_flag=%d , tail_flag=%d\n fileno = %d",key_flag,head_flag,tail_flag,tail_flag);
	
	//program main
	ofstream outcvc;
    int x_ROUND = 8;
	int y_ROUND = 8;
	int ROUND = x_ROUND+y_ROUND;





    
	//gen CVC
	outcvc.open(filename);
//variable claim

	outcvc<<"LAT0 : ARRAY BITVECTOR(8) OF BITVECTOR(1);"<<endl;
	for(int input_lc=0x0;input_lc<16;input_lc++)
	{
		for(int output_lc=0x0;output_lc<16;output_lc++)
		{
			outcvc<<hex<<"ASSERT( LAT0[0bin"<<tobits(input_lc,4)<<tobits(output_lc,4)<<"] = 0bin"<<((LAT0[input_lc][output_lc] == 0)?0:1)<<" );"<<endl;
		}
	}
	outcvc<<dec;

	outcvc<<"LAT1 : ARRAY BITVECTOR(8) OF BITVECTOR(1);"<<endl;
	for(int input_lc=0x0;input_lc<16;input_lc++)
	{
		for(int output_lc=0x0;output_lc<16;output_lc++)
		{
			outcvc<<hex<<"ASSERT( LAT1[0bin"<<tobits(input_lc,4)<<tobits(output_lc,4)<<"] = 0bin"<<((LAT1[input_lc][output_lc] == 0)?0:1)<<" );"<<endl;
		}
	}
	outcvc<<dec;

	outcvc<<"LAT2 : ARRAY BITVECTOR(8) OF BITVECTOR(1);"<<endl;
	for(int input_lc=0x0;input_lc<16;input_lc++)
	{
		for(int output_lc=0x0;output_lc<16;output_lc++)
		{
			outcvc<<hex<<"ASSERT( LAT2[0bin"<<tobits(input_lc,4)<<tobits(output_lc,4)<<"] = 0bin"<<((LAT2[input_lc][output_lc] == 0)?0:1)<<" );"<<endl;
		}
	}
	outcvc<<dec;

	outcvc<<"LAT3 : ARRAY BITVECTOR(8) OF BITVECTOR(1);"<<endl;
	for(int input_lc=0x0;input_lc<16;input_lc++)
	{
		for(int output_lc=0x0;output_lc<16;output_lc++)
		{
			outcvc<<hex<<"ASSERT( LAT3[0bin"<<tobits(input_lc,4)<<tobits(output_lc,4)<<"] = 0bin"<<((LAT3[input_lc][output_lc] == 0)?0:1)<<" );"<<endl;
		}
	}
	outcvc<<dec;

	outcvc<<"LAT4 : ARRAY BITVECTOR(8) OF BITVECTOR(1);"<<endl;
	for(int input_lc=0x0;input_lc<16;input_lc++)
	{
		for(int output_lc=0x0;output_lc<16;output_lc++)
		{
			outcvc<<hex<<"ASSERT( LAT4[0bin"<<tobits(input_lc,4)<<tobits(output_lc,4)<<"] = 0bin"<<((LAT4[input_lc][output_lc] == 0)?0:1)<<" );"<<endl;
		}
	}
	outcvc<<dec;

	outcvc<<"LAT5 : ARRAY BITVECTOR(8) OF BITVECTOR(1);"<<endl;
	for(int input_lc=0x0;input_lc<16;input_lc++)
	{
		for(int output_lc=0x0;output_lc<16;output_lc++)
		{
			outcvc<<hex<<"ASSERT( LAT5[0bin"<<tobits(input_lc,4)<<tobits(output_lc,4)<<"] = 0bin"<<((LAT5[input_lc][output_lc] == 0)?0:1)<<" );"<<endl;
		}
	}
	outcvc<<dec;

	outcvc<<"LAT6 : ARRAY BITVECTOR(8) OF BITVECTOR(1);"<<endl;
	for(int input_lc=0x0;input_lc<16;input_lc++)
	{
		for(int output_lc=0x0;output_lc<16;output_lc++)
		{
			outcvc<<hex<<"ASSERT( LAT6[0bin"<<tobits(input_lc,4)<<tobits(output_lc,4)<<"] = 0bin"<<((LAT6[input_lc][output_lc] == 0)?0:1)<<" );"<<endl;
		}
	}
	outcvc<<dec;

	outcvc<<"LAT7 : ARRAY BITVECTOR(8) OF BITVECTOR(1);"<<endl;
	for(int input_lc=0x0;input_lc<16;input_lc++)
	{
		for(int output_lc=0x0;output_lc<16;output_lc++)
		{
			outcvc<<hex<<"ASSERT( LAT7[0bin"<<tobits(input_lc,4)<<tobits(output_lc,4)<<"] = 0bin"<<((LAT7[input_lc][output_lc] == 0)?0:1)<<" );"<<endl;
		}
	}
	outcvc<<dec;

//variable claim
	//forward state variable claim
    for(int round=0;round<x_ROUND;round++)
    {
        for(int pos=0;pos<8;pos++)
        {
            outcvc<<"x_Lin_"<<round<<"_"<<pos<<" , x_Rin_"<<round<<"_"<<pos<<" , x_Sin_"<<round<<"_"<<pos<<" , x_Sout_"<<round<<"_"<<pos<<" , x_Rot_"<<round<<"_"<<pos<<" , x_Pin_"<<round<<"_"<<pos;
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
    //backward state variable claim
    for(int round=0;round<y_ROUND;round++)
    {
        for(int pos=0;pos<8;pos++)
        {
            outcvc<<"y_Lin_"<<round<<"_"<<pos<<" , y_Rin_"<<round<<"_"<<pos<<" , y_Sin_"<<round<<"_"<<pos<<" , y_Sout_"<<round<<"_"<<pos<<" , y_Rot_"<<round<<"_"<<pos<<" , y_Pin_"<<round<<"_"<<pos;
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
    //key state claim 
    for(int round=0;round<ROUND;round++)
    {
        for(int pos=0;pos<20;pos++)
        {
            outcvc<<"Kin_"<<round<<"_"<<pos<<" , Rin_"<<round<<"_"<<pos<<" , Rout_"<<round<<"_"<<pos<<" , Sout_"<<round<<"_"<<pos<<" , Kout_"<<round<<"_"<<pos;
            if(pos<19)
            {
                outcvc<<" , ";
            }
            else
            {
                outcvc<<" : BITVECTOR(4);"<<endl;
            }
            
        }
    }
    for(int pos=0;pos < 20;pos++)
    {
        outcvc<<"Kin_"<<ROUND<<"_"<<pos<<" : BITVECTOR(4);"<<endl;
    }

    //AASERT 
    //state forward update
    for(int round=0;round<x_ROUND;round++)
    {
        for(int pos=0;pos<8;pos++)
        {   
            if(round<x_ROUND-1)
            {    
                string a = "x_Lin_"+to_string(round)+"_"+to_string(pos);
                string b = "x_Sin_"+to_string(round)+"_"+to_string(pos);
                outcvc<<"ASSERT( x_Rin_"<<(round+1)<<"_"<<pos<<" = "<<branch(a,b)<<" );"<<endl;
                outcvc<<"ASSERT( x_Lin_"<<(round+1)<<"_"<<pos<<" = x_Rot_"<<round<<"_"<<pos<<" );"<<endl;
            }
            outcvc<<"ASSERT( x_Pin_"<<round<<"_"<<p[pos]<<" = x_Sout_"<<round<<"_"<<pos<<" );"<<endl;
            outcvc<<"ASSERT( x_Rot_"<<round<<"_"<<rot[pos]<<" = x_Rin_"<<round<<"_"<<pos<<" );"<<endl;
            outcvc<<"ASSERT( x_Pin_"<<round<<"_"<<pos<<" = x_Rot_"<<round<<"_"<<pos<<" );"<<endl;
            //pass Sbox
            outcvc<<"ASSERT( NOT( LAT"<<pos<<"[x_Sin_"<<round<<"_"<<pos<<"@x_Sout_"<<round<<"_"<<pos<<"] = 0bin0 ) );"<<endl;

            //key XOR
            outcvc<<"ASSERT( x_Sin_"<<round<<"_"<<pos<<" = Kout_"<<round<<"_"<<pos<<" );"<<endl;

        }
    }
    //state backward update
    for(int round=0;round<y_ROUND;round++)
    {
        for(int pos=0;pos<8;pos++)
        {   
            if(round<y_ROUND-1)
            {    
                string a = "y_Lin_"+to_string(round)+"_"+to_string(pos);
                string b = "y_Sin_"+to_string(round)+"_"+to_string(pos);
                outcvc<<"ASSERT( y_Rin_"<<(round+1)<<"_"<<pos<<" = "<<branch(a,b)<<" );"<<endl;
                outcvc<<"ASSERT( y_Lin_"<<(round+1)<<"_"<<pos<<" = y_Rot_"<<round<<"_"<<pos<<" );"<<endl;
            }
            outcvc<<"ASSERT( y_Pin_"<<round<<"_"<<p[pos]<<" = y_Sout_"<<round<<"_"<<pos<<" );"<<endl;
            outcvc<<"ASSERT( y_Rot_"<<round<<"_"<<rot[pos]<<" = y_Rin_"<<round<<"_"<<pos<<" );"<<endl;
            outcvc<<"ASSERT( y_Pin_"<<round<<"_"<<pos<<" = y_Rot_"<<round<<"_"<<pos<<" );"<<endl;
            //pass Sbox
            outcvc<<"ASSERT( NOT( LAT"<<pos<<"[y_Sin_"<<round<<"_"<<pos<<"@y_Sout_"<<round<<"_"<<pos<<"] = 0bin0 ) );"<<endl;

            //key XOR
            outcvc<<"ASSERT( y_Sin_"<<round<<"_"<<pos<<" = Kout_"<<x_ROUND+round<<"_"<<pos<<" );"<<endl;

        }
    }
    //last round state
    for(int pos=0;pos<8;pos++)
    {
        outcvc<<"y_Lin_"<<y_ROUND<<"_"<<pos<<" , y_Rin_"<<y_ROUND<<"_"<<pos;
        if(pos<7)
        {
            outcvc<<" , ";
        }
        else
        {
            outcvc<<" : BITVECTOR(4);"<<endl;
        }   
     
    }
    for(int pos=0;pos<8;pos++)
    {
        string a = "y_Lin_"+to_string(y_ROUND-1)+"_"+to_string(pos);
        string b = "y_Sin_"+to_string(y_ROUND-1)+"_"+to_string(pos);
        outcvc<<"ASSERT( y_Rin_"<<y_ROUND<<"_"<<pos<<" = "<<branch(a,b)<<" );"<<endl;       
        outcvc<<"ASSERT( y_Rot_"<<y_ROUND-1<<"_"<<pos<<" = y_Lin_"<<y_ROUND<<"_"<<pos<<" );"<<endl; 
    }
    //lianjie state
    for(int pos=0;pos<8;pos++)
    {
        string a = "x_Lin_"+to_string(x_ROUND-1)+"_"+to_string(pos);
        string b = "x_Sin_"+to_string(x_ROUND-1)+"_"+to_string(pos);
        outcvc<<"ASSERT( y_Rin_0"<<"_"<<pos<<" = "<<branch(a,b)<<" );"<<endl;
        outcvc<<"ASSERT( y_Lin_0"<<"_"<<pos<<" = x_Rot_"<<(x_ROUND-1)<<"_"<<pos<<" );"<<endl;    
    }


    //key schedule update
    for(int round=0;round<ROUND;round++)
    {
        for(int pos=0;pos<20;pos++)
        {
            //key pass branch

            if(pos < 8)
            {
                string a = "Kout_"+to_string(round)+"_"+to_string(pos);
                string b = "Sout_"+to_string(round)+"_"+to_string(pos);
                outcvc<<"ASSERT( Kin_"<<round+1<<"_"<<pos<<" = "<<branch(a,b)<<" );"<<endl;
            }
            else
            {
                outcvc<<"ASSERT( Sout_"<<round<<"_"<<pos<<" = Kin_"<<(round+1)<<"_"<<pos<<" );"<<endl;
            }

            //key pass Sbox            
            if(pos == 0)
            {                
                outcvc<<"ASSERT( NOT( LAT1[Rout_"<<round<<"_"<<pos<<"@Sout_"<<round<<"_"<<pos<<"] = 0bin0 ) );"<<endl;  
            }
            else if(pos == 1)
            {
                outcvc<<"ASSERT( NOT( LAT0[Rout_"<<round<<"_"<<pos<<"@Sout_"<<round<<"_"<<pos<<"] = 0bin0 ) );"<<endl;  
            }
            else
            {
                outcvc<<"ASSERT( Rout_"<<round<<"_"<<pos<<" = Sout_"<<round<<"_"<<pos<<" );"<<endl;
            }
            
            //key pass Rotate

            outcvc<<"ASSERT( Rin_"<<round<<"_"<<krot[pos]<<" = Kin_"<<round<<"_"<<pos<<" );"<<endl;
            if(pos == 0)
            {
                for(int i=0;i<4;i++)
                {
                    if(i == 3)
                    {
                        outcvc<<"ASSERT( Rout_"<<round<<"_19"<<"[0:0] = Rin_"<<round<<"_"<<pos<<"[3:3] );"<<endl;
                    }
                    else
                    {
                        outcvc<<"ASSERT( Rout_"<<round<<"_"<<pos<<"["<<i+1<<":"<<i+1<<"] = Rin_"<<round<<"_"<<pos<<"["<<i<<":"<<i<<"] );"<<endl;                        
                    }
                    
                }   
            }
            else
            {
                for(int i=0;i<4;i++)
                {
                    if(i == 3)
                    {
                        outcvc<<"ASSERT( Rout_"<<round<<"_"<<(pos-1)<<"[0:0] = Rin_"<<round<<"_"<<pos<<"[3:3] );"<<endl;
                    }
                    else
                    {
                        outcvc<<"ASSERT( Rout_"<<round<<"_"<<pos<<"["<<i+1<<":"<<i+1<<"] = Rin_"<<round<<"_"<<pos<<"["<<i<<":"<<i<<"] );"<<endl;                        
                    }
                    
                }                
            }
            



            
        }
    }
//set key condition
	//assert active state
	for(int pos=0;pos<20;pos++)
	{
		if(pos<8)
		{
            if (head_flag < 8)
            {
                if(pos != head_flag)
                {
                    outcvc<<"ASSERT( x_Lin_0_"<<pos<<" = 0bin0000  );"<<endl;
                }
                outcvc<<"ASSERT( x_Rin_0_"<<pos<<" = 0bin0000  );"<<endl;
            }
            else
            {
                if( pos != (head_flag-8) )
                {
                    outcvc<<"ASSERT( x_Rin_0_"<<pos<<" = 0bin0000  );"<<endl;
                }
                outcvc<<"ASSERT( x_Lin_0_"<<pos<<" = 0bin0000  );"<<endl;                
            }
            
            if (tail_flag < 8)
            {
                if(pos == tail_flag)
                {
                    outcvc<<"ASSERT( NOT( y_Lin_"<<y_ROUND<<"_"<<pos<<" = 0bin0000 ) );"<<endl;
                }
                else
                {
                    outcvc<<"ASSERT( y_Lin_"<<y_ROUND<<"_"<<pos<<" = 0bin0000 );"<<endl;
                }	
                outcvc<<"ASSERT( y_Rin_"<<y_ROUND<<"_"<<pos<<" = 0bin0000 );"<<endl;
            }
            else
            {
                if( pos == (tail_flag-8) )
                {
                    outcvc<<"ASSERT( NOT( y_Rin_"<<y_ROUND<<"_"<<pos<<" = 0bin0000 ) );"<<endl;
                }
                else
                {
                    outcvc<<"ASSERT( y_Rin_"<<y_ROUND<<"_"<<pos<<" = 0bin0000 );"<<endl;
                }	
                outcvc<<"ASSERT( y_Lin_"<<y_ROUND<<"_"<<pos<<" = 0bin0000 );"<<endl;                
            }
            
			
		}

        
		if(pos == key_flag)
		{
			outcvc<<"ASSERT( Kin_0_"<<pos<<"[3:3] = 0bin0 );"<<endl;
		}
        

		outcvc<<"ASSERT( Kin_"<<ROUND<<"_"<<pos<<" = 0bin0000 );"<<endl;
        

	}



  	
	outcvc<<"QUERY(FALSE);"<<endl;
	outcvc<<"COUNTEREXAMPLE;"<<endl;
	
	outcvc.close();
    return 0;
}
