#pragma once

#include "Prompt.h"
#include "Tokenizer.h"
#include "LlamaModel.h"
#include "LlamaContext.h"
#include <llama.h>
#include <mtmd.h>
#include <string>
#include <memory>

class GemmaModelProvider {
private:
    std::shared_ptr<LlamaModel> model;
    std::shared_ptr<LlamaContext> context;
    // Probably should be inline, but we wanna generate model and ocntext before the tokenizer...
    std::unique_ptr<Tokenizer> tokenizer;
public:
    GemmaModelProvider(
        const std::string& modelPath,
        const std::string& mmprojPath,
        const Prompt* systemPrompt
    );

    std::string processPrompt(const Prompt* prompt);
};
