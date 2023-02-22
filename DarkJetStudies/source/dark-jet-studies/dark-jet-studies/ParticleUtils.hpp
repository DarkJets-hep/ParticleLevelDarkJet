#pragma once

#include <xAODTruth/TruthParticle.h>
#include <fastjet/PseudoJet.hh>
#include <vector>

typedef std::vector<const xAOD::TruthParticle*> Particles;

inline Particles parents(const xAOD::TruthParticle *particle){
    const std::size_t n = particle->nParents();
    Particles result;
    for(std::size_t i = 0; i < n; i++){
        if(particle->parent(i) != nullptr) result.push_back(particle->parent(i));
    }
    return result;
}

inline Particles children(const xAOD::TruthParticle *particle){
    const std::size_t n = particle->nChildren();
    Particles result;
    for(std::size_t i = 0; i < n; i++){
        if(particle->child(i) != nullptr) result.push_back(particle->child(i));
    }
    return result;
}

inline bool isVisible(const xAOD::TruthParticle *particle){
    return particle->isCharged() || particle->isPhoton() || particle->isHadron();
}

inline double invariantMass(const TLorentzVector &momentum){
    return std::sqrt(std::abs(momentum * momentum));
}
