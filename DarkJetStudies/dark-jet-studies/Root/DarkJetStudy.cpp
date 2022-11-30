#include <TLorentzVector.h>
#include <xAODMetaData/FileMetaData.h>
#include <xAODTruth/TruthParticleContainer.h>
#include <xAODEventInfo/EventInfo.h>
#include <fastjet/PseudoJet.hh>
#include <fastjet/ClusterSequence.hh>
#include "dark-jet-studies/DarkJetStudy.h"
#include "dark-jet-studies/ParticleUtils.hpp"
#include "dark-jet-studies/Jet.hpp"
#include "dark-jet-studies/ParticleSort.hpp"
#include "dark-jet-studies/Darkness.hpp"

double DarkJetStudy::jetRadius = 1.0;
std::regex DarkJetStudy::darkRegex("^49[0-9]{3}([013-9][0-9]|[0-9][0-24-9])$");

DarkJetStudy::DarkJetStudy(const std::string& name, ISvcLocator *pSvcLocator):
    EL::AnaAlgorithm(name, pSvcLocator),
    _numEvents(0),
    _leadingJetPTPlot(nullptr),
    _subLeadingJetPTPlot(nullptr),
    _thirdLeadingJetPTPlot(nullptr),
    _dijetInvariantMassPlot(nullptr),
    _leadingJetInvisiblePlot(nullptr),
    _subLeadingJetInvisiblePlot(nullptr),
    _thirdLeadingJetInvisiblePlot(nullptr),
    _leadingJetDarknessPlot(nullptr),
    _subLeadingJetDarknessPlot(nullptr),
    _thirdLeadingJetDarknessPlot(nullptr)
{}


StatusCode DarkJetStudy::initialize(){
    ANA_CHECK(this->book(TH1D(
        "leadingJetPT", ";#it{p_{T}} of jet (GeV);Number of events",
        this->_bins, 0.0, this->_maxPT    //x bins, min x, max x
    )));
    this->_leadingJetPTPlot = this->hist("leadingJetPT");
    ANA_CHECK(this->book(TH1D(
        "subLeadingJetPT", ";#it{p_{T}} of jet (GeV);Number of events",
        this->_bins, 0.0, this->_maxPT    //x bins, min x, max x
    )));
    this->_subLeadingJetPTPlot = this->hist("subLeadingJetPT");
    ANA_CHECK(this->book(TH1D(
        "thirdLeadingJetPT", ";#it{p_{T}} of jet (GeV);Number of events",
        this->_bins, 0.0, this->_maxPT    //x bins, min x, max x
    )));
    this->_thirdLeadingJetPTPlot = this->hist("thirdLeadingJetPT");
    ANA_CHECK(this->book(TH1D(
        "dijetInvariantMass", ";Dijet invariant mass (GeV);Number of events",
        this->_bins, 0.0, this->_maxPT    //x bins, min x, max x
    )));
    this->_dijetInvariantMassPlot = this->hist("dijetInvariantMass");
    ANA_CHECK(this->book(TH1D(
        "leadingJetInvisible", ";Fraction of pT of the jet that is invisible (%);Number of events",
        this->_bins, 0.0, 100.0    //x bins, min x, max x
    )));
    this->_leadingJetInvisiblePlot = this->hist("leadingJetInvisible");
    ANA_CHECK(this->book(TH1D(
        "subLeadingJetInvisible", ";Fraction of pT of the jet that is invisible (%);Number of events",
        this->_bins, 0.0, 100.0    //x bins, min x, max x
    )));
    this->_subLeadingJetInvisiblePlot = this->hist("subLeadingJetInvisible");
    ANA_CHECK(this->book(TH1D(
        "thirdLeadingJetInvisible", ";Fraction of pT of the jet that is invisible (%);Number of events",
        this->_bins, 0.0, 100.0    //x bins, min x, max x
    )));
    this->_thirdLeadingJetInvisiblePlot = this->hist("thirdLeadingJetInvisible");
    ANA_CHECK(this->book(TH1D(
        "leadingJetDarkness", ";Fraction of pT of the jet that has dark ancestors (%);Number of events",
        this->_bins, 0.0, 100.0    //x bins, min x, max x
    )));
    this->_leadingJetDarknessPlot = this->hist("leadingJetDarkness");
    ANA_CHECK(this->book(TH1D(
        "subLeadingJetDarkness", ";Fraction of pT of the jet that has dark ancestors (%);Number of events",
        this->_bins, 0.0, 100.0    //x bins, min x, max x
    )));
    this->_subLeadingJetDarknessPlot = this->hist("subLeadingJetDarkness");
    ANA_CHECK(this->book(TH1D(
        "thirdLeadingJetDarkness", ";Fraction of pT of the jet that has dark ancestors (%);Number of events",
        this->_bins, 0.0, 100.0    //x bins, min x, max x
    )));
    this->_thirdLeadingJetDarknessPlot = this->hist("thirdLeadingJetDarkness");

    return StatusCode::SUCCESS;
}


