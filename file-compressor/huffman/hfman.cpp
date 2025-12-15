// cat test-data.txt | ./hfman

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cstdint>

using namespace std;

/* ---------------- Huffman Tree ---------------- */

struct Node {
    unsigned char ch;
    uint64_t freq;
    Node *left, *right;

    Node(unsigned char c, uint64_t f)
        : ch(c), freq(f), left(nullptr), right(nullptr) {}

    Node(Node* l, Node* r)
        : ch(0), freq(l->freq + r->freq), left(l), right(r) {}
};

struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

/* ---------------- Generate Codes ---------------- */

void build_codes(Node* node, const string& prefix,
                 unordered_map<unsigned char, string>& codes) {
    if (!node) return;

    if (!node->left && !node->right) {
        codes[node->ch] = prefix.empty() ? "0" : prefix;
        return;
    }

    build_codes(node->left, prefix + "0", codes);
    build_codes(node->right, prefix + "1", codes);
}

/* ---------------- Main ---------------- */

int main() {
    vector<unsigned char> input;
    unsigned char byte;

    /* Read binary data from stdin */
    while (cin.read((char*)&byte, 1)) {
        input.push_back(byte);
    }

    if (input.empty()) return 0;

    /* Count frequencies */
    unordered_map<unsigned char, uint64_t> freq;
    for (unsigned char c : input) freq[c]++;

    /* Build Huffman tree */
    priority_queue<Node*, vector<Node*>, Compare> pq;
    for (auto& p : freq)
        pq.push(new Node(p.first, p.second));

    while (pq.size() > 1) {
        Node* a = pq.top(); pq.pop();
        Node* b = pq.top(); pq.pop();
        pq.push(new Node(a, b));
    }

    Node* root = pq.top();

    unordered_map<unsigned char, string> codes;
    build_codes(root, "", codes);

    string bits;
    for (unsigned char c : input)
        bits += codes[c];

    uint64_t bit_len = bits.size();

    /* Pack bits into bytes */
    vector<uint8_t> compressed;
    uint8_t current = 0;
    int count = 0;

    for (char b : bits) {
        current = (current << 1) | (b == '1');
        count++;

        if (count == 8) {
            compressed.push_back(current);
            current = 0;
            count = 0;
        }
    }

    if (count > 0) {
        current <<= (8 - count);
        compressed.push_back(current);
    }

    /* Write output file */
    ofstream out("output.huf", ios::binary);

    out.write("HUF1", 4);

    uint32_t n = freq.size();
    out.write((char*)&n, sizeof(n));

    for (auto& p : freq) {
        out.write((char*)&p.first, 1);
        out.write((char*)&p.second, sizeof(uint64_t));
    }

    out.write((char*)&bit_len, sizeof(bit_len));
    out.write((char*)compressed.data(), compressed.size());

    out.close();

    cout << "Compressed to output.huf\n";
    return 0;
}
