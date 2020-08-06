//
// Created by binybrion on 7/3/20.
//

#ifndef PANDEMIC_HOYA_2002_SIMULATION_CONFIGURATION_HPP
#define PANDEMIC_HOYA_2002_SIMULATION_CONFIGURATION_HPP

#include <nlohmann/json.hpp>

struct simulation_configuration
{
    // To make the parameters of the correction_factors variable more obvious
    using infection_threshold = float;
    using mobility_correction_factor = std::array<float, 2>; // The first value is the mobility correction factor;
                                                             // The second one is the hysteresis factor.

    std::map<infection_threshold, mobility_correction_factor> correction_factors;
    double disobedient;
    int precision;
    using phase_rates = std::vector<            // The age sub_division
                        std::vector<double>>;   // The stage of infection

    phase_rates virulence_rates;
    phase_rates recovery_rates;
    phase_rates mobility_rates;
    phase_rates fatality_rates;

    double hospital_infected_capacity;
    double over_capacity_fatality_modifier;
    bool SIIRS_model = true;
};

void from_json(const nlohmann::json& j, simulation_configuration &v) {

    j.at("disobedient").get_to(v.disobedient);
    j.at("precision").get_to(v.precision);
    j.at("virulence_rates").get_to(v.virulence_rates);
    j.at("SIIRS_model").get_to(v.SIIRS_model);
    j.at("recovery_rates").get_to(v.recovery_rates);
    j.at("mobility_rates").get_to(v.mobility_rates);
    j.at("fatality_rates").get_to(v.fatality_rates);
    j.at("hospital_infected_capacity").get_to(v.hospital_infected_capacity);
    j.at("over_capacity_fatality_modifier").get_to(v.over_capacity_fatality_modifier);

    std::map<std::string, std::array<float, 2>> unparsed_infection_correction_factors;

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

        if(infection_threshold < 0.0f || infection_threshold > 1.0f) {
            std::string error_message = "Invalid key specified (a values must be in the range of [0, 1]:";
            error_message += "Key: " + i.first;

            throw std::invalid_argument{error_message};
        }

        if(i.second.front() < 0.0f || i.second.front() > 1.0f) {
            std::string error_message = "For the key: " + i.first + " the mobility factor must be in the range of: [0, 1].";
            error_message += " Erroneous value: " + std::to_string(i.second.front());

            throw std::invalid_argument{error_message};
        }

        if(i.second.back() < 0.0f || i.second.back() > infection_threshold) {
            std::string error_message = "Invalid hysteresis specified for the key: " + i.first;
            error_message += " The hysteresis value (" + std::to_string(i.second.back()) + ") must not exceed:";
            error_message += "(" + std::to_string(i.second.front()) + ")";

            throw std::runtime_error{error_message};
        }

        v.correction_factors.insert({infection_threshold, i.second});
    }

    for(int i = 0; i < v.recovery_rates.size(); ++i) {
        for(int k = 0; k < v.recovery_rates[i].size(); ++k){
            // A sum of greater than one refers to more than the entire population of an infection stage.
            assert(v.recovery_rates[i][k] + v.fatality_rates[i][k] <= 1.0f && "The recovery rate + fatality rate cannot exceed 1!");
        }
        assert(v.fatality_rates[i].back() <= 1.0f && "The fatality rate cannot exceed one!"); // Assert because the recovery rate has
                                                                                               // one less entry than the fatality rates.
    }
}

#endif //PANDEMIC_HOYA_2002_SIMULATION_CONFIGURATION_HPP
