#pragma once

#include <llama.h>
#include <vector>

class Tokenizer {
public:
    virtual std::vector<llama_token> generateTokens() = 0;
    virtual ~Tokenizer() = default;
};
