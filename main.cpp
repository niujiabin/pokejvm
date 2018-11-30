#include<fstream>
#include<iostream>
#include<cstring>
#include <cstdint>
#include <netinet/in.h>

using namespace std;

int main() {

    const char *classFileName = "/home/jiabinniu/CLionProjects/pokejvm/java/Test.class";

    ifstream inFile(classFileName, ios::binary);
    if (!inFile) {
        cerr << endl << "Read File error" << endl;
        return -1;
    }

    //获取魔术

    uint32_t real_magic = 3405691582;

    uint32_t magic = 0;

    uint16_t minor_version = 0;
    uint16_t major_version = 0;

    inFile.read((char *) &magic, sizeof(magic));
    magic = htonl(magic);

    //检查魔术
    if (magic == real_magic) {
        cout << "check magic succeed" << endl;
    } else {
        cout << "check magic failed" << endl;
    }

    //判断major_version 和　minor_version
    inFile.read((char *) &minor_version, sizeof(minor_version));
    minor_version = htonl(minor_version);

    inFile.read((char *) &major_version, sizeof(major_version));
    major_version = htonl(major_version);

    cout << "minor version:" << minor_version << endl;
    cout << "major version:" << major_version << endl;


}