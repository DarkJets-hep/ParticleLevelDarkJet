#include <Rivet/Analysis.hh>
#include <Rivet/Projections/FinalState.hh>
#include <Rivet/Projections/ChargedFinalState.hh>
#include <Rivet/Projections/FastJets.hh>
#include <Rivet/Math/Vector4.hh>
#include <TCanvas.h>
#include <TH2D.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "../Headers/Decay.hpp"
#include "../Headers/ParticleName.hpp"
#include "../Headers/PlotParticle.hpp"
#include "../Headers/ParticleSort.hpp"

namespace Rivet{
    class ExcitedQuarkDecay: public Analysis{
    public:
        ExcitedQuarkDecay():
            Analysis("ExcitedQuarkDecay"),
            _pdf("ExcitedQuarkDecay.pdf"),
            _pTFlow(
                "", ";Rapidity #it{y};Azimuth #it{#phi};Jet #it{p}_{T} [GeV]",
                200, -4, 4,    //y bins, min y, max y
                157, 0, 2*M_PI    //phi bins, min phi, max phi
            ){}

        virtual void init() override{
            const FinalState cnfs;
            const ChargedFinalState cfs(cnfs);
            this->declare(cnfs, "FS");
            this->declare(cfs, "CFS");
            this->declare(FastJets(cnfs, FastJets::ANTIKT, this->_jetRadius), "Jets");

            this->_canvas.Print(this->_pdf + "[");
        }

        virtual void analyze(const Event& event) override{
            //Find the excited quark by starting with the most energetic particle in the most energetic jet and following its most energetic parents. If it wasn't found as a parent of the highest energy particle, go to the next highest energy particle.
            const FinalState &cnfs = this->apply<FinalState>(event, "FS");
            const Jets &jets = this->apply<FastJets>(event, "Jets").jetsByPt();
            bool particleIsValid = false;
            Particle excitedParticle;
            for(const Jet &jet: jets){
                for(const Particle &initialParticle: particlesByEnergy(jet.particles())){
                    excitedParticle = initialParticle;
                    while(excitedParticle.parents().size() > 0){
                        excitedParticle = particlesByEnergy(excitedParticle.parents())[0];
                        if(std::abs(excitedParticle.pid()) % 1000000 < 100 && Decay::fromChild(excitedParticle).parents().size() == 2 && Decay::fromParent(excitedParticle).children().size() == 2){    //Use std::abs(excitedParticle.pid()) % 1000000 < 100 to check if the particle is elementary, the isComposite() method is not reliable as sometimes it returns false for hadrons
                            particleIsValid = true;
                            break;
                        }
                    }
                    if(particleIsValid){
                        break;
                    }
                }
                if(particleIsValid){
                    break;
                }
            }
            if(!particleIsValid){
                std::cout << "Excited quark not found" << std::endl << std::endl;
                return;
            }
            while(excitedParticle.children().size() == 1){
                excitedParticle = excitedParticle.children()[0];
            }

            //Count the decay modes of the excited quarks
            const Decay decay = Decay::fromParent(excitedParticle);
            const PdgId pdgid = excitedParticle.pid();

            //Print info about the excited quark
            std::cout << "Particle: " << particleName(pdgid) << std::endl;
            std::cout << "Energy: " << excitedParticle.energy() << std::endl;
            std::cout << "Mass: " << excitedParticle.mass() << std::endl;
            std::cout << "Parent particles: " << Decay::fromChild(excitedParticle) << std::endl;
            std::cout << "Decays into: " << decay << std::endl;
            std::cout << "Dijet energy: " << (jets[0].energy() + jets[1].energy()) << std::endl;
            std::cout << "Dijet invariant mass: " << (jets[0].momentum() + jets[1].momentum()).mass() << std::endl;

            //Draw the axes
            this->_pTFlow.Draw("colz");
            this->_pTFlow.Reset();
            this->_pTFlow.SetStats(0);

            //Plot the jets
            const auto jetPolygons = plotJets({jets[0], jets[1]}, this->_jetRadius, EColor::kYellow - 7);

            //Plot the stable particles
            for(const Particle &finalParticle: cnfs.particles()){
                int color;
                if(finalParticle.charge3() == 0){    //Neutral
                    if(finalParticle.pid() == PID::PHOTON) color = EColor::kSpring + 4;
                    else if(finalParticle.isHadron()) color = EColor::kGreen + 3;
                    else color = EColor::kGray;
                }
                else if(finalParticle.charge3() < 0){    //Negative
                    if(finalParticle.isHadron()) color = EColor::kBlue + 2;
                    else color = EColor::kBlue;
                }
                else if(finalParticle.charge3() > 0){    //Positive
                    if(finalParticle.isHadron()) color = EColor::kRed + 2;
                    else color = EColor::kRed;
                }
                plotParticle(finalParticle, 6, color, false);
            }

            //Plot the excited quark and its decay products
            plotParticle(excitedParticle, 29);
            for(const Particle &child: excitedParticle.children()){
                plotParticle(child, 20);
            }

            //Print the page
            this->_pTFlow.Draw("axis same");
            this->_canvas.Print(this->_pdf);
            std::cout << std::endl;
        }

        virtual void finalize() override{
            this->_canvas.Print(this->_pdf + "]");
        }

    private:
        static constexpr double _jetRadius = 1.0;
        const TString _pdf;
        TCanvas _canvas;
        TH2D _pTFlow;
    };

    DECLARE_RIVET_PLUGIN(ExcitedQuarkDecay);
}
