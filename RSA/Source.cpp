#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>
#include <cryptopp/integer.h>

using namespace std;
using namespace CryptoPP;

int main()
{
    /*ofstream output;
    output.open("out.txt");
    Integer e1("0x11"), n1("0x04823f9fe38141d93f1244be161b20f");
    Integer e2("0x10001"), n2("0x9711ea5183d50d6a91114f1d7574cd52621b35499b4d3563ec95406a994099c9");
    RSA::PublicKey PU1;
    PU1.Initialize(n1, e1);
    RSA::PublicKey PU2;
    PU2.Initialize(n2, e2);

    Integer m1, m2, c1, c2;
    string mes1 = "Hello World!";
    string mes2 = "RSA is public key.";
    m1 = Integer((const byte*)mes1.data(), mes1.size());
    m2 = Integer((const byte*)mes2.data(), mes2.size());

    //encryption
    c1 = PU1.ApplyFunction(m1);
    c2 = PU2.ApplyFunction(m2);
    string out1 = IntToString(c1, 16);
    string out2 = IntToString(c2, 16);
    output << hex << out1 << endl;
    output << hex << out2 << endl;
    
    //decryption
    Integer e("0x1d35"), n("0xc4b361851de35f080d3ca7352cbf372d"), d("0x53a0a95b089cf23adb5cc73f0700000");
    Integer c("0xa02d51d0e87efe1defc19f3ee899c31d"), r;
    Integer correct;
    AutoSeededRandomPool prng;
    RSA::PrivateKey PR;
    string recover;
    for (int i = 0; i < 16 * 16; i++) {
        try {
            PR.Initialize(n, e, d);
            correct = d;
        }
        catch (...) {}
        d++;
    }

    r = PR.CalculateInverse(prng, c);
    size_t req = r.MinEncodedSize();
    recover.resize(req);
    r.Encode((byte*)recover.data(), recover.size());
    string out = IntToString(correct, 16);
    output << out << endl;
    output << recover << endl;
    */

    //on-site test

    Integer n("253963006250652707627402859040685100389"), e("65537"), d("42772482296155483517134936268603049473");
    Integer c("31639169974475525248366103533531939340");
    Integer m;
    string recover;
    for (int i = 0; i < 10; i++) {
        m = a_exp_b_mod_c(c, d, n);
        stringstream ss;
        ss << m;
        string temp;
        ss >> temp;
        temp.pop_back();
        int sum = 0;
        for (int j = 0; j < temp.size(); j++) {
            sum += (temp[j] - '0');
        }
        sum %= 10;
        if (sum == 9) {
            stringstream s;
            string num;
            s << m;
            s >> num;
            num.pop_back();
            cout << num << endl;
        }
        c += 1;
    }

    return 0;
}
    

