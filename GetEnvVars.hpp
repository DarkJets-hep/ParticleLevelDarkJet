#pragma once

#include <TString.h>
#include <TObjString.h>
#include <TObjArray.h>
#include <iostream>
#include <cstdlib>

inline int getIntFromEnvVar(const char *name, int def){
    const char *envVar = std::getenv(name);
    if(envVar != nullptr && TString(envVar).IsDigit()){
        return TString(envVar).Atoi();
    }
    std::cout << "Environment variable " << name << " not set, defaulting to " << def << ". Did you forget to use export?" << std::endl;
    return def;
}

template<typename String> String getStringFromEnvVar(const char *name, const String &def){
    const char *envVar = std::getenv(name);
    if(envVar != nullptr){
        return envVar;
    }
    std::cout << "Environment variable " << name << " not set, defaulting to " << def << ". Did you forget to use export?" << std::endl;
    return def;
}

inline std::vector<int> getIntVectorFromEnvVar(const char *name, const std::vector<int> &def){
    const char *envVar = std::getenv(name);
    std::vector<int> result;
    if(envVar != nullptr){
        TObjArray *splitString = TString(envVar).Tokenize(",");
        for(const TObject *obj: *splitString){
            const TString str = static_cast<const TObjString*>(obj)->GetString();
            if(str.IsDigit()){
                result.push_back(str.Atoi());
            }
        }
        delete[] splitString;
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
        std::cout << " Did you forget to use export?" << std::endl;
    }
    return def;
}