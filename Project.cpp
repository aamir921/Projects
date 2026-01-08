#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <cmath>

using namespace std;

// Utility to convert a substring to an integer representation
int StringToInt(const string& message) 
{
    int num = 0;
    for (char ch : message) 
    {
        num = num * 100 + static_cast<int>(ch); // Convert each character to its ASCII value and append
    }
    return num;
}

// Utility to convert an integer back to a string representation
string IntToString(int num) 
{
    string message;
    while (num > 0) 
    {
        char ch = static_cast<char>(num % 100);
        message = ch + message;
        num /= 100;
    }
    return message;
}

// Euclidean Algorithm to calculate GCD
int EuclidAlgGCD(int a, int b) 
{
    while (b != 0) 
    {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Extended Euclidean Algorithm
int ExtendedEuclidAlgGCD(int a, int b, int &s, int &t) 
{
    if (b == 0) 
    {
        s = 1;
        t = 0;
        return a;
    }
    int s1, t1;
    int gcd = ExtendedEuclidAlgGCD(b, a % b, s1, t1);
    s = t1;
    t = s1 - (a / b) * t1;
    return gcd;
}

// Modular Arithmetic
int mod(int a, int n) 
{
    int result = a % n;
    if (result < 0) result += n;
    return result;
}

// Finding a Relatively Prime Number
int RelativelyPrime(int n) 
{
    int candidate = 2;
    while (candidate < n) 
    {
        if (EuclidAlgGCD(n, candidate) == 1) 
        {
            return candidate;
        }
        candidate++;
    }
    return -1;
}

// Modular Inverse
int inverse(int a, int n) 
{
    int s, t;
    int d = ExtendedEuclidAlgGCD(n, a, s, t);
    if (d == 1) 
    {
        return mod(t, n);
    } 
    else 
    {
        cout << "a and n are not relatively prime!\n";
        return -1;
    }
}

// Prime Checking Function
bool IsPrime(int n) 
{
    if (n <= 1) return false;
    for (int i = 2; i <= sqrt(n); ++i) 
    {
        if (n % i == 0) return false;
    }
    return true;
}

// RSA Encode function
int Encode(int M, int e, int PQ) 
{
    int result = 1;
    for (int i = 0; i < e; ++i) 
    {
        result = (result * M) % PQ;
    }
    return result;
}

// RSA Decode function
int Decode(int C, int d, int PQ) 
{
    int result = 1;
    for (int i = 0; i < d; ++i) 
    {
        result = (result * C) % PQ;
    }
    return result;
}

// Function to split a message into integer blocks
vector<int> SplitMessageIntoBlocks(const string& message, int maxBlockSize) 
{
    vector<int> blocks;
    for (size_t i = 0; i < message.size(); i += maxBlockSize) 
    {
        string chunk = message.substr(i, maxBlockSize);
        blocks.push_back(StringToInt(chunk));
    }
    return blocks;
}

// Function to join decrypted blocks into a message
string JoinBlocksIntoMessage(const vector<int>& blocks) 
{
    string message;
    for (int block : blocks) 
    {
        message += IntToString(block);
    }
    return message;
}

int main() 
{
    int P, Q;
    cout << "Welcome to the Enhanced RSA Encryption Adventure!\n";
    cout << "Enter two secret prime numbers to form your encryption keys.\n";

    // Input and verify P
    cout << "Enter prime number P: ";
    cin >> P;
    while (!IsPrime(P)) 
    {
        cout << "Oops! P is not a prime. Please enter a prime number: ";
        cin >> P;
    }

    // Input and verify Q
    cout << "Enter prime number Q: ";
    cin >> Q;
    while (!IsPrime(Q)) 
    {
        cout << "Oops! Q is not a prime. Please enter a prime number: ";
        cin >> Q;
    }

    int PQ = P * Q;
    int phi = (P - 1) * (Q - 1);

    // Choosing e
    int e = RelativelyPrime(phi);
    if (e == -1) 
    {
        cout << "Could not find a number relatively prime to phi.\n";
        return -1;
    }

    // Calculating d (modular inverse of e)
    int d = inverse(e, phi);
    if (d == -1) 
    {
        cout << "Could not find an inverse for e.\n";
        return -1;
    }

    cout << "Public key (e, PQ): (" << e << ", " << PQ << ")" << endl;
    cout << "Private key d: " << d << endl;

    // User message input
    cin.ignore();  // Clear newline from previous input
    string message;
    cout << "Enter a message to encrypt (in all CAPS): ";
    getline(cin, message);

    // Split message into blocks small enough to be encoded with RSA
    int maxBlockSize = to_string(PQ).length() / 2;  // Estimate block size
    vector<int> messageBlocks = SplitMessageIntoBlocks(message, maxBlockSize);

    // Encrypt each block
    vector<int> encryptedBlocks;
    for (int block : messageBlocks) 
    {
        encryptedBlocks.push_back(Encode(block, e, PQ));
    }

    // Display encrypted message
    cout << "Encrypted message: ";
    for (int encryptedBlock : encryptedBlocks) 
    {
        cout << encryptedBlock << " ";
    }
    cout << endl;

    // Decrypt each block
    vector<int> decryptedBlocks;
    for (int block : encryptedBlocks) 
    {
        decryptedBlocks.push_back(Decode(block, d, PQ));
    }

    // Join decrypted blocks into the original message
    string decryptedMessage = JoinBlocksIntoMessage(decryptedBlocks);
    cout << "Decrypted message: " << decryptedMessage << endl;

    assert(message == decryptedMessage);
    cout << "RSA algorithm verification successful!" << endl;

    return 0;
}
