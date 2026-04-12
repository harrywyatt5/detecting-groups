#include "Tokenizer.h"

#include "LlamaContext.h"
#include "LlamaModel.h"
#include "Prompt.h"
#include "SystemPrompt.h"
#include "ModelPrompt.h"
#include "MixedUserPrompt.h"
#include <llama.h>
#include <mtmd.h>
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <cstdint>

Tokenizer::Tokenizer(
    const std::string& mmprojPath,
    const mtmd_context_params& params,
    std::shared_ptr<LlamaContext> contextPtr,
    std::shared_ptr<LlamaModel> modelPtr
) : model(modelPtr), context(contextPtr), modelPrompt(std::make_unique<ModelPrompt>()), cachedSystemPromptEnd(0), resetCacheNextPrompt(false) {
    mtmdContext = mtmd_init_from_file(mmprojPath.c_str(), model->ptr(), params);

    if (!mtmdContext) {
        throw std::runtime_error("Could not load Tokenizer: could not load mtmd_context");
    }
}

int64_t Tokenizer::registerPrompt(const Prompt* prompt) {
    mtmd_input_text input;
    input.text = prompt->getPrompt().c_str();
    input.add_special = false;
    input.parse_special = true;

    // We don't allow image prompts in the system prompt
    mtmd::input_chunks chunks;
    chunks.ptr.reset(mtmd_input_chunks_init());

    const MixedUserPrompt* mixedUserPrompt;
    if ((mixedUserPrompt = dynamic_cast<const MixedUserPrompt*>(prompt)) == nullptr) {
        mtmd_tokenize(mtmdContext, chunks.ptr.get(), &input, nullptr, 0);
    } else {
        const mtmd_bitmap* constBitmap = mixedUserPrompt->getBitmap().ptr.get();
        mtmd_tokenize(mtmdContext, chunks.ptr.get(), &input, &constBitmap, 1);
    }
    
    int64_t totalTokens = 0;
    for (size_t i = 0; i < chunks.size(); ++i) {
        const mtmd_input_chunk* chunk = chunks[i];
        auto chunkType = mtmd_input_chunk_get_type(chunk);

        size_t currChunkSize = 0;
        if (chunkType == MTMD_INPUT_CHUNK_TYPE_TEXT) {
            const llama_token* token = mtmd_input_chunk_get_tokens_text(chunk, &currChunkSize);

            llama_batch batch = llama_batch_get_one(const_cast<llama_token*>(token), currChunkSize);
            llama_decode(context->ptr(), batch);
        } else if (chunkType == MTMD_INPUT_CHUNK_TYPE_IMAGE) {
            mtmd_encode_chunk(mtmdContext, chunk);
            float* embed = mtmd_get_output_embd(mtmdContext);
            currChunkSize = mtmd_input_chunk_get_n_tokens(chunk);

            std::vector<llama_token> dummyVec(currChunkSize, 0);
            llama_batch batch = llama_batch_get_one(dummyVec.data(), currChunkSize);
            // We basically fill in the object with false 0, and then just reset the pointer to null
            // and provide the image embed instead
            batch.token = nullptr;
            batch.embd = embed;
            llama_decode(context->ptr(), batch);
        } else {
            std::cerr << "Skipping unknown chunk type\n";
        }

        totalTokens += currChunkSize;
    }

    return totalTokens;
}

void Tokenizer::registerSystemPrompt(const Prompt* prompt) {
    if (dynamic_cast<const SystemPrompt*>(prompt) == nullptr) {
        std::cerr << "Prompt given to registerSystemPrompt is not a SystemPrompt. Expect undefined behaviour\n";
    }

    cachedSystemPromptEnd = registerPrompt(prompt); 
}

void Tokenizer::registerAdditionalPrompts(const Prompt* prompt) {
    if (resetCacheNextPrompt) {
        auto* memory = llama_get_memory(context->ptr());
        llama_memory_seq_rm(memory, 0, cachedSystemPromptEnd, -1);
        resetCacheNextPrompt = false;
    }

    registerPrompt(prompt);
}

void Tokenizer::terminatePrompt() {
    resetCacheNextPrompt = true;

    registerPrompt(modelPrompt.get());
}

Tokenizer::~Tokenizer() {
    mtmd_free(mtmdContext);
}
