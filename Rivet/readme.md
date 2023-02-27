# How to run this code
## Setup

On your personal computer, you don't need to do anything special to set this up. On lxplus, whenever you open a new terminal and want to run this code, navigate to this folder and run the following command:

```
source setup
```

## Compling and running the code

To compile and run the code, run CompileAndRun.sh with the following syntax:

```
OPTION1=value1 OPTION2=value2 ./CompileAndRun.sh <analysisName> <inputFile>
```

`<analysisName>` is the name of the analysis you want to run (which corresponds to the name of the source file without `.cpp` at the end). If you're in another working directory than the source file, you can't specify the absolute path, you have to navigate to the same directory as the source file is in.

`<inputFile>` is the path to an EVNT or HEPMC file. HEPMC files can be opened on any computer with Rivet and Root installed, EVNT files can only be opened on lxplus.

For the options, all analyses have the `DARK_REGEX` option, which is a regex that defines which PDG ID corresponds to a dark particle. The default is `^490[0-9][1-9][0-9]{2}$` which works for most models.

## Running the code

After you've set up the environment and compiled the code as above, you can run it using the `runDarkJetStudy` command. The general syntax of this command is `runDarkJetStudy [options] inputfile`, where `inputfile` is the path of an AOD or DAOD file. The options are the following:

 - `-d <regex>`, `--dark-regex <regex>`: A regex that defines which PDG ID corresponds to a dark particle. Defaults to `^49[0-9]{3}([013-9][0-9]|[0-9][0-24-9])$`, which matches any PDG ID in 49xxxxx, except 4900023 which is used for the Z' boson.
 - `-h`, `--help`: Show this help text and exit
 - `-n <integer>`, `--num-events <integer>`: Set the number of events to `<integer>`, defaults to 10
 - `-o <path>`, `--output <path>`: Set the path of the output ROOT file to `<path>`, defaults to `RecoTruthEfficiency.root`

The input file must be an AOD or DAOD file with the following contents:

- `TruthParticles`
- `EventInfo`
- At least one of the following (they will be searched for in that order):
    - `AntiKt10RCEMPFlowJets`
    - `AntiKt4EMPFlowJets`
    - Both `JetETMissChargedParticleFlowObjects` and `JetETMissNeutralParticleFlowObjects`

To check the contents of an AOD or DAOD file, you can run `checkxAOD.py inputfile`.
