#include <xAODRootAccess/Init.h>
#include <AnaAlgorithm/AnaAlgorithmConfig.h>
#include <SampleHandler/SampleHandler.h>
#include <SampleHandler/ToolsDiscovery.h>
#include <SampleHandler/SampleLocal.h>
#include <EventLoop/Job.h>
#include <EventLoop/OutputStream.h>
#include <EventLoop/DirectDriver.h>
#include <EventLoopGrid/PrunDriver.h>
#include <TSystem.h>
#include <TDatime.h>
#include <iostream>
#include "dark-jet-studies/DarkJetStudy.h"

int main(int argc, char **argv){
    TString sampleName = "sample";
    TString submitDir = "/tmp/DarkJetStudy_DATE", infile = "";
    TString outputFile = "RecoTruthEfficiency.root";
    int nEvents = 10;    //How many events to run over

    //Pick up arguments
    for(int argi = 1; argi < argc; ++argi){
        TString arg = argv[argi];
        if(arg.Contains(".root")){
            if(!gSystem->AccessPathName(arg.Data())){
                infile = arg;
            }
            else{
                std::cerr << "File " << argv[argi] << " does not exist." << std::endl;
                return EXIT_FAILURE;
            }
        }
        else if(arg == "-n" || arg == "--num-events"){
            try{
                nEvents = std::stoi(argv[++argi]);
            }
            catch(const std::invalid_argument &e){
                std::cout << "Invalid number of events: " << argv[argi] << std::endl;
                return EXIT_FAILURE;
            }
            catch(const std::out_of_range &e){
                std::cout << "Number of events " << argv[argi] << " is out of range." << std::endl;
                return EXIT_FAILURE;
            }
        }
        else if(arg == "-r" || arg == "--jet-radius"){
            DarkJetStudy::jetRadius = std::stod(argv[++argi]);
        }
        else if(arg == "-o" || arg == "--output"){
            outputFile = argv[++argi];
        }
        else if(arg == "-d" || arg == "--dark-regex"){
            DarkJetStudy::darkRegex = std::regex(argv[++argi]);
        }
        else if(arg == "-h" || arg == "--help"){
            std::cout << "Usage: runDarkJetStudy [options] infile" << std::endl
                << "Options:" << std::endl
                << "  -d, --dark-regex <regex>:          A regex that defines which PDG ID corresponds to a dark particle. Defaults to ^49[0-9]{3}([013-9][0-9]|[0-9][0-24-9])$, which matches any PDG ID in 49xxxxx, except 4900023 which is used for the Z' boson." << std::endl
                << "  -h, --help:                        Show this help text and exit" << std::endl
                << "  -n, --num-events <integer>:        Set the number of events to <integer>, defaults to 10" << std::endl
                << "  -o, --output <path>:               Set the path of the output ROOT file to <path>, defaults to RecoTruthEfficiency.root" << std::endl
                << "  -r, --jet-radius <float>:          Set the jet radius to <float>, defaults to 1.0" << std::endl;
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

    if(submitDir.Contains("DATE")){
        TDatime now = TDatime();
        submitDir.ReplaceAll("DATE",Form("%d.%.2d.%.2d_%.2d.%.2d.%.2d",
                                     now.GetYear(),now.GetMonth(),now.GetDay(),
                                     now.GetHour(),now.GetMinute(),now.GetSecond()));
    }


    if(!gSystem->AccessPathName(submitDir.Data())){    //If the file exists
        std::cout << "Output directory " << submitDir.Data() << " already exists. Do you want to delete it and recreate it?" << std::endl << "(y or n)";
        char answer = 'n';
        std::cin >> answer;
        if(answer == 'y'){
            std::system(("rm -rf " + submitDir).Data());
        }
        else{
            return EXIT_FAILURE;
        }
    }

    printf("\n");
    printf("  %20s  %s\n", "SampleName:", sampleName.Data());
    printf("  %20s  %s\n", "OutputDir:", submitDir.Data());

    xAOD::Init().ignore();

    EL::Job job;
    SH::SampleHandler sh;
    sh.setMetaString("nc_tree", "CollectionTree");

    //Add our algorithm to the job
    EL::AnaAlgorithmConfig alg;
    alg.setType("DarkJetStudy");

    //Set the name of the algorithm (this is the name use with messages)
    alg.setName("DarkJetStudy");

    if(nEvents > 0){
        job.options()->setDouble(EL::Job::optMaxEvents, nEvents);
    }

    //If one or several root files are specified as arugment, run over these
    std::unique_ptr<SH::SampleLocal> sample(new SH::SampleLocal(sampleName.Data()));
    sample->add(infile.Data());

    //Add the files to the sample handler
    sh.add(sample.release());

    job.sampleHandler(sh);
    job.algsAdd(alg);
    job.outputAdd(EL::OutputStream("ANALYSIS"));

    //Run the job using the local/direct driver:
    EL::DirectDriver driver;
    driver.submit(job, submitDir.Data());

    //Copy the root file to the output location
    std::system(("cp " + submitDir + "/hist-sample.root " + outputFile).Data());

    return 0;
}
