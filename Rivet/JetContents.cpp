#include <Rivet/Analysis.hh>
#include <Rivet/Projections/FinalState.hh>
#include <Rivet/Projections/ChargedFinalState.hh>
#include <Rivet/Projections/FastJets.hh>
#include <Rivet/Math/Vector4.hh>
#include <iostream>
#include <vector>
#include <map>
#include "../Headers/Darkness.hpp"
#include "../Headers/Decay.hpp"
#include "../Headers/ParticleName.hpp"
#include "../Headers/ParticleSort.hpp"

namespace Rivet{
    class JetContents: public Analysis{
    public:
        JetContents(): Analysis("JetContents"), _darkParticles(0), _darkPT(0.0), _totalNumberOfParticles(0), _totalPT(0.0), _firstEvent(true){}

        virtual void init() override{
            const FinalState cnfs;
            const ChargedFinalState cfs(cnfs);
            this->declare(cnfs, "FS");
            this->declare(cfs, "CFS");
            this->declare(FastJets(cnfs, FastJets::ANTIKT, 1.0, JetAlg::Muons::ALL, JetAlg::Invisibles::ALL), "Jets");
        }

        virtual void analyze(const Event& event) override{
            const FinalState &cnfs = apply<FinalState>(event, "FS");
            const Particles &cparticles = apply<FinalState>(event, "CFS").particles();
            const Jets &jets = apply<FastJets>(event, "Jets").jetsByPt();

            //Calculate the particle contents of the jet
            for(const Jet &jet: jets){
                for(const Particle &particle: jet.particles()){
                    const PdgId pdgid = particle.pid();
                    this->_jetContents[pdgid]++;
                    this->_totalNumberOfParticles++;
                    this->_jetContentsByPT[pdgid] += particle.pT();
                    this->_totalPT += particle.pT();

                    if(hasDarkAncestor(particle)){
                        this->_darkParticles++;
                        this->_darkPT += particle.pT();
                    }

                    this->_decays[pdgid][Decay::fromChild(particle)]++;
                }
            }

            //Calculate the total energy and 3-momentum (only do this for the first event, otherwise there will be so much output that it will be unreadable)
            if(this->_firstEvent){
                double totalEnergy = 0, totalChargedEnergy = 0;
                Vector3 totalMomentum(0, 0, 0), totalChargedMomentum(0, 0, 0);

                for(const Particle &particle: cnfs.particles()){
                    totalEnergy += particle.momentum().t();
                    totalMomentum += particle.momentum().vector3();
                }

                for(const Particle &chargedParticle: cparticles){
                    totalChargedEnergy += chargedParticle.momentum().t();
                    totalChargedMomentum += chargedParticle.momentum().vector3();
                }

                std::cout << "Total energy: " << totalEnergy << " GeV" << std::endl;
                std::cout << "Total 3-momentum: " << totalMomentum.mod() << " GeV" << std::endl;
                std::cout << "Total energy of charged particles: " << totalChargedEnergy << " GeV" << std::endl;
                std::cout << "Total 3-momentum of charged particles: " << totalChargedMomentum.mod() << " GeV" << std::endl;
                this->_firstEvent = false;
            }
        }

        virtual void finalize() override{
            //Sort the particles by frequency
            const auto sortedJetContents = sortMap(this->_jetContents);
            const auto sortedJetContentsByPT = sortMap(this->_jetContentsByPT);

            //Print and plot the jet contents
            std::cout << std::endl << "Average multiplicity fraction of all jets in all events:" << std::endl << "----" << std::endl;
            for(const std::pair<int, int> &particleCount: sortedJetContents){
                const double fraction = 100.0 * particleCount.second / this->_totalNumberOfParticles;
                std::cout << particleName(particleCount.first) << ": " << fraction << "%" << std::endl;
            }

            //Print the jet contents by pT
            std::cout << std::endl << "Average pT-fraction of all jets in all events:" << std::endl << "----" << std::endl;
            for(const std::pair<int, double> &particlePt: sortedJetContentsByPT){
                std::cout << particleName(particlePt.first) << ": " << (100.0 * particlePt.second / this->_totalPT) << "%" << std::endl;
            }

            //Print the parents of photons and leptons
            for(PdgId child: {PID::PHOTON, PID::ELECTRON, PID::MUON}){
                const auto sortedParents = sortMap(this->_decays[child]);
                std::cout << std::endl << "Parent particles of " << particleName(child) << ":" << std::endl << "----" << std::endl;
                for(const std::pair<Decay, int> &parentCount: sortedParents){
                    const double fraction = 100.0 * parentCount.second / this->_jetContents[child];
                    if(fraction < (child == PID::PHOTON ? 0.1 : 1)){
                        break;
                    }
                    std::cout << parentCount.first << ": " << fraction << "%" << std::endl;
                }
            }

            //Print the darkness
            std::cout << std::endl;
            std::cout << "Multiplicity fraction of particles with dark ancestors: " << (100.0 * this->_darkParticles / this->_totalNumberOfParticles) << "%" << std::endl;
            std::cout << "pT-fraction of particles with dark ancestors: " << (100.0 * this->_darkPT / this->_totalPT) << "%" << std::endl;
        }

    private:
        std::map<PdgId, int> _jetContents;
        std::map<PdgId, double> _jetContentsByPT;
        int _darkParticles;
        double _darkPT;
        std::map<PdgId, std::map<Decay, int>> _decays;
        int _totalNumberOfParticles;
        double _totalPT;
        bool _firstEvent;
    };

    DECLARE_RIVET_PLUGIN(JetContents);
}