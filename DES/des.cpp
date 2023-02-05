#include <iostream>
#include <algorithm>
#include <fstream>
#include <cstring>
#include <bitset>
#include <sstream>
#include <vector>
#include <map>
using namespace std;

int pc1[56] = {57, 49, 41, 33, 25, 17, 9,
				1, 58, 50, 42, 34, 26, 18,
				10, 2, 59, 51, 43, 35, 27,
				19, 11, 3, 60, 52, 44, 36,
				63, 55, 47, 39, 31, 23, 15,
				7, 62, 54, 46, 38, 30, 22,
				14, 6, 61, 53, 45, 37, 29,
				21, 13, 5, 28, 20, 12, 4};
//Key - Compression Table
int pc2[48] = {14, 17, 11, 24, 1, 5,
					3, 28, 15, 6, 21, 10,
					23, 19, 12, 4, 26, 8,
					16, 7, 27, 20, 13, 2,
					41, 52, 31, 37, 47, 55,
					30, 40, 51, 45, 33, 48,
					44, 49, 39, 56, 34, 53,
					46, 42, 50, 36, 29, 32};
//bits for different round keys to shift			
int keyshift[16] = {1, 1, 2, 2,
					2, 2, 2, 2,
					1, 2, 2, 2,
					2, 2, 2, 1};
//Initial Permutation 
int ip[64] = {58, 50, 42, 34, 26, 18, 10, 2,
						60, 52, 44, 36, 28, 20, 12, 4,
						62, 54, 46, 38, 30, 22, 14, 6,
						64, 56, 48, 40, 32, 24, 16, 8,
						57, 49, 41, 33, 25, 17, 9, 1,
						59, 51, 43, 35, 27, 19, 11, 3,
						61, 53, 45, 37, 29, 21, 13, 5,
						63, 55, 47, 39, 31, 23, 15, 7};
//Expansion Table
int expand[48] = {32, 1, 2, 3, 4, 5, 4, 5,
				6, 7, 8, 9, 8, 9, 10, 11,
				12, 13, 12, 13, 14, 15, 16, 17,
				16, 17, 18, 19, 20, 21, 20, 21,
				22, 23, 24, 25, 24, 25, 26, 27,
				28, 29, 28, 29, 30, 31, 32, 1};

//S-box Table
int sbox[8][4][16] = {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
					0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
					4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
					15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13},
					{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
					3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
					0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
					13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9},
					{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
					13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
					13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
					1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12},
					{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
					13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
					10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
					3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14},
					{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
					14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
					4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
					11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3},
					{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
					10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
					9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
					4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13},
					{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
					13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
					1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
					6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12},
					{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
					1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
					7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
					2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}};
//Permutation Table
int per[32] = {16, 7, 20, 21,
				29, 12, 28, 17,
				1, 15, 23, 26,
				5, 18, 31, 10,
				2, 8, 24, 14,
				32, 27, 3, 9,
				19, 13, 30, 6,
				22, 11, 4, 25};
//Final Permutation Table
int inverse_ip[64] = {40, 8, 48, 16, 56, 24, 64, 32,
					39, 7, 47, 15, 55, 23, 63, 31,
					38, 6, 46, 14, 54, 22, 62, 30,
					37, 5, 45, 13, 53, 21, 61, 29,
					36, 4, 44, 12, 52, 20, 60, 28,
					35, 3, 43, 11, 51, 19, 59, 27,
					34, 2, 42, 10, 50, 18, 58, 26,
					33, 1, 41, 9, 49, 17, 57, 25};

string hextoascii(string s){
	string ascii = "";
	for (size_t i = 0; i < s.length(); i += 2){
		string part = s.substr(i, 2);
		char ch = stoul(part, nullptr, 16);
		ascii += ch;
	}
	return ascii;
}

string asciitobin(string s){
	string bin = "";
	for (int i = 0; i < 8; i++){
		bitset<8> temp(s[i]);	
		bin += temp.to_string();
	}
	return bin;
}

