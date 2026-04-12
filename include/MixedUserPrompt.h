#pragma once

#include "UserPrompt.h"
#include <mtmd.h>
#include <string>

class MixedUserPrompt : public UserPrompt {
private:
    mtmd::bitmap bitmap;

    static const std::string MEDIA_TAG;
public:
    MixedUserPrompt(mtmd::bitmap image);
    MixedUserPrompt(const std::string& basePrompt, mtmd::bitmap image);
    const std::string& getPrompt() const override;
    const mtmd::bitmap& getBitmap() const;
};
