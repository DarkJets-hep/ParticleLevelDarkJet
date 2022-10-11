#pragma once

#include <Rivet/Tools/ParticleName.hh>
#include <string>
#include <regex>

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
        case 2224: return "DELTAPLUSPLUS";
        case 2214: return "DELTAPLUS";
        case 2114: return "DELTA0";
        case 1114: return "DELTAMINUS";
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
        case 1103: return "DD_DIQUARK";
        case 2101: case 2103: return "UD_DIQUARK";
        case 2203: return "UU_DIQUARK";
        case 3101: case 3103: return "SD_DIQUARK";
        case 3201: case 3203: return "SU_DIQUARK";
        case 3303: return "SS_DIQUARK";
        case 4101: case 4103: return "CD_DIQUARK";
        case 4201: case 4203: return "CU_DIQUARK";
        case 4301: case 4303: return "CS_DIQUARK";
        case 4403: return "CC_DIQUARK";
        case 5101: case 5103: return "BD_DIQUARK";
        case 5201: case 5203: return "BU_DIQUARK";
        case 5301: case 5303: return "BS_DIQUARK";
        case 5401: case 5403: return "BC_DIQUARK";
        case 5503: return "BB_DIQUARK";
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

    if(pdgid / 1000000 == 4){
        const std::string groundParticle = particleName(pdgid % 4000000);
        if(groundParticle.rfind("Unknown particle", 0) != 0){
            return "EXCITED_" + groundParticle;
        }
    }

    return "Unknown particle " + std::to_string(pdgid);
}

