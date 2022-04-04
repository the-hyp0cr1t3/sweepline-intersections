#include <utils.hpp>

void utils::open_file(std::ifstream &inFile, const std::string &fname) {
    inFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);

    try {
        inFile.open(EXAMPLES_PATH + fname);

    } catch (const std::ifstream::failure &e) {
        try {
            inFile.open(fname);

        } catch (const std::ifstream::failure &e) {
            std::cerr << "Error: Could not open " << fname << std::endl;
            std::cerr << e.what() << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    inFile.exceptions(std::ifstream::badbit);
}
