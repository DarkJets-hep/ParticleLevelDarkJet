#include <TLorentzVector.h>
#include <xAODMetaData/FileMetaData.h>
#include <xAODTruth/TruthParticleContainer.h>
#include <xAODJet/JetContainer.h>
#include <xAODEventInfo/EventInfo.h>
#include <xAODPFlow/FlowElementContainer.h>
#include <fastjet/PseudoJet.hh>
#include <fastjet/ClusterSequence.hh>
#include "dark-jet-studies/DarkJetStudy.h"
#include "dark-jet-studies/ParticleUtils.hpp"
#include "dark-jet-studies/Jet.hpp"
#include "dark-jet-studies/ParticleSort.hpp"
#include "dark-jet-studies/Darkness.hpp"

std::regex DarkJetStudy::darkRegex("^49[0-9]{3}([013-9][0-9]|[0-9][0-24-9])$");

DarkJetStudy::DarkJetStudy(const std::string& name, ISvcLocator *pSvcLocator):
    EL::AnaAlgorithm(name, pSvcLocator),
    _numEvents(0),
    _jetRadiusTimes10(10),
    _efficiencyData(this->_deltaRBins + 1),
    _responseSum(0.0),
    _numberOfEventsWithResponse(0),

    _leadingJetPTPlot(nullptr),
    _subLeadingJetPTPlot(nullptr),
    _thirdLeadingJetPTPlot(nullptr),
    _dijetInvariantMassPlot(nullptr),

    _cutLeadingJetPTPlot(nullptr),
    _cutSubLeadingJetPTPlot(nullptr),
    _cutThirdLeadingJetPTPlot(nullptr),
    _cutDijetInvariantMassPlot(nullptr),

    _leadingJetInvisiblePlot(nullptr),
    _subLeadingJetInvisiblePlot(nullptr),
    _thirdLeadingJetInvisiblePlot(nullptr),
    _leadingJetDarknessPlot(nullptr),
    _subLeadingJetDarknessPlot(nullptr),
    _thirdLeadingJetDarknessPlot(nullptr),

    _cutLeadingJetInvisiblePlot(nullptr),
    _cutSubLeadingJetInvisiblePlot(nullptr),
    _cutThirdLeadingJetInvisiblePlot(nullptr),
    _cutLeadingJetLeptonPlot(nullptr),
    _cutSubLeadingJetLeptonPlot(nullptr),
    _cutThirdLeadingJetLeptonPlot(nullptr),

    _responsePlot(nullptr),
    _leadingEfficiencyPlot(nullptr),
    _subLeadingEfficiencyPlot(nullptr),
    _thirdLeadingEfficiencyPlot(nullptr)
{}


