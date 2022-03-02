#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
set(DK2NU $ENV{DK2NU})
set(ROOTSYS $ENV{ROOTSYS})

include_directories(${PROJECT_SOURCE_DIR}/include 
		    ${PROJECT_SOURCE_DIR}/physics
                    ${Geant4_INCLUDE_DIR}
		    ${DK2NU}/include
		    ${ROOTSYS}/include )	    

file(GLOB sources  ${PROJECT_SOURCE_DIR}/src/*.cc)

add_executable(NuBeam ${sources})
target_link_libraries(NuBeam  ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} ${DK2NU}/lib/libdk2nuTree.so) 

add_custom_command(TARGET NuBeam
	           COMMAND ups active | grep -e Active -e gcc -e geant4 -e dk2nu -e cmake -e jobsub -e ifdhc -e boost > upssetup.tmp
                   COMMAND objcopy --add-section ups_setup=upssetup.tmp NuBeam
                   DEPENDS NuBeam
                   COMMENT "Embedding ups setup into binary.\n\tobjcopy --dump-section ups_setup=/dev/stdout NuBeam\nStored environment will be reproduced automatically with submitJobs.py script.")

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS NuBeam  DESTINATION ${PROJECT_SOURCE_DIR}/bin/$ENV{GEANT4_VERSION})

