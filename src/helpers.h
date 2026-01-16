#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>


inline void trim(std::string &s) {
    while (!s.empty() && (s.front() == ' ' || s.front() == '\t' || s.front() == '\n' || s.front() == '\r')) {
        s.erase(s.begin());
    }
    while (!s.empty() && (s.back() == ' ' || s.back() == '\t' || s.back() == '\n' || s.back() == '\r')) {
        s.pop_back();
    }
}

inline void remove_all_whitespace(std::string &s) {
    s.erase(std::remove_if(s.begin(), s.end(), [](unsigned char c) {
        return c == ' ' || c == '\t' || c == '\n' || c == '\r';
    }), s.end());
}

// Devuelve una copia en minúsculas (para comparar sin distinguir mayúsculas/minúsculas)
inline std::string toLowerCopy(std::string s)
{
    for (char &ch : s)
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    return s;
}

