#include "ModelPrompt.h"

#include <string>

ModelPrompt::ModelPrompt() {
    prompt = "<|turn>model\n";
}

const std::string& ModelPrompt::getPrompt() const {
    return prompt;
}
