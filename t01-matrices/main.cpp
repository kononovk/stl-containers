#include <iostream>
#include <string>
#include <fstream>
#include "matrices.hpp"

void logic(int argc, char *argv[]) {
    if (argc % 2 != 0) {
        throw std::runtime_error("wrong input format");
    }
    std::string_view add("--add"), mult("--mult");
    Matrix m1;
    {
        std::ifstream is(argv[1]);
        if (!is.is_open()) {
            throw std::runtime_error("file does not exist");
        }
        is >> m1;
        if (m1.colsNumber() == 0 || m1.rowsNumber() == 0) {
            throw std::runtime_error("empty matrix");
        }
    }

    for (int i = 1; i < argc - 2; i += 2) {
        std::ifstream is = std::ifstream(argv[i + 2]);
        if (!is.is_open()) {
            throw std::runtime_error("file does not exist");
        }
        Matrix m2;
        is >> m2;
        if (m2.colsNumber() == 0 || m2.rowsNumber() == 0) {
            throw std::runtime_error("empty matrix");
        }
        std::string_view operation(argv[i + 1]);
        if (operation == add) {
            m1 += m2;
        } else if (operation == mult) {
            m1 *= m2;
        } else {
            throw std::runtime_error("can not understand operation");
        }
    }
    std::cout << m1;
}


int main(int argc, char *argv[]) {
    try {
        logic(argc, argv);
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

