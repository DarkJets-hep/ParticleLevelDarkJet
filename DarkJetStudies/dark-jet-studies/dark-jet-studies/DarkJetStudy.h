#pragma once

#include <AnaAlgorithm/AnaAlgorithm.h>
#include <TH1D.h>
#include <vector>
#include <regex>
#include <memory>

class DarkJetStudy: public EL::AnaAlgorithm{
public:
    DarkJetStudy(const std::string& name, ISvcLocator* pSvcLocator);

    virtual StatusCode initialize() override;
    virtual StatusCode execute() override;
    virtual StatusCode finalize() override;

    static double jetRadius;
    static std::regex darkRegex;

private:
    void plotHistogram(TH1D &histogram);
    void plotHistograms(const std::vector<TH1D*> &histograms);

    int _numEvents;

    std::map<int, int> _jetMultiplicityData;    //Contains the number of jets with pT > 30GeV as key, and the number of events with that key as value
    std::map<int, int> _darkJetMultiplicity20Data, _darkJetMultiplicity50Data, _darkJetMultiplicity80Data;    //Same as above but only counts jets with darkness > 20% / 50% / 80%

    static constexpr int _bins = 50;
    static constexpr double _maxPT = 3e3;
    TH1 *_leadingJetPTPlot, *_subLeadingJetPTPlot, *_thirdLeadingJetPTPlot, *_dijetInvariantMassPlot;
    TH1 *_leadingJetInvisiblePlot, *_subLeadingJetInvisiblePlot, *_thirdLeadingJetInvisiblePlot, *_leadingJetDarknessPlot, *_subLeadingJetDarknessPlot, *_thirdLeadingJetDarknessPlot;
};
