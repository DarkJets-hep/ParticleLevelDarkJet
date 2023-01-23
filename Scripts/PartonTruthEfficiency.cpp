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
#include "../Headers/Darkness.hpp"
#include "../Headers/Decay.hpp"
#include "../Headers/ParticleName.hpp"
#include "../Headers/PlotParticle.hpp"
#include "../Headers/ParticleSort.hpp"
#include "../Headers/GetEnvVars.hpp"
#include "../Headers/Legend.hpp"

enum PlotColor{
    NONE = 0,
    PARTON = 1,
    JET = 2,
    CHARGE = 3,
    TYPE = 4
};

namespace Rivet{
    class PartonTruthEfficiency: public Analysis{
    public:
        PartonTruthEfficiency():
            Analysis("PartonTruthEfficiency"),
            _numberOfPlots(0),
            _jetRadius(getDoubleFromEnvVar("JET_RADIUS", 1.0)),
            _includeInvisibles(getIntFromEnvVar("INCLUDE_INVISIBLES", 0)),
            _pdf(getStringFromEnvVar("PDF_FILENAME", TString("../Outputs/PartonTruthEfficiency.pdf"))),
            _plotSecondChildren(getIntFromEnvVar("PLOT_SECOND_CHILDREN", 0)),
            _pTFlow(
                "", ";Rapidity #it{y};Azimuth #it{#phi};Jet #it{p}_{T} [GeV]",
                200, -4, 4,    //y bins, min y, max y
                157, 0, 2*M_PI    //phi bins, min phi, max phi
            ),
            _efficiencyData(this->_deltaRBins + 1),
            _totalPT(0.0),
            _purePT(0.0),
            _responseSum(0.0),
            _numberOfEventsWithResponse(0),
            _partonPTPlot(
                "", ";Parton #it{p_{T}} (GeV);Number of events",
                this->_bins, 0.0, this->_maxPT/2    //x bins, min x, max x
            ),
            _partonInvariantMassPlot(
                "", ";Di-parton invariant mass (GeV);Number of events",
                this->_bins, 0.0, this->_maxPT    //x bins, min x, max x
            ),
            _jetResponsePlot(
                "", ";Response (Truth jet #it{p_{T}} / Parton #it{p_{T}});Number of events",
                this->_bins, 0.0, this->_maxResponse    //x bins, min x, max x
            ),
            _fsResponsePlot(
                "", ";#it{p_{T}} of final state children of parton / Parton #it{p_{T}};Number of events",
                this->_bins, 0.0, this->_maxResponse*3/2    //x bins, min x, max x
            ),
            _fsInJetResponsePlot(
                "", ";#it{p_{T}} of final state children of parton in truth jet / Parton #it{p_{T}};Number of events",
                this->_bins, 0.0, this->_maxResponse    //x bins, min x, max x
            ),
            _leadingJetPTPlot(
                "", ";#it{p_{T}} of truth jet (GeV);Number of events",
                this->_bins, 0.0, this->_maxPT/2    //x bins, min x, max x
            ),
            _subLeadingJetPTPlot(
                "", ";#it{p_{T}} of truth jet (GeV);Number of events",
                this->_bins, 0.0, this->_maxPT/2    //x bins, min x, max x
            ),
            _thirdLeadingJetPTPlot(
                "", ";#it{p_{T}} of truth jet (GeV);Number of events",
                this->_bins, 0.0, this->_maxPT/2    //x bins, min x, max x
            ),
            _dijetInvariantMassPlot(
                "", ";Truth dijet invariant mass (GeV);Number of events",
                this->_bins, 0.0, this->_maxPT    //x bins, min x, max x
            ),
            _leadingJetInvisiblePlot(
                "", ";Invisibility of truth jet (%);Number of events",
                this->_bins, 0.0, 100.0    //x bins, min x, max x
            ),
            _subLeadingJetInvisiblePlot(
                "", ";Invisibility of truth jet (%);Number of events",
                this->_bins, 0.0, 100.0    //x bins, min x, max x
            ),
            _thirdLeadingJetInvisiblePlot(
                "", ";Invisibility of truth jet (%);Number of events",
                this->_bins, 0.0, 100.0    //x bins, min x, max x
            ),
            _leadingJetDarknessPlot(
                "", ";Darkness of truth jet (%);Number of events",
                this->_bins, 0.0, 100.0    //x bins, min x, max x
            ),
            _subLeadingJetDarknessPlot(
                "", ";Darkness of truth jet (%);Number of events",
                this->_bins, 0.0, 100.0    //x bins, min x, max x
            ),
            _thirdLeadingJetDarknessPlot(
                "", ";Darkness of truth jet (%);Number of events",
                this->_bins, 0.0, 100.0    //x bins, min x, max x
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
            this->declare(FastJets(cnfs, FastJets::ANTIKT, this->_jetRadius, JetAlg::Muons::ALL, this->_includeInvisibles ? JetAlg::Invisibles::ALL : JetAlg::Invisibles::NONE), "Jets");

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
                    double deltaPhi = parton.phi() - newJet.phi();
                    if(deltaPhi < -M_PI) deltaPhi += 2 * M_PI;
                    else if(deltaPhi > M_PI) deltaPhi -= 2 * M_PI;
                    const double newDeltaR = std::sqrt(deltaY * deltaY + deltaPhi * deltaPhi);
                    if(newDeltaR < deltaR){
                        deltaR = newDeltaR;
                        jet = newJet;
                    }
                }

