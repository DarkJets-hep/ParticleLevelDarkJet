#pragma once

#include <xAODTruth/TruthParticleContainer.h>
#include <cmath>
#include <vector>

inline double thrust(const xAOD::TruthParticleContainer *particles){
    if(particles->size() < 3){
        return 1.0;
    }

    double momentumSum = 0.0;
    std::vector<TVector3> momentums;
    for(const xAOD::TruthParticle *particle: *particles){
        TVector3 momentum = particle->p4().Vect();
        momentumSum += momentum.Mag();
        momentums.push_back(momentum);
    }
    std::sort(momentums.begin(), momentums.end(), [](const TVector3 &a, const TVector3 &b){
        return a.Mag2() > b.Mag2();
    });
    std::vector<double> ts;
    for(int i = 0; i < 4; i++){
        TVector3 n;
        int sign = i;
        for(const TVector3 &p: momentums){
            n += sign % 2 ? p : -p;
            sign /= 2;
        }
        n = n.Unit();

        double diff = 999.0;
        while(diff > 1e-5){
            TVector3 newN;
            for(const TVector3 &p: momentums){
                newN += n.Dot(p) > 0 ? p : -p;
            }
            newN = newN.Unit();
            diff = (n - newN).Mag();
            n = newN;
        }

        double t = 0.0;
        for(const TVector3 &p: momentums){
            t += std::fabs(n.Dot(p));
        }
        ts.push_back(t);
    }
    return *std::max_element(ts.begin(), ts.end()) / momentumSum;
}