StatusCode DarkJetStudy::execute(){
    this->_numEvents++;
    if(this->_numEvents % 100 == 0){
        std::cout << "Event " << this->_numEvents << std::endl;
    }

    const xAOD::TruthParticleContainer *particles = nullptr;
    const xAOD::EventInfo *eventInfo = nullptr;
    ANA_CHECK(this->evtStore()->retrieve(particles, "TruthParticles"));
    ANA_CHECK(this->evtStore()->retrieve(eventInfo, "EventInfo"));

    std::vector<fastjet::PseudoJet> jetParticles;
    std::vector<const xAOD::TruthParticle*> finalStateParticles;
    for(const xAOD::TruthParticle *particle: *particles){
        if(particle->status() != 1){    //If the particle is unstable
            continue;
        }
        if(particle->barcode() >= 200000){    //If the particle was created through interactions with the detector
            continue;
        }

        jetParticles.push_back(fastjet::PseudoJet(particle->px(), particle->py(), particle->pz(), particle->e()));
        jetParticles.back().set_user_index(particle->index());
        finalStateParticles.push_back(particle);
    }

    //Manual jet building
    static fastjet::JetDefinition antikt(fastjet::antikt_algorithm, jetRadius, fastjet::E_scheme, fastjet::Best);
    //Forms pseudojets from stable particles.
    fastjet::ClusterSequence clustSeqAntikt(jetParticles, antikt);
    std::vector<Jet> jets;
    const auto pseudoJets = sorted_by_pt(clustSeqAntikt.inclusive_jets(5000.0));
    for(const fastjet::PseudoJet &jet: pseudoJets){
        jets.push_back(Jet(jet, particles));
    }

    //Jet pT and invariant mass
    this->_leadingJetPTPlot->Fill(jets[0].pT() / 1000);
    this->_subLeadingJetPTPlot->Fill(jets[1].pT() / 1000);
    this->_thirdLeadingJetPTPlot->Fill(jets[2].pT() / 1000);
    this->_dijetInvariantMassPlot->Fill(invariantMass(jets[0].momentum() + jets[1].momentum()) / 1000);

    //Invisibility and darkness
    this->_leadingJetInvisiblePlot->Fill(pTInvisibility(jets[0]) * 100.0);
    this->_subLeadingJetInvisiblePlot->Fill(pTInvisibility(jets[1]) * 100.0);
    this->_thirdLeadingJetInvisiblePlot->Fill(pTInvisibility(jets[2]) * 100.0);
    this->_leadingJetDarknessPlot->Fill(pTDarkness(jets[0]) * 100.0);
    this->_subLeadingJetDarknessPlot->Fill(pTDarkness(jets[1]) * 100.0);
    this->_thirdLeadingJetDarknessPlot->Fill(pTDarkness(jets[2]) * 100.0);

    //Jet multiplicity
    int jetMultiplicity = 0, darkJetMultiplicity20 = 0, darkJetMultiplicity50 = 0, darkJetMultiplicity80 = 0;
    for(const Jet &jet: jets){
        if(jet.pT() < 30000){
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

    return StatusCode::SUCCESS;
}


StatusCode DarkJetStudy::finalize(){
    //Plot the jet multiplicity
    const int maxMultiplicity = std::max_element(this->_jetMultiplicityData.begin(), this->_jetMultiplicityData.end(), [](const std::pair<int, int> &a, const std::pair<int, int> &b){return a.first < b.first;})->first;
    ANA_CHECK(this->book(TH1D(
        "jetMultiplicity", ";Jet multiplicity;Number of events",
        maxMultiplicity * 2 + 2, 0.0, maxMultiplicity + 1    //x bins, min x, max x
    )));
    TH1 *jetMultiplicityPlot = this->hist("jetMultiplicity");
    ANA_CHECK(this->book(TH1D(
        "darkJetMultiplicity20", ";Jet multiplicity;Number of events",
        maxMultiplicity * 2 + 2, 0.0, maxMultiplicity + 1    //x bins, min x, max x
    )));
    TH1 *darkJetMultiplicity20Plot = this->hist("darkJetMultiplicity20");
    ANA_CHECK(this->book(TH1D(
        "darkJetMultiplicity50", ";Jet multiplicity;Number of events",
        maxMultiplicity * 2 + 2, 0.0, maxMultiplicity + 1    //x bins, min x, max x
    )));
    TH1 *darkJetMultiplicity50Plot = this->hist("darkJetMultiplicity50");
    ANA_CHECK(this->book(TH1D(
        "darkJetMultiplicity80", ";Jet multiplicity;Number of events",
        maxMultiplicity * 2 + 2, 0.0, maxMultiplicity + 1    //x bins, min x, max x
    )));
    TH1 *darkJetMultiplicity80Plot = this->hist("darkJetMultiplicity80");

    for(const auto &multiplicityEventsPair: this->_jetMultiplicityData){
        jetMultiplicityPlot->Fill(multiplicityEventsPair.first - 0.1, multiplicityEventsPair.second);
        jetMultiplicityPlot->Fill(multiplicityEventsPair.first + 0.1, multiplicityEventsPair.second);
    }
    for(const auto &multiplicityEventsPair: this->_darkJetMultiplicity20Data){
        darkJetMultiplicity20Plot->Fill(multiplicityEventsPair.first - 0.1, multiplicityEventsPair.second);
        darkJetMultiplicity20Plot->Fill(multiplicityEventsPair.first + 0.1, multiplicityEventsPair.second);
    }
    for(const auto &multiplicityEventsPair: this->_darkJetMultiplicity50Data){
        darkJetMultiplicity50Plot->Fill(multiplicityEventsPair.first - 0.1, multiplicityEventsPair.second);
        darkJetMultiplicity50Plot->Fill(multiplicityEventsPair.first + 0.1, multiplicityEventsPair.second);
    }
    for(const auto &multiplicityEventsPair: this->_darkJetMultiplicity80Data){
        darkJetMultiplicity80Plot->Fill(multiplicityEventsPair.first - 0.1, multiplicityEventsPair.second);
        darkJetMultiplicity80Plot->Fill(multiplicityEventsPair.first + 0.1, multiplicityEventsPair.second);
    }

    return StatusCode::SUCCESS;
}
