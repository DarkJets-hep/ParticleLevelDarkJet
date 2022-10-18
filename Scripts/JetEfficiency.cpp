#include <Rivet/Analysis.hh>
#include <Rivet/Projections/FinalState.hh>
#include <Rivet/Projections/ChargedFinalState.hh>
#include <Rivet/Projections/FastJets.hh>
#include <Rivet/Math/Vector4.hh>
#include <TCanvas.h>
#include <TH1D.h>
#include <TH2D.h>
#include <iostream>
#include <algorithm>
#include <memory>
#include "../Headers/Decay.hpp"
#include "../Headers/ParticleName.hpp"
#include "../Headers/PlotParticle.hpp"
#include "../Headers/ParticleSort.hpp"
#include "../Headers/GetEnvVars.hpp"

enum PlotColor{
    NONE = 0,
    PARTON = 1,
    JET = 2,
    CHARGE = 3,
    TYPE = 4
};

namespace Rivet{
    class JetEfficiency: public Analysis{
    public:
        JetEfficiency():
            Analysis("JetEfficiency"),
            _numberOfPlots(0),
            _jetRadius(getDoubleFromEnvVar("JET_RADIUS", 1.0)),
            _pdf(getStringFromEnvVar("PDF_FILENAME", TString("../Outputs/JetEfficiency")) + ".pdf"),
            _plotSecondChildren(getIntFromEnvVar("PLOT_SECOND_CHILDREN", 0)),
            _pTFlow(
                "", ";Rapidity #it{y};Azimuth #it{#phi};Jet #it{p}_{T} [GeV]",
                200, -4, 4,    //y bins, min y, max y
                157, 0, 2*M_PI    //phi bins, min phi, max phi
            ),
            _efficiencyData(this->_deltaRBins),
            _totalPT(0),
            _purePT(0),
            _partonPTPlot(
                "", ";Parton #it{p_{T}} (GeV);Number of events",
                this->_pTBins, 0.0, this->_maxPT    //x bins, min x, max x
            ),
            _jetResponsePlot(
                "", ";Jet #it{p_{T}} / Parton #it{p_{T}};Number of events",
                this->_pTBins, 0.0, this->_maxResponse    //x bins, min x, max x
            ),
            _fsResponsePlot(
                "", ";#it{p_{T}} of final state children of parton / Parton #it{p_{T}};Number of events",
                this->_pTBins, 0.0, this->_maxResponse    //x bins, min x, max x
            ),
            _fsInJetResponsePlot(
                "", ";#it{p_{T}} of final state children of parton in jet / Parton #it{p_{T}};Number of events",
                this->_pTBins, 0.0, this->_maxResponse    //x bins, min x, max x
            ),
            _resonancePdgId(getIntVectorFromEnvVar("RES_PDGID", std::vector<int>{4900023}))
        {
            this->_plotColor = static_cast<PlotColor>(getIntFromEnvVar("PLOT_COLOR", 1));
            if(this->_plotColor < 0 || this->_plotColor > 4){
                std::cout << "Invalid plot color " << this->_plotColor << ". Valid options are: (0) no color, (1) by parton, (2) by jet, (3) by charge, (4) by particle type." << std::endl;
                this->_plotColor = PlotColor::NONE;
            }
        }

        virtual void init() override{
            const FinalState cnfs;
            const ChargedFinalState cfs(cnfs);
            this->declare(cnfs, "FS");
            this->declare(cfs, "CFS");
            this->declare(FastJets(cnfs, FastJets::ANTIKT, this->_jetRadius), "Jets");

            this->_canvas.Print(this->_pdf + "[");
        }

