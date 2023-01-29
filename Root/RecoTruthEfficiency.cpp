#include <TSystem.h>
#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TMarker.h>
#include <iostream>
#include "../Headers/Legend.hpp"

int main(int argc, char **argv){
    TString infile, outfile;

    for(int argi = 1; argi < argc; argi++){
        TString arg = argv[argi];
        if(arg.Contains(".root") || ((arg == "-i" || arg == "--input") && argi++)){
            arg = argv[argi];
            if(!gSystem->AccessPathName(arg.Data())){
                infile = arg;
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
            std::cout << "Usage: RecoTruthEfficiency [options] infile" << std::endl
                << "Options:" << std::endl
                << "  -h, --help:          Show this help text and exit" << std::endl
                << "  -i, --input <path>:  Opens the specified root file as input file" << std::endl
                << "  -o, --output <path>: Specifies which file the PDF output should be written to, defaults to the same as the input file except with .pdf instead of .root" << std::endl;
            return 0;
        }
        else{
            std::cerr << "Cannot interpret argument: " << argv[argi] << std::endl;
            return EXIT_FAILURE;
        }
    }
    if(infile == ""){
        std::cerr << "No input file specified. Needs to be given as arugment." << std::endl;
        return EXIT_FAILURE;
    }
    if(outfile == ""){
        outfile = infile;
        outfile.ReplaceAll(".root", ".pdf");
    }

    //Get the histograms
    TFile *file = TFile::Open(infile);
    std::vector<std::vector<TH1D*>> plotGroups;
    const std::vector<int> colors = {EColor::kOrange - 3, EColor::kGreen + 2, EColor::kMagenta + 2};
    for(TObject *o: *file->GetListOfKeys()){
        const TString name = o->GetName();
        if(name.BeginsWith("EventLoop_") || !name.BeginsWith("RecoTruthEfficiency_")){
            continue;
        }
        TH1D *histogram = dynamic_cast<TH1D*>(file->Get(name));
        if(histogram != nullptr){
            histogram->SetStats(0);
            const TString xTitle = histogram->GetXaxis()->GetTitle();
            const TString yTitle = histogram->GetYaxis()->GetTitle();
            if(plotGroups.size() == 0 || xTitle != plotGroups.back().back()->GetXaxis()->GetTitle() || yTitle != plotGroups.back().back()->GetYaxis()->GetTitle()){
                plotGroups.push_back({histogram});
            }
            else{
                const std::size_t colorIndex = plotGroups.back().size() - 1;
                if(colorIndex < colors.size()){
                    histogram->SetLineColor(colors[colorIndex]);
                }
                else if(colorIndex == colors.size()){
                    std::cout << "Warning: more colors needed for plot " << histogram->GetXaxis()->GetTitle() << std::endl;
                }
                plotGroups.back().push_back(histogram);
            }
        }
    }

    //Plot the histograms
    TCanvas canvas;
    canvas.Print(outfile + "[");
    for(std::vector<TH1D*> &plotGroup: plotGroups){
        std::sort(plotGroup.begin(), plotGroup.end(), [](const TH1D *a, const TH1D *b){
            return a->GetMaximum() > b->GetMaximum();
        });
        for(std::size_t i = 0; i < plotGroup.size(); i++){
            TH1D *histogram = plotGroup[i];
            if(i == 0){
                histogram->Draw("hist");
            }
            else{
                histogram->Draw("histsame");
            }
        }
        const TString model = modelName(infile);
        drawTitle(plotGroup[0], "Anti-#it{k_{t}}, #it{R} = " + TString(model.BeginsWith("EJ") ? "0.4" : "1.0") + ", with invisibles" + TString(plotGroup.size() > 3 ? ", p_{T} cut = 100 GeV" : "") + "\nModel " + model + ", Z' #rightarrow q_{D} #bar{q}_{D}");
        if(plotGroup.size() > 1){
            const auto legend = drawLegend(plotGroup[0], colors, plotGroup.size() == 3 ? std::vector<TString>{"Leading jet", "Subleading jet", "Third leading jet"} : std::vector<TString>{"No darkness cut", "20% Darkness cut", "50% Darkness cut", "80% Darkness cut"});
            canvas.Print(outfile);    //Repeat this both in if and else blocks so that the legend doesn't go out of scope
        }
        else{
            canvas.Print(outfile);
        }
    }
    canvas.Print(outfile + "]");

    return 0;
}