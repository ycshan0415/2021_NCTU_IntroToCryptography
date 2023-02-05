#include "cryptlib.h"
#include "rijndael.h"
#include "modes.h"
#include "files.h"
#include "osrng.h"
#include "hex.h"
#include <string>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;
using namespace CryptoPP;
int main()
{
    ofstream output;
    output.open("aes-out.txt");
    string plaintext = "AES is the US block cipher standard.";
    string cipher, encoded;
    string key = "keyis84932731830";
    byte k[16];
    for (int i = 0; i < 16; i++)
        k[i] = key[i];

    string a = "0000000000000000";
    byte iv0[16];
    for (int i = 0; i < 16; i++)
        iv0[i] = a[i];
    string b = "9999999999999999";
    byte iv9[16];
    for (int i = 0; i < 16; i++)
        iv9[i] = b[i];
    SecByteBlock key0(k, sizeof(k)), iv_0(iv0, sizeof(iv0)), iv_9(iv9, sizeof(iv9));
    
    //CFB
    AlgorithmParameters params = MakeParameters(Name::FeedbackSize(), 4)//4 bytes
        (Name::IV(), ConstByteArrayParameter(iv_0));
    CFB_Mode< AES >::Encryption enc;
    enc.SetKey(key0, key0.size(), params);
    //encrypt
    StringSource ss1(plaintext, true, new StreamTransformationFilter(enc, new StringSink(cipher)));
    StringSource ss2(cipher, true, new HexEncoder(new StringSink(encoded)));
    output << "cipher text: " << encoded << endl;
    encoded.clear();
    cipher.clear();
    
    //CBC
    CBC_Mode< AES >::Encryption e0000;
    e0000.SetKeyWithIV(key0, key0.size(), iv_0);
    StringSource ss3(plaintext, true, new StreamTransformationFilter(e0000, new StringSink(cipher), StreamTransformationFilter::ZEROS_PADDING));
    StringSource ss4(cipher, true, new HexEncoder(new StringSink(encoded)));
    output << "cipher text: " << encoded << endl;
    encoded.clear();
    cipher.clear();

    CBC_Mode< AES >::Encryption e9999;
    e9999.SetKeyWithIV(key0, key0.size(), iv_9);
    StringSource ss5(plaintext, true, new StreamTransformationFilter(e9999, new StringSink(cipher)));//pkcs7
    StringSource ss6(cipher, true, new HexEncoder(new StringSink(encoded)));
    output << "cipher text: " << encoded << endl;
    encoded.clear();
    cipher.clear();

    //ECB
    ECB_Mode< AES >::Encryption e;
    e.SetKey(key0, key0.size());
    StringSource ss7(plaintext, true, new StreamTransformationFilter(e, new StringSink(cipher)));
    StringSource ss8(cipher, true, new HexEncoder(new StringSink(encoded)));
    output << "cipher text: " << encoded << endl;
    encoded.clear();
    cipher.clear();

    //decrypt
    string plain;
    ECB_Mode< AES >::Decryption d;
    // ECB Mode does not use an IV
    d.SetKey(key0, key0.size());
    // The StreamTransformationFilter removes
    //  padding as required.
    StringSource ss9(cipher, true, new StreamTransformationFilter(d, new StringSink(plain)));// StreamTransformationFilter
     // StringSource
    cout << "plain text: " << plain << endl;
    plain.clear();

    CBC_Mode< AES >::Decryption d;
    d.SetKeyWithIV(key0, key0.size(), iv_0);
    // The StreamTransformationFilter removes
    //  padding as required.
    StringSource ss10(cipher, true, new StreamTransformationFilter(d, new StringSink(plain))); // StreamTransformationFilter
    // StringSource
    cout << "plain text: " << plain << endl;
    plain.clear();


    CFB_Mode< AES >::Decryption d;
    dec.SetKeyWithIV(key, key.size(), iv_0);
    // The StreamTransformationFilter removes
    //  padding as required.
    StringSource ss11(cipher, true, new StreamTransformationFilter(d, new StringSink(plain))); // StreamTransformationFilter
    // StringSource
    cout << "plain text: " << plain << endl;
    plain.clear();

    return 0;
}