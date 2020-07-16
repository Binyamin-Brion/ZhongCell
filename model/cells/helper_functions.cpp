//
// Created by binybrion on 7/16/20.
//

#include "helper_functions.hpp"

std::map<float, float> parse_json_string_pairs(const std::map<std::string, float> &neighbour_influence_map) {

    std::map<float, float> parsed_pairs;
    
    for (const auto &i : neighbour_influence_map) {
        float infection_threshold;
        try {

            infection_threshold = std::stof(i.first);
        }
        catch(std::invalid_argument &e) {
            // If the key is invalid, the default error message from the exception is not informative.
            // Therefore it is re-thrown with a more descriptive message.
            throw std::invalid_argument{"Failed to parse infection correction factor key: " + i.first};
        }

        if(infection_threshold < 0.0f || infection_threshold > 1.0f || i.second < 0.0f || i.second > 1.0f) {
            std::string error_message = "Invalid key-pair specified (both values must be in the range of [0, 1]:";
            error_message += "Key: " + i.first + " , ";
            error_message += "Mapped value: " + std::to_string(i.second);

            throw std::invalid_argument{error_message};
        }
        parsed_pairs.insert({infection_threshold, i.second});
    }

    // Due to how the infection ranges are processed, keys of 0.0 and 1.0 must always exist.
    if (parsed_pairs.find(0.0f) == parsed_pairs.end()) {
        parsed_pairs.insert({0.0f, 0.0f});
    }
    // TODO Maybe we can remove this (the condition is only valid for a single case, a bit strange)
    if (parsed_pairs.find(1.0f) == parsed_pairs.end()) {
        parsed_pairs.insert({1.0f, 0.0});       // The mapped value is never used since the key is 1.0f, as there
        // is no upper bound range with 1.0 as the lower bound range.
        // See movement_correction_factor() in zhong_cell.hpp for more details.
        // As a result, the mapped value does not matter.
    }

    return parsed_pairs;
}