//
// Created by binybrion on 7/3/20.
//

#ifndef PANDEMIC_HOYA_2002_SIMULATION_CONFIGURATION_HPP
#define PANDEMIC_HOYA_2002_SIMULATION_CONFIGURATION_HPP

#include <nlohmann/json.hpp>

struct simulation_configuration
{
    // TODO why do you define new types for referring to floats?
    using infection_threshold = float;
    using mobility_correction_factor = float;

    std::map<infection_threshold, mobility_correction_factor> correction_factors;
    double disobedient;
    int precision;
    using phase_rates = std::vector<            // The age sub_division
                        std::vector<double>>;   // The stage of infection

    phase_rates virulence_rates;
    phase_rates recovery_rates;
    phase_rates mobility_rates;
};

void from_json(const nlohmann::json& j, simulation_configuration &v) {

    j.at("disobedient").get_to(v.disobedient);
    j.at("precision").get_to(v.precision);
    j.at("virulence_rates").get_to(v.virulence_rates);
    j.at("recovery_rates").get_to(v.recovery_rates);
    j.at("mobility_rates").get_to(v.mobility_rates);

    std::map<std::string, float> unparsed_infection_correction_factors;

    j.at("infection_correction_factors").get_to(unparsed_infection_correction_factors);

    for (const auto &i : unparsed_infection_correction_factors) {
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
        v.correction_factors.insert({infection_threshold, i.second});
    }

    // Due to how the infection ranges are processed, keys of 0.0 and 1.0 must always exist.
    if (v.correction_factors.find(0.0f) == v.correction_factors.end()) {
        v.correction_factors.insert({0.0f, 0.0f});
    }
    // TODO Maybe we can remove this (the condition is only valid for a single case, a bit strange)
    if (v.correction_factors.find(1.0f) == v.correction_factors.end()) {
        v.correction_factors.insert({1.0f, 0.0});       // The mapped value is never used since the key is 1.0f, as there
                                                                    // is no upper bound range with 1.0 as the lower bound range.
                                                                    // See movement_correction_factor() in zhong_cell.hpp for more details.
                                                                    // As a result, the mapped value does not matter.
    }
}

#endif //PANDEMIC_HOYA_2002_SIMULATION_CONFIGURATION_HPP
