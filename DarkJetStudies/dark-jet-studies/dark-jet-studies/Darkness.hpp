#pragma once

#include <xAODTruth/TruthParticleContainer.h>
#include <string>
#include <regex>
#include "dark-jet-studies/ParticleSort.hpp"
#include "dark-jet-studies/DarkJetStudy.h"
#include "dark-jet-studies/ParticleUtils.hpp"
#include "dark-jet-studies/Jet.hpp"

static bool isDark(const xAOD::TruthParticle *particle){
    return std::regex_search(std::to_string(particle->absPdgId()), DarkJetStudy::darkRegex);
}

inline bool hasDarkAncestor(const xAOD::TruthParticle *particle){
    if(isDark(particle)){
        return true;
    }
    while(parents(particle).size() > 0){
        particle = particlesByEnergy(parents(particle))[0];
        if(isDark(particle)){
            return true;
        }
    }
    return false;
}

inline double multiplicityDarkness(const Jet &jet){
    int result = 0;
    for(const xAOD::TruthParticle *particle: jet.particles()){
        if(hasDarkAncestor(particle)){
            result++;
        }
    }
    return 1.0 * result / jet.particles().size();
}

inline double pTDarkness(const Jet &jet){
    double result = 0.0;
    for(const xAOD::TruthParticle *particle: jet.particles()){
        if(hasDarkAncestor(particle)){
            result += particle->pt();
        }
    }
    return std::min(result / jet.pT(), 1.0);
}

inline double multiplicityInvisibility(const Jet &jet){
    int result = 0;
    for(const xAOD::TruthParticle *particle: jet.particles()){
        if(!isVisible(particle)){
            result++;
        }
    }
    return 1.0 * result / jet.particles().size();
}

inline double pTInvisibility(const Jet &jet){
    double result = 0.0;
    for(const xAOD::TruthParticle *particle: jet.particles()){
        if(!isVisible(particle)){
            result += particle->pt();
        }
    }
    return std::min(result / jet.pT(), 1.0);
}
