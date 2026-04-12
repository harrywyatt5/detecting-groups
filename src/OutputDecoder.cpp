#include "OutputDecoder.h"

#include "LlamaBackendSingleton.h"
#include "LlamaContext.h"
#include "LlamaModel.h"
#include <llama.h>
#include <string>
#include <memory>

OutputDecoder::OutputDecoder(
    const llama_sampler_chain_params& params,
    std::shared_ptr<LlamaContext> context,
    std::shared_ptr<LlamaModel> model
) : contextPtr(context), modelPtr(model) {
    backend = LlamaBackendSingleton::getOrCreateInstance();
    vocab = model->getLlamaVocabPtr();
    sampler = llama_sampler_chain_init(params);
    llama_sampler_chain_add(sampler, llama_sampler_init_greedy());
}

std::string OutputDecoder::sampleModelOutput(int maxTokens = 128) {
    llama_sampler_reset(sampler);

    std::string finalOutput;
    finalOutput.reserve(300);

    for (int i = 0; i < maxTokens; ++i) {
        llama_token tokenId = llama_sampler_sample(sampler, contextPtr->ptr(), -1);

        if (llama_vocab_is_eog(vocab, tokenId)) {
            break;
        }

        // Write into char buffer and the append to our curr string
        char buf[128];
        int n = llama_token_to_piece(vocab, tokenId, buf, sizeof(buf), 0, true);
        finalOutput.append(buf, n);

        // We have to propagate the token to the context to weight
        // the next output
        llama_batch batch = llama_batch_get_one(&tokenId, 1);
        llama_decode(contextPtr->ptr(), batch);
    }

    return finalOutput;
}

OutputDecoder::~OutputDecoder() {
    // Dont have to free the vocab because it belongs to the model
    vocab = nullptr;
    if (sampler != nullptr) {
        llama_sampler_free(sampler);
    }
}