source /cvmfs/larsoft.opensciencegrid.org/products/setups
source /cvmfs/fermilab.opensciencegrid.org/products/common/etc/setup
setup gcc v4_9_3
setup geant4 v4_10_2_p02b -q e10:prof
setup dk2nu v01_04_01f -q e10:prof:r6
setup cmake v3_3_2
setup jobsub_client
setup ifdhc

#use boost for beamHist.c
setup boost v1_61_0 -q e10:prof