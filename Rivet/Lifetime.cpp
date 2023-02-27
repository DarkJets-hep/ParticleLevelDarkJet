#include <Rivet/Analysis.hh>
#include <Rivet/Projections/FinalState.hh>
#include <Rivet/Projections/ChargedFinalState.hh>
#include <Rivet/Projections/FastJets.hh>
#include <Rivet/Math/Vector4.hh>
#include <TCanvas.h>
#include <TH1D.h>
#include <vector>
#include <map>
#include <algorithm>
#include "../Headers/ParticleName.hpp"

namespace Rivet{
    class Lifetime: public Analysis{
    public:
        Lifetime(): Analysis("Lifetime"){}

        virtual void init() override{
            const FinalState cnfs;
            const ChargedFinalState cfs(cnfs);
            this->declare(cnfs, "FS");
            this->declare(cfs, "CFS");
            this->declare(FastJets(cnfs, FastJets::ANTIKT, 1.0, JetAlg::Muons::ALL, JetAlg::Invisibles::ALL), "Jets");
        }

        virtual void analyze(const Event& event) override{
            for(Particle particle: event.allParticles()){
                //If the particle is stable on detector scales, we can't find its lifetime
                if(particle.children().size() == 0){
                    continue;
                }

                //If the particle just decays into itself, ignore it, we will use the last copy
                if(particle.children().size() == 1 && particle.children()[0].pid() == particle.pid()){
                    continue;
                }

                const double endTime = particle.children()[0].origin().t();
                while(particle.parents().size() == 1 && particle.parents()[0].pid() == particle.pid()){
                    particle = particle.parents()[0];
                }
                const double startTime = particle.origin().t();
                const double lifetime = endTime - startTime;
                if(lifetime > 0){
                    this->_lifetimes[particle.abspid()].push_back(lifetime);
                }
            }
        }

        virtual void finalize() override{
            const TString pdf = "../Outputs/Lifetime.pdf";
            TCanvas canvas;
            canvas.Print(pdf + "[");
            for(const auto &particleLifetimePair: this->_lifetimes){
                const PdgId pdgid = particleLifetimePair.first;
                const std::vector<double> &lifetimes = particleLifetimePair.second;
                const double maxLifetime = *std::max_element(lifetimes.begin(), lifetimes.end());
                TH1D plot(
                    "", (";Lifetime of #it{" + absParticleNameAsTLatex(pdgid) + "} (10^{-1} mm/c);Number of particles").c_str(),
                    this->_bins, 0.0, maxLifetime    //x bins, min x, max x
                );
                for(double lifetime: lifetimes){
                    plot.AddBinContent(lifetime * this->_bins / maxLifetime);
                }
                plot.SetStats(0);
                plot.Draw("colz");
                canvas.Print(pdf);
            }
            canvas.Print(pdf + "]");
        }

    private:
        std::map<PdgId, std::vector<double>> _lifetimes;
        static constexpr int _bins = 50;
    };

    DECLARE_RIVET_PLUGIN(Lifetime);
}