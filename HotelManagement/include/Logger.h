#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class Logger {
private:
    static Logger* instance;
    string logFile;

    Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    string getCurrentTime() const;

public:
    enum class Level { INFO, WARNING, ERROR };

    static Logger& getInstance();
    static void destroy();

    void log(Level level, const string& message);
    void info(const string& message);
    void warning(const string& message);
    void error(const string& message);

    vector<string> getRecentLogs(int n) const;
    void clearLogs();

    void operator()(const string& message);
    void operator()(Level level, const string& message);
};

#endif
