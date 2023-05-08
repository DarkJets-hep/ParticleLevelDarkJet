#pragma once

#include <Rivet/Particle.hh>
#include <Rivet/Jet.hh>
#include <TLatex.h>
#include <TMarker.h>
#include <TPolyLine.h>
#include <cmath>
#include <vector>
#include <algorithm>
#include <memory>
#include "../Headers/ParticleName.hpp"

static void plotParticle(const Rivet::Particle &particle, int style, int color = EColor::kBlack, bool showLabel = true, double size = 1.0){
    if(std::abs(particle.rapidity()) > 4){
        return;    //Don't draw particles that don't fit in the plot area
    }
    TMarker marker;
    marker.SetMarkerStyle(style);
    marker.SetMarkerColor(color);
    marker.SetMarkerSize(size);
    marker.DrawMarker(particle.rapidity(), particle.phi());
    if(showLabel){
        TLatex().DrawLatex(particle.rapidity() + 0.1, particle.phi() - 0.1, TString::Format("#color[%d]{#it{%s}}", color, particleNameAsTLatex(particle.pid()).c_str()));
    }
}

//Always assign the return value of this function to a variable even if it isn't used, otherwise the TPolyLine will be deleted from memory and won't be drawn
inline std::vector<std::unique_ptr<TPolyLine>> plotJets(const std::vector<Rivet::Jet> &jets, double jetRadius, std::vector<int> colors){
    std::vector<std::unique_ptr<TPolyLine>> jetPolygons;
    for(unsigned int i = 0; i < jets.size(); i++){
        const Rivet::Jet &jet = jets[i];
        std::vector<double> x[2], y[2];    //Create arrays of two std::vectors. x[0] will be an std::vector with the x-coordinates of the main part of the jet, x[1] will be an std::vector containing the x-coordinates of part wrapped around the phi-axis (it will be empty if the entire jet fits within the plotting area).
        int previousIndex = -1;
        bool previousParticleDrawn = true;
        for(double theta = 0.0; theta < 2 * M_PI; theta += 0.1){
            const double rapidity = jet.rapidity() + std::sin(theta) * jetRadius;
            double phi = jet.phi() + std::cos(theta) * jetRadius;
            const int index = phi > 0 && phi < 2 * M_PI;
            const bool particleDrawn = std::abs(rapidity) <= 4;
            const bool indexChanged = previousIndex != index && previousIndex != -1;
            const bool particleDrawnChanged = previousParticleDrawn != particleDrawn && previousIndex != -1;
            if(phi < 0){
                phi += 2 * M_PI;
            }
            if(phi > 2 * M_PI){
                phi -= 2 * M_PI;
            }
            if(particleDrawnChanged && !indexChanged){
                x[index].push_back(std::copysignf(4.0, rapidity));
                y[index].push_back(phi);
            }
            if(particleDrawn){
                if(indexChanged){
                    x[previousIndex].push_back(rapidity);
                    y[previousIndex].push_back(theta >= M_PI ? 2 * M_PI : 0);
                    x[index].push_back(rapidity);
                    y[index].push_back(theta < M_PI ? 2 * M_PI : 0);
                }
                x[index].push_back(rapidity);
                y[index].push_back(phi);
            }
            else if(indexChanged){
                if(rapidity > 0){
                    x[index].push_back(4);
                    x[previousIndex].push_back(4);
                    y[index].push_back(2 * M_PI);
                    y[previousIndex].push_back(0);
                }
                else{
                    x[index].push_back(-4);
                    x[previousIndex].push_back(-4);
                    y[index].push_back(0);
                    y[previousIndex].push_back(2 * M_PI);
                }
            }
            previousIndex = index;
        }
        for(int index: {0, 1}){
            if(x[index].size() > 0){
                std::unique_ptr<TPolyLine> polygon(new TPolyLine(x[index].size(), x[index].data(), y[index].data()));
                polygon->SetFillColor(colors[i]);
                polygon->SetFillStyle(1001);
                polygon->Draw("f");
                jetPolygons.push_back(std::move(polygon));
            }
        }
    }
    return jetPolygons;
}

inline std::vector<std::unique_ptr<TPolyLine>> plotJets(const std::vector<Rivet::Jet> &jets, double jetRadius, int color = EColor::kGray){
    std::vector<int> colors(jets.size());
    std::fill(colors.begin(), colors.end(), color);
    return plotJets(jets, jetRadius, colors);
}