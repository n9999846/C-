#ifndef FILE_HELPER_H
#define FILE_HELPER_H

#include <vector>
#include <string>

using namespace std;

class FileHelper {
public:
    static bool fileExists(const string& path);
    static vector<string> readLines(const string& path);
    static bool writeLines(const string& path, const vector<string>& lines);
    static bool appendLine(const string& path, const string& line);
    static bool backupFile(const string& path);
};

#endif
