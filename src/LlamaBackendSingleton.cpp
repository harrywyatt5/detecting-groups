#include "LlamaBackendSingleton.h"

#include <llama.h>
#include <memory>

std::shared_ptr<LlamaBackendSingleton> LlamaBackendSingleton::instance = nullptr;

LlamaBackendSingleton::LlamaBackendSingleton() {
    llama_backend_init();
}

LlamaBackendSingleton::~LlamaBackendSingleton() {
    llama_backend_free();
}

std::shared_ptr<LlamaBackendSingleton> LlamaBackendSingleton::getOrCreateInstance() {
    if (instance == nullptr) {
        instance = std::shared_ptr<LlamaBackendSingleton>(new LlamaBackendSingleton());
    }

    return instance;
}
