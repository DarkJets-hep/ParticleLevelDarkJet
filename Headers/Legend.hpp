#pragma once

#include <TPave.h>
#include <TLine.h>
#include <TEllipse.h>
#include <TLatex.h>
#include <TH1D.h>
#include <TH2D.h>
#include <vector>
#include <memory>
#include <regex>

#define TH1D_PAGE_COORDINATES2(x, y) (histogram->GetBinLowEdge(histogram->GetNbinsX()) + histogram->GetBinWidth(histogram->GetNbinsX())) * (x), histogram->GetMaximum() * 1.07 * (y)
#define TH1D_PAGE_COORDINATES4(x1, y1, x2, y2) TH1D_PAGE_COORDINATES2(x1, y1), TH1D_PAGE_COORDINATES2(x2, y2)
#define TH2D_PAGE_COORDINATES2(x, y) ((histogram->GetXaxis()->GetXmax() - histogram->GetXaxis()->GetXmin()) * (x) + histogram->GetXaxis()->GetXmin()), ((histogram->GetYaxis()->GetXmax() - histogram->GetYaxis()->GetXmin()) * (y) * 1.02 + histogram->GetYaxis()->GetXmin())
#define TH2D_PAGE_COORDINATES4(x1, y1, x2, y2) TH2D_PAGE_COORDINATES2(x1, y1), TH2D_PAGE_COORDINATES2(x2, y2)

inline TString modelName(const TString &fileName){
    const std::string stdFileName = fileName.Data();
    std::smatch modelNameMatches;
    if(std::regex_search(stdFileName, modelNameMatches, std::regex("Model([A-Z0-9]+)", std::regex_constants::ECMAScript | std::regex_constants::icase))){
        return modelNameMatches[1].str();
    }
    return "?";
}

inline void drawLatex(TLatex &latex, const TH1D *histogram, double x, double y, const TString &str){
    latex.DrawLatex(TH1D_PAGE_COORDINATES2(x, y), str);
}

inline void drawLatex(TLatex &latex, const TH2D *histogram, double x, double y, const TString &str){
    latex.DrawLatex(TH2D_PAGE_COORDINATES2(x, y), str);
}

template<typename TH1D_or_TH2D>
static void drawTitle(const TH1D_or_TH2D *histogram, const TString &title){
    TLatex latex;
    latex.SetTextSize(0.025);
    std::istringstream stream(title.Data());
    std::string line;
    std::vector<TString> lines;
    while(std::getline(stream, line)){
        lines.push_back(line);
    }
    for(std::size_t i = 0; i < lines.size(); i++){
        drawLatex(latex, histogram, 0, 1 + (lines.size() - i - 1) * 0.05, "#bf{" + lines[i] + "}");
    }
}

//Always assign the return value of this function to a variable even if it isn't used, otherwise the objects will be deleted from memory and won't be drawn
static std::vector<std::unique_ptr<TObject>> drawLegend(const TH1D *histogram, const std::vector<int> &colors, const std::vector<TString> &legends){
    std::vector<std::unique_ptr<TObject>> objects;

    TLatex latex;
    latex.SetTextSize(0.025);
    latex.SetTextAlign(12);    //Center vertically

    const double xOffset = (histogram->GetMaximumBin() > histogram->GetNbinsX() * 2 / 3) ? 0.05 : 0.65;
    const double yOffset = -0.05;

    std::unique_ptr<TPave> box(new TPave(TH1D_PAGE_COORDINATES4(xOffset, yOffset + 0.95 - legends.size() * 0.05, xOffset + 0.3, yOffset + 1)));
    box->Draw();
    objects.push_back(std::move(box));

    for(std::size_t i = 0; i < colors.size() + 1 && i < legends.size(); i++){
        std::unique_ptr<TLine> line(new TLine(TH1D_PAGE_COORDINATES4(xOffset + 0.02, yOffset + 0.95 - i * 0.05, xOffset + 0.1, yOffset + 0.95 - i * 0.05)));
        line->SetLineColor(i == 0 ? TH1D().GetLineColor() : colors[i - 1]);
        line->Draw();
        objects.push_back(std::move(line));
        drawLatex(latex, histogram, xOffset + 0.12, yOffset + 0.95 - i * 0.05, "#bf{" + legends[i] + "}");
    }

    return objects;
}

