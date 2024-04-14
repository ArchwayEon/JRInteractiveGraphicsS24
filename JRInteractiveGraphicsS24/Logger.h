#pragma once
#include <sstream>
#include <vector>

class Logger
{
protected:
    static std::stringstream _log;

public:
    static const std::string GetLog(){
        return _log.str();
    }

    static void Log(const std::string& message) {
        _log << message << std::endl;
    }

    static void Log(const char* message) {
        _log << message << std::endl;
    }

    static void Log(const std::vector<char>& infoLog) {
        std::copy(
            infoLog.begin(), infoLog.end(), 
            std::ostream_iterator<char>(_log, "")
        );
    }
};



