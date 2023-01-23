# The name of the package:
atlas_subdir (dark-jet-studies)

find_package( FastJet COMPONENTS fastjetplugins fastjettools )
find_package( FastJetContrib COMPONENTS )
find_package( ROOT COMPONENTS MathCore RIO Core Tree Hist Matrix Physics REQUIRED )

# Add the shared library:
atlas_add_library (dark-jet-studies-lib
  dark-jet-studies/*.h Root/*.cpp
  PUBLIC_HEADERS dark-jet-studies
  LINK_LIBRARIES xAODCore xAODBase xAODPFlow PathResolver xAODMetaData AsgAnalysisInterfaces AssociationUtilsLib 
  AnaAlgorithmLib EventLoop EventLoopGrid SampleHandler MCTruthClassifierLib PMGToolsLib xAODMissingET
  ${FASTJETCONTRIB_LIBRARIES} ${FASTJET_LIBRARIES}
  )

if (XAOD_STANDALONE)
 # Add the dictionary (for AnalysisBase only):
 atlas_add_dictionary (dark-jet-studies-dict
   dark-jet-studies/dark-jet-studies-dict.h
   dark-jet-studies/selection.xml
   dark-jet-studies/DarkJetStudy.h
   LINK_LIBRARIES dark-jet-studies-lib)
endif ()

if (XAOD_STANDALONE)
  atlas_add_executable( runDarkJetStudy util/runDarkJetStudy.cpp 
                        INCLUDE_DIRS ${ROOT_INCLUDE_DIRS}
                        LINK_LIBRARIES ${ROOT_LIBRARIES} dark-jet-studies-lib
                      )
endif ()