        virtual void analyze(const Event& event) override{
            //Find the excited quark
            const FinalState &cnfs = this->apply<FinalState>(event, "FS");
            const Jets &jets = this->apply<FastJets>(event, "Jets").jetsByPt();
            Particle excitedQuark;
            for(const Particle &particle: event.allParticles()){
                if(std::find(this->_resonancePdgId.begin(), this->_resonancePdgId.end(), particle.pid()) != this->_resonancePdgId.end()){
                    excitedQuark = particle;
                    break;
                }
            }
            if(excitedQuark.pid() == PID::ANY){
                std::cout << "Resonance particle not found." << std::endl;
                return;
            }
            while(excitedQuark.children().size() == 1){
                excitedQuark = excitedQuark.children()[0];
            }

            //Count the decay mode of the particle
            this->_numberOfParticles[excitedQuark.pid()]++;
            this->_decays[excitedQuark.pid()][Decay::fromParent(excitedQuark)]++;

            //Count the efficiency and purity of the jets
            for(const Particle &parton: excitedQuark.children()){
                double deltaR = 1e6;    //Start with something that's guaranteed to be much larger than the actual deltaR
                Jet jet;
                for(const Jet &newJet: {jets[0], jets[1]}){
                    const double deltaY = parton.rapidity() - newJet.rapidity();
                    const double deltaPhi = parton.phi() - newJet.phi();
                    const double newDeltaR = std::sqrt(deltaY * deltaY + deltaPhi * deltaPhi);
                    if(newDeltaR < deltaR){
                        deltaR = newDeltaR;
                        jet = newJet;
                    }
                }

                //Efficiency
                for(int i = deltaR * this->_deltaRBins / this->_deltaRMax; i < this->_deltaRBins; i++){
                    this->_efficiencyData[i]++;
                }

                //Purity
                for(const Particle &particle: jet.particles()){
                    if(particleIsFromParton(particle, parton)){
                        this->_purePT += particle.pT();
                    }
                    this->_totalPT += particle.pT();
                }

                //Plot the pT of the partons
                if(parton.pT() < this->_maxPT){
                    this->_partonPTPlot.AddBinContent(parton.pT() * this->_pTBins / this->_maxPT);
                    if(!this->_partonPTPlotByType.count(parton.abspid())){
                        this->_partonPTPlotByType.insert({parton.abspid(), std::unique_ptr<TH1D>(new TH1D(
                            "", (";#it{p_{T}}(#it{" + absParticleNameAsTLatex(parton.abspid()) + "}) (GeV);Number of events").c_str(),
                            this->_pTBins, 0.0, this->_maxPT    //x bins, min x, max x
                        ))});
                    }
                    this->_partonPTPlotByType[parton.abspid()]->AddBinContent(parton.pT() * this->_pTBins / this->_maxPT);
                }
                double fsPT = 0.0, fsInJetPT = 0.0;
                for(const Particle &particle: cnfs.particles()){
                    if(particleIsFromParton(particle, parton)){
                        fsPT += particle.pT();
                        if(jet.containsParticle(particle)){
                            fsInJetPT += particle.pT();
                        }
                    }
                }
                if(deltaR <= this->_jetRadius){
                    if(jet.pT() < parton.pT() * this->_maxResponse){
                        this->_jetResponsePlot.AddBinContent(jet.pT() / parton.pT() * this->_pTBins / this->_maxResponse);
                    }
                    if(fsInJetPT < parton.pT() * this->_maxResponse){
                        this->_fsInJetResponsePlot.AddBinContent(fsInJetPT / parton.pT() * this->_pTBins / this->_maxResponse);
                    }
                }
                if(fsPT < parton.pT() * this->_maxResponse){
                    this->_fsResponsePlot.AddBinContent(fsPT / parton.pT() * this->_pTBins / this->_maxResponse);
                }
            }

            //Find the children of the particle
            Particles children, partons;
            for(Particle child: excitedQuark.children()){
                children.push_back(child);
                if(this->_plotSecondChildren && child.mass() > 10){
                    while(child.children().size() == 1){
                        child = child.children()[0];
                    }
                    const Particles secondChildren = child.children();
                    partons.insert(partons.end(), secondChildren.begin(), secondChildren.end());
                    children.insert(children.end(), secondChildren.begin(), secondChildren.end());
                }
                else{
                    partons.push_back(child);
                }
            }

            //Only plot the 10 events of each kind, but allow 20 events for decay modes that can be more interesting (W- or Z-bosons since they can decay further)
            if(this->_decays[excitedQuark.pid()][Decay::fromParent(excitedQuark)] > (partons.size() == 2 ? 10 : 20)){
                return;
            }

            //Draw the axes
            this->_pTFlow.Draw("colz");
            this->_pTFlow.Reset();
            this->_pTFlow.SetStats(0);

            //Plot the jets
            const auto jetPolygons = plotJets({jets[0], jets[1]}, this->_jetRadius, {particleColor(jets[0], jets, partons), particleColor(jets[1], jets, partons)});

            //Plot the stable particles
            for(const Particle &finalParticle: cnfs.particles()){
                plotParticle(finalParticle, 6, particleColor(finalParticle, jets, partons), false, 1.5);
            }

            //Plot the excited quark and its decay products
            plotParticle(excitedQuark, 29);
            for(const Particle &child: children){
                plotParticle(child, 20, this->_plotColor == PlotColor::PARTON ? particleColor(child, jets, partons) : EColor::kBlack);
            }

            //Print the page
            this->_pTFlow.Draw("axis same");
            this->_canvas.Print(this->_pdf);
        }

        virtual void finalize() override{
            //Plot the efficiency
            TH1D efficiencyPlot(
                "", ";#it{#Delta R};Efficiency (%)",
                this->_deltaRBins, 0.0, this->_deltaRMax    //x bins, min x, max x
            );
            for(int i = 0; i < this->_deltaRBins; i++){
                efficiencyPlot.AddBinContent(i, 50.0 * this->_efficiencyData[i] / this->numEvents());
            }
            this->plotHistogram(efficiencyPlot);

            //Plot the pT and response
            this->plotHistogram(this->_partonPTPlot);
            for(const auto &pdgidPlotPair: this->_partonPTPlotByType){
                this->plotHistogram(*pdgidPlotPair.second);
            }
            this->plotHistogram(this->_jetResponsePlot);
            this->plotHistogram(this->_fsResponsePlot);
            this->plotHistogram(this->_fsInJetResponsePlot);

            //Close the plot
            this->_canvas.Print(this->_pdf + "]");

            //Print the purity decay modes
            for(PdgId parent: this->_resonancePdgId){
                const auto decays = sortMap(this->_decays[parent]);
                std::cout << std::endl << "Decay modes of " << particleName(parent) << ":" << std::endl << "----" << std::endl;
                for(const std::pair<Decay, int> &decayCount: decays){
                    std::cout << decayCount.first << ": " << (100.0 * decayCount.second / this->_numberOfParticles[parent]) << "%" << std::endl;
                }
            }
            std::cout << std::endl << "Purity: " << (100.0 * this->_purePT / this->_totalPT) << "%" << std::endl;
        }

