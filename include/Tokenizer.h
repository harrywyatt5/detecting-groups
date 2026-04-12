#pragma once

#include "LlamaContext.h"
#include "LlamaModel.h"
#include <llama.h>
#include <mtmd.h>
#include <memory>

class Tokenizer {
private:
    std::shared_ptr<LlamaModel> model;
    std::shared_ptr<LlamaContext> context;
    mtmd_context* mtmdContext;
    int cachedSystemPromptEnd;
public:
    Tokenizer(
        const std::string& mmprojPath,
        const mtmd_context_params& params,
        std::shared_ptr<LlamaContext> context,
        std::shared_ptr<LlamaModel> model
    );
    void registerSystemPrompt();
    void resetUserPrompt();
    void registerUserPrompt();
    void registerUserPromptWithMultimedia();
};
