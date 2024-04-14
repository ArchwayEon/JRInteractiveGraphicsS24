#include "TextFile.h"
#include <sstream>
#include <fstream>
#include "Logger.h"

bool TextFile::Read(const std::string& filePath)
{
    std::stringstream ss;
    std::ifstream fin{};
    fin.open(filePath.c_str());
    if (fin.fail()) {
        ss << "Could not open: " << filePath << std::endl;
        Logger::Log(ss.str());
        return false;
    }

    std::string line;
    while (!fin.eof()) {
        getline(fin, line);
        Trim(line);
        if (line != "") { // Skip blank lines
            ss << line << std::endl;
        }
    }
    fin.close();
    data = ss.str();
    return true;
}

void TextFile::Trim(std::string& str)
{
    const std::string delimiters = " \f\n\r\t\v";
    str.erase(str.find_last_not_of(delimiters) + 1);
    str.erase(0, str.find_first_not_of(delimiters));
}
