#pragma once

#include <detecting_groups_custom_msg/msg/present_groups.hpp>
#include <detecting_groups_custom_msg/msg/group.hpp>
#include <string>

class TextOutput {
private:
    std::string internal;

    std::string_view trimInternal() const;
    std::string_view removeExternalChannels(std::string_view input) const;
    static std::string WHITESPACE;
    static std::string CHANNEL_START;
    static std::string CHANNEL_END;
public:
    TextOutput(std::string internalStr) : internal(std::move(internalStr)) {}

    const std::string& getRawString() const;
    detecting_groups_custom_msg::msg::PresentGroups generateStructuredResponse() const;
};
