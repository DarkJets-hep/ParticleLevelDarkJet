#pragma once

#include <Rivet/Particle.hh>
#include <Rivet/Jet.hh>
#include <string>
#include <regex>
#include "../Headers/ParticleSort.hpp"
#include "../Headers/GetEnvVars.hpp"

static bool particleIsDark(const Rivet::Particle &particle){
    static const std::regex darkParticleRegex(
        getStringFromEnvVar(
            //Allow to override dark regex by setting an environment variable
            "DARK_REGEX",
            //Default regex that matches PDGIDs of dark particles
            std::string("^490[0-9][1-9][0-9]{2}$")
        )
    );
    return std::regex_search(
        std::to_string(particle.abspid()),
        darkParticleRegex
    );
}

static bool hasDarkAncestor(Rivet::Particle particle){
    if(particleIsDark(particle)){
        return true;
    }
    while(particle.parents().size() > 0){
        particle = particlesByEnergy(particle.parents())[0];
        if(particleIsDark(particle)){
            return true;
        }
    }
    return false;
}

static double pTDarkness(const Rivet::Jet &jet){
    Rivet::FourMomentum darkMomentum;
    for(const Rivet::Particle &particle: jet.particles()){
        if(hasDarkAncestor(particle)){
            darkMomentum += particle.momentum();
        }
    }
    return std::min(darkMomentum.pT() / jet.pT(), 1.0);
}

//Method to judge if a jet is dark. The default, recommended cut is 80%. 
//Note that jet should be built including invisible particles.
static bool jetIsDark(const Rivet::Jet &jet, double darknessCut = 0.8){
    return pTDarkness(jet) >= darknessCut;
}

static double multiplicityDarkness(const Rivet::Jet &jet){
    int darkMultiplicity = 0;
    for(const Rivet::Particle &particle: jet.particles()){
        if(hasDarkAncestor(particle)){
            darkMultiplicity++;
        }
    }
    return 1.0 * darkMultiplicity / jet.particles().size();
}

static double multiplicityInvisibility(const Rivet::Jet &jet){
    int darkMultiplicity = 0;
    for(const Rivet::Particle &particle: jet.particles()){
        if(!particle.isVisible()){
            darkMultiplicity++;
        }
    }
    return 1.0 * darkMultiplicity / jet.particles().size();
}

static double pTInvisibility(const Rivet::Jet &jet){
    Rivet::FourMomentum darkMomentum;
    for(const Rivet::Particle &particle: jet.particles()){
        if(!particle.isStable()){
            std::cout << "Unstable particle in jet" << std::endl;
        }
        if(!particle.isVisible()){
            darkMomentum += particle.momentum();
        }
    }
    return std::min(darkMomentum.pT() / jet.pT(), 1.0);
}