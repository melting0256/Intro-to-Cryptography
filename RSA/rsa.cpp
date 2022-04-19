#pragma comment(lib,"cryptlib.lib")

#include<iostream>
#include<fstream>
#include "CryptoPP/rsa.h"
#include "CryptoPP/osrng.h"

using namespace std;
using namespace CryptoPP;

int main() {
	ofstream Output;
	Output.open("out.txt");

	//Encrypt
	RSA::PublicKey pubkey[3];

	Integer e1("0x11"), n1("0xb14022eef719f1bb");
	pubkey[0].Initialize(n1, e1);
	string mesg1 = "Alice";
	int keylen1 = 64;
	if (mesg1.size() <= (keylen1 / 8)) {
		Integer M((const byte*)mesg1.data(), mesg1.size());
		Integer C = pubkey[0].ApplyFunction(M);
		std::string s = IntToString(C, 16);
		//cout << hex << s<< endl;
	}


	Integer e2("0x11"), n2("0x04823f9fe38141d93f1244be161b20f");
	pubkey[1].Initialize(n2, e2);
	string mesg2 = "Hello World!";
	int keylen2 = 128;
	if (mesg2.size() <= (keylen2 / 8)) {
		Integer M((const byte*)mesg2.data(), mesg2.size());
		Integer C = pubkey[1].ApplyFunction(M);
		std::string s = IntToString(C, 16);
		Output << hex << s << endl;
	}

	Integer e3("0x10001"), n3("0x9711ea5183d50d6a91114f1d7574cd52621b35499b4d3563ec95416a994099c9");
	pubkey[2].Initialize(n3, e3);
	string mesg3("RSA is public key.");
	int keylen3 = 256;
	if (mesg3.size() <= (keylen3 / 8)) {
		Integer M((const byte*)mesg3.data(), mesg3.size());
		Integer C = pubkey[2].ApplyFunction(M);
		std::string s = IntToString(C, 16);
		Output << hex << s << endl;
	}

	//Decrypt
	RSA::PrivateKey prvkey[2];
	AutoSeededRandomPool prng;
	Integer e4("0x11"), n4("0xc45350fa19fa8d93"), d4("0x454a950c5bcbaa41");
	Integer C("0xa4a59490b843eea0"), M;
	prvkey[0].Initialize(n4, e4, d4);
	M = prvkey[0].CalculateInverse(prng, C);
	string mesg;
	mesg.resize(M.MinEncodedSize());
	M.Encode((byte*)mesg.data(), mesg.size());
	//cout << "1 " << mesg << endl;

	Integer e5("0x1d35"), n5("0xc4b361851de35f080d3ca7352cbf372d");
	Integer Cipher("0xa02d51d0e87efe1defc19f3ee899c31d"), M1;
	Integer sub_d5("0x53a0a95b089cf23adb5cc73f0700000");
	for (int i = 0; i < 1048575; i++) {
		string mesg;
		try {
			prvkey[1].Initialize(n5, e5, sub_d5);
			M1 = prvkey[1].CalculateInverse(prng, Cipher);
			mesg.resize(M1.MinEncodedSize());
			M1.Encode((byte*)mesg.data(), mesg.size());
			//cout << "2 " << mesg << endl;
			std::string s = IntToString(sub_d5, 16);
			Output << hex << s << endl;
			Output << mesg << endl;
			break;
		}
		catch (...) {}
		sub_d5++;
	}
	return 0;
}