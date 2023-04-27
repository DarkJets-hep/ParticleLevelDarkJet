#include <TSystem.h>
#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TMarker.h>
#include <iostream>
#include "Legend.hpp"

int main(int argc, char **argv){
    std::vector<TString> infiles;
    TString outfile = "ModelCompare.pdf";

    for(int argi = 1; argi < argc; argi++){
        TString arg = argv[argi];
        if(arg.Contains(".root")){
            arg = argv[argi];
            if(!gSystem->AccessPathName(arg.Data())){
                infiles.push_back(arg);
            }
            else{
                std::cerr << "File " << argv[argi] << " does not exist." << std::endl;
                return EXIT_FAILURE;
            }
        }
        else if(arg == "-o" || arg == "--output"){
            outfile = argv[++argi];
        }
        else if(arg == "-h" || arg == "--help"){
            std::cout << "Usage: ModelCompare [options] infile1 [infile2 [infile3 [...]]]" << std::endl
                << "Options:" << std::endl
                << "  -h, --help:          Show this help text and exit" << std::endl
                << "  -o, --output <path>: Specifies which file the PDF output should be written to, defaults to ModelCompare.pdf" << std::endl;
            return 0;
        }
        else{
            std::cerr << "Cannot interpret argument: " << argv[argi] << std::endl;
            return EXIT_FAILURE;
        }
    }
    if(infiles.size() == 0){
        std::cerr << "No input file specified. Needs to be given as arugment." << std::endl;
        return EXIT_FAILURE;
    }

    //Get the histograms
    std::map<TString, std::vector<std::pair<TString, TH1D*>>> plotGroups;
    const std::vector<int> colors = {EColor::kOrange - 3, EColor::kGreen + 2, EColor::kMagenta + 2, EColor::kRed + 1};
    for(const TString &infile: infiles){
        TFile *file = TFile::Open(infile);
        for(TObject *o: *file->GetListOfKeys()){
            const TString name = o->GetName();
            if(name.BeginsWith("EventLoop_") || !name.BeginsWith("ModelCompare_")){
                continue;
            }
            TH1D *histogram = dynamic_cast<TH1D*>(file->Get(name));
            if(histogram != nullptr){
                histogram->SetStats(0);
                const std::size_t colorIndex = plotGroups[name].size() - 1;
                if(colorIndex >= 0){
                    if(colorIndex < colors.size()){
                        histogram->SetLineColor(colors[colorIndex]);
                    }
                    else if(colorIndex == colors.size()){
                        std::cout << "Warning: more colors needed for plot " << histogram->GetXaxis()->GetTitle() << std::endl;
                    }
                }
                plotGroups[name].push_back({modelName(infile), histogram});
            }
        }
    }

    //Plot the histograms
    TCanvas canvas;
    canvas.Print(outfile + "[");
    for(const auto &p: plotGroups){
        std::vector<std::pair<TString, TH1D*>> plotGroup = p.second;
        const TString name = plotGroup[0].second->GetName();
        const bool logscale = name.EndsWith("Invisible") || name.EndsWith("Lepton");
        canvas.SetLogy(logscale);
        std::sort(plotGroup.begin(), plotGroup.end(), [](const std::pair<TString, TH1D*> &a, const std::pair<TString, TH1D*> &b){
            return a.second->GetMaximum() > b.second->GetMaximum();
        });
        for(std::size_t i = 0; i < plotGroup.size(); i++){
            TH1D *histogram = plotGroup[i].second;
            if(i == 0){
                histogram->Draw("hist");
            }
            else{
                histogram->Draw("histsame");
            }
        }
        TString jetRadius, process;
        std::vector<TString> models;
        for(const auto &s: p.second){
            const TString model = s.first;
            if(model.BeginsWith("EJ")){
                if(jetRadius == "1.0"){
                    std::cout << "Warning: mixing jet radiuses: EJ models have jet radius 0.4, others have jet radius 1.0." << std::endl;
                }
                jetRadius = "0.4";
                process = "X' #bar{X}' #rightarrow q #bar{q}_{D} #bar{q} q_{D}";
            }
            else{
                if(jetRadius == "0.4"){
                    std::cout << "Warning: mixing jet radiuses: EJ models have jet radius 0.4, others have jet radius 1.0." << std::endl;
                }
                jetRadius = "1.0";
                process = "Z' #rightarrow q_{D} #bar{q}_{D}";
            }
            models.push_back("Model " + model);
        }
        drawTitle(plotGroup[0].second, "Anti-#it{k_{t}}, #it{R} = " + jetRadius + ", with invisibles, p_{T} cut = 100 GeV" + (name.BeginsWith("ModelCompare_Cut_") ? ", Darkness cut = 80%" : "") + "\n" + process, logscale);
        const auto legend = drawLegend(plotGroup[0].second, colors, models, logscale);
        canvas.Print(outfile);
    }
    canvas.Print(outfile + "]");

    return 0;
}
