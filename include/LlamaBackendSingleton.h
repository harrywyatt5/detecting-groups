#pragma once

#include <llama.h>
#include <memory>

class LlamaBackendSingleton {
private:
    LlamaBackendSingleton();

    static std::shared_ptr<LlamaBackendSingleton> instance;
public:
    ~LlamaBackendSingleton();
    static std::shared_ptr<LlamaBackendSingleton> getOrCreateInstance();
};
