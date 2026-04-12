#include "GemmaModelProvider.h"

#include "LlamaModel.h"
#include <llama.h>
#include <string>
#include <memory>

GemmaModelProvider::GemmaModelProvider(
    const std::string& modelPath,
    const std::string& mmprojPath,
    const Prompt* systemPrompt
) {
    llama_model_params params = llama_model_default_params();
    model = std::make_shared<LlamaModel>(modelPath, params);

    llama_context_params contextParams = llama_context_default_params();
    contextParams.n_ctx = 4096;
    context = std::make_shared<LlamaContext>(model->ptr(), contextParams);

    mtmd_context_params mtmdContext = mtmd_context_params_default();
    tokenizer = std::make_unique<Tokenizer>(mmprojPath, mtmdContext, context, model);
    tokenizer->registerSystemPrompt(systemPrompt);
}

std::string GemmaModelProvider::processPrompt(const Prompt* prompt) {
    tokenizer->registerAdditionalPrompts(prompt);
    tokenizer->terminatePrompt();

    // TODO: do some magic and get an output here

    return "";
}

