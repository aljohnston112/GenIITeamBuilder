#include "rapidjson/reader.h"
#include <iostream>

using namespace rapidjson;
using namespace std;

struct MyHandler {

    bool Null() { return true; }

    bool Bool(bool b) { return true; }

    bool Int(int i) { return true; }

    bool Uint(unsigned u) { return true; }

    bool Int64(int64_t i) { return true; }

    bool Uint64(uint64_t u) { return true; }

    bool Double(double d) { return true; }

    bool RawNumber(const char *str, SizeType length, bool copy) {
        return true;
    }

    bool String(const char *str, SizeType length, bool copy) {
        return true;
    }

    bool StartObject() { return true; }

    bool Key(const char *str, SizeType length, bool copy) {
        return true;
    }

    bool EndObject(SizeType memberCount) { return true; }

    bool StartArray() { return true; }

    bool EndArray(SizeType elementCount) { return true; }

};
