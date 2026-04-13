#pragma once

#include "LlamaBackendSingleton.h"
#include "LlamaContext.h"
#include "LlamaModel.h"
#include "Prompt.h"
#include "ModelPrompt.h"
#include <llama.h>
#include <mtmd.h>
#include <memory>
#include <string>
#include <cstdint>

class Tokenizer {
private:
    std::shared_ptr<LlamaBackendSingleton> backend;
    std::shared_ptr<LlamaModel> model;
    std::shared_ptr<LlamaContext> context;
    std::unique_ptr<ModelPrompt> modelPrompt;
    mtmd_context* mtmdContext;
    int64_t cachedSystemPromptEnd;
    bool resetCacheNextPrompt;

    int64_t registerPrompt(const Prompt* prompt);
public:
    Tokenizer(
        const std::string& mmprojPath,
        const mtmd_context_params& params,
        std::shared_ptr<LlamaContext> contextPtr,
        std::shared_ptr<LlamaModel> modelPtr
    );
    void registerSystemPrompt(const Prompt* prompt);
    void registerAdditionalPrompts(const Prompt* prompt);
    void terminatePrompt();

    Tokenizer(const Tokenizer&) = delete;
    Tokenizer& operator=(const Tokenizer&) = delete;
    ~Tokenizer();
};
