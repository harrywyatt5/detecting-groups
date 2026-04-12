#pragma once

#include <llama.h>
#include <memory>

class LlamaBackendSingleton {
private:
    LlamaBackendSingleton();
    ~LlamaBackendSingleton();

    static std::shared_ptr<LlamaBackendSingleton> instance;
public:
    static std::shared_ptr<LlamaBackendSingleton> getOrCreateInstance();
};
