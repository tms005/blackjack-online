#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>

using namespace std;


void show_bit_int( int* z	);

void bbbIntToChar(	int* source,
								char* destination);

void bbbCharToInt(	char * source,
								int* destination);

void show_bit_char(	char* z );


int  _2TbbbCharToInt(	char* source,
								int* destination,
								int s_len,
								int d_len);

int  _2TbbbIntToChar(	int* source,
								char* destination,
								int s_len,
								int d_len);


int main(void)
{
int i[3]; // lub inne dowolne

i[0]=0xF32D9016;
i[1]=0x011000FF;
i[1]=0x000da000;

show_bit_int(&i[0]);
cout<<endl;
show_bit_int(&i[1]);
cout<<endl;
show_bit_int(&i[2]);
cout<<endl;
cout<<endl<<endl;

char z[12];
int h[3];
_2TbbbIntToChar(	i,z, 3, 12);

/*
bbbIntToChar(&i[0], &z[0]); 
bbbIntToChar(&i[1], &z[4]);
bbbIntToChar(&i[2], &z[8]);
*/

cout<<"Int to Char:"<<endl;
int a,b;

for( a =0;a<8+1;a+=4){
for( b =3;b>-1;b--){

show_bit_char(&z[a+b]);
//cout<<"\n"<<a+b<<"\n"<<endl;
if((a+b)%4==0) cout<<endl;
}}
cout<<"Char to Int:"<<endl;
_2TbbbCharToInt(	z,h, 12, 3);

show_bit_int(h);
cout<<endl;
show_bit_int(h+1);
cout<<endl;
show_bit_int(h+2);

cout<<endl;
system("PAUSE");
return 0;
}




void bbbIntToChar(int* source,
							char* destination)
{// zalozenie: source to wskaznik na pojedynczy int, zas destination to wskaznik na 4 znaki
for(int x=0;x<4;x++) *(destination+x) =0; // czyszczê przestrzeñ docelow¹
int i;
unsigned int smask=1;
unsigned char dmask;
for(i=0;i<4;i++)
	{
		dmask=1;
		for(int k=0;k<8;k++)
		{
			if((*source) & smask) *(destination+i) |=dmask;
			dmask<<=1;
			smask<<=1;
		}
	}
}

void bbbCharToInt(	char* source,
								int* destination)
{
	int smask,dmask=1;
	*destination=0;
	for(int i=3;i>-1;i--) 
	{
		smask=1;
		for(int j=0;j<8;j++)
		{
			if( *(source+3-i) &smask) *destination |=dmask;
			dmask<<=1;
			smask<<=1;
		}
	}
}


int  _2TbbbCharToInt(	char* source,
								int* destination,
								int s_len,
								int d_len)
{																		//zwraca 0 jesli ok -1 jesli blad
	if(s_len== d_len*(sizeof(int)/sizeof(char)) )
	{
	int i;
	for(i=0;i<d_len;i++) 
		{
		bbbCharToInt(source+(i*(sizeof(int)/sizeof(char))), destination+i);

		}
	return 0;
	}
	else return -1;
}

int  _2TbbbIntToChar(	int* source,
								char* destination,
								int s_len,
								int d_len)
{
	if(d_len== s_len*(sizeof(int)/sizeof(char)) )
	{
		int i;
		for(i=0;i<s_len;i++) 
			{
			bbbIntToChar(source+i, destination+(i*(sizeof(int)/sizeof(char))));
			}
	return 0;
	}
return -1;
}



void show_bit_char(char* z)
{
	unsigned char mask=0x80;
	for(int i=0;i<sizeof(char)*8;i++) 
		{
		if(mask & *z) cout<<"1";
		else cout<<"0";
		mask>>=1;
		}

}


void show_bit_int(int* z)
{
	unsigned int mask=0x80000000;
	for(int i=0;i<sizeof(int)*8;i++) 
		{
		if(mask & *z) cout<<"1";
		else cout<<"0";
		mask>>=1;
		}

}