    private:
        void plotHistogram(TH1D &histogram){
            histogram.SetStats(0);
            histogram.Draw("colz");
            this->_canvas.Print(this->_pdf);
        }

        static bool particleIsFromParton(Particle particle, const Particle &parton){
            while(particle.parents().size() > 0){
                if(particle.isSame(parton)){
                    return true;
                }
                particle = particlesByEnergy(particle.parents())[0];
            }
            return false;
        }

        int particleColor(const ParticleBase &particleOrJet, const Jets &jets, const Particles &partons){
            const Particle *particle = dynamic_cast<const Particle*>(&particleOrJet);
            const Jet *jet = dynamic_cast<const Jet*>(&particleOrJet);
            switch(this->_plotColor){
            case PlotColor::PARTON:
                for(Particle parentParticle: (jet ? particlesByEnergy(jet->particles()) : Particles{*particle})){
                    const std::vector<int> colors{EColor::kRed, EColor::kBlue, EColor::kGreen + 2};
                    while(parentParticle.parents().size() > 0){
                        for(unsigned int i = 0; i < partons.size() && i < colors.size(); i++){
                            if(parentParticle.isSame(particlesByEnergy(partons)[i])){
                                return colors[i] + (jet ? -9 : 0);
                            }
                        }
                        parentParticle = particlesByEnergy(parentParticle.parents())[0];
                    }
                }
                break;
            case PlotColor::JET:
                for(const std::pair<Jet, int> &jetColorPair: std::vector<std::pair<Jet, int>>{{jets[0], EColor::kRed}, {jets[1], EColor::kBlue}}){
                    if(jet != nullptr && jet->size() == jetColorPair.first.size() && jet->momentum() == jetColorPair.first.momentum()){
                        return jetColorPair.second - 9;
                    }
                    else if(particle != nullptr && jetColorPair.first.containsParticle(*particle)){
                        return jetColorPair.second;
                    }
                }
                break;
            case PlotColor::CHARGE:
                if(particle != nullptr){
                    //Positive
                    if(particle->charge3() > 0){
                        return EColor::kRed;
                    }
                    //Negative
                    else if(particle->charge3() < 0){
                        return EColor::kGreen + 1;
                    }
                    //Neutral
                    else{
                        return EColor::kBlue + 1;
                    }
                }
                break;
            case PlotColor::TYPE:
                //Jets
                if(jet != nullptr){
                    return EColor::kBlue - 9;
                }
                //Partons
                else if(particle->abspid() < 10 || particle->abspid() == 21){
                    return EColor::kMagenta + 2;
                }
                //Bosons (excluding gluons)
                else if(particle->abspid() / 10 == 2){
                    return EColor::kOrange;
                }
                //Leptons
                else if(particle->abspid() / 10 == 1){
                    return EColor::kGreen + 1;
                }
                //Mesons
                else if(particle->abspid() >= 100 && particle->abspid() < 1000){
                    return EColor::kRed;
                }
                //Baryons or diquarks
                else if(particle->abspid() >= 1000 && particle->abspid() < 10000){
                    //Diquarks
                    if(particle->abspid() % 100 < 10){
                        return EColor::kPink + 7;
                    }
                    //Baryons
                    else{
                        return EColor::kRed + 2;
                    }
                }
                //Unknown particles
                break;
            }
            return jet ? EColor::kGray : EColor::kBlack;
        }

        int _numberOfPlots;
        std::map<PdgId, int> _numberOfParticles;
        std::map<PdgId, std::map<Decay, int>> _decays;

        const double _jetRadius;
        const TString _pdf;
        const bool _plotSecondChildren;
        TCanvas _canvas;
        TH2D _pTFlow;

        static constexpr int _deltaRBins = 20;
        static constexpr double _deltaRMax = 2.0;
        std::vector<double> _efficiencyData;
        double _totalPT, _purePT;

        static constexpr int _pTBins = 50;
        static constexpr double _maxPT = 3e3;
        static constexpr double _maxResponse = 2.0;
        TH1D _partonPTPlot, _jetResponsePlot, _fsResponsePlot, _fsInJetResponsePlot;
        std::map<PdgId, std::unique_ptr<TH1D>> _partonPTPlotByType;

        const std::vector<PdgId> _resonancePdgId;
        PlotColor _plotColor;
    };

    DECLARE_RIVET_PLUGIN(JetEfficiency);
}