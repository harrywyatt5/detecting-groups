#include "MixedUserPrompt.h"

#include "UserPrompt.h"
#include <mtmd.h>
#include <string>

const std::string MixedUserPrompt::MEDIA_TAG = "<__media__>\n";

MixedUserPrompt::MixedUserPrompt(mtmd::bitmap image) : bitmap(std::move(image)) {
    prompt = UserPrompt::PREFIX + MixedUserPrompt::MEDIA_TAG + UserPrompt::SUFFIX;
}

MixedUserPrompt::MixedUserPrompt(const std::string& basePrompt, mtmd::bitmap image) : bitmap(std::move(image)) {
    prompt = UserPrompt::PREFIX + MixedUserPrompt::MEDIA_TAG + basePrompt + UserPrompt::SUFFIX;
}

const std::string& MixedUserPrompt::getPrompt() const {
    return prompt;
}

const mtmd::bitmap& MixedUserPrompt::getBitmap() const {
    return bitmap;
}
