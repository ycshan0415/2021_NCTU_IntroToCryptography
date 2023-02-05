#include <iostream>
#include <fstream>
#include <sha.h>
#include <hex.h>
using namespace std;
using namespace CryptoPP;

string Int2hex(Integer i) {
	string result;
	stringstream ss;
	ss << hex << i;
	result = ss.str();
	result.pop_back();
	const string zero = "0";
	for (int t = result.size(); t < 8; t++)
		result.insert(0, zero);
	return result;
}

string hex2Ascii(string hex) {
	string ascii = "";
	for (int i = 0; i < hex.length(); i++) {
		string part = hex.substr(i, 2);
		char c = stoul(part, nullptr, 16);
		ascii += c;
	}
	return ascii;
}

char _int2char(int i){
	stringstream ss;
	ss << i;
	char const *c = ss.str().c_str();
	return *c;
}

char int2hex(int i) {
	if (i <= 9)
		return '0' + i;
	else {
		switch (i) {
			case 10:
				return 'A';
			case 11:
				return 'B';
			case 12:
				return 'C';
			case 13:
				return 'D';
			case 14:
				return 'E';
			case 15:
				return 'F';
		}
	}	
}

string _hash(int i, string digest, ofstream &output) {
	//output << i << endl;//leading zeros
	string prehash = digest;
	output << prehash << endl;//previous hash
	Integer Intnonce = 0x00000000;
	string nonce = "00000000";
	string hexnonce = Int2hex(Intnonce);
	string strnonce = hex2Ascii(hexnonce);
	prehash = prehash + nonce;
	SHA256 hash;

	for (int i0 = 0; i0 < 16; i0++) {
		for (int i1 = 0; i1 < 16; i1++) {
			for (int i2 = 0; i2 < 16; i2++) {
				for (int i3 = 0; i3 < 16; i3++) {
					for (int i4 = 0; i4 < 16; i4++) {
						for (int i5 = 0; i5 < 16; i5++) {
							for (int i6 = 0; i6 < 16; i6++) {
								for (int i7 = 0; i7 < 16; i7++) {
									prehash[64] = int2hex(i0);
									prehash[65] = int2hex(i1);
									prehash[66] = int2hex(i2);
									prehash[67] = int2hex(i3);
									prehash[68] = int2hex(i4);
									prehash[69] = int2hex(i5);
									prehash[70] = int2hex(i6);
									prehash[71] = int2hex(i7);

									string temp;
									string hashvalue;

									StringSource (prehash, true, new HexDecoder(new StringSink(temp)));
									StringSource (temp, true, new HashFilter(hash, new HexEncoder(new StringSink(hashvalue))));

									int count = 0;
									for (int t = 0; t < 8; t++) {
										if (hashvalue[t] == '0')
											count++;
										else
											break;
									}
									if (count == i) {
										output << int2hex(i0);
										output << int2hex(i1);
										output << int2hex(i2);
										output << int2hex(i3);
										output << int2hex(i4);
										output << int2hex(i5);
										output << int2hex(i6);
										output << int2hex(i7) << endl;//nonce
										output << hashvalue << endl;//hash value
										return hashvalue;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

int main() {
	string ID = "109550087";
	ofstream output;
	output.open("out.txt");
	SHA256 hash;
	string digest;
	StringSource (ID, true, new HashFilter(hash, new HexEncoder(new StringSink(digest))));
	Integer Intnonce = 0x00000000;
	string nonce = "00000000";
	string hexnonce = Int2hex(Intnonce);
	string strnonce = hex2Ascii(hexnonce);
	string msg = digest + nonce;
	string encoded;
	StringSource (msg, true, new HexDecoder(new StringSink(encoded)));
	string hash_value;
	StringSource (encoded, true, new HashFilter(hash, new HexEncoder(new StringSink(hash_value))));
	
	output << "0" << endl;//leading zeros
	output << digest << endl;//previous hash
	output << nonce << endl;//nonce
	output << hash_value << endl;//hash value
	output << "1" << endl;
	string hash_value1 = _hash(1, hash_value, output);
	output << "2" << endl;
	string hash_value2 = _hash(2, hash_value1, output);
	output << "3" << endl;
	string hash_value3 = _hash(3, hash_value2, output);
	output << "4" << endl;
	string hash_value4 = _hash(4, hash_value3, output);
	output << "5" << endl;
	string hash_value5 = _hash(5, hash_value4, output);
	output << "6" << endl;
	string hash_value6 = _hash(6, hash_value5, output);
	output << "7" << endl;
	string hash_value7 = _hash(7, hash_value6, output);
	output << "8" << endl;
	string hash_value8 = _hash(8, hash_value7, output);

	return 0;
}

