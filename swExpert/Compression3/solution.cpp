// J (32) A (16) N (8) G (4) S E O K

#include <iostream>
#include <bitset>

#define MAXK 10000000

int  comp(unsigned char dest[10000000], unsigned char src[10000000]) {

    int bitMask[8] = { 0b00000000,
                       0b00000001,
                       0b00000011,
                       0b00000111,
                       0b00001111,
                       0b00011111,
                       0b00111111,
                       0b01111111
    };

    int charIdx[256];

    charIdx['J'] = 0;
    charIdx['A'] = 1;
    charIdx['N'] = 2;
    charIdx['G'] = 3;
    charIdx['S'] = 4;
    charIdx['E'] = 5;
    charIdx['O'] = 6;
    charIdx['K'] = 7;

    int bitCount[8] = { 1, 2, 3, 4, 6, 6, 6, 6 };

    int bitSum = 0;

    int bitNum[8] = { 0, 2, 6, 14, 63, 62, 61, 60 };


    for (int i = 0; i < MAXK; i++) {
        int cnt = bitCount[charIdx[src[i]]];

        int remain = 8 - (bitSum % 8);
        if (remain < cnt) {
            dest[bitSum >> 3] <<= remain;
            dest[bitSum >> 3] |= (bitNum[charIdx[src[i]]] >> (cnt - remain) );
            bitSum += remain;
            cnt -= remain;
        }

        dest[bitSum >> 3] <<= cnt;
        dest[bitSum >> 3] |= (bitNum[charIdx[src[i]]] & bitMask[cnt]);

        bitSum += cnt;
    }

    int shift = 8 - (bitSum % 8);
    dest[bitSum >> 3] <<= shift;
    bitSum += shift;

    return bitSum / 8;
}

void decomp(unsigned char src[10000000], unsigned char dest[10000000]) {

    unsigned char charArr[64];

    charArr[0] = 'J';
    charArr[2] = 'A';
    charArr[6] = 'N';
    charArr[14] = 'G';
    charArr[63] = 'S';
    charArr[62] = 'E';
    charArr[61] = 'O';
    charArr[60] = 'K';

    int bitSum = 0;

    int buffer = 0;

    int bitMask[8] = { 0b10000000,
                       0b01000000,
                       0b00100000,
                       0b00010000,
                       0b00001000,
                       0b00000100,
                       0b00000010,
                       0b00000001
    };

    int idx = 0;
    int cnt = 0;

    while (idx < MAXK) {

//        std::bitset<8> x(dest[bitSum >> 3]);
//        std::cout << (bitSum >> 3) << ": " << x << std::endl;

        unsigned char val = dest[bitSum >> 3];
        int maskNum = bitSum % 8;

        unsigned char masked = val & bitMask[maskNum];
        masked >>= (7 - maskNum);

        buffer <<= 1;
        buffer |= masked;

        cnt++;
        bitSum++;

        if (cnt <= 4 && masked == 0) {
            cnt = 0;
            src[idx++] = charArr[buffer];
            buffer = 0;
        }

        else if (cnt == 6) {
            cnt = 0;
            src[idx++] = charArr[buffer];
            buffer = 0;
        }

    }

}