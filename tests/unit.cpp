#include <iostream>

#include "../src/bdecoder.hpp"

int main(int argc, char** argv)
{
    std::string encoded("d3:bar4:spam3:fooi42ee");
    std::cout << "Test CASE # 1" << std::endl;

    BencodeNode root
        (
            BDecoder(encoded).decode()
        );

    std::cout << encoded << "\n" << root.toString() << "\n" << root.encode() << "\n";

    return 0;
}
