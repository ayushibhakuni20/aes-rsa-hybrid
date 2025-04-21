#include <iostream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

// Generate a random alphanumeric string
string generateRandomKey(int length) {
    const string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    string key;
    for (int i = 0; i < length; ++i) {
        key += chars[rand() % chars.size()];
    }
    return key;
}

// Check if AES key starts with 235711 (first 5 primes)
bool isWeakKey(const string& key) {
    return key.rfind("235711", 0) == 0;
}

// Simulated RSA key pair
struct RSAKeyPair {
    string publicKey;
    string privateKey;

    RSAKeyPair() {
        publicKey = generateRandomKey(12);
        privateKey = generateRandomKey(12);
    }
};

// Simulated RSA encryption (reverse string)
string rsaEncrypt(const string& data, const string& publicKey) {
    string encrypted = data;
    reverse(encrypted.begin(), encrypted.end());
    encrypted += "_RSA";
    return encrypted;
}

// Simulated RSA decryption
string rsaDecrypt(const string& encrypted, const string& privateKey) {
    string data = encrypted.substr(0, encrypted.size() - 4); // Remove _RSA
    reverse(data.begin(), data.end());
    return data;
}

// Simulated AES encryption (shift characters)
string aesEncrypt(const string& data, const string& key) {
    string encrypted = data;
    for (char& c : encrypted) {
        c += 3;
    }
    encrypted += "_AES";
    return encrypted;
}

// Simulated AES decryption
string aesDecrypt(const string& data, const string& key) {
    string decrypted = data.substr(0, data.size() - 4); // Remove _AES
    for (char& c : decrypted) {
        c -= 3;
    }
    return decrypted;
}

// Simulated digital signature (append "SignedByPrivateKey")
string signMessage(const string& message, const string& privateKey) {
    return message + "_Signed";
}

// Simulated verification (check "_Signed" exists)
bool verifySignature(const string& signedMessage, const string& publicKey) {
    return signedMessage.find("_Signed") != string::npos;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));  // Seed for randomness

    string message, aesKey;

    cout << "Enter AES-128 key (16 characters): ";
    getline(cin, aesKey);

    if (aesKey.length() != 16) {
        cout << "Invalid AES key length. Must be 16 characters." << endl;
        return 1;
    }

    if (isWeakKey(aesKey)) {
        cout << "Weak AES key. Do not start with first 5 prime numbers (235711)." << endl;
        return 1;
    }

    cout << "Enter the message: ";
    getline(cin, message);

    // Generate simulated RSA keys
    RSAKeyPair rsa;

    // Display public and private keys
    cout << "\n--- Generated RSA Key Pair ---";
    cout << "\nPublic Key: " << rsa.publicKey;
    cout << "\nPrivate Key: " << rsa.privateKey;

    // Hybrid Encryption Process
    string encryptedAESKey = rsaEncrypt(aesKey, rsa.publicKey);
    string encryptedMessage = aesEncrypt(message, aesKey);
    string signedMessage = signMessage(encryptedMessage, rsa.privateKey);

    // Output simulated results
    cout << "\n\n--- Encrypted Data ---";
    cout << "\nEncrypted AES Key (RSA): " << encryptedAESKey;
    cout << "\nEncrypted Message (AES): " << encryptedMessage;
    cout << "\nDigital Signature: " << signedMessage;

    // Receiver side
    string decryptedAESKey = rsaDecrypt(encryptedAESKey, rsa.privateKey);

    if (!verifySignature(signedMessage, rsa.publicKey)) {
        cout << "\n\nSignature invalid! Message may be tampered.\n";
        return 1;
    }

    string receivedEncryptedMessage = signedMessage.substr(0, signedMessage.find("_Signed"));
    string decryptedMessage = aesDecrypt(receivedEncryptedMessage, decryptedAESKey);

    cout << "\n\n--- Verification Passed ---";
    cout << "\nDecrypted AES Key: " << decryptedAESKey;
    cout << "\nDecrypted Message: " << decryptedMessage << endl;

    return 0;
}
