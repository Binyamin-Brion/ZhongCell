//
// Created by binybrion on 6/30/20.
//

#ifndef PANDEMIC_HOYA_2002_SIR_H
#define PANDEMIC_HOYA_2002_SIR_H


#include <iostream>
#include <nlohmann/json.hpp>

struct SIR
{
    SIR(unsigned int pop, unsigned int phase, int num_inf, int num_rec, float initial_infected, float border_length, float land_area)
            :
                population(pop),
                phase(phase),
                num_inf(num_inf),
                num_rec(num_rec),
                deaths{0},
                border_length{border_length},
                land_area{land_area}
    {
        initialize();
    }

    SIR()
            :
            population{0},
            phase{0},
            susceptible{1}
    {

    }

    void initialize()
    {
        infected.push_back(initial_infected);

        infected.insert(infected.end(), num_inf, 0);

        recovered.insert(recovered.end(), num_rec, 0);

        susceptible = 1 - initial_infected;
    }

    bool operator != (const SIR &other)
    {
        bool neq = population != other.population       ||
                    phase != other.phase                ||
                    susceptible != other.susceptible    ||
                    deaths != other.deaths;

        int i = 0;

        while(!neq && i < num_inf)
        {
            neq = infected[i] != other.infected[i];

            i += 1;
        }

        i = 0;

        while(!neq && i < num_rec)
        {
            neq = recovered[i] != other.recovered[i];

            i += 1;
        }

        return neq;
    }


    unsigned int population;
    unsigned int phase;
    int num_inf;
    int num_rec;
    float susceptible;
    float initial_infected;
    std::vector<float> infected;
    std::vector<float> recovered;
    float deaths;

    float border_length;
    float land_area;
};

bool operator < (const SIR& lhs, const SIR& rhs){ return true; }

std::ostream &operator << (std::ostream &os, const SIR &sir)
{
    os << "<" << sir.population << "," << sir.phase << "," << sir.num_inf << "," << sir.num_rec << "," << sir.susceptible;

    for(auto infected : sir.infected)
    {
        os << "," << infected;
    }

    for(auto recovered : sir.recovered)
    {
        os << "," << recovered;
    }

    os << "," << sir.deaths;

    os << ">";

    return os;
}

void from_json(const nlohmann::json &json, SIR &sir)
{
    json.at("population").get_to(sir.population);
    json.at("susceptible").get_to(sir.susceptible);
    json.at("infected").get_to(sir.infected);
    json.at("recovered").get_to(sir.recovered);
    json.at("num_infected").get_to(sir.num_inf);
    json.at("num_recovered").get_to(sir.num_rec);
    json.at("initial_infected").get_to(sir.initial_infected);
    json.at("border_length").get_to(sir.border_length);
    json.at("land_area").get_to(sir.land_area);

    sir.initialize();
}

#endif //PANDEMIC_HOYA_2002_SIR_H
