//
// Created by binybrion on 7/3/20.
//

#ifndef PANDEMIC_HOYA_2002_SIMULATION_CONFIGURATION_HPP
#define PANDEMIC_HOYA_2002_SIMULATION_CONFIGURATION_HPP

#include <nlohmann/json.hpp>
#include "helper_functions.hpp"

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

    v.correction_factors = parse_json_string_pairs(unparsed_infection_correction_factors);
}

#endif //PANDEMIC_HOYA_2002_SIMULATION_CONFIGURATION_HPP