                //Efficiency
                for(int i = deltaR * this->_deltaRBins / this->_deltaRMax; i < this->_deltaRBins + 1; i++){
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
                    this->_partonPTPlot.Fill(parton.pT());
                    if(!this->_partonPTPlotByType.count(parton.abspid())){
                        this->_partonPTPlotByType.insert({parton.abspid(), std::unique_ptr<TH1D>(new TH1D(
                            "", (";#it{p_{T}}(#it{" + absParticleNameAsTLatex(parton.abspid()) + "}) (GeV);Number of events").c_str(),
                            this->_bins, 0.0, this->_maxPT    //x bins, min x, max x
                        ))});
                    }
                    this->_partonPTPlotByType[parton.abspid()]->Fill(parton.pT());
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
                        this->_jetResponsePlot.Fill(jet.pT() / parton.pT());
                        this->_responseSum += jet.pT() / parton.pT();
                        this->_numberOfEventsWithResponse++;
                    }
                    if(fsInJetPT < parton.pT() * this->_maxResponse && fsInJetPT > 0){
                        this->_fsInJetResponsePlot.Fill(fsInJetPT / parton.pT());
                    }
                }
                if(fsPT < parton.pT() * this->_maxResponse && fsPT > 0){
                    this->_fsResponsePlot.Fill(fsPT / parton.pT());
                }
            }

            //Parton invariant mass
            this->_partonInvariantMassPlot.Fill((excitedQuark.children()[0].momentum() + excitedQuark.children()[1].momentum()).mass());

            //Jet pT and invariant mass
            this->_leadingJetPTPlot.Fill(jets[0].pT());
            this->_subLeadingJetPTPlot.Fill(jets[1].pT());
            this->_thirdLeadingJetPTPlot.Fill(jets[2].pT());
            this->_dijetInvariantMassPlot.Fill((jets[0].momentum() + jets[1].momentum()).mass());
            
            //Invisibility and darkness
            this->_leadingJetInvisiblePlot.Fill(pTInvisibility(jets[0]) * 100.0);
            this->_subLeadingJetInvisiblePlot.Fill(pTInvisibility(jets[1]) * 100.0);
            this->_thirdLeadingJetInvisiblePlot.Fill(pTInvisibility(jets[2]) * 100.0);
            this->_leadingJetDarknessPlot.Fill(pTDarkness(jets[0]) * 100.0);
            this->_subLeadingJetDarknessPlot.Fill(pTDarkness(jets[1]) * 100.0);
            this->_thirdLeadingJetDarknessPlot.Fill(pTDarkness(jets[2]) * 100.0);

            //Jet multiplicity
            int jetMultiplicity = 0, darkJetMultiplicity20 = 0, darkJetMultiplicity50 = 0, darkJetMultiplicity80 = 0;
            for(const Jet &jet: jets){
                if(jet.pT() < 30){
                    break;
                }
                jetMultiplicity++;
                const double darkness = pTDarkness(jet);
                if(darkness > 0.2){
                    darkJetMultiplicity20++;
                }
                if(darkness > 0.5){
                    darkJetMultiplicity50++;
                }
                if(darkness > 0.8){
                    darkJetMultiplicity80++;
                }
            }
            this->_jetMultiplicityData[jetMultiplicity]++;
            this->_darkJetMultiplicity20Data[darkJetMultiplicity20]++;
            this->_darkJetMultiplicity50Data[darkJetMultiplicity50]++;
            this->_darkJetMultiplicity80Data[darkJetMultiplicity80]++;

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
                int marker = 20;
                if(!finalParticle.isVisible()){
                    if(finalParticle.isNeutrino()){
                        marker = 24;
                    }
                    else{    //Dark matter
                        marker = 25;
                    }
                }
                plotParticle(finalParticle, marker, particleColor(finalParticle, jets, partons), false, 0.3);
            }

            //Plot the excited quark and its decay products
            plotParticle(excitedQuark, 29);
            for(const Particle &child: children){
                plotParticle(child, 20, this->_plotColor == PlotColor::PARTON ? particleColor(child, jets, partons) : EColor::kBlack);
            }

            //Print the page
            this->_pTFlow.Draw("axis same");
            drawTitle(&this->_pTFlow, this->title());
            const std::vector<TString> &legends = this->_particleColorLegends.at(this->_plotColor);
            const auto legend = drawLegend(&this->_pTFlow, this->_particleColors.at(this->_plotColor), (this->_plotColor == PlotColor::PARTON && !this->_plotSecondChildren) ? std::vector<TString>(legends.begin(), legends.end() - 1) : legends);
            this->_canvas.Print(this->_pdf);
        }

        virtual void finalize() override{
            //Plot the efficiency
            TH1D efficiencyPlot(
                "", ";#it{#Delta R};Efficiency between parton and truth jet (%)",
                this->_deltaRBins, 0.0, this->_deltaRMax    //x bins, min x, max x
            );
            for(int i = 0; i < this->_deltaRBins + 1; i++){
                efficiencyPlot.AddBinContent(i, 50.0 * this->_efficiencyData[i] / this->numEvents());
            }
            this->plotHistogram(efficiencyPlot);

            //Plot the pT and response
            this->plotHistogram(this->_partonPTPlot);
            for(const auto &pdgidPlotPair: this->_partonPTPlotByType){
                this->plotHistogram(*pdgidPlotPair.second);
            }
            this->plotHistogram(this->_partonInvariantMassPlot);
            this->plotHistogram(this->_jetResponsePlot);
            this->plotHistogram(this->_fsResponsePlot);
            this->plotHistogram(this->_fsInJetResponsePlot);
            this->plotHistograms({&this->_leadingJetPTPlot, &this->_subLeadingJetPTPlot, &this->_thirdLeadingJetPTPlot}, std::vector<TString>{"Leading jet", "Subleading jet", "Third leading jet"});
            this->plotHistogram(this->_dijetInvariantMassPlot);
            if(this->_includeInvisibles){
                this->plotHistograms({&this->_leadingJetInvisiblePlot, &this->_subLeadingJetInvisiblePlot, &this->_thirdLeadingJetInvisiblePlot}, std::vector<TString>{"Leading jet", "Subleading jet", "Third leading jet"});
            }
            this->plotHistograms({&this->_leadingJetDarknessPlot, &this->_subLeadingJetDarknessPlot, &this->_thirdLeadingJetDarknessPlot}, std::vector<TString>{"Leading jet", "Subleading jet", "Third leading jet"});

            //Plot the jet multiplicity
            const int maxMultiplicity = std::max_element(this->_jetMultiplicityData.begin(), this->_jetMultiplicityData.end(), [](const std::pair<int, int> &a, const std::pair<int, int> &b){return a.first < b.first;})->first;
            TH1D darkJetMultiplicity80Plot(
                "", ";Truth jet multiplicity;Number of events",
                maxMultiplicity * 2 + 2, 0.0, maxMultiplicity + 1    //x bins, min x, max x
            ),
            &darkJetMultiplicity50Plot = *static_cast<TH1D*>(darkJetMultiplicity80Plot.Clone()),
            &darkJetMultiplicity20Plot = *static_cast<TH1D*>(darkJetMultiplicity80Plot.Clone()),
            &jetMultiplicityPlot = *static_cast<TH1D*>(darkJetMultiplicity80Plot.Clone());
            for(const auto &multiplicityEventsPair: this->_jetMultiplicityData){
                jetMultiplicityPlot.Fill(multiplicityEventsPair.first - 0.1, multiplicityEventsPair.second);
                jetMultiplicityPlot.Fill(multiplicityEventsPair.first + 0.1, multiplicityEventsPair.second);
            }
            for(const auto &multiplicityEventsPair: this->_darkJetMultiplicity20Data){
                darkJetMultiplicity20Plot.Fill(multiplicityEventsPair.first - 0.1, multiplicityEventsPair.second);
                darkJetMultiplicity20Plot.Fill(multiplicityEventsPair.first + 0.1, multiplicityEventsPair.second);
            }
            for(const auto &multiplicityEventsPair: this->_darkJetMultiplicity50Data){
                darkJetMultiplicity50Plot.Fill(multiplicityEventsPair.first - 0.1, multiplicityEventsPair.second);
                darkJetMultiplicity50Plot.Fill(multiplicityEventsPair.first + 0.1, multiplicityEventsPair.second);
            }
            for(const auto &multiplicityEventsPair: this->_darkJetMultiplicity80Data){
                darkJetMultiplicity80Plot.Fill(multiplicityEventsPair.first - 0.1, multiplicityEventsPair.second);
                darkJetMultiplicity80Plot.Fill(multiplicityEventsPair.first + 0.1, multiplicityEventsPair.second);
            }
            this->plotHistograms({&jetMultiplicityPlot, &darkJetMultiplicity20Plot, &darkJetMultiplicity50Plot, &darkJetMultiplicity80Plot}, std::vector<TString>{"No darkness cut", "20% Darkness cut", "50% Darkness cut", "80% Darkness cut"}, ", p_{T} cut = 30 GeV");

            //Close the plot
            this->_canvas.Print(this->_pdf + "]");

            //Print the decay modes
            for(PdgId parent: this->_resonancePdgId){
                const auto decays = sortMap(this->_decays[parent]);
                std::cout << std::endl << "Decay modes of " << particleName(parent) << ":" << std::endl << "----" << std::endl;
                for(const std::pair<Decay, int> &decayCount: decays){
                    std::cout << decayCount.first << ": " << (100.0 * decayCount.second / this->_numberOfParticles[parent]) << "%" << std::endl;
                }
            }

            //Print the efficiency, purity and response
            std::cout << std::endl;
            std::cout << "Purity: " << (100.0 * this->_purePT / this->_totalPT) << "%" << std::endl;
            std::cout << "Efficiency at DeltaR = R: " << (50.0 * this->_efficiencyData[this->_efficiencyData.size() * this->_jetRadius / this->_deltaRMax] / this->numEvents()) << "%" << std::endl;
            std::cout << "Average response: " << (this->_responseSum / this->_numberOfEventsWithResponse) << std::endl;
        }

    private:
        TString title(const TString &extraLabel = "") const{
            return "Anti-#it{k_{t}}, #it{R} = " + std::regex_replace(std::to_string(this->_jetRadius), std::regex("([1-9]|\\.0)0+$"), "$1") + ", " + (this->_includeInvisibles ? "with" : "without") + " invisibles" + extraLabel + "\nModel " + modelName(this->_pdf) + ", Z' #rightarrow q_{D} #bar{q}_{D}";
        }

        void plotHistogram(TH1D &histogram){
            histogram.SetStats(0);
            histogram.Draw("colz");
            drawTitle(&histogram, this->title());
            this->_canvas.Print(this->_pdf);
        }

        void plotHistograms(std::vector<TH1D*> histograms, const std::vector<TString> &legends, const TString &extraLabel = ""){
            for(std::size_t i = 1; i < histograms.size() && i < this->_lineColors.size() + 1; i++){
                histograms[i]->SetLineColor(this->_lineColors[i - 1]);
            }
            std::sort(histograms.begin(), histograms.end(), [](const TH1D *a, const TH1D *b){
                return a->GetMaximum() > b->GetMaximum();
            });
            bool first = true;
            for(TH1D *histogram: histograms){
                histogram->SetStats(0);
                histogram->Draw(first ? "hist" : "histsame");
                first = false;
            }
            drawTitle(histograms[0], this->title(extraLabel));
            const auto legend = drawLegend(histograms[0], this->_lineColors, legends);
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
            const std::vector<int> &colors = this->_particleColors.at(this->_plotColor);
            switch(this->_plotColor){
            case PlotColor::PARTON:
                for(Particle parentParticle: (jet ? particlesByEnergy(jet->particles()) : Particles{*particle})){
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
                for(const std::pair<Jet, int> &jetColorPair: std::vector<std::pair<Jet, int>>{{jets[0], colors[0]}, {jets[1], colors[1]}}){
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
                    return colors[1 - (particle->charge3() > 0) + (particle->charge3() < 0)];
                }
                break;
            case PlotColor::TYPE:
                //Jets
                if(jet != nullptr){
                    return colors[0];
                }
                //Partons
                else if(particle->abspid() < 10 || particle->abspid() == 21){
                    return colors[1];
                }
                //Bosons (excluding gluons)
                else if(particle->abspid() / 10 == 2){
                    return colors[2];
                }
                //Leptons
                else if(particle->abspid() / 10 == 1){
                    return colors[3];
                }
                //Mesons
                else if(particle->abspid() >= 100 && particle->abspid() < 1000){
                    return colors[4];
                }
                //Baryons or diquarks
                else if(particle->abspid() >= 1000 && particle->abspid() < 10000){
                    //Diquarks
                    if(particle->abspid() % 100 < 10){
                        return colors[5];
                    }
                    //Baryons
                    else{
                        return colors[6];
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
        const bool _includeInvisibles;
        const TString _pdf;
        const bool _plotSecondChildren;
        TCanvas _canvas;
        TH2D _pTFlow;

        static constexpr int _deltaRBins = 20;
        static constexpr double _deltaRMax = 2.0;
        std::vector<double> _efficiencyData;
        double _totalPT, _purePT;
        double _responseSum;
        int _numberOfEventsWithResponse;
        std::map<int, int> _jetMultiplicityData;    //Contains the number of jets with pT > 30GeV as key, and the number of events with that key as value
        std::map<int, int> _darkJetMultiplicity20Data, _darkJetMultiplicity50Data, _darkJetMultiplicity80Data;    //Same as above but only counts jets with darkness > 20% / 50% / 80%

        static constexpr int _bins = 50;
        static constexpr double _maxPT = 3e3;
        static constexpr double _maxResponse = 2.0;
        TH1D _partonPTPlot, _partonInvariantMassPlot, _jetResponsePlot, _fsResponsePlot, _fsInJetResponsePlot;
        std::map<PdgId, std::unique_ptr<TH1D>> _partonPTPlotByType;
        TH1D _leadingJetPTPlot, _subLeadingJetPTPlot, _thirdLeadingJetPTPlot, _dijetInvariantMassPlot;
        TH1D _leadingJetInvisiblePlot, _subLeadingJetInvisiblePlot, _thirdLeadingJetInvisiblePlot, _leadingJetDarknessPlot, _subLeadingJetDarknessPlot, _thirdLeadingJetDarknessPlot;

        const std::vector<PdgId> _resonancePdgId;
        static inline const std::vector<int> _lineColors{EColor::kOrange - 3, EColor::kGreen + 2, EColor::kMagenta + 2};
        static inline const std::map<PlotColor, std::vector<int>> _particleColors{
            {PlotColor::PARTON, std::vector<int>{EColor::kRed, EColor::kBlue, EColor::kGreen + 2}},
            {PlotColor::JET, std::vector<int>{EColor::kRed, EColor::kBlue}},
            {PlotColor::CHARGE, std::vector<int>{EColor::kRed, EColor::kBlue + 1, EColor::kGreen + 1}},
            {PlotColor::TYPE, std::vector<int>{EColor::kBlue - 9, EColor::kMagenta + 2, EColor::kOrange, EColor::kGreen + 1, EColor::kRed, EColor::kPink + 7, EColor::kRed + 2}},
            {PlotColor::NONE, std::vector<int>{}}
        };
        static inline const std::map<PlotColor, std::vector<TString>> _particleColorLegends{
            {PlotColor::PARTON, std::vector<TString>{"Leading parton & children", "Subleading parton & children", "Third leading parton & children"}},
            {PlotColor::JET, std::vector<TString>{"Leading jet", "Subleading jet"}},
            {PlotColor::CHARGE, std::vector<TString>{"Positive", "Neutral", "Negative"}},
            {PlotColor::TYPE, std::vector<TString>{"Jet", "Parton", "Boson", "Lepton", "Meson", "Diquark", "Baryon"}},
            {PlotColor::NONE, std::vector<TString>{}}
        };
        PlotColor _plotColor;
    };

    DECLARE_RIVET_PLUGIN(PartonTruthEfficiency);
}