string hextobin(string s){
	map <char, string> hex;
	hex['0'] = "0000";
	hex['1'] = "0001";
	hex['2'] = "0010";
	hex['3'] = "0011";
	hex['4'] = "0100";
	hex['5'] = "0101";
	hex['6'] = "0110";
	hex['7'] = "0111";
	hex['8'] = "1000";
	hex['9'] = "1001";
	hex['A'] = "1010";
	hex['B'] = "1011";
	hex['C'] = "1100";
	hex['D'] = "1101";
	hex['E'] = "1110";
	hex['F'] = "1111";

	string bin = "";
	for (int i = 0; i < s.size(); i++) {
		bin += hex[s[i]];
	}
	return bin;
}

string bintohex(string s){
	map<string, string> mp;
	mp["0000"] = "0";
	mp["0001"] = "1";
	mp["0010"] = "2";
	mp["0011"] = "3";
	mp["0100"] = "4";
	mp["0101"] = "5";
	mp["0110"] = "6";
	mp["0111"] = "7";
	mp["1000"] = "8";
	mp["1001"] = "9";
	mp["1010"] = "A";
	mp["1011"] = "B";
	mp["1100"] = "C";
	mp["1101"] = "D";
	mp["1110"] = "E";
	mp["1111"] = "F";
	string hex = "";
	for (int i = 0; i < s.length(); i += 4){
		string ch = "";
		ch += s[i];
		ch += s[i + 1];
		ch += s[i + 2];
		ch += s[i + 3];
		hex += mp[ch];
	}
	return hex;
}

string permute(string k, int* arr, int n)
{
	string per = "";
	for (int i = 0; i < n; i++)
		per += k[arr[i] - 1];
	return per;
}

string shift_left(string key, int shifts){
	string s = "";
	for (int i = 0; i < shifts; i++){
		for (int j = 1; j < 28; j++)
			s += key[j];
		s += key[0];
		key = s;
		s = "";
	}
	return key;
}

string Xor(string a, string b){
	string ans = "";
	for (int i = 0; i < a.size(); i++){
		if (a[i] == b[i])
			ans += "0";
		else
			ans += "1";
	}
	return ans;
}

string encrypt(string text, vector<string> rk,int a){
	text = hextobin(text);
	text = permute(text, ip, 64);

	//get left and right
	string left = text.substr(0, 32);
	string right = text.substr(32, 32);

	for (int i = 0; i < 16; i++){
		//Expand 32->48 
		string rexpand = permute(right, expand, 48);

		//xor round key and rexpand
		string xored = Xor(rk[i], rexpand);

		//S-box
		string result = "";
		for (int i = 0; i < 8; i++){
			int row = 2 * int(xored[i * 6] - '0') + int(xored[i * 6 + 5] - '0');
			int col = 8 * int(xored[i * 6 + 1] - '0') + 4 * int(xored[i * 6 + 2] - '0') + 2 * int(xored[i * 6 + 3] - '0') + int(xored[i * 6 + 4] - '0');
			int val = sbox[i][row][col];
			result += char(val / 8 + '0');
			val = val % 8;
			result += char(val / 4 + '0');
			val = val % 4;
			result += char(val / 2 + '0');
			val = val % 2;
			result += char(val + '0');
		}
		result = permute(result, per, 32);
		//xor left and result
		xored = Xor(result, left);
		left = xored;
		//Swap left and right
		if (i != 15)
			swap(left, right);
	}

	string combine = left + right;
	//inverse permutation
	string cipher = bintohex(permute(combine, inverse_ip, 64));
	return cipher;
}

int main(){
	string key, ct;
	key = "3719839";
	ct = "2BADF07ADD648074";
	
	key = asciitobin(key);
	key = permute(key, pc1, 56);

	string left = key.substr(0, 28);
	string right = key.substr(28, 28);

	vector<string> rk;
	for (int i = 0; i < 16; i++){
		left = shift_left(left, keyshift[i]);
		right = shift_left(right, keyshift[i]);

		string combine = left + right;
		string roundkey = permute(combine, pc2, 48);
		rk.push_back(roundkey);
	}
	
	reverse(rk.begin(), rk.end());
	string text = encrypt(ct, rk, 1);
	cout << "\nPlain Text: " << hextoascii(text) << endl;


	return 0;
}