//Always assign the return value of this function to a variable even if it isn't used, otherwise the objects will be deleted from memory and won't be drawn
static std::vector<std::unique_ptr<TObject>> drawLegend(const TH2D *histogram, const std::vector<int> &colors, const std::vector<TString> &legends){
    std::vector<std::unique_ptr<TObject>> objects;

    TLatex latex;
    latex.SetTextSize(0.025);
    latex.SetTextAlign(12);    //Center vertically

    std::unique_ptr<TPave> box(new TPave(TH2D_PAGE_COORDINATES4(0.8, 0.65 - legends.size() * 0.05, 1.1, 1)));
    box->Draw();
    objects.push_back(std::move(box));

    for(std::size_t i = 0; i < colors.size() + 1 && i < legends.size(); i++){
        TMarker marker;
        marker.SetMarkerStyle(20);
        marker.SetMarkerColor(colors[i]);
        marker.SetMarkerSize(0.3);
        marker.DrawMarker(TH2D_PAGE_COORDINATES2(0.83, 0.95 - i * 0.05));
        drawLatex(latex, histogram, 0.85, 0.95 - i * 0.05, "#bf{" + legends[i] + "}");
    }

    TMarker resonanceParticle;
    resonanceParticle.SetMarkerStyle(29);
    resonanceParticle.DrawMarker(TH2D_PAGE_COORDINATES2(0.82, 0.95 - legends.size() * 0.05));
    TMarker parton;
    parton.SetMarkerStyle(20);
    parton.DrawMarker(TH2D_PAGE_COORDINATES2(0.84, 0.95 - legends.size() * 0.05));
    drawLatex(latex, histogram, 0.85, 0.95 - legends.size() * 0.05, "#bf{Parton level particles}");

    drawLatex(latex, histogram, 0.82, 0.9 - legends.size() * 0.05, "Truth level particles");

    TMarker smParticle;
    smParticle.SetMarkerStyle(20);
    smParticle.SetMarkerSize(0.3);
    smParticle.DrawMarker(TH2D_PAGE_COORDINATES2(0.83, 0.85 - legends.size() * 0.05));
    drawLatex(latex, histogram, 0.85, 0.85 - legends.size() * 0.05, "#bf{Visible SM particle}");

    TMarker neutrino;
    neutrino.SetMarkerStyle(24);
    neutrino.SetMarkerSize(0.3);
    neutrino.DrawMarker(TH2D_PAGE_COORDINATES2(0.83, 0.8 - legends.size() * 0.05));
    drawLatex(latex, histogram, 0.85, 0.8 - legends.size() * 0.05, "#bf{Invisible SM particle}");

    TMarker darkMatter;
    darkMatter.SetMarkerStyle(25);
    darkMatter.SetMarkerSize(0.3);
    darkMatter.DrawMarker(TH2D_PAGE_COORDINATES2(0.83, 0.75 - legends.size() * 0.05));
    drawLatex(latex, histogram, 0.85, 0.75 - legends.size() * 0.05, "#bf{Dark particle}");

    std::unique_ptr<TEllipse> jet(new TEllipse(TH2D_PAGE_COORDINATES2(0.83, 0.7 - legends.size() * 0.05), 0.1, 0.07));
    jet->SetFillColor(EColor::kGray);
    jet->SetFillStyle(1001);
    jet->SetLineWidth(0);
    jet->Draw("f");
    objects.push_back(std::move(jet));
    drawLatex(latex, histogram, 0.85, 0.7 - legends.size() * 0.05, "#bf{Jet}");

    return objects;
}

#undef TH1D_PAGE_COORDINATES2
#undef TH1D_PAGE_COORDINATES4
#undef TH2D_PAGE_COORDINATES2
#undef TH2D_PAGE_COORDINATES4