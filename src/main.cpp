/**
 * Copyright (c) 2020, Román Cárdenas Rodríguez
 * ARSLab - Carleton University
 * GreenLSI - Polytechnic University of Madrid
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <fstream>
#include <cadmium/modeling/dynamic_coupled.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/common_loggers.hpp>
#include "../model/zhong_coupled.hpp"
#include <experimental/filesystem>

using namespace std;
using namespace cadmium;
using namespace cadmium::celldevs;

using TIME = float;

/*************** Loggers *******************/
static ofstream out_messages("../logs/pandemic_outputs.txt");
struct oss_sink_messages{
    static ostream& sink(){
        return out_messages;
    }
};
static ofstream out_state("../logs/pandemic_state.txt");
struct oss_sink_state{
    static ostream& sink(){
        return out_state;
    }
};

using state=logger::logger<logger::logger_state, dynamic::logger::formatter<TIME>, oss_sink_state>;
using log_messages=logger::logger<logger::logger_messages, dynamic::logger::formatter<TIME>, oss_sink_messages>;
using global_time_mes=logger::logger<logger::logger_global_time, dynamic::logger::formatter<TIME>, oss_sink_messages>;
using global_time_sta=logger::logger<logger::logger_global_time, dynamic::logger::formatter<TIME>, oss_sink_state>;

using logger_top=logger::multilogger<state, log_messages, global_time_mes, global_time_sta>;


int main(int argc, char ** argv) {
    if (argc < 2) {
        cout << "Program used with wrong parameters. The program must be invoked as follows:";
        cout << argv[0] << " SCENARIO_CONFIG.json [MAX_SIMULATION_TIME (default: 500)]" << endl;
        return -1;
    }

    try {

        // Check to see if file exists along with an error message because otherwise, the JSON library fails and gives
        // a useless error message.
        if(!std::experimental::filesystem::exists(argv[1])) {
            throw std::runtime_error{"The passed in file does not exist: " + std::string{argv[1]}};
        }

        zhong_coupled<TIME> test = zhong_coupled<TIME>("pandemic_hoya");
        std::string scenario_config_file_path = argv[1];
        test.add_cells_json(scenario_config_file_path);
        test.couple_cells();

        std::shared_ptr<cadmium::dynamic::modeling::coupled<TIME>> t = std::make_shared<zhong_coupled<TIME>>(test);

        cadmium::dynamic::engine::runner<TIME, logger_top> r(t, {0});
        float sim_time = (argc > 2)? atof(argv[2]) : 500;
        r.run_until(sim_time);
    }
    catch(const std::exception &e) {

        std::ofstream print_error{"execution_errors.txt", std::ios::trunc};

        // Hopefully user sees this- not much can be done if a file to write an exception could not be created and the user
        // can't see what the exception is in the terminal if the std::cerr command fails.
        if(!print_error.is_open()) {
            std::cerr << "Unable to create a file to write exceptions to!" << std::endl;

            return -1;
        }

        std::cerr << e.what() << std::endl;

        print_error << e.what();

        // Just to be explicit about this- could omit due to RAII.
        print_error.flush();

        print_error.close();

        return -2;
    }

    return 0;
}
