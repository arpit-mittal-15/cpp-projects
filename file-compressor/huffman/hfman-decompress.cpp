#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <cstdint>
#include <cstring>

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

/* ---------------- Helpers ---------------- */

string output_name(const string& input) {
    size_t pos = input.find_last_of('.');
    if (pos == string::npos) return input + ".dec";
    return input.substr(0, pos) + ".dec";
}

/* ---------------- Main ---------------- */

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <file.huf>\n";
        return 1;
    }

    ifstream in(argv[1], ios::binary);
    if (!in) {
        cerr << "Cannot open input file\n";
        return 1;
    }

    string outname = output_name(argv[1]);
    ofstream out(outname, ios::binary);
    if (!out) {
        cerr << "Cannot create output file\n";
        return 1;
    }

    /* ---- Read magic ---- */
    char magic[4];
    in.read(magic, 4);
    if (memcmp(magic, "HUF1", 4) != 0) {
        cerr << "Invalid HUF file\n";
        return 1;
    }

    /* ---- Read symbol count ---- */
    uint32_t n;
    in.read((char*)&n, sizeof(n));

    /* ---- Read frequency table ---- */
    priority_queue<Node*, vector<Node*>, Compare> pq;

    for (uint32_t i = 0; i < n; i++) {
        unsigned char ch;
        uint64_t freq;
        in.read((char*)&ch, 1);
        in.read((char*)&freq, sizeof(freq));
        pq.push(new Node(ch, freq));
    }

    /* ---- Build tree ---- */
    while (pq.size() > 1) {
        Node* a = pq.top(); pq.pop();
        Node* b = pq.top(); pq.pop();
        pq.push(new Node(a, b));
    }
    Node* root = pq.top();

    /* ---- Read bit length ---- */
    uint64_t bit_len;
    in.read((char*)&bit_len, sizeof(bit_len));

    /* ---- Read compressed data ---- */
    vector<uint8_t> data((bit_len + 7) / 8);
    in.read((char*)data.data(), data.size());

    /* ---- Decode ---- */
    Node* curr = root;
    uint64_t bits = 0;

    for (uint8_t byte : data) {
        for (int i = 7; i >= 0 && bits < bit_len; i--) {
            int bit = (byte >> i) & 1;
            curr = bit ? curr->right : curr->left;

            if (!curr->left && !curr->right) {
                out.write((char*)&curr->ch, 1);
                curr = root;
            }
            bits++;
        }
    }

    cout << "Decompressed: " << outname << "\n";
    return 0;
}
