#include "Logger.h"
#include <ctime>
#include <sstream>
#include <iomanip>

// ========== 单例实现 ==========

Logger* Logger::instance = nullptr;

Logger::Logger() : logFile("data/system.log") {}

Logger& Logger::getInstance() {
    if (instance == nullptr) {
        instance = new Logger();
    }
    return *instance;
}

void Logger::destroy() {
    delete instance;
    instance = nullptr;
}

// ========== 内部工具 ==========

string Logger::getCurrentTime() const {
    time_t now = time(nullptr);
    tm* t = localtime(&now);
    ostringstream oss;
    oss << setfill('0')
        << (1900 + t->tm_year) << "-"
        << setw(2) << (1 + t->tm_mon) << "-"
        << setw(2) << t->tm_mday << " "
        << setw(2) << t->tm_hour << ":"
        << setw(2) << t->tm_min << ":"
        << setw(2) << t->tm_sec;
    return oss.str();
}

// ========== 核心日志方法 ==========

void Logger::log(Level level, const string& message) {
    string levelStr;
    switch (level) {
        case Level::INFO:    levelStr = "INFO";    break;
        case Level::WARNING: levelStr = "WARNING"; break;
        case Level::ERROR:   levelStr = "ERROR";   break;
    }

    string entry = "[" + getCurrentTime() + "] [" + levelStr + "] " + message;

    ofstream file(logFile, ios::app);
    if (file) {
        file << entry << endl;
    }
}

void Logger::info(const string& message) {
    log(Level::INFO, message);
}

void Logger::warning(const string& message) {
    log(Level::WARNING, message);
}

void Logger::error(const string& message) {
    log(Level::ERROR, message);
}

vector<string> Logger::getRecentLogs(int n) const {
    vector<string> lines;
    ifstream file(logFile);
    if (!file) return lines;

    string line;
    while (getline(file, line)) {
        lines.push_back(line);
    }

    // 取最后 n 条
    if (n < static_cast<int>(lines.size())) {
        vector<string> recent(lines.end() - n, lines.end());
        return recent;
    }
    return lines;
}

void Logger::clearLogs() {
    ofstream file(logFile, ios::trunc);
}

// ========== operator() 重载 ==========

void Logger::operator()(const string& message) {
    log(Level::INFO, message);
}

void Logger::operator()(Level level, const string& message) {
    log(level, message);
}
