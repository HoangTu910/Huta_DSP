#ifndef IDSPBlock_hpp
#define IDSPBlock_hpp

#include <cstdint>
#include <vector>

class IDSPBlock {
public:
    virtual ~IDSPBlock() = default;
    virtual void process(std::vector<double>& buffer) = 0;
};

#endif // IDSPBlock_hpp