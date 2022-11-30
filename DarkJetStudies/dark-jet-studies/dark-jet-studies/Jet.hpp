#pragma once

#include <xAODTruth/TruthParticleContainer.h>
#include <fastjet/PseudoJet.hh>
#include <algorithm>
#include "dark-jet-studies/ParticleUtils.hpp"

class Jet: public fastjet::PseudoJet{
public:
    Jet(const fastjet::PseudoJet &jet, const xAOD::TruthParticleContainer *particles): fastjet::PseudoJet(jet), _particles(particles){}

    Particles particles() const{
        Particles result;
        for(const fastjet::PseudoJet &particle: this->constituents()){
            result.push_back((*this->_particles)[particle.user_index()]);
        }
        return result;
    }

    bool containsParticle(const xAOD::TruthParticle *particle) const{
        const Particles particles = this->particles();
        return std::find(particles.begin(), particles.end(), particle) != particles.end();
    }

    double pT() const{
        return this->perp();
    }
    double pt() const{
        return this->perp();
    }

    TLorentzVector p4() const{
        const auto m = this->four_mom();
        return TLorentzVector(m[0], m[1], m[2], m[3]);
    }
    TLorentzVector momentum() const{
        return this->p4();
    }

private:
    const xAOD::TruthParticleContainer *_particles;
};
