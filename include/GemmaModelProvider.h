#pragma once

#include "TextOutput.h"
#include "LlamaBackendSingleton.h"
#include "OutputDecoder.h"
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
    std::shared_ptr<LlamaBackendSingleton> backend;
    std::shared_ptr<LlamaModel> model;
    std::shared_ptr<LlamaContext> context;
    // Probably should be inline, but we wanna generate model and ocntext before the tokenizer...
    std::unique_ptr<Tokenizer> tokenizer;
    std::unique_ptr<OutputDecoder> decoder;
public:
    GemmaModelProvider(
        const std::string& modelPath,
        const std::string& mmprojPath,
        const Prompt* systemPrompt
    );

    TextOutput processPrompt(const Prompt* prompt);
};
