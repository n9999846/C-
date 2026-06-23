#include "FileHelper.h"
#include <fstream>
#include <sstream>

bool FileHelper::fileExists(const string& path) {
    ifstream file(path);
    return file.good();
}

vector<string> FileHelper::readLines(const string& path) {
    vector<string> lines;
    ifstream file(path);
    if (!file) return lines;

    string line;
    while (getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}

bool FileHelper::writeLines(const string& path, const vector<string>& lines) {
    ofstream file(path);
    if (!file) return false;

    for (size_t i = 0; i < lines.size(); i++) {
        file << lines[i];
        if (i < lines.size() - 1) file << endl;
    }
    return true;
}

bool FileHelper::appendLine(const string& path, const string& line) {
    ofstream file(path, ios::app);
    if (!file) return false;
    file << line << endl;
    return true;
}

bool FileHelper::backupFile(const string& path) {
    // 构造备份文件名: xxx.txt -> xxx_backup.txt
    string backupPath;
    size_t dotPos = path.rfind('.');
    if (dotPos != string::npos) {
        backupPath = path.substr(0, dotPos) + "_backup" + path.substr(dotPos);
    } else {
        backupPath = path + "_backup";
    }

    ifstream src(path, ios::binary);
    if (!src) return false;

    ofstream dst(backupPath, ios::binary);
    if (!dst) return false;

    dst << src.rdbuf();
    return true;
}
