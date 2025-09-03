#include "../includes/pipeline/AudioPipeline.hpp"

void AudioPipeline::addBlock(std::unique_ptr<IDSPBlock> block)
{
    m_chain.push_back(std::move(block));
}

void AudioPipeline::process(std::vector<double> &buffer) {
    for(auto &block : m_chain) {
        block->process(buffer);
    }
}