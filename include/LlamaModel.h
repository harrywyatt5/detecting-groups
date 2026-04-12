#pragma once

#include <llama.h>
#include <string>
#include <stdexcept>

class LlamaModel {
private:
    llama_model* model;
public:
    LlamaModel(const std::string& modelPath, const llama_model_params& params) {
        model = llama_model_load_from_file(modelPath.c_str(), params);

        if (!model) {
            throw std::runtime_error("Failed to load model");
        }
    }

    const llama_model* ptr() const {
        return model;
    }

    llama_model* ptr() {
        return model;
    }

    ~LlamaModel() {
        if (model != nullptr) {
            llama_free_model(model);
            model = nullptr;
        }
    }
};
