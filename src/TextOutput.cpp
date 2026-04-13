#include "TextOutput.h"

#include "Json.h"
#include <detecting_groups_custom_msg/msg/present_groups.hpp>
#include <detecting_groups_custom_msg/msg/group.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using json = nlohmann::json;

std::string TextOutput::WHITESPACE = " \n\r\t\f\v";
std::string TextOutput::CHANNEL_START = "<|channel>";
std::string TextOutput::CHANNEL_END = "<channel|>";

std::string_view TextOutput::trimInternal() const {
    // Cast to string view
    std::string_view view(internal);
    size_t startPos = view.find_first_not_of(WHITESPACE);
    if (startPos == std::string_view::npos) return "";

    size_t endPos = view.find_last_not_of(WHITESPACE);
    return view.substr(startPos, endPos - startPos + 1);
}

std::string_view TextOutput::removeExternalChannels(std::string_view input) const {
    auto pos = input.find(TextOutput::CHANNEL_START);
    if (pos == std::string_view::npos) return input;

    auto endPos = input.find(TextOutput::CHANNEL_END, pos);
    if (endPos == std::string_view::npos) {
        std::cerr << "Channel tag is never closed!\n";
        return "";
    }

    auto startOfDataPos = endPos + TextOutput::CHANNEL_END.length();
    return input.substr(startOfDataPos, input.length() - startOfDataPos);
}

const std::string& TextOutput::getRawString() const {
    return internal;
}

detecting_groups_custom_msg::msg::PresentGroups TextOutput::generateStructuredResponse() const {
    detecting_groups_custom_msg::msg::PresentGroups msg;
    std::string_view trimmedInternalStr = removeExternalChannels(trimInternal());

    // Array to keep track of who we have seen in the JSON data already
    std::vector<int> accountedFor;
    try {
        json data = json::parse(trimmedInternalStr);

        if (data.is_array()) {
            for (const auto& subArr : data) {
                // Skip non-array subelements
                if (!subArr.is_array()) {
                    std::cerr << "Malforemd LLM output. Skipping invalid array entry...\n";
                    continue;
                }

                detecting_groups_custom_msg::msg::Group group;

                for (const auto& id : subArr) {
                    if (!id.is_number_integer()) {
                        std::cerr << "Malformed LLM output. Skipping invalid element in group\n";
                        continue;
                    }
                    
                    int idAsInt = id.get<int>();
                    if (std::find(accountedFor.begin(), accountedFor.end(), idAsInt) != accountedFor.end()) {
                        std::cerr << "Additional instance of " + std::to_string(idAsInt) + " will be ignored\n";
                        continue;
                    }

                    accountedFor.push_back(idAsInt);
                    group.id.push_back(idAsInt);
                }

                // If we ended up even adding people to this group
                if (group.id.size() > 0) {
                    msg.groups.push_back(group);
                }
            }
        } else {
            std::cerr << "Malformed LLM output. Root object was not an array...\n";
        }
    } catch (const json::parse_error& e) {
        std::cerr << "Failed to parse LLM output. It's likely we got something invalid\n";
        std::cerr << "DEBUG: Actual output from LLM was: " << internal << "\n";
    }

    return msg;
}
