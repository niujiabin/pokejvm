#include<fstream>
#include<iostream>
#include<cstring>
#include <cstdint>
#include <netinet/in.h>

using namespace std;

#define CONSTANT_Methodref 10
#define CONSTANT_Fieldref 9
#define CONSTANT_InterfaceMethodref 11

struct cp_info {
    uint8_t tag;
    uint16_t name_index;
};

/**
 *
 *
00000000: cafe babe 0000 0034 0029 0a00 0900 1a09  .......4.)......
00000010: 0003 001b 0700 1c0a 0003 001a 0a00 0300  ................
00000020: 1d09 001e 001f 0a00 0300 200a 0021 0022  .......... ..!."
00000030: 0700 2301 0004 6e61 6d65 0100 124c 6a61  ..#...name...Lja
00000040: 7661 2f6c 616e 672f 5374 7269 6e67 3b01  va/lang/String;.
00000050: 0003 6167 6501 0001 4901 0006 3c69 6e69  ..age...I...<ini
00000060: 743e 0100 0328 2956 0100 0443 6f64 6501  t>...()V...Code.
00000070: 000f 4c69 6e65 4e75 6d62 6572 5461 626c  ..LineNumberTabl
00000080: 6501 0006 6765 7441 6765 0100 0328 2949  e...getAge...()I
00000090: 0100 0673 6574 4167 6501 0004 2849 2956  ...setAge...(I)V
000000a0: 0100 046d 6169 6e01 0016 285b 4c6a 6176  ...main...([Ljav
000000b0: 612f 6c61 6e67 2f53 7472 696e 673b 2956  a/lang/String;)V
000000c0: 0100 0a53 6f75 7263 6546 696c 6501 0009  ...SourceFile...
000000d0: 5465 7374 2e6a 6176 610c 000e 000f 0c00  Test.java.......
000000e0: 0c00 0d01 0004 5465 7374 0c00 1400 1507  ......Test......
000000f0: 0024 0c00 2500 260c 0012 0013 0700 270c  .$..%.&.......'.
00000100: 0028 0015 0100 106a 6176 612f 6c61 6e67  .(.....java/lang
00000110: 2f4f 626a 6563 7401 0010 6a61 7661 2f6c  /Object...java/l
00000120: 616e 672f 5379 7374 656d 0100 036f 7574  ang/System...out
00000130: 0100 154c 6a61 7661 2f69 6f2f 5072 696e  ...Ljava/io/Prin
00000140: 7453 7472 6561 6d3b 0100 136a 6176 612f  tStream;...java/
00000150: 696f 2f50 7269 6e74 5374 7265 616d 0100  io/PrintStream..
00000160: 0570 7269 6e74 0021 0003 0009 0000 0002  .print.!........
00000170: 0000 000a 000b 0000 0000 000c 000d 0000  ................
00000180: 0004 0001 000e 000f 0001 0010 0000 001d  ................
00000190: 0001 0001 0000 0005 2ab7 0001 b100 0000  ........*.......

 * @return
 */
int main() {

    const char *classFileName = "/home/jiabinniu/CLionProjects/pokejvm/java/Test.class";

    ifstream inFile((classFileName), ios::binary);
    if (!inFile) {
        cerr << endl << "Read File error" << endl;
        return -1;
    }

    //获取魔术

    uint32_t real_magic = 0xCAFEBABE;

    uint32_t magic = 0;
    uint16_t minor_version = 0;
    uint16_t major_version = 0;
    uint16_t constant_pool_count = 0;
    cp_info **constant_pool = nullptr;

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
    minor_version = htons(minor_version);
    // a = htonl(minor_version);

    inFile.read((char *) &major_version, sizeof(major_version));
    major_version = htons(major_version);

    cout << "minor version:" << minor_version << endl;
    cout << "major version:" << major_version << endl;

    //验证版本号

    //常量池数量
    inFile.read((char *) &constant_pool_count, sizeof(constant_pool_count));
    constant_pool_count = htons(constant_pool_count);
    cout << "constant_pool_count:" << constant_pool_count << endl;

    if (constant_pool_count != -1) {
        constant_pool = new cp_info *[constant_pool_count - 1];
        //常量池信息
        for (int i = 0; i < 1; ++i) {
            //获取常量池的tag
            uint8_t tag = inFile.peek();
            cout << "tag:" << (int) tag << endl;
            switch (tag) {
                case CONSTANT_Fieldref:
                    cout << "parse constant pool match CONSTANT_Fieldref" << endl;
                case CONSTANT_Methodref:
                    cout << "parse constant pool match CONSTANT_Methodref" << endl;
                case CONSTANT_InterfaceMethodref:
                    cout << "parse constant pool match CONSTANT_InterfaceMethodref" << endl;
                    //class_index
                    break;
            }
        }
    }
}