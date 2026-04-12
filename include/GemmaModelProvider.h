#pragma once

#include "Tokenizer.h"
#include "LlamaContext.h"
#include <llama.h>
#include <mtmd.h>
#include <string>
#include <memory>

class GemmaModelProvider {
private:
    llama_model* model;
    std::shared_ptr<LlamaContext> context;
    Tokenizer tokenizer;
    const llama_vocab* vocab;
public:
    GemmaModelProvider(const std::string& modelPath, const std::string& mmprojPath);
};
