#pragma once

#include <llama.h>
#include <mtmd.h>
#include <string>

class LlamaModel {
private:
    llama_model* model;
    llama_context* context;
    mtmd_context* mtmdContext;

    int kvCacheEnd;
public:
    LlamaModel(const std::string& modelPath, const std::string& mmprojPath);
};
