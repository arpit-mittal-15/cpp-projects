// cat rle.cpp | ./rle compressed.rle

#include <iostream>
#include <fstream>
#include <cstdint>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: rle_compress <output_file>\n";
        return 1;
    }

    ofstream out(argv[1], ios::binary);
    if (!out) {
        cerr << "Failed to open output file\n";
        return 1;
    }

    unsigned char prev, curr;
    uint8_t count = 1;

    // Read first byte
    if (!cin.read((char*)&prev, 1)) {
        return 0; // empty input
    }

    // Read rest
    while (cin.read((char*)&curr, 1)) {
        if (curr == prev && count < 255) {
            count++;
        } else {
            out.write((char*)&count, 1);
            out.write((char*)&prev, 1);
            prev = curr;
            count = 1;
        }
    }

    // Write last run
    out.write((char*)&count, 1);
    out.write((char*)&prev, 1);

    out.close();
    return 0;
}
