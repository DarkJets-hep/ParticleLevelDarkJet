# How to run this code
## First setup

To set this up for the first time, download this folder to anywhere on lxplus, navigate to that folder in the command line, and run the following commands:

```
mkdir build
source setup
cd build
cmake ../source
source */setup.sh
```

## Setup on each new terminal

Whenever you open a new terminal and want to run this code, navigate to this folder and run the following command:

```
source setup
```

## Compling the code

To compile the code (after you've set up the environment as above), run the following command:

```
cmake --build $TestArea
```

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