StatusCode DarkJetStudy::initialize(){
    ANA_CHECK(this->book(TH1D(
        "RecoTruthEfficiency_leadingJetPT", ";#it{p_{T}} of truth jet (GeV);Number of events",
        this->_bins, 0.0, this->_maxPT/2    //x bins, min x, max x
    )));
    this->_leadingJetPTPlot = this->hist("RecoTruthEfficiency_leadingJetPT");

    ANA_CHECK(this->book(TH1D(
        "RecoTruthEfficiency_subLeadingJetPT", ";#it{p_{T}} of truth jet (GeV);Number of events",
        this->_bins, 0.0, this->_maxPT/2    //x bins, min x, max x
    )));
    this->_subLeadingJetPTPlot = this->hist("RecoTruthEfficiency_subLeadingJetPT");

    ANA_CHECK(this->book(TH1D(
        "RecoTruthEfficiency_thirdLeadingJetPT", ";#it{p_{T}} of truth jet (GeV);Number of events",
        this->_bins, 0.0, this->_maxPT/2    //x bins, min x, max x
    )));
    this->_thirdLeadingJetPTPlot = this->hist("RecoTruthEfficiency_thirdLeadingJetPT");

    ANA_CHECK(this->book(TH1D(
        "RecoTruthEfficiency_dijetInvariantMass", ";Truth dijet invariant mass (GeV);Number of events",
        this->_bins, 0.0, this->_maxPT    //x bins, min x, max x
    )));
    this->_dijetInvariantMassPlot = this->hist("RecoTruthEfficiency_dijetInvariantMass");


    ANA_CHECK(this->book(TH1D(
        "ModelCompare_leadingJetPT", ";#it{p_{T}} of leading truth jet (GeV);Number of events",
        this->_bins, 0.0, this->_maxPT/2    //x bins, min x, max x
    )));
    this->_cutLeadingJetPTPlot = this->hist("ModelCompare_leadingJetPT");

    ANA_CHECK(this->book(TH1D(
        "ModelCompare_subLeadingJetPT", ";#it{p_{T}} of subleading truth jet (GeV);Number of events",
        this->_bins, 0.0, this->_maxPT/2    //x bins, min x, max x
    )));
    this->_cutSubLeadingJetPTPlot = this->hist("ModelCompare_subLeadingJetPT");

    ANA_CHECK(this->book(TH1D(
        "ModelCompare_thirdLeadingJetPT", ";#it{p_{T}} of third leading truth jet (GeV);Number of events",
        this->_bins, 0.0, this->_maxPT/2    //x bins, min x, max x
    )));
    this->_cutThirdLeadingJetPTPlot = this->hist("ModelCompare_thirdLeadingJetPT");

    ANA_CHECK(this->book(TH1D(
        "ModelCompare_dijetInvariantMass", ";Truth dijet invariant mass (GeV);Number of events",
        this->_bins, 0.0, this->_maxPT    //x bins, min x, max x
    )));
    this->_cutDijetInvariantMassPlot = this->hist("ModelCompare_dijetInvariantMass");


    ANA_CHECK(this->book(TH1D(
        "RecoTruthEfficiency_leadingJetInvisible", ";Invisibility of truth jet (%);Number of events",
        this->_bins, 0.0, 100.0    //x bins, min x, max x
    )));
    this->_leadingJetInvisiblePlot = this->hist("RecoTruthEfficiency_leadingJetInvisible");

    ANA_CHECK(this->book(TH1D(
        "RecoTruthEfficiency_subLeadingJetInvisible", ";Invisibility of truth jet (%);Number of events",
        this->_bins, 0.0, 100.0    //x bins, min x, max x
    )));
    this->_subLeadingJetInvisiblePlot = this->hist("RecoTruthEfficiency_subLeadingJetInvisible");

    ANA_CHECK(this->book(TH1D(
        "RecoTruthEfficiency_thirdLeadingJetInvisible", ";Invisibility of truth jet (%);Number of events",
        this->_bins, 0.0, 100.0    //x bins, min x, max x
    )));
    this->_thirdLeadingJetInvisiblePlot = this->hist("RecoTruthEfficiency_thirdLeadingJetInvisible");


    ANA_CHECK(this->book(TH1D(
        "RecoTruthEfficiency_leadingJetDarkness", ";Darkness of truth jet (%);Number of events",
        this->_bins, 0.0, 100.0    //x bins, min x, max x
    )));
    this->_leadingJetDarknessPlot = this->hist("RecoTruthEfficiency_leadingJetDarkness");

    ANA_CHECK(this->book(TH1D(
        "RecoTruthEfficiency_subLeadingJetDarkness", ";Darkness of truth jet (%);Number of events",
        this->_bins, 0.0, 100.0    //x bins, min x, max x
    )));
    this->_subLeadingJetDarknessPlot = this->hist("RecoTruthEfficiency_subLeadingJetDarkness");

    ANA_CHECK(this->book(TH1D(
        "RecoTruthEfficiency_thirdLeadingJetDarkness", ";Darkness of truth jet (%);Number of events",
        this->_bins, 0.0, 100.0    //x bins, min x, max x
    )));
    this->_thirdLeadingJetDarknessPlot = this->hist("RecoTruthEfficiency_thirdLeadingJetDarkness");


    ANA_CHECK(this->book(TH1D(
        "ModelCompare_leadingJetInvisible", ";Invisibility of leading truth jet (%);Number of events",
        this->_bins, 0.0, 100.0    //x bins, min x, max x
    )));
    this->_cutLeadingJetInvisiblePlot = this->hist("ModelCompare_leadingJetInvisible");

    ANA_CHECK(this->book(TH1D(
        "ModelCompare_subLeadingJetInvisible", ";Invisibility of subleading truth jet (%);Number of events",
        this->_bins, 0.0, 100.0    //x bins, min x, max x
    )));
    this->_cutSubLeadingJetInvisiblePlot = this->hist("ModelCompare_subLeadingJetInvisible");

    ANA_CHECK(this->book(TH1D(
        "ModelCompare_thirdLeadingJetInvisible", ";Invisibility third leading of truth jet (%);Number of events",
        this->_bins, 0.0, 100.0    //x bins, min x, max x
    )));
    this->_cutThirdLeadingJetInvisiblePlot = this->hist("ModelCompare_thirdLeadingJetInvisible");


    ANA_CHECK(this->book(TH1D(
        "ModelCompare_leadingJetLepton", ";Lepton fraction of leading truth jet (%);Number of events",
        this->_bins, 0.0, 100.0    //x bins, min x, max x
    )));
    this->_cutLeadingJetLeptonPlot = this->hist("ModelCompare_leadingJetLepton");

    ANA_CHECK(this->book(TH1D(
        "ModelCompare_subLeadingJetLepton", ";Lepton fraction of subleading truth jet (%);Number of events",
        this->_bins, 0.0, 100.0    //x bins, min x, max x
    )));
    this->_cutSubLeadingJetLeptonPlot = this->hist("ModelCompare_subLeadingJetLepton");

    ANA_CHECK(this->book(TH1D(
        "ModelCompare_thirdLeadingJetLepton", ";Lepton fraction of third truth jet (%);Number of events",
        this->_bins, 0.0, 100.0    //x bins, min x, max x
    )));
    this->_cutThirdLeadingJetLeptonPlot = this->hist("ModelCompare_thirdLeadingJetLepton");


    ANA_CHECK(this->book(TH1D(
        "RecoTruthEfficiency_response", ";Response (Reconstruction jet #it{p_{T}} / Truth jet #it{p_{T}});Number of events",
        this->_bins, 0.0, this->_maxResponse    //x bins, min x, max x
    )));
    this->_responsePlot = this->hist("RecoTruthEfficiency_response");

    ANA_CHECK(this->book(TH1D(
        "RecoTruthEfficiency_leadingEfficiency", ";#it{#Delta R};Number of events",
        this->_deltaRBins, 0.0, this->_deltaRMax    //x bins, min x, max x
    )));
    this->_leadingEfficiencyPlot = this->hist("RecoTruthEfficiency_leadingEfficiency");

    ANA_CHECK(this->book(TH1D(
        "RecoTruthEfficiency_subLeadingEfficiency", ";#it{#Delta R};Number of events",
        this->_deltaRBins, 0.0, this->_deltaRMax    //x bins, min x, max x
    )));
    this->_subLeadingEfficiencyPlot = this->hist("RecoTruthEfficiency_subLeadingEfficiency");

    ANA_CHECK(this->book(TH1D(
        "RecoTruthEfficiency_thirdLeadingEfficiency", ";#it{#Delta R};Number of events",
        this->_deltaRBins, 0.0, this->_deltaRMax    //x bins, min x, max x
    )));
    this->_thirdLeadingEfficiencyPlot = this->hist("RecoTruthEfficiency_thirdLeadingEfficiency");

    return StatusCode::SUCCESS;
}


