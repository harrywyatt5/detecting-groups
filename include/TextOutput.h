#pragma once

#include <detecting_groups_custom_msg/msg/present_groups.hpp>
#include <detecting_groups_custom_msg/msg/group.hpp>
#include <string>

class TextOutput {
private:
    std::string internal;

    std::string_view trimInternal() const;
    static std::string WHITESPACE;
public:
    TextOutput(std::string internalStr) : internal(std::move(internalStr)) {}

    const std::string& getRawString() const;
    detecting_groups_custom_msg::msg::PresentGroups generateStructuredResponse() const;
};
