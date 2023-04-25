#pragma once
#include <string>
#include <nlohmann/json.hpp>

// using json = nlohmann::json;

namespace internship {

    nlohmann::json loadOSfromJSON(const std::string& jsonFileName); // loads json and removes any objects without os set to true

    int differenceInDays(std::string d1, std::string d2); // calculates a difference in days between 2 dates
    
    void addSupportPeriod(nlohmann::json& data); // adds a supportCycle parameter in days to every version 
    
    nlohmann::json getLongestVersion(nlohmann::json data); // returns a json file with longest support period for any version
    
    nlohmann::json getNewestVersion(nlohmann::json data); // returns a json file with longest support period for the latest verssion
    
    void sortJson(nlohmann::json& data); // sorts resulting json by days
    
    void task(const std::string& jsonFileName, int elementsCount); // completes task

    // do not remove this function
    void solution(const std::string& jsonFileName, int elementsCount);
}