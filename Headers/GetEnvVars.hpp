#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <sstream>

inline int getIntFromEnvVar(const char *name, int def){
    const char *envVar = std::getenv(name);
    if(envVar != nullptr){
        try{
            return std::stoi(envVar);
        }
        catch(const std::invalid_argument &e){
            std::cout << "Ignoring non-integer value " << envVar << " in " << name << "." << std::endl;
        }
        catch(const std::out_of_range &e){
            std::cout << "Ignoring out of range value " << envVar << " in " << name << "." << std::endl;
        }
    }
    std::cout << "Environment variable " << name << " not set, defaulting to " << def << ". Did you accidentally define it with a semicolon?" << std::endl;
    return def;
}

inline double getDoubleFromEnvVar(const char *name, double def){
    const char *envVar = std::getenv(name);
    if(envVar != nullptr){
        return std::stod(envVar);
    }
    std::cout << "Environment variable " << name << " not set, defaulting to " << def << ". Did you accidentally define it with a semicolon?" << std::endl;
    return def;
}

template<typename String> String getStringFromEnvVar(const char *name, const String &def){
    const char *envVar = std::getenv(name);
    if(envVar != nullptr){
        return envVar;
    }
    std::cout << "Environment variable " << name << " not set, defaulting to " << def << ". Did you accidentally define it with a semicolon?" << std::endl;
    return def;
}

inline std::vector<int> getIntVectorFromEnvVar(const char *name, const std::vector<int> &def){
    const char *envVar = std::getenv(name);
    std::vector<int> result;
    if(envVar != nullptr){
        std::istringstream stream(envVar);
        std::string str;
        while(std::getline(stream, str, ',')){
            try{
                result.push_back(std::stoi(str));
            }
            catch(const std::invalid_argument &e){
                std::cout << "Ignoring non-integer value " << str << " in " << name << "." << std::endl;
            }
            catch(const std::out_of_range &e){
                std::cout << "Ignoring out of range value " << str << " in " << name << "." << std::endl;
            }
        }
    }
    if(result.size() > 0){
        return result;
    }
    if(envVar == nullptr){
        std::cout << "Environment variable " << name << " not set, ";
    }
    else{
        std::cout << "Environment variable " << name << " does not contain any valid integers, ";
    }
    std::cout << "defaulting to (";
    for(unsigned int i = 0; i < def.size(); i++){
        if(i != 0){
            std::cout << ",";
        }
        std::cout << def[i];
    }
    std::cout << ").";
    if(envVar == nullptr){
        std::cout << " Did you accidentally define it with a semicolon?" << std::endl;
    }
    return def;
}