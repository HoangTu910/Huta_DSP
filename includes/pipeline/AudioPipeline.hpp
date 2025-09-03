#ifndef _AUDIO_PIPELINE_HPP_
#define _AUDIO_PIPELINE_HPP_

#include <vector>
#include <memory>
#include "../interface/IDSPBlock.hpp"

class AudioPipeline {
public:
    void addBlock(std::unique_ptr<IDSPBlock> block);
    void process(std::vector<double>& buffer);
private:
    std::vector<std::unique_ptr<IDSPBlock>> m_chain;
};

#endif