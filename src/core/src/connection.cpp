#include <string>
#include "connection.h"

using Core::ConnectionPool;

#ifdef ENABLE_MYSQL
ConnectionPool<Core::Osirose> &Core::connectionPool = ConnectionPool<Core::Osirose>::getInstance();
#endif

static bool replaceAll(std::string &str, const std::string &from, const std::string &to) {
    size_t pos = 0;
    bool isFound = false;
    while ((pos = str.find(from, pos)) != std::string::npos) {
        str.replace(pos, from.length(), to);
        pos += to.length();
        isFound = true;
    }
    return isFound;
}

std::string Core::escapeData(const std::string &data) {
    std::string result{data};
    replaceAll(result, "'", "\\'");
    replaceAll(result, "\"", "\\\"");
    replaceAll(result, "%", "\\%");
    replaceAll(result, "_", "\\_");
    replaceAll(result, ";", "");
    return result;
}
