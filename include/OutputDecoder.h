#pragma once

#include "LlamaModel.h"
#include "LlamaContext.h"
#include "LlamaBackendSingleton.h"
#include <llama.h>
#include <string>
#include <memory>

class OutputDecoder {
private:
    std::shared_ptr<LlamaBackendSingleton> backend;
    std::shared_ptr<LlamaContext> contextPtr;
    std::shared_ptr<LlamaModel> modelPtr;
    llama_sampler* sampler;
    const llama_vocab* vocab;
public:
    OutputDecoder(
        const llama_sampler_chain_params& params,
        std::shared_ptr<LlamaContext> context,
        std::shared_ptr<LlamaModel> model
    );

    std::string sampleModelOutput(int maxTokens = 128);
    ~OutputDecoder();
};
