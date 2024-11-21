#include <iostream>
#include <queue>
#include <unordered_map>
#include <string>
using namespace std;

class Node {
public:
    char character;
    int frequency;
    Node* left;
    Node* right;

    Node(char ch, int freq) : character(ch), frequency(freq), left(nullptr), right(nullptr) {}
};

class Compare {
public:
    bool operator()(Node* left, Node* right) {
        return left->frequency > right->frequency;
    }
};

class HuffmanTree {
private:
    Node* root;
    unordered_map<char, string> huffmanCodes;

    void generateCodes(Node* node, string code) {
        if (!node) return;

        if (!node->left && !node->right) {
            huffmanCodes[node->character] = code;
        }

        generateCodes(node->left, code + "0");
        generateCodes(node->right, code + "1");
    }

    void deleteTree(Node* node) {
        if (!node) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

public:
    HuffmanTree() : root(nullptr) {}

    ~HuffmanTree() {
        deleteTree(root);
    }

    void buildTree(const string& text) {
        unordered_map<char, int> freqMap;
        for (char ch : text) {
            freqMap[ch]++;
        }

        priority_queue<Node*, vector<Node*>, Compare> pq;
        for (const auto& pair : freqMap) {
            pq.push(new Node(pair.first, pair.second));
        }

        while (pq.size() > 1) {
            Node* left = pq.top(); pq.pop();
            Node* right = pq.top(); pq.pop();

            Node* newNode = new Node('\0', left->frequency + right->frequency);
            newNode->left = left;
            newNode->right = right;

            pq.push(newNode);
        }

        root = pq.top();
        generateCodes(root, "");
    }

    void printCodes() {
        for (const auto& pair : huffmanCodes) {
            cout << pair.first << " : " << pair.second << endl;
        }
    }

    string encode(const string& text) {
        string encodedText;
        for (char ch : text) {
            encodedText += huffmanCodes[ch];
        }
        return encodedText;
    }

    string decode(const string& encodedText) {
        string decodedText;
        Node* currentNode = root;
        for (char bit : encodedText) {
            currentNode = (bit == '0') ? currentNode->left : currentNode->right;

            if (!currentNode->left && !currentNode->right) {
                decodedText += currentNode->character;
                currentNode = root;
            }
        }
        return decodedText;
    }
};

int main() {
    string text;
    cout << "Enter text to encode: ";
    getline(cin, text);

    HuffmanTree huffmanTree;
    huffmanTree.buildTree(text);

    cout << "\nHuffman Codes:\n";
    huffmanTree.printCodes();

    string encodedText = huffmanTree.encode(text);
    cout << "\nEncoded Text: " << encodedText << endl;

    string decodedText = huffmanTree.decode(encodedText);
    cout << "\nDecoded Text: " << decodedText << endl;

    return 0;
}
