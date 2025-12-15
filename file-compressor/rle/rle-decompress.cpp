// cat compressed.rle | ./rle-decompress restored.cpp

#include <iostream>
#include <fstream>
#include <cstdint>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: rle_decompress <output_file>\n";
        return 1;
    }

    ofstream out(argv[1], ios::binary);
    if (!out) return 1;

    uint8_t count;
    unsigned char value;

    while (cin.read((char*)&count, 1) &&
           cin.read((char*)&value, 1)) {
        for (int i = 0; i < count; i++)
            out.write((char*)&value, 1);
    }

    return 0;
}
