#include "base64Decoder.hpp"
#include "serialization/exceptions.hpp"

using namespace smr;

static std::map<char, char> CHAR_INDEXES {
    { 'A', 0 }, { 'B', 1 }, { 'C', 2 }, { 'D', 3 }, { 'E', 4 }, { 'F', 5 }, { 'G', 6 }, { 'H', 7 }, { 'I', 8 },
    { 'J', 9 }, { 'K', 10 }, { 'L', 11 }, { 'M', 12 }, { 'N', 13 }, { 'O', 14 }, { 'P', 15 }, { 'Q', 16 }, { 'R', 17 },
    { 'S', 18 }, { 'T', 19 }, { 'U', 20 }, { 'V', 21 }, { 'W', 22 }, { 'X', 23 }, { 'Y', 24 }, { 'Z', 25 },
    { 'a', 26 }, { 'b', 27 }, { 'c', 28 }, { 'd', 29 }, { 'e', 30 }, { 'f', 31 }, { 'g', 32 }, { 'h', 33 }, { 'i', 34 },
    { 'j', 35 }, { 'k', 36 }, { 'l', 37 }, { 'm', 38 }, { 'n', 39 }, { 'o', 40 }, { 'p', 41 }, { 'q', 42 }, { 'r', 43 },
    { 's', 44 }, { 't', 45 }, { 'u', 46 }, { 'v', 47 }, { 'w', 48 }, { 'x', 49 }, { 'y', 50 }, { 'z', 51 },
    { '0', 52 }, { '1', 53 }, { '2', 54 }, { '3', 55 }, { '4', 56 }, { '5', 57 }, { '6', 58 }, { '7', 59 }, { '8', 60 }, { '9', 61 },
    { '+', 62 }, { '/', 63 }
};

static inline bool isBase64(char c) {
    return std::isalnum(c) || c == '+' || c == '/';
}

void Base64Decoder::flush() {
    for (auto i = 0; i < _buf4Index; i++) {
        _buf4[i] = CHAR_INDEXES[_buf4[i]];
    }

    _buf3[0] = (_buf4[0] << 2) + ((_buf4[1] & 0x30) >> 4);
    _buf3[1] = ((_buf4[1] & 0xf) << 4) + ((_buf4[2] & 0x3c) >> 2);
    _buf3[2] = ((_buf4[2] & 0x3) << 6) + _buf4[3];

    for (auto i = 0; i < (_buf4Index - 1); i++) {
        _oss << _buf3[i];
    }
}

Serializable Base64Decoder::decode() {
    while (_is.peek() != -1 && _is.peek() != '=') {
        auto next = _is.get();
        if (!isBase64(next)) {
            throw UnexpectedTokenException(next);
        }
        _buf4[_buf4Index++] = next;
        if (_buf4Index == 4) {
            flush();
            _buf4Index = 0;
        }
    }
    if (_buf4Index > 0) {
        flush();
    }
    return _oss.str();
}