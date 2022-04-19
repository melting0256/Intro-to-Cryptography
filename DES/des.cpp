#include <iostream>
#include <fstream>
#include <string>
#include<bitset>
#include <sstream>
#include <cmath>
using namespace std;

bitset<64>key;
bitset<48>sub_key[16];

int ip[64] = { 58, 50, 42, 34, 26, 18, 10, 2,
			 60, 52, 44, 36, 28, 20, 12, 4,
			 62, 54, 46, 38, 30, 22, 14, 6,
			 64, 56, 48, 40, 32, 24, 16, 8,
			 57, 49, 41, 33, 25, 17, 9,  1,
			 59, 51, 43, 35, 27, 19, 11, 3,
			 61, 53, 45, 37, 29, 21, 13, 5,
			 63, 55, 47, 39, 31, 23, 15, 7 };//initial permutation
int pc1[56] = {57, 49, 41, 33, 25, 17, 9,
			   1, 58, 50, 42, 34, 26, 18,
			  10,  2, 59, 51, 43, 35, 27,
			  19, 11,  3, 60, 52, 44, 36,
			  63, 55, 47, 39, 31, 23, 15,
			   7, 62, 54, 46, 38, 30, 22,
			  14,  6, 61, 53, 45, 37, 29,
			  21, 13,  5, 28, 20, 12,  4}; //key 64 bits to 56 bits
int pc2[48] = { 14, 17, 11, 24, 1, 5,
                3, 28, 15, 6, 21, 10,
                23, 19, 12, 4, 26, 8,
                16, 7, 27, 20, 13, 2,
                41, 52, 31, 37, 47, 55,
                30, 40, 51, 45, 33, 48,
                44, 49, 39, 56, 34, 53,
                46, 42, 50, 36, 29, 32 };//key 56 to 48 bits
int key_shift[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};// left shift
int expand[48] =  { 32, 1, 2, 3, 4, 5, 4, 5,
                    6, 7, 8, 9, 8, 9, 10, 11,
                    12, 13, 12, 13, 14, 15, 16, 17,
                    16, 17, 18, 19, 20, 21, 20, 21,
                    22, 23, 24, 25, 24, 25, 26, 27,
                    28, 29, 28, 29, 30, 31, 32, 1 };//32 bits to 48 bits
int s_box[8][4][16] = {
	{
		{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
		{0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
		{4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
		{15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}
	},//s_box1
	{
		{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
		{3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
		{0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
		{13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}
	}, //s_box2
	{
		{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
		{13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
		{13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
		{1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}
	}, //s_box3
	{
		{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
		{13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
		{10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
		{3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}
	},//s_box4
	{
		{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
		{14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
		{4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
		{11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}
	},//s_box5
	{
		{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
		{10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
		{9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
		{4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}
	}, //s_box6
	{
		{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
		{13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
		{1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
		{6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}
	}, //s_box7
	{
		{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
		{1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
		{7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
		{2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
	} //s_box8
};
int p[32] = { 16, 7, 20, 21,
            29, 12, 28, 17,
            1, 15, 23, 26,
            5, 18, 31, 10,
            2, 8, 24, 14,
            32, 27, 3, 9,
            19, 13, 30, 6,
            22, 11, 4, 25 };
int ip_1[64] = { 40, 8, 48, 16, 56, 24, 64, 32,
                39, 7, 47, 15, 55, 23, 63, 31,
                38, 6, 46, 14, 54, 22, 62, 30,
                37, 5, 45, 13, 53, 21, 61, 29,
                36, 4, 44, 12, 52, 20, 60, 28,
                35, 3, 43, 11, 51, 19, 59, 27,
                34, 2, 42, 10, 50, 18, 58, 26,
                33, 1, 41, 9, 49, 17, 57, 25 };//final permutation




bitset<32> f_function(bitset<32> right,bitset<48>key){
    bitset<48> expand_right;

    for(int i=0;i<48;i++){
        expand_right[47-i]=right[32-expand[i]];

    }

    expand_right=expand_right^key;//xor;

    bitset<32> output;
    bitset<32> tmp;
	int x = 0;
    for(int i=0; i<8; i++)
	{
		int row = expand_right[47-(i*6)]*2 + expand_right[47-(i*6+5)];
		int col = expand_right[47-(i * 6 + 1)]*8 + expand_right[47-(i * 6 + 2)]*4 + expand_right[47-(i * 6 + 3)]*2+ expand_right[47-(i * 6 + 4)];
		int num = s_box[i][row][col];
		bitset<4> binary(num);
		for(int j = 0; j < 4; j++)
            tmp[31 - (i * 4 + j)] = binary[3 - j];
	}
	for(int i=0; i<32; i++)
		output[31-i] = tmp[32-p[i]];

	return output;
}
bitset<28>key_leftshift(bitset<28>key,int lshift){
bitset<28>temp=key;
for(int i = 0; i < 28; i++)
    key[i] = temp[((i - lshift) % 28 + 28) % 28];
return key;
}
bitset<56> shift_LR(bitset<56> realkey,const int round){
	bitset<28> left;
	bitset<28> right;
	for(int i=0;i<28;i++){
		left[i] = realkey[i];
	}
	for(int i=0;i<28;i++){
		right[i]=realkey[i+28];
	}
	left=key_leftshift(left,key_shift[round]);
	right=key_leftshift(right,key_shift[round]);
	for(int i=0;i<28;i++){
		realkey[i]=left[i];
	}
	for(int i=0;i<28;i++){
		realkey[i+28]=right[i];
	}
	return realkey;
}
void getkey(){
	bitset<56> realkey;
	bitset<48> compressKey;
	for(int i=0;i<56;i++){
		realkey[55-i]=key[64-pc1[i]];
	}
	for(int round=0;round<16;++round){
		realkey=shift_LR(realkey,round);
		for(int i=0;i<48;i++){
			compressKey[47-i]=realkey[56-pc2[i]];
			sub_key[round]=compressKey;
		}
	}
}
bitset<64> StringToBitset(string s)
{
    bitset<64> output;
	for(int i=0; i<8; i++){
        bitset<8> b(s[i]);
        for(int j = 0; j < 8; j++)
            output[63 - (i * 8 + j)] = b[7 - j];
    }
    return output;
}

bitset<64> HexStringToBitset(string s){
  stringstream ss;
  long long unsigned int temp;
  ss << hex << s;
  ss >> temp;
  bitset<64> bit(temp);
  return bit;
}

void PrintBitsetToHex(bitset<64> Bits, ostream& out){
    for(int i = 0; i < 8; i++){
        bitset<8> tmp;
        for(int j = 0; j < 8; j++)
            tmp[7 - j] = Bits[63 - (i * 8 + j)];
        out << hex << tmp.to_ulong();
    }
    out << endl;
}

void PrintBitsetToString(bitset<64> Bits, ostream& out){
  for(int i = 0; i < 8; i++){
    int num = 0;
    for(int j = 0; j < 8; j++)
      if(Bits[63 - (i * 8 + j)])
        num += pow(2, 7 - j);
    out << (char)num;
    }
    out << endl;
}
bitset<64> encrypt(bitset<64>plain)
{
	bitset<64> cipher;
	bitset<64> currentBits;
	bitset<32> left;
	bitset<32> right;
	bitset<32> newLeft;
	for(int i=0; i<64; i++)
		currentBits[63-i] = plain[64-ip[i]];
	for(int i=32; i<64; i++)
		left[i-32] = currentBits[i];
	for(int i=0; i<32; i++)
		right[i] = currentBits[i];
	for(int round=0; round<16; round++)
	{
		newLeft = right;
		right = left ^ f_function(right,sub_key[round]);
		left = newLeft;
	}

	for(int i=0; i<32; i++)
		cipher[i] = left[i];
	for(int i=32; i<64; i++)
		cipher[i] = right[i-32];
	currentBits = cipher;
	for(int i=0; i<64; i++)
		cipher[63-i] = currentBits[64-ip_1[i]];
	return cipher;
}
bitset<64> decrypt(bitset<64>cipher)
{
	bitset<64> plain;
	bitset<64> currentBits;
	bitset<32> left;
	bitset<32> right;
	bitset<32> newLeft;
	for(int i=0; i<64; i++)
		currentBits[63-i] = cipher[64-ip[i]];
	for(int i=32; i<64; i++)
		left[i-32] = currentBits[i];
	for(int i=0; i<32; i++)
		right[i] = currentBits[i];
	for(int round=0; round<16; round++)
	{
		newLeft = right;
		right = left ^ f_function(right,sub_key[15-round]);
		left = newLeft;
	}
	for(int i=0; i<32; i++)
		plain[i] = left[i];
	for(int i=32; i<64; i++)
		plain[i] = right[i-32];
	currentBits = plain;
	for(int i=0; i<64; i++)
		plain[63-i] = currentBits[64-ip_1[i]];

	return plain;
}
int main() {
string cipher;
string plain;
string k;//key;
string file;
ifstream input;
ofstream output;
input.open("DES-Key-Ciphertext.txt");
output.open("des-out.txt");
while(input>>k>>cipher){
    key=StringToBitset(k);

    getkey();
    PrintBitsetToString(decrypt(HexStringToBitset(cipher)),output);
}
/*input.close();
output.close();
input.open("DES-Key-Plaintext.txt");
output.open("DES-Plaintext-Out.txt");
while(getline(input,file)){
    stringstream f(file);
    plain.clear();
    k.clear();
    char k_tmp, p_tmp;
		for(int i = 0; i < 8; i++)
			f >> noskipws >> k_tmp, k += k_tmp;

		f >> noskipws >> k_tmp;

		for(int i = 0; i < 8; i++)
			f>> noskipws >> p_tmp, plain += p_tmp;
        key=StringToBitset(k);
        getkey();
        PrintBitsetToHex(encrypt(StringToBitset(plain)),output);
}*/


    return 0;
}
