#pragma once

#include <Rivet/Particle.hh>
#include <Rivet/Jet.hh>
#include <string>
#include <regex>
#include "../Headers/ParticleSort.hpp"
#include "../Headers/GetEnvVars.hpp"

static bool isDark(const Rivet::Particle &particle){
    static const std::regex darkParticleRegex(getStringFromEnvVar("DARK_REGEX", std::string("^490[0-9][1-9][0-9]{2}$")));
    return std::regex_search(std::to_string(particle.abspid()), darkParticleRegex);
}

inline bool hasDarkAncestor(Rivet::Particle particle){
    if(isDark(particle)){
        return true;
    }
    while(particle.parents().size() > 0){
        particle = particlesByEnergy(particle.parents())[0];
        if(isDark(particle)){
            return true;
        }
    }
    return false;
}

inline double multiplicityDarkness(const Rivet::Jet &jet){
    int result = 0;
    for(const Rivet::Particle &particle: jet.particles()){
        if(hasDarkAncestor(particle)){
            result++;
        }
    }
    return 1.0 * result / jet.particles().size();
}

inline double pTDarkness(const Rivet::Jet &jet){
    double result = 0.0;
    for(const Rivet::Particle &particle: jet.particles()){
        if(hasDarkAncestor(particle)){
            result += particle.pT();
        }
    }
    return std::min(result / jet.pT(), 1.0);
}

inline double multiplicityInvisibility(const Rivet::Jet &jet){
    int result = 0;
    for(const Rivet::Particle &particle: jet.particles()){
        if(!particle.isVisible()){
            result++;
        }
    }
    return 1.0 * result / jet.particles().size();
}

inline double pTInvisibility(const Rivet::Jet &jet){
    double result = 0.0;
    for(const Rivet::Particle &particle: jet.particles()){
        if(!particle.isStable()){
            std::cout << "Unstable particle in jet" << std::endl;
        }
        if(!particle.isVisible()){
            result += particle.pT();
        }
    }
    return std::min(result / jet.pT(), 1.0);
}