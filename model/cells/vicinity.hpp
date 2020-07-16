//
// Created by binybrion on 6/29/20.
//

#ifndef CELL_DEVS_ZHONG_DEVEL_VICINITY_H
#define CELL_DEVS_ZHONG_DEVEL_VICINITY_H

#include <functional>
#include <cmath>
#include <nlohmann/json.hpp>
#include "helper_functions.hpp"

struct vicinity
{
    double correlation = 1.0f;
    std::map<float, float> neighbourhood_influence_rates;

    explicit vicinity(double correlation) : correlation{correlation} {}

    vicinity(){}
};

void from_json(const nlohmann::json &json, vicinity &vicinity)
{
   json.at("correlation").get_to(vicinity.correlation);

   std::map<std::string, float> unparsed_neighbourhood_influence_rates;

    json.at("neighbour_influence").get_to(unparsed_neighbourhood_influence_rates);

    vicinity.neighbourhood_influence_rates = parse_json_string_pairs(unparsed_neighbourhood_influence_rates);
}

#endif //CELL_DEVS_ZHONG_DEVEL_VICINITY_H