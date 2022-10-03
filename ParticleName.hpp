#pragma once

#include "Rivet/Tools/ParticleName.hh"
#include <string>

//Use this instead of the built-in Rivet::PID::ParticleNames::particleName, because the built-in one is incomplete
static std::string particleName(int pdgid){
    switch(pdgid){
        case Rivet::PID::ELECTRON: return "ELECTRON";
        case Rivet::PID::POSITRON: return "POSITRON";
        case Rivet::PID::MUON: return "MUON";
        case Rivet::PID::ANTIMUON: return "ANTIMUON";
        case Rivet::PID::TAU: return "TAU";
        case Rivet::PID::ANTITAU: return "ANTITAU";
        case Rivet::PID::NU_E: return "NU_E";
        case Rivet::PID::NU_EBAR: return "NU_EBAR";
        case Rivet::PID::NU_MU: return "NU_MU";
        case Rivet::PID::NU_MUBAR: return "NU_MUBAR";
        case Rivet::PID::NU_TAU: return "NU_TAU";
        case Rivet::PID::NU_TAUBAR: return "NU_TAUBAR";
        case Rivet::PID::PHOTON: return "PHOTON";
        case Rivet::PID::GLUON: return "GLUON";
        case Rivet::PID::WPLUSBOSON: return "WPLUSBOSON";
        case Rivet::PID::WMINUSBOSON: return "WMINUSBOSON";
        case Rivet::PID::ZBOSON: return "ZBOSON";
        case Rivet::PID::HIGGS: return "HIGGS";
        case Rivet::PID::DQUARK: return "DQUARK";
        case Rivet::PID::UQUARK: return "UQUARK";
        case Rivet::PID::SQUARK: return "SQUARK";
        case Rivet::PID::CQUARK: return "CQUARK";
        case Rivet::PID::BQUARK: return "BQUARK";
        case Rivet::PID::TQUARK: return "TQUARK";
        case Rivet::PID::PROTON: return "PROTON";
        case Rivet::PID::ANTIPROTON: return "ANTIPROTON";
        case Rivet::PID::NEUTRON: return "NEUTRON";
        case Rivet::PID::ANTINEUTRON: return "ANTINEUTRON";
        case Rivet::PID::PI0: return "PI0";
        case Rivet::PID::PIPLUS: return "PIPLUS";
        case Rivet::PID::PIMINUS: return "PIMINUS";
        case Rivet::PID::RHO0: return "RHO0";
        case Rivet::PID::RHOPLUS: return "RHOPLUS";
        case Rivet::PID::RHOMINUS: return "RHOMINUS";
        case Rivet::PID::K0L: return "K0L";
        case Rivet::PID::K0S: return "K0S";
        case Rivet::PID::K0: return "K0";
        case Rivet::PID::KPLUS: return "KPLUS";
        case Rivet::PID::KMINUS: return "KMINUS";
        case 313: return "KSTAR0";
        case 323: return "KSTARPLUS";
        case -323: return "KSTARMINUS";
        case Rivet::PID::ETA: return "ETA";
        case Rivet::PID::ETAPRIME: return "ETAPRIME";
        case Rivet::PID::PHI: return "PHI";
        case Rivet::PID::OMEGA: return "OMEGA";
        case Rivet::PID::ETAC: return "ETAC";
        case Rivet::PID::JPSI: return "JPSI";
        case Rivet::PID::PSI2S: return "PSI2S";
        case Rivet::PID::D0: return "D0";
        case Rivet::PID::D0BAR: return "D0BAR";
        case Rivet::PID::DPLUS: return "DPLUS";
        case Rivet::PID::DMINUS: return "DMINUS";
        case Rivet::PID::DSTARPLUS: return "DSTARPLUS";
        case Rivet::PID::DSTARMINUS: return "DSTARMINUS";
        case 423: return "DSTAR0";
        case Rivet::PID::DSPLUS: return "DSPLUS";
        case Rivet::PID::DSMINUS: return "DSMINUS";
        case 433: return "DSSTARPLUS";
        case -433: return "DSSTARMINUS";
        case Rivet::PID::ETAB: return "ETAB";
        case Rivet::PID::UPSILON1S: return "UPSILON1S";
        case Rivet::PID::UPSILON2S: return "UPSILON2S";
        case Rivet::PID::UPSILON3S: return "UPSILON3S";
        case Rivet::PID::UPSILON4S: return "UPSILON4S";
        case Rivet::PID::B0: return "B0";
        case Rivet::PID::B0BAR: return "B0BAR";
        case Rivet::PID::BPLUS: return "BPLUS";
        case Rivet::PID::BMINUS: return "BMINUS";
        case 513: return "BSTAR0";
        case 523: return "BSTARPLUS";
        case -523: return "BSTARMINUS";
        case Rivet::PID::B0S: return "B0S";
        case 533: return "BSTAR0S";
        case Rivet::PID::BCPLUS: return "BCPLUS";
        case Rivet::PID::BCMINUS: return "BCMINUS";
        case 543: return "BCSTARPLUS";
        case -543: return "BCSTARMINUS";
        case Rivet::PID::LAMBDA: return "LAMBDA";
        case Rivet::PID::SIGMA0: return "SIGMA0";
        case Rivet::PID::SIGMAPLUS: return "SIGMAPLUS";
        case Rivet::PID::SIGMAMINUS: return "SIGMAMINUS";
        case 3214: return "SIGMASTAR0";
        case 3224: return "SIGMASTARPLUS";
        case 3114: return "SIGMASTARMINUS";
        case Rivet::PID::SIGMAB: return "SIGMAB";
        case Rivet::PID::SIGMABPLUS: return "SIGMABPLUS";
        case Rivet::PID::SIGMABMINUS: return "SIGMABMINUS";
        case Rivet::PID::LAMBDACPLUS: return "LAMBDACPLUS";
        case Rivet::PID::LAMBDAB: return "LAMBDAB";
        case Rivet::PID::XI0: return "XI0";
        case Rivet::PID::XIMINUS: return "XIMINUS";
        case Rivet::PID::XIPLUS: return "XIPLUS";
        case 3324: return "XISTAR0";
        case 3314: return "XISTARMINUS";
        case -3314: return "XISTARPLUS";
        case Rivet::PID::XI0B: return "XI0B";
        case Rivet::PID::XIBMINUS: return "XIBMINUS";
        case Rivet::PID::XI0C: return "XI0C";
        case Rivet::PID::XICPLUS: return "XICPLUS";
        case Rivet::PID::OMEGAMINUS: return "OMEGAMINUS";
        case Rivet::PID::OMEGAPLUS: return "OMEGAPLUS";
        case Rivet::PID::OMEGABMINUS: return "OMEGABMINUS";
        case Rivet::PID::OMEGA0C: return "OMEGA0C";
        case Rivet::PID::REGGEON: return "REGGEON";
        case Rivet::PID::POMERON: return "POMERON";
        case Rivet::PID::ODDERON: return "ODDERON";
        case Rivet::PID::GRAVITON: return "GRAVITON";
        case Rivet::PID::NEUTRALINO1: return "NEUTRALINO1";
        case Rivet::PID::GRAVITINO: return "GRAVITINO";
        case Rivet::PID::GLUINO: return "GLUINO";
        case Rivet::PID::BPRIME: return "BPRIME";
        case Rivet::PID::TPRIME: return "TPRIME";
        case Rivet::PID::LPRIME: return "LPRIME";
        case Rivet::PID::NUPRIME: return "NUPRIME";
        case Rivet::PID::DEUTERON: return "DEUTERON";
        case Rivet::PID::ALUMINIUM: return "ALUMINIUM";
        case Rivet::PID::COPPER: return "COPPER";
        case Rivet::PID::XENON: return "XENON";
        case Rivet::PID::GOLD: return "GOLD";
        case Rivet::PID::LEAD: return "LEAD";
        case Rivet::PID::URANIUM: return "URANIUM";
        case Rivet::PID::ANY: return "*";
    }

    if(pdgid < 0){
        const std::string antiparticle = particleName(-pdgid);
        if(antiparticle.rfind("Unknown particle", 0) != 0){
            return antiparticle + "_BAR";
        }
    }

    return "Unknown particle " + std::to_string(pdgid);
}

