#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <bitset>
#include <memory>

using namespace std;

struct Node {
    char ch;
    int freq;
    shared_ptr<Node> left, right;

    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
    Node(int f, shared_ptr<Node> l, shared_ptr<Node> r) : ch('\0'), freq(f), left(l), right(r) {}
};

class Huffman {
private:
    static void generateCodes(shared_ptr<Node> root, const string& str,
                              unordered_map<char, string>& huffmanCode) {
        if (!root) return;

        if (!root->left && !root->right) {
            huffmanCode[root->ch] = str;
        }

        generateCodes(root->left, str + "0", huffmanCode);
        generateCodes(root->right, str + "1", huffmanCode);
    }

public:
    static void compress(const string& inputFile, const string& outputFile) {
        ifstream inFile(inputFile, ios::binary);
        if (!inFile) {
            cerr << "Cannot open input file: " << inputFile << endl;
            return;
        }

        string data((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
        inFile.close();

        if (data.empty()) {
            cerr << "Input file is empty!" << endl;
            return;
        }

        unordered_map<char, int> freq;
        for (char ch : data) {
            freq[ch]++;
        }

        auto compare = [](shared_ptr<Node> a, shared_ptr<Node> b) {
            return a->freq > b->freq;
        };

        priority_queue<shared_ptr<Node>, vector<shared_ptr<Node>>, decltype(compare)> pq(compare);

        for (auto& pair : freq) {
            pq.push(make_shared<Node>(pair.first, pair.second));
        }

        while (pq.size() > 1) {
            auto left = pq.top(); pq.pop();
            auto right = pq.top(); pq.pop();
            int sum = left->freq + right->freq;
            pq.push(make_shared<Node>(sum, left, right));
        }

        auto root = pq.top();

        unordered_map<char, string> huffmanCode;
        generateCodes(root, "", huffmanCode);

        ofstream outFile(outputFile, ios::binary);

        outFile << freq.size() << "\n";
        for (auto& pair : freq) {
            outFile << (int)pair.first << " " << pair.second << "\n";
        }

        string encoded = "";
        for (char ch : data) {
            encoded += huffmanCode[ch];
        }

        int padding = 8 - (encoded.length() % 8);
        if (padding != 8) {
            encoded.append(padding, '0');
        }

        outFile << padding << "\n";

        for (size_t i = 0; i < encoded.length(); i += 8) {
            string byte = encoded.substr(i, 8);
            char c = bitset<8>(byte).to_ulong();
            outFile.put(c);
        }

        outFile.close();
        cout << "Compression successful: " << inputFile << " -> " << outputFile << endl;
    }

    static void decompress(const string& inputFile, const string& outputFile) {
        ifstream inFile(inputFile, ios::binary);
        if (!inFile) {
            cerr << "Cannot open input file: " << inputFile << endl;
            return;
        }

        int size;
        inFile >> size;
        inFile.get();

        unordered_map<char, int> freq;
        for (int i = 0; i < size; i++) {
            int ch, frequency;
            inFile >> ch >> frequency;
            inFile.get();
            freq[(char)ch] = frequency;
        }

        int padding;
        inFile >> padding;
        inFile.get();

        auto compare = [](shared_ptr<Node> a, shared_ptr<Node> b) {
            return a->freq > b->freq;
        };
        priority_queue<shared_ptr<Node>, vector<shared_ptr<Node>>, decltype(compare)> pq(compare);

        for (auto& pair : freq) {
            pq.push(make_shared<Node>(pair.first, pair.second));
        }

        while (pq.size() > 1) {
            auto left = pq.top(); pq.pop();
            auto right = pq.top(); pq.pop();
            pq.push(make_shared<Node>(left->freq + right->freq, left, right));
        }

        auto root = pq.top();

        string encoded = "";
        char c;
        while (inFile.get(c)) {
            encoded += bitset<8>(c).to_string();
        }

        if (padding != 8) {
            encoded = encoded.substr(0, encoded.length() - padding);
        }

        ofstream outFile(outputFile, ios::binary);
        auto current = root;

        for (char bit : encoded) {
            if (bit == '0') current = current->left;
            else current = current->right;

            if (!current->left && !current->right) {
                outFile.put(current->ch);
                current = root;
            }
        }

        outFile.close();
        cout << "Decompression successful: " << inputFile << " -> " << outputFile << endl;
    }
};

int main() {
    int choice;
    string inputFile;
    string outputFile;

    cout << "=== Huffman File Compression ===\n";
    cout << "1. Compress File\n";
    cout << "2. Decompress File\n";
    cout << "3. Exit\n";
    cout << "Enter your choice: ";
    cin >> choice;

    system("cls");
    cout << "Enter input filename: ";
    cin >> inputFile;
    cout << "Enter output filename: ";
    cin >> outputFile;

    if (choice == 1) {
        Huffman::compress(inputFile, outputFile);
    } else if (choice == 2) {
        Huffman::decompress(inputFile, outputFile);
    } else if (choice == 3) {
        cout << "Exiting program . . .\n";
        exit(1);
    } else {
        cout << "Invalid choice!" << endl;
    }

    return 0;
}