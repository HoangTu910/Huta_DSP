#ifndef _DEBUG_H_
#define _DEBUG_H_

#include<iostream>

#define debug(x) std::cerr << #x << " = " << x << std::endl
#define debug2(x, y) std::cerr << #x << " = " << x << ", " << #y << " = " << y << std::endl
#define debugVec(v) std::cerr << #v << " = "; for (auto _ : v) std::cerr << _ << ' '; std::cerr << '\n';

#endif