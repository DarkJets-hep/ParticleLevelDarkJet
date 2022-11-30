#include <TSystem.h>
#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <iostream>

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
            std::cout << "Usage: root2pdf [options] infile" << std::endl
                << "Options:" << std::endl
                << "  -h, --help:            Show this help text and exit" << std::endl
                << "  -i, --input <integer>: Opens the specified root file as input file" << std::endl
                << "  -o, --output <path>:   Specifies which file the PDF output should be written to, defaults to the same as the input file except with .pdf instead of .root" << std::endl;
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

    TFile *file = TFile::Open(infile);
    TCanvas canvas;
    canvas.Print(outfile + "[");
    TString previousTitle = "{";    //Choose something that's not valid TLatex so that it won't be in the title
    const std::vector<int> colors = {EColor::kOrange - 3, EColor::kGreen + 2, EColor::kMagenta + 2};
    int colorIndex = 0;
    for(TObject *o: *file->GetListOfKeys()){
        if(TString(o->GetName()).Contains("EventLoop_")){
            continue;
        }
        TH1D *histogram = dynamic_cast<TH1D*>(file->Get(o->GetName()));
        if(histogram != nullptr){
            histogram->SetStats(0);
            if(previousTitle != "{" && histogram->GetXaxis()->GetTitle() == previousTitle){
                histogram->SetLineColor(colors[colorIndex]);
                colorIndex++;
                histogram->Draw("histsame");
            }
            else{
                if(previousTitle != "{"){
                    canvas.Print(outfile);
                }
                histogram->Draw("hist");
                colorIndex = 0;
                previousTitle = histogram->GetXaxis()->GetTitle();
            }
        }
    }
    canvas.Print(outfile);
    canvas.Print(outfile + "]");

    return 0;
}