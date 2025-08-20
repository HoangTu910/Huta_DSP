#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <iostream>
#include <vector>
#include <string>

namespace Debug {
    template <typename T>
    void printVar(const char* name, const T& value,
                  const char* file, int line, const char* func) {
        std::cerr << "[" << file << ":" << line << " in " << func << "] "
                  << name << " = " << value << std::endl;
    }

    template <typename T1, typename T2>
    void printVar2(const char* name1, const T1& value1,
                   const char* name2, const T2& value2,
                   const char* file, int line, const char* func) {
        std::cerr << "[" << file << ":" << line << " in " << func << "] "
                  << name1 << " = " << value1 << ", "
                  << name2 << " = " << value2 << std::endl;
    }

    template <typename T>
    void printVec(const char* name, const std::vector<T>& v,
                  const char* file, int line, const char* func) {
        std::cerr << "[" << file << ":" << line << " in " << func << "] "
                  << name << " = ";
        for (auto& x : v) std::cerr << x << " ";
        std::cerr << std::endl;
    }
}

#define hu_debug(x) Debug::printVar(#x, (x), __FILE__, __LINE__, __func__)
#define hu_debug2(x,y) Debug::printVar2(#x, (x), #y, (y), __FILE__, __LINE__, __func__)
#define hu_debugVec(v) Debug::printVec(#v, (v), __FILE__, __LINE__, __func__)
#define hu_alert(msg) std::cerr << "[" << __FILE__ << ":" << __LINE__ << " in " << __func__ << "] " << msg << std::endl

#endif
