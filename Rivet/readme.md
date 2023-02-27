# How to run this code
## Setup

On your personal computer, you don't need to do anything special to set this up. On lxplus, whenever you open a new terminal and want to run this code, navigate to this folder and run the following command:

```
source setup
```

## Compling and running the code

To compile and run the code, run CompileAndRun.sh with the following syntax:

```
[OPTION1=value1 [OPTION2=value2 [...]]] ./CompileAndRun.sh <analysisName> <inputFile>
```

`<analysisName>` is the name of the analysis you want to run (which corresponds to the name of the source file without `.cpp` at the end). If you're in another working directory than the source file, you can't specify the absolute path, you have to navigate to the same directory as the source file is in.

`<inputFile>` is the path to an EVNT or HEPMC file. HEPMC files can be opened on any computer with Rivet and Root installed, EVNT files can only be opened on lxplus.

For the options, all analyses have the `DARK_REGEX` option, which is a regex that defines which PDG ID corresponds to a dark particle. The default is `^490[0-9][1-9][0-9]{2}$` which works for most models. The sign of the PDG ID is ignored, so this also matches negative PDG IDs.

In addition, the PartionTruthEfficiency analysis has the following options:

- `JET_RADIUS`: Defines the jet radius used to build jets. Defaults to `1.0`.
- `INCLUDE_INVISIBLES`: `1` if invisibles should be included in the jet building (default), `0` if they shouldn't.
- `PDF_FILENAME`: The path that the ouptut PDF file should be written to. Defaults to `../Outputs/PartonTruthEfficiency.pdf`.
- `PLOT_SECOND_CHILDREN`: If the resonance particle decays into a particle with mass >= 50 GeV (for example if the X' boson emits a SM or dark gluon, which is equivalent to it decaying into a gluon and another X' boson with mass >= 50 GeV), determines whether to plot the children of that particle. `0` if they shouldn't be plotted (default), `1` if they should.
- `RES_PDGID`: A comma-seperated list of PDG IDs to look for when looking for the resonance particle. Defaults to `4900001,4900023`, which looks for an X' boson or a Z' boson. This is sensitive to the sign, so `4900001` looks for an X' boson but not an anti-X' boson. To look for an anti-X' boson instead, use `-4900001`.
