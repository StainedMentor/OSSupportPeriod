#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

#include <date/date.h>
#include <nlohmann/json.hpp>
#include <boost/algorithm/string.hpp>


#include "internship.h"

using json = nlohmann::json;
using namespace date;

namespace internship {

    json loadOSfromJSON(const std::string& jsonFileName){ // loads json and removes any objects without os set to true
        std::ifstream f(jsonFileName);
        json input = json::parse(f);

        for(auto it = input.begin(); it != input.end(); ) {

            if (it->at("os") == false) {
                it = input.erase(it);
            }
            else it++;
        }
        return input;
    }

    int differenceInDays(std::string d1, std::string d2){ // calculates a difference in days between 2 dates
        std::istringstream s1, s2;
        sys_days ymd1,ymd2;

        s1.str(d1);
        s2.str(d2);

        s1 >> date::parse("%F", ymd1);
        s2 >> date::parse("%F", ymd2);

        return (ymd1-ymd2+days{1}).count();
    }
 
    
    void addSupportPeriod(json& data){ // adds a supportCycle parameter in days to every version 
        for(const auto& [id, product] : data.items()) {
            for(const auto& [versionId, version] : product["versions"].items()) {
                try {
                    auto d1 = version["eol"].get<std::string>();
                    auto d2 = version["releaseDate"].get<std::string>();
                    version["supportCycle"] = differenceInDays(d1,d2);
                }
                catch (const std::exception& e) {
                    version["supportCycle"] = 0;
                }
            }
        }
    }
    
    json getLongestVersion(json data){ // returns a json file with longest support period for any version
        json output = json::array();

        for(const auto& [id, product] : data.items()) {
            output += {{"name", product["name"]}};
                int longest = 0;
                for(const auto& [versionId, version] : product["versions"].items()) {
                    if (version["supportCycle"] > product["vesions"][longest]["supportCycle"]){
                        longest = std::stoi(versionId);
                    }
                }
            int pos = std::stoi(id);
            output[pos]["cycle"] = product["versions"][longest]["cycle"];
            output[pos]["days"] = product["versions"][longest]["supportCycle"];
        }

        return output;
    }

    json getNewestVersion(json data){ // returns a json file with longest support period for the latest verssion
        json output = json::array();

       for(const auto& [id, product] : data.items()) {
            output += {{"name", product["name"]}};
            int pos = std::stoi(id);

            output[pos]["cycle"] = product["versions"][0]["cycle"];
            output[pos]["days"] = product["versions"][0]["supportCycle"];
        }

        return output;
    }

    void sortJson(json& data){ // sorts resulting json by days
        for(auto i = data.begin(); i != data.end()-1; ++i) {
            auto end = data.end() - std::distance(data.begin(),i)-1;
            for(auto j = data.begin(); j != end; ++j) {
                if (j->at("days")  > (j+1)->at("days")){
                            j->swap(*(j+1));
                }

            }
        }
    }

    void task(const std::string& jsonFileName, int elementsCount) { // completes task

        json data = loadOSfromJSON(jsonFileName);
        addSupportPeriod(data);
        json outputData = getNewestVersion(data);
        sortJson(outputData);

            for(auto i = outputData.end()-1;  std::distance(i, outputData.end()) <= elementsCount; --i) {
                std::string os = i->at("name").get<std::string>() + " " + i->at("cycle").get<std::string>() + " ";
                boost::erase_all(os, "\"");
                std::cout<< os << i->at("days") << "\n";
            }

    }

    void solution(const std::string& jsonFileName, int elementsCount) {
        task(jsonFileName, elementsCount);
    }
}