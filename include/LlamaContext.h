#pragma once

#include <llama.h>

class LlamaContext {
private:
    llama_context* context;
public:
    LlamaContext(llama_model* model, const llama_context_params& params) {
        context = llama_init_from_model(model, params);
    }

    llama_context* ptr() {
        return context;
    }

    const llama_context* ptr() const {
        return context;
    }

    ~LlamaContext() {
        if (context != nullptr) {
            llama_free(context);
            context = nullptr;
        }
    }
};
