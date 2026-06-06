#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;

const int TABLE_SIZE = 20011; // Prime number

//--------------------------------------------------
// Modular Hash
//--------------------------------------------------
unsigned int modularHash(const string& word)
{
    unsigned int sum = 0;

    for(char c : word)
        sum += c;

    return sum % TABLE_SIZE;
}

//--------------------------------------------------
// Polynomial Rolling Hash
//--------------------------------------------------
unsigned int polynomialHash(const string& word)
{
    const int p = 31;

    unsigned long long hash = 0;
    unsigned long long power = 1;

    for(char c : word)
    {
        hash += (c - 'a' + 1) * power;
        power *= p;
    }

    return hash % TABLE_SIZE;
}

//--------------------------------------------------
// FNV-1a Hash
//--------------------------------------------------
unsigned int fnv1aHash(const string& word)
{
    unsigned int hash = 2166136261;

    for(char c : word)
    {
        hash ^= c;
        hash *= 16777619;
    }

    return hash % TABLE_SIZE;
}

//--------------------------------------------------
// Collision Counter
//--------------------------------------------------
int countCollisions(vector<string>& words, int hashType)
{
    vector<bool> occupied(TABLE_SIZE,false);

    int collisions = 0;

    for(string word : words)
    {
        unsigned int index;

        if(hashType == 1)
            index = modularHash(word);

        else if(hashType == 2)
            index = polynomialHash(word);

        else
            index = fnv1aHash(word);

        if(occupied[index])
            collisions++;
        else
            occupied[index] = true;
    }

    return collisions;
}

//--------------------------------------------------
// Load Dictionary
//--------------------------------------------------
vector<string> loadWords(string filename)
{
    vector<string> words;

    ifstream file(filename);

    string word;

    while(file >> word)
        words.push_back(word);

    return words;
}

//--------------------------------------------------
// Main
//--------------------------------------------------
int main()
{
    vector<string> words = loadWords("dictionary.txt");

    cout << "Words Loaded: "
         << words.size() << endl;

    auto start1 = chrono::high_resolution_clock::now();
    int modCollisions = countCollisions(words,1);
    auto end1 = chrono::high_resolution_clock::now();

    auto start2 = chrono::high_resolution_clock::now();
    int polyCollisions = countCollisions(words,2);
    auto end2 = chrono::high_resolution_clock::now();

    auto start3 = chrono::high_resolution_clock::now();
    int fnvCollisions = countCollisions(words,3);
    auto end3 = chrono::high_resolution_clock::now();

    cout << "\nRESULTS\n";
    cout << "---------------------\n";

    cout << "Modular Hash:\n";
    cout << "Collisions = "
         << modCollisions << endl;

    cout << "Time = "
         << chrono::duration_cast<chrono::microseconds>
         (end1-start1).count()
         << " microseconds\n\n";

    cout << "Polynomial Hash:\n";
    cout << "Collisions = "
         << polyCollisions << endl;

    cout << "Time = "
         << chrono::duration_cast<chrono::microseconds>
         (end2-start2).count()
         << " microseconds\n\n";

    cout << "FNV-1a Hash:\n";
    cout << "Collisions = "
         << fnvCollisions << endl;

    cout << "Time = "
         << chrono::duration_cast<chrono::microseconds>
         (end3-start3).count()
         << " microseconds\n";

    return 0;
}