static std::string particleNameAsTLatex(int pdgid){
    switch(pdgid){
        case Rivet::PID::ELECTRON: return "e^{-}";
        case Rivet::PID::POSITRON: return "e^{+}";
        case Rivet::PID::MUON: return "#mu^{-}";
        case Rivet::PID::ANTIMUON: return "#mu^{+}";
        case Rivet::PID::TAU: return "#tau^{-}";
        case Rivet::PID::ANTITAU: return "#tau^{+}";
        case Rivet::PID::NU_E: return "#nu_{e}";
        case Rivet::PID::NU_EBAR: return "#bar{#nu}_{e}";
        case Rivet::PID::NU_MU: return "#nu_{#mu}";
        case Rivet::PID::NU_MUBAR: return "#bar{#nu}_{#mu}";
        case Rivet::PID::NU_TAU: return "#nu_{#tau}";
        case Rivet::PID::NU_TAUBAR: return "#bar{#nu}_{#tau}";
        case Rivet::PID::PHOTON: return "#gamma";
        case Rivet::PID::GLUON: return "g";
        case Rivet::PID::WPLUSBOSON: return "W^{+}";
        case Rivet::PID::WMINUSBOSON: return "W^{-}";
        case Rivet::PID::ZBOSON: return "Z";
        case Rivet::PID::HIGGS: return "H";
        case Rivet::PID::DQUARK: return "d";
        case Rivet::PID::UQUARK: return "u";
        case Rivet::PID::SQUARK: return "s";
        case Rivet::PID::CQUARK: return "c";
        case Rivet::PID::BQUARK: return "b";
        case Rivet::PID::TQUARK: return "t";
        case Rivet::PID::PROTON: return "p";
        case Rivet::PID::ANTIPROTON: return "#bar{p}";
        case Rivet::PID::NEUTRON: return "n";
        case Rivet::PID::ANTINEUTRON: return "#bar{n}";
        case 2224: return "#Delta^{++}";
        case 2214: return "#Delta^{+}";
        case 2114: return "#Delta^{0}";
        case 1114: return "#Delta^{-}";
        case Rivet::PID::PI0: return "#pi^{0}";
        case Rivet::PID::PIPLUS: return "#pi^{+}";
        case Rivet::PID::PIMINUS: return "#pi^{-}";
        case Rivet::PID::RHO0: return "#rho^{0}";
        case Rivet::PID::RHOPLUS: return "#rho^{+}";
        case Rivet::PID::RHOMINUS: return "#rho^{-}";
        case Rivet::PID::K0L: return "K^{0}_{L}";
        case Rivet::PID::K0S: return "K^{0}_{S}";
        case Rivet::PID::K0: return "K^{0}";
        case Rivet::PID::KPLUS: return "K^{+}";
        case Rivet::PID::KMINUS: return "K^{-}";
        case 313: return "K*^{0}";
        case 323: return "K*^{+}";
        case -323: return "K*^{-}";
        case Rivet::PID::ETA: return "#eta";
        case Rivet::PID::ETAPRIME: return "#eta'";
        case Rivet::PID::PHI: return "#phi";
        case Rivet::PID::OMEGA: return "#omega";
        case Rivet::PID::ETAC: return "#eta_{c}";
        case Rivet::PID::JPSI: return "J/#Psi";
        case Rivet::PID::PSI2S: return "#Psi(2S)";
        case Rivet::PID::D0: return "D^{0}";
        case Rivet::PID::D0BAR: return "#bar{D}^{0}";
        case Rivet::PID::DPLUS: return "D^{+}";
        case Rivet::PID::DMINUS: return "D^{-}";
        case Rivet::PID::DSTARPLUS: return "D*^{+}";
        case Rivet::PID::DSTARMINUS: return "D*^{-}";
        case 423: return "D*^{0}";
        case Rivet::PID::DSPLUS: return "D^{+}_{s}";
        case Rivet::PID::DSMINUS: return "D^{-}_{s}";
        case 433: return "D*^{+}_{s}";
        case -433: return "D*^{-}_{s}";
        case Rivet::PID::ETAB: return "#eta_{b}";
        case Rivet::PID::UPSILON1S: return "#varUpsilon";
        case Rivet::PID::UPSILON2S: return "#varUpsilon(2S)";
        case Rivet::PID::UPSILON3S: return "#varUpsilon(3S)";
        case Rivet::PID::UPSILON4S: return "#varUpsilon(4S)";
        case Rivet::PID::B0: return "B^{0}";
        case Rivet::PID::B0BAR: return "#bar{B}^{0}";
        case Rivet::PID::BPLUS: return "B^{+}";
        case Rivet::PID::BMINUS: return "B^{-}";
        case 513: return "B*^{0}";
        case 523: return "B*^{+}";
        case -523: return "B*^{-}";
        case Rivet::PID::B0S: return "B^{0}_{s}";
        case 533: return "B*^{0}_{s}";
        case Rivet::PID::BCPLUS: return "B^{+}_{c}";
        case Rivet::PID::BCMINUS: return "B^{-}_{c}";
        case 543: return "B*^{+}_{c}";
        case -543: return "B*^{-}_{c}";
        case Rivet::PID::LAMBDA: return "#Lambda";
        case Rivet::PID::SIGMA0: return "#Sigma^{0}";
        case Rivet::PID::SIGMAPLUS: return "#Sigma^{+}";
        case Rivet::PID::SIGMAMINUS: return "#Sigma^{-}";
        case 3214: return "#Sigma*^{0}";
        case 3224: return "#Sigma*^{+}";
        case 3114: return "#Sigma*^{-}";
        case Rivet::PID::SIGMAB: return "#Sigma^{0}_{b}";
        case Rivet::PID::SIGMABPLUS: return "#Sigma^{+}_{b}";
        case Rivet::PID::SIGMABMINUS: return "#Sigma^{-}_{b}";
        case Rivet::PID::LAMBDACPLUS: return "#Lambda^{+}_{c}";
        case Rivet::PID::LAMBDAB: return "#Lambda_{b}";
        case Rivet::PID::XI0: return "#Xi^{0}";
        case Rivet::PID::XIMINUS: return "#Xi^{-}";
        case Rivet::PID::XIPLUS: return "#Xi^{+}";
        case 3324: return "#Xi*^{0}";
        case 3314: return "#Xi*^{+}";
        case -3314: return "#Xi*^{-}";
        case Rivet::PID::XI0B: return "#Xi^{0}_{b}";
        case Rivet::PID::XIBMINUS: return "#Xi^{-}_{b}";
        case Rivet::PID::XI0C: return "#Xi^{0}_{c}";
        case Rivet::PID::XICPLUS: return "#Xi^{+}_{c}";
        case Rivet::PID::OMEGAMINUS: return "#Omega^{-}";
        case Rivet::PID::OMEGAPLUS: return "#Omega^{+}";
        case Rivet::PID::OMEGABMINUS: return "#Omega^{-}_{b}";
        case Rivet::PID::OMEGA0C: return "#Omega^{0}_{c}";
        case 1103: return "dd";
        case 2101: case 2103: return "ud";
        case 2203: return "uu";
        case 3101: case 3103: return "sd";
        case 3201: case 3203: return "su";
        case 3303: return "ss";
        case 4101: case 4103: return "cd";
        case 4201: case 4203: return "cu";
        case 4301: case 4303: return "cs";
        case 4403: return "cc";
        case 5101: case 5103: return "bd";
        case 5201: case 5203: return "bu";
        case 5301: case 5303: return "bs";
        case 5401: case 5403: return "bc";
        case 5503: return "bb";
        case Rivet::PID::ANY: return "*";
    }

    if(pdgid < 0){
        const std::string antiparticle = particleNameAsTLatex(-pdgid);
        if(antiparticle.rfind("Unknown particle", 0) != 0){
            return std::regex_replace(antiparticle, std::regex("^#?[A-Za-z]+"), "#bar{$&}");
        }
    }

    if(pdgid / 1000000 == 4 && pdgid % 4000000 < 100){
        const std::string groundParticle = particleNameAsTLatex(pdgid % 4000000);
        if(groundParticle.rfind("Unknown particle", 0) != 0){
            return groundParticle + "*";
        }
    }

    return "Unknown particle " + std::to_string(pdgid);
}