static std::string particleNotation(int pdgid){
    switch(pdgid){
        case Rivet::PID::ELECTRON: return "e\u207b";
        case Rivet::PID::POSITRON: return "e\u207a";
        case Rivet::PID::MUON: return "\u03bc\u207b";
        case Rivet::PID::ANTIMUON: return "\u03bc\u207a";
        case Rivet::PID::TAU: return "\u03c4\u207b";
        case Rivet::PID::ANTITAU: return "\u03c4\u207a";
        case Rivet::PID::NU_E: return "\u03bd\u2091";
        case Rivet::PID::NU_EBAR: return "\u03bd\u0305\u2091";
        case Rivet::PID::NU_MU: return "\u03bd\u03bc";
        case Rivet::PID::NU_MUBAR: return "\u03bd\u0305\u03bc";
        case Rivet::PID::NU_TAU: return "\u03bd\u207b";
        case Rivet::PID::NU_TAUBAR: return "\u03bd\u0305\u207b";
        case Rivet::PID::PHOTON: return "\u03b3";
        case Rivet::PID::GLUON: return "g";
        case Rivet::PID::WPLUSBOSON: return "W\u207a";
        case Rivet::PID::WMINUSBOSON: return "W\u207b";
        case Rivet::PID::ZBOSON: return "Z";
        case Rivet::PID::HIGGS: return "H";
        case Rivet::PID::DQUARK: return "d";
        case Rivet::PID::UQUARK: return "u";
        case Rivet::PID::SQUARK: return "s";
        case Rivet::PID::CQUARK: return "c";
        case Rivet::PID::BQUARK: return "b";
        case Rivet::PID::TQUARK: return "t";
        case Rivet::PID::PROTON: return "p";
        case Rivet::PID::ANTIPROTON: return "p\u0305";
        case Rivet::PID::NEUTRON: return "n";
        case Rivet::PID::ANTINEUTRON: return "n\u0305";
        case Rivet::PID::PI0: return "\u03c0\u2070";
        case Rivet::PID::PIPLUS: return "\u03c0\u207a";
        case Rivet::PID::PIMINUS: return "\u03c0\u207b";
        case Rivet::PID::RHO0: return "\u03c1\u2070";
        case Rivet::PID::RHOPLUS: return "\u03c1\u207a";
        case Rivet::PID::RHOMINUS: return "\u03c1\u207b";
        case Rivet::PID::K0L: return "K\u2070\u2097";
        case Rivet::PID::K0S: return "K\u2070\u209b";
        case Rivet::PID::K0: return "K\u2070";
        case Rivet::PID::KPLUS: return "K\u207a";
        case Rivet::PID::KMINUS: return "K\u207b";
        case 313: return "K*\u2070";
        case 323: return "K*\u207a";
        case -323: return "K*\u207b";
        case Rivet::PID::ETA: return "\u03b7";
        case Rivet::PID::ETAPRIME: return "\u03b7'";
        case Rivet::PID::PHI: return "\u03c6";
        case Rivet::PID::OMEGA: return "\u03c9";
        case Rivet::PID::ETAC: return "\u03b7c";
        case Rivet::PID::JPSI: return "J/\u03a8";
        case Rivet::PID::PSI2S: return "\u03a8(2S)";
        case Rivet::PID::D0: return "D\u2070";
        case Rivet::PID::D0BAR: return "D\u0305\u2070";
        case Rivet::PID::DPLUS: return "D\u207a";
        case Rivet::PID::DMINUS: return "D\u207b";
        case Rivet::PID::DSTARPLUS: return "D*\u207a";
        case Rivet::PID::DSTARMINUS: return "D*\u207b";
        case 423: return "D*\u2070";
        case Rivet::PID::DSPLUS: return "D\u207as";
        case Rivet::PID::DSMINUS: return "D\u207bs";
        case 433: return "D*\u207as";
        case -433: return "D*\u207bs";
        case Rivet::PID::ETAB: return "\u03b7b";
        case Rivet::PID::UPSILON1S: return "\u03a5";
        case Rivet::PID::UPSILON2S: return "\u03a5(2S)";
        case Rivet::PID::UPSILON3S: return "\u03a5(3S)";
        case Rivet::PID::UPSILON4S: return "\u03a5(4S)";
        case Rivet::PID::B0: return "B\u2070";
        case Rivet::PID::B0BAR: return "B\u0305\u2070";
        case Rivet::PID::BPLUS: return "B\u207a";
        case Rivet::PID::BMINUS: return "B\u207b";
        case 513: return "B*\u2070";
        case 523: return "B*\u207a";
        case -523: return "B*\u207b";
        case Rivet::PID::B0S: return "B\u2070s";
        case 533: return "B*\u2070s";
        case Rivet::PID::BCPLUS: return "B\u207ac";
        case Rivet::PID::BCMINUS: return "B\u207bc";
        case 543: return "B*\u207ac";
        case -543: return "B*\u207bc";
        case Rivet::PID::LAMBDA: return "\u039b";
        case Rivet::PID::SIGMA0: return "\u03a3\u2070";
        case Rivet::PID::SIGMAPLUS: return "\u03a3\u207a";
        case Rivet::PID::SIGMAMINUS: return "\u03a3\u207b";
        case 3214: return "\u03a3*\u2070";
        case 3224: return "\u03a3*\u207a";
        case 3114: return "\u03a3*\u207b";
        case Rivet::PID::SIGMAB: return "\u03a3\u2070b";
        case Rivet::PID::SIGMABPLUS: return "\u03a3\u207ab";
        case Rivet::PID::SIGMABMINUS: return "\u03a3\u207bb";
        case Rivet::PID::LAMBDACPLUS: return "\u039b\u207ac";
        case Rivet::PID::LAMBDAB: return "\u039bb";
        case Rivet::PID::XI0: return "\u039e\u2070";
        case Rivet::PID::XIMINUS: return "\u039e\u207b";
        case Rivet::PID::XIPLUS: return "\u039e\u207a";
        case 3324: return "\u039e*\u2070";
        case 3314: return "\u039e*\u207a";
        case -3314: return "\u039e*\u207b";
        case Rivet::PID::XI0B: return "\u039e\u2070b";
        case Rivet::PID::XIBMINUS: return "\u039e\u207bb";
        case Rivet::PID::XI0C: return "\u039e\u2070c";
        case Rivet::PID::XICPLUS: return "\u039e\u207ac";
        case Rivet::PID::OMEGAMINUS: return "\u03a9\u207b";
        case Rivet::PID::OMEGAPLUS: return "\u03a9\u207a";
        case Rivet::PID::OMEGABMINUS: return "\u03a9\u207bb";
        case Rivet::PID::OMEGA0C: return "\u03a9\u2070c";
        case Rivet::PID::ANY: return "*";
    }

    if(pdgid < 0){
        const std::string antiparticle = particleNotation(-pdgid);
        if(antiparticle.rfind("Unknown particle", 0) != 0){
            return "\u0305" + antiparticle;
        }
    }

    return "Unknown particle " + std::to_string(pdgid);
}