StatusCode DarkJetStudy::execute(){
    this->_numEvents++;
    if(this->_numEvents % 100 == 0){
        std::cout << "Event " << this->_numEvents << std::endl;
    }

    const xAOD::TruthParticleContainer *particles = nullptr;
    const xAOD::JetContainer *xAODRecoJets = nullptr;
    std::vector<fastjet::PseudoJet> recoJets;
    const xAOD::EventInfo *eventInfo = nullptr;
    ANA_CHECK(this->evtStore()->retrieve(particles, "TruthParticles"));
    std::cout.setstate(std::ios_base::failbit);
    if(this->evtStore()->retrieve(xAODRecoJets, "AntiKt10RCEMPFlowJets") == StatusCode::SUCCESS){
        std::cout.clear();
        //If the reconstruction jets are directly available, convert them to fastjet jets so that the type is always the same
        for(const xAOD::Jet *jet: *xAODRecoJets){
            recoJets.push_back(fastjet::PseudoJet(jet->px(), jet->py(), jet->pz(), jet->e()));
        }
    }
    else if(this->evtStore()->retrieve(xAODRecoJets, "AntiKt4EMPFlowJets") == StatusCode::SUCCESS){
        std::cout.clear();
        this->_jetRadiusTimes10 = 4;
        for(const xAOD::Jet *jet: *xAODRecoJets){
            recoJets.push_back(fastjet::PseudoJet(jet->px(), jet->py(), jet->pz(), jet->e()));
        }
    }
    else{
        std::cout.clear();
        //If the reconstruction jets aren't directly available, build them from reconstruction particles
        const xAOD::FlowElementContainer *chargedParticles = nullptr, *neutralParticles = nullptr;
        ANA_CHECK(this->evtStore()->retrieve(chargedParticles, "JetETMissChargedParticleFlowObjects"));
        ANA_CHECK(this->evtStore()->retrieve(neutralParticles, "JetETMissNeutralParticleFlowObjects"));
        std::vector<fastjet::PseudoJet> recoJetParticles;
        for(const xAOD::FlowElementContainer *container: {chargedParticles, neutralParticles}){
            for(const xAOD::FlowElement *particle: *container){
                recoJetParticles.push_back(fastjet::PseudoJet(particle->p4().X(), particle->p4().Y(), particle->p4().Z(), particle->e()));
            }
        }
        const fastjet::ClusterSequence recoClustSeqAntikt(recoJetParticles, fastjet::JetDefinition(fastjet::antikt_algorithm, 1.0, fastjet::E_scheme, fastjet::Best));
        recoJets = sorted_by_pt(recoClustSeqAntikt.inclusive_jets(5000.0));
    }
    ANA_CHECK(this->evtStore()->retrieve(eventInfo, "EventInfo"));

    std::vector<fastjet::PseudoJet> jetParticles;
    for(const xAOD::TruthParticle *particle: *particles){
        if(particle->status() != 1){    //If the particle is unstable
            continue;
        }
        if(particle->barcode() >= 200000){    //If the particle was created through interactions with the detector
            continue;
        }

        jetParticles.push_back(fastjet::PseudoJet(particle->px(), particle->py(), particle->pz(), particle->e()));
        jetParticles.back().set_user_index(particle->index());
    }

    //Manual jet building
    std::vector<Jet> jets;
    const fastjet::ClusterSequence clustSeqAntikt(jetParticles, fastjet::JetDefinition(fastjet::antikt_algorithm, this->_jetRadiusTimes10 / 10.0, fastjet::E_scheme, fastjet::Best));
    const std::vector<fastjet::PseudoJet> pseudoJets = sorted_by_pt(clustSeqAntikt.inclusive_jets(5000.0));
    for(const fastjet::PseudoJet &jet: pseudoJets){
        jets.push_back(Jet(jet, particles));
    }

    //Efficiency and response
    for(const Jet &truthJet: jets){
        double deltaR = 1e6;    //Start with something that's guaranteed to be much larger than the actual deltaR
        const fastjet::PseudoJet *recoJet = nullptr;
        for(const fastjet::PseudoJet &newJet: recoJets){
            const double deltaY = truthJet.rapidity() - newJet.rapidity();
            double deltaPhi = truthJet.phi() - newJet.phi();
            if(deltaPhi < -M_PI) deltaPhi += 2 * M_PI;
            else if(deltaPhi > M_PI) deltaPhi -= 2 * M_PI;
            const double newDeltaR = std::sqrt(deltaY * deltaY + deltaPhi * deltaPhi);
            if(newDeltaR < deltaR){
                deltaR = newDeltaR;
                recoJet = &newJet;
            }
        }

        //Efficiency
        for(int i = deltaR * this->_deltaRBins / this->_deltaRMax; i < this->_deltaRBins + 1; i++){
            this->_efficiencyData[i]++;
        }
        if(jets.size() > 0 && truthJet == jets[0]){
            this->_leadingEfficiencyPlot->Fill(deltaR);
        }
        else if(jets.size() > 1 && truthJet == jets[1]){
            this->_subLeadingEfficiencyPlot->Fill(deltaR);
        }
        else if(jets.size() > 2 && truthJet == jets[2]){
            this->_thirdLeadingEfficiencyPlot->Fill(deltaR);
            break;
        }

        //Response
        if(deltaR <= 0.3){
            const double response = recoJet->pt() / truthJet.pt();
            this->_responsePlot->Fill(response);
            this->_responseSum += response;
            this->_numberOfEventsWithResponse++;
            const double invisibility = pTInvisibility(truthJet);
            const double darkness = pTDarkness(truthJet);
            this->_responseSumByInvisibility[invisibility * this->_bins] += response;
            this->_numberOfEventsByInvisibility[invisibility * this->_bins]++;
            this->_responseSumByDarkness[darkness * this->_bins] += response;
            this->_numberOfEventsByDarkness[darkness * this->_bins]++;
        }
    }

    //Jet pT and invariant mass
    if(jets.size() > 0) this->_leadingJetPTPlot->Fill(jets[0].pT() / 1000);
    if(jets.size() > 1) this->_subLeadingJetPTPlot->Fill(jets[1].pT() / 1000);
    if(jets.size() > 2) this->_thirdLeadingJetPTPlot->Fill(jets[2].pT() / 1000);
    if(jets.size() > 1) this->_dijetInvariantMassPlot->Fill(invariantMass(jets[0].momentum() + jets[1].momentum()) / 1000);

    //Invisibility and darkness
    if(jets.size() > 0) this->_leadingJetInvisiblePlot->Fill(pTInvisibility(jets[0]) * 100.0);
    if(jets.size() > 1) this->_subLeadingJetInvisiblePlot->Fill(pTInvisibility(jets[1]) * 100.0);
    if(jets.size() > 2) this->_thirdLeadingJetInvisiblePlot->Fill(pTInvisibility(jets[2]) * 100.0);
    if(jets.size() > 0) this->_leadingJetDarknessPlot->Fill(pTDarkness(jets[0]) * 100.0);
    if(jets.size() > 1) this->_subLeadingJetDarknessPlot->Fill(pTDarkness(jets[1]) * 100.0);
    if(jets.size() > 2) this->_thirdLeadingJetDarknessPlot->Fill(pTDarkness(jets[2]) * 100.0);

    //Jet multiplicity
    int jetMultiplicity = 0, darkJetMultiplicity20 = 0, darkJetMultiplicity50 = 0, darkJetMultiplicity80 = 0;
    for(const Jet &jet: jets){
        if(jet.pT() < 100000){    //pT cut: 100 GeV
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

    //Cut with >=2 dark jets, pT > 100 GeV, darkness >= 80%
    if(darkJetMultiplicity80 >= 2){
        const Jet *leadingTruthJet = nullptr, *subLeadingTruthJet = nullptr, *thirdLeadingTruthJet = nullptr;
        for(const Jet &jet: jets){
            if(jet.pT() < 100000 || pTDarkness(jet) < 0.8){
                continue;
            }
            if(leadingTruthJet == nullptr) leadingTruthJet = &jet;
            else if(subLeadingTruthJet == nullptr) subLeadingTruthJet = &jet;
            else if(thirdLeadingTruthJet == nullptr){
                thirdLeadingTruthJet = &jet;
                break;
            }
        }

        //pT and invariant mass
        this->_cutLeadingJetPTPlot->Fill(leadingTruthJet->pT() / 1000);
        this->_cutSubLeadingJetPTPlot->Fill(subLeadingTruthJet->pT() / 1000);
        if(thirdLeadingTruthJet != nullptr) this->_cutThirdLeadingJetPTPlot->Fill(thirdLeadingTruthJet->pT() / 1000);
        this->_cutDijetInvariantMassPlot->Fill(invariantMass(leadingTruthJet->momentum() + subLeadingTruthJet->momentum()) / 1000);

        //Invisibility and lepton fraction
        this->_cutLeadingJetInvisiblePlot->Fill(pTInvisibility(*leadingTruthJet) * 100.0);
        this->_cutSubLeadingJetInvisiblePlot->Fill(pTInvisibility(*subLeadingTruthJet) * 100.0);
        if(thirdLeadingTruthJet != nullptr) this->_cutThirdLeadingJetInvisiblePlot->Fill(pTInvisibility(*thirdLeadingTruthJet) * 100.0);
        this->_cutLeadingJetLeptonPlot->Fill(pTLeptonFraction(*leadingTruthJet) * 100.0);
        this->_cutSubLeadingJetLeptonPlot->Fill(pTLeptonFraction(*subLeadingTruthJet) * 100.0);
        if(thirdLeadingTruthJet != nullptr) this->_cutThirdLeadingJetLeptonPlot->Fill(pTLeptonFraction(*thirdLeadingTruthJet) * 100.0);
    }

    return StatusCode::SUCCESS;
}


StatusCode DarkJetStudy::finalize(){
    //Plot the response by invisibility and darkness
    ANA_CHECK(this->book(TH1D(
        "RecoTruthEfficiency_responseByInvisibility", ";Invisibility of truth jet (%);Average response",
        this->_bins, 0.0, 100.0    //x bins, min x, max x
    )));
    TH1 *responseByInvisibilityPlot = this->hist("RecoTruthEfficiency_responseByInvisibility");
    ANA_CHECK(this->book(TH1D(
        "RecoTruthEfficiency_responseByDarkness", ";Darkness of truth jet (%);Average response",
        this->_bins, 0.0, 100.0    //x bins, min x, max x
    )));
    TH1 *responseByDarknessPlot = this->hist("RecoTruthEfficiency_responseByDarkness");
    for(int i = 0; i < this->_bins; i++){
        if(this->_numberOfEventsByInvisibility[i] != 0){
            responseByInvisibilityPlot->AddBinContent(i + 1, this->_responseSumByInvisibility[i] / this->_numberOfEventsByInvisibility[i]);
        }
        if(this->_numberOfEventsByDarkness[i] != 0){
            responseByDarknessPlot->AddBinContent(i + 1, this->_responseSumByDarkness[i] / this->_numberOfEventsByDarkness[i]);
        }
    }

    std::cout << "Average response: " << (this->_responseSum / this->_numberOfEventsWithResponse) << std::endl;

    //Plot the jet multiplicity
    const int maxMultiplicity = std::max_element(this->_jetMultiplicityData.begin(), this->_jetMultiplicityData.end(), [](const std::pair<int, int> &a, const std::pair<int, int> &b){return a.first < b.first;})->first;
    ANA_CHECK(this->book(TH1D(
        "RecoTruthEfficiency_jetMultiplicity", ";Jet multiplicity;Number of events",
        maxMultiplicity * 2 + 2, 0.0, maxMultiplicity + 1    //x bins, min x, max x
    )));
    TH1 *jetMultiplicityPlot = this->hist("RecoTruthEfficiency_jetMultiplicity");
    ANA_CHECK(this->book(TH1D(
        "RecoTruthEfficiency_darkJetMultiplicity20", ";Jet multiplicity;Number of events",
        maxMultiplicity * 2 + 2, 0.0, maxMultiplicity + 1    //x bins, min x, max x
    )));
    TH1 *darkJetMultiplicity20Plot = this->hist("RecoTruthEfficiency_darkJetMultiplicity20");
    ANA_CHECK(this->book(TH1D(
        "RecoTruthEfficiency_darkJetMultiplicity50", ";Jet multiplicity;Number of events",
        maxMultiplicity * 2 + 2, 0.0, maxMultiplicity + 1    //x bins, min x, max x
    )));
    TH1 *darkJetMultiplicity50Plot = this->hist("RecoTruthEfficiency_darkJetMultiplicity50");
    ANA_CHECK(this->book(TH1D(
        "RecoTruthEfficiency_darkJetMultiplicity80", ";Jet multiplicity;Number of events",
        maxMultiplicity * 2 + 2, 0.0, maxMultiplicity + 1    //x bins, min x, max x
    )));
    TH1 *darkJetMultiplicity80Plot = this->hist("RecoTruthEfficiency_darkJetMultiplicity80");

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
