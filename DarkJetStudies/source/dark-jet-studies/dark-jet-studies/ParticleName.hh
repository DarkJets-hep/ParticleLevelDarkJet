//This is a portion of the ParticleName.hh file included with Rivet

#ifndef RIVET_PARTICLENAME_HH
#define RIVET_PARTICLENAME_HH

// cython includes termio.h on some systems,
// which #defines a B0 = 0 macro
#undef B0

typedef int PdgId;

namespace Rivet {

  namespace PID {


    /// Static const convenience particle ID names

    /// Special wildcard particle name
    static const PdgId ANY = 10000;

    /// @name Charged leptons
    //@{
    static const PdgId ELECTRON = 11;
    static const PdgId POSITRON = -ELECTRON;
    static const PdgId EMINUS = ELECTRON;
    static const PdgId EPLUS = POSITRON;
    static const PdgId MUON = 13;
    static const PdgId ANTIMUON = -MUON;
    static const PdgId TAU = 15;
    static const PdgId ANTITAU = -TAU;
    //@}

    /// @name Neutrinos
    //@{
    static const PdgId NU_E = 12;
    static const PdgId NU_EBAR = -NU_E;
    static const PdgId NU_MU = 14;
    static const PdgId NU_MUBAR = -NU_MU;
    static const PdgId NU_TAU = 16;
    static const PdgId NU_TAUBAR = -NU_TAU;
    //@}

    /// @name Bosons
    //@{
    static const PdgId PHOTON = 22;
    static const PdgId GAMMA = PHOTON;
    static const PdgId GLUON = 21;
    static const PdgId WPLUSBOSON = 24;
    static const PdgId WMINUSBOSON = -WPLUSBOSON;
    static const PdgId WPLUS = WPLUSBOSON;
    static const PdgId WMINUS = WMINUSBOSON;
    static const PdgId Z0BOSON = 23;
    static const PdgId ZBOSON = Z0BOSON;
    static const PdgId Z0 = Z0BOSON;
    static const PdgId HIGGSBOSON = 25;
    static const PdgId HIGGS = HIGGSBOSON;
    static const PdgId H0BOSON = HIGGSBOSON;
    static const PdgId HBOSON = HIGGSBOSON;
    //@}

    /// @name Quarks
    //@{
    static const PdgId DQUARK = 1;
    static const PdgId UQUARK = 2;
    static const PdgId SQUARK = 3;
    static const PdgId CQUARK = 4;
    static const PdgId BQUARK = 5;
    static const PdgId TQUARK = 6;
    //@}

    /// @name Nucleons
    //@{
    static const PdgId PROTON = 2212;
    static const PdgId ANTIPROTON = -PROTON;
    static const PdgId PBAR = ANTIPROTON;
    static const PdgId NEUTRON = 2112;
    static const PdgId ANTINEUTRON = -NEUTRON;
    //@}

    /// @name Light mesons
    //@{
    static const PdgId PI0 = 111;
    static const PdgId PIPLUS = 211;
    static const PdgId PIMINUS = -PIPLUS;
    static const PdgId RHO0 = 113;
    static const PdgId RHOPLUS = 213;
    static const PdgId RHOMINUS = -RHOPLUS;
    static const PdgId K0L = 130;
    static const PdgId K0S = 310;
    static const PdgId K0 = 311;
    static const PdgId KPLUS = 321;
    static const PdgId KMINUS = -KPLUS;
    static const PdgId ETA = 221;
    static const PdgId ETAPRIME = 331;
    static const PdgId PHI = 333;
    static const PdgId OMEGA = 223;
    //@}

    /// @name Charmonia
    //@{
    static const PdgId ETAC = 441;
    static const PdgId JPSI = 443;
    static const PdgId PSI2S = 100443;
    //@}

    /// @name Charm mesons
    //@{
    static const PdgId D0 = 421;
    static const PdgId D0BAR = -421;
    static const PdgId DPLUS = 411;
    static const PdgId DMINUS = -DPLUS;
    static const PdgId DSTARPLUS = 413;
    static const PdgId DSTARMINUS = -DSTARPLUS;
    static const PdgId DSPLUS = 431;
    static const PdgId DSMINUS = -DSPLUS;
    //@}

    /// @name Bottomonia
    //@{
    static const PdgId ETAB = 551;
    static const PdgId UPSILON1S = 553;
    static const PdgId UPSILON2S = 100553;
    static const PdgId UPSILON3S = 200553;
    static const PdgId UPSILON4S = 300553;
    //@}

    /// @name b mesons
    //@{
    static const PdgId B0 = 511;
    static const PdgId B0BAR = -511;
    static const PdgId BPLUS = 521;
    static const PdgId BMINUS = -BPLUS;
    static const PdgId B0S = 531;
    static const PdgId BCPLUS = 541;
    static const PdgId BCMINUS = -BCPLUS;
    //@}

    /// @name Baryons
    //@{
    static const PdgId LAMBDA = 3122;
    static const PdgId SIGMA0 = 3212;
    static const PdgId SIGMAPLUS = 3222;
    static const PdgId SIGMAMINUS = 3112;
    static const PdgId SIGMAB = 5212;
    static const PdgId SIGMABPLUS = 5222;
    static const PdgId SIGMABMINUS = 5112;
    static const PdgId LAMBDACPLUS = 4122;
    static const PdgId LAMBDACMINUS = 4122;
    static const PdgId LAMBDAB = 5122;
    static const PdgId XI0 = 3322;
    static const PdgId XIMINUS = 3312;
    static const PdgId XIPLUS = -XIMINUS;
    static const PdgId XI0B = 5232;
    static const PdgId XIBMINUS = 5132;
    static const PdgId XI0C = 4132;
    static const PdgId XICPLUS = 4232;
    static const PdgId OMEGAMINUS = 3334;
    static const PdgId OMEGAPLUS = -OMEGAMINUS;
    static const PdgId OMEGABMINUS = 5332;
    static const PdgId OMEGA0C = 4332;
    //@}

    /// @name Exotic/weird stuff
    //@{
    static const PdgId REGGEON = 110;
    static const PdgId POMERON = 990;
    static const PdgId ODDERON = 9990;
    static const PdgId GRAVITON = 39;
    static const PdgId NEUTRALINO1 = 1000022;
    static const PdgId GRAVITINO = 1000039;
    static const PdgId GLUINO = 1000021;
    static const int BPRIME = 7;
    static const int TPRIME = 8;
    static const int LPRIME = 17;
    static const int NUPRIME = 18;
    // static const int DARKMATTERSCALAR = 1000051;
    // static const int DARKMATTERFERMION = 1000052;
    // static const int DARKMATTERVECTOR = 1000053;
    /// @todo Add axion, black hole remnant, etc. on demand
    //@}

    /// @name Nuclei
    //@{
    static const PdgId DEUTERON = 1000010020;
    static const PdgId ALUMINIUM = 1000130270;
    static const PdgId COPPER = 1000290630;
    static const PdgId XENON = 1000541290;
    static const PdgId GOLD = 1000791970;
    static const PdgId LEAD = 1000822080;
    static const PdgId URANIUM = 1000922380;
    /// @todo Add other nuclei on demand
    //@}



    /// Handler for particle name code <-> string conversion
    ///
    /// @todo Hide this utility class or equiv in an unnamed namespace in a .cc file.

  }

}

#endif
