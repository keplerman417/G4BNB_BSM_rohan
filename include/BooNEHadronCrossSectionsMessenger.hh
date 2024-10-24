#ifndef BooNEHadronCrossSectionsMessenger_h
#define BooNEHadronCrossSectionsMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class BooNEHadronCrossSections;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

class BooNEHadronCrossSectionsMessenger: public G4UImessenger
{
public:
  BooNEHadronCrossSectionsMessenger(BooNEHadronCrossSections*);
  ~BooNEHadronCrossSectionsMessenger();

  void SetNewValue(G4UIcommand* command, G4String newValues);

private:

  BooNEHadronCrossSections* theBooNEHadronCrossSections;

  G4UIdirectory*  booneCrossSectionsDirectory;
  G4UIdirectory*  booneProtonCrossSectionsDirectory;
  G4UIdirectory*  booneNeutronCrossSectionsDirectory;
  G4UIdirectory*  boonePionPlusCrossSectionsDirectory;
  G4UIdirectory*  boonePionMinusCrossSectionsDirectory;
  G4UIdirectory*  booneEtaCrossSectionsDirectory;
  G4UIdirectory*  booneEtapCrossSectionsDirectory;
  G4UIdirectory*  boonePionZeroCrossSectionsDirectory;
  
  G4UIcommand*  proBeMomentumRangeCmd;
  G4UIcommand*  proAlMomentumRangeCmd;
  G4UIcommand*  neuBeMomentumRangeCmd;
  G4UIcommand*  neuAlMomentumRangeCmd;
  G4UIcommand*  pipBeMomentumRangeCmd;
  G4UIcommand*  pipAlMomentumRangeCmd;
  G4UIcommand*  pimBeMomentumRangeCmd;
  G4UIcommand*  pimAlMomentumRangeCmd;
  G4UIcommand*  etaBeMomentumRangeCmd;
  G4UIcommand*  etaAlMomentumRangeCmd;
  G4UIcommand*  etapBeMomentumRangeCmd;
  G4UIcommand*  etapAlMomentumRangeCmd;
  G4UIcommand*  pizeroBeMomentumRangeCmd;
  G4UIcommand*  pizeroAlMomentumRangeCmd;

  G4UIcommand* totProBeXsecParCmd;
  G4UIcommand* ineProBeXsecParCmd;
  G4UIcommand* qelProBeXsecParCmd;
  G4UIcommand* totProAlXsecParCmd;
  G4UIcommand* ineProAlXsecParCmd;
  G4UIcommand* qelProAlXsecParCmd;

  G4UIcommand* totNeuBeXsecParCmd;
  G4UIcommand* ineNeuBeXsecParCmd;
  G4UIcommand* qelNeuBeXsecParCmd;
  G4UIcommand* totNeuAlXsecParCmd;
  G4UIcommand* ineNeuAlXsecParCmd;
  G4UIcommand* qelNeuAlXsecParCmd;

  G4UIcommand* totPipBeXsecParCmd;
  G4UIcommand* inePipBeXsecParCmd;
  G4UIcommand* qelPipBeXsecParCmd;
  G4UIcommand* totPipAlXsecParCmd;
  G4UIcommand* inePipAlXsecParCmd;
  G4UIcommand* qelPipAlXsecParCmd;

  G4UIcommand* totPimBeXsecParCmd;
  G4UIcommand* inePimBeXsecParCmd;
  G4UIcommand* qelPimBeXsecParCmd;
  G4UIcommand* totPimAlXsecParCmd;
  G4UIcommand* inePimAlXsecParCmd;
  G4UIcommand* qelPimAlXsecParCmd;

  
  G4UIcommand* totPiZeroBeXsecParCmd;
  G4UIcommand* inePiZeroBeXsecParCmd;
  G4UIcommand* qelPiZeroBeXsecParCmd;
  G4UIcommand* totPiZeroAlXsecParCmd;
  G4UIcommand* inePiZeroAlXsecParCmd;
  G4UIcommand* qelPiZeroAlXsecParCmd;
  
  G4UIcommand* totEtaBeXsecParCmd;
  G4UIcommand* ineEtaBeXsecParCmd;
  G4UIcommand* qelEtaBeXsecParCmd;
  G4UIcommand* totEtaAlXsecParCmd;
  G4UIcommand* ineEtaAlXsecParCmd;
  G4UIcommand* qelEtaAlXsecParCmd;


  G4UIcommand* totEtapBeXsecParCmd;
  G4UIcommand* ineEtapBeXsecParCmd;
  G4UIcommand* qelEtapBeXsecParCmd;
  G4UIcommand* totEtapAlXsecParCmd;
  G4UIcommand* ineEtapAlXsecParCmd;
  G4UIcommand* qelEtapAlXsecParCmd;



  
  G4double totProBeXsecPar[5], ineProBeXsecPar[5], qelProBeXsecPar[5];
  G4double totProAlXsecPar[5], ineProAlXsecPar[5], qelProAlXsecPar[5];
  G4double totNeuBeXsecPar[5], ineNeuBeXsecPar[5], qelNeuBeXsecPar[5];
  G4double totNeuAlXsecPar[5], ineNeuAlXsecPar[5], qelNeuAlXsecPar[5];
  G4double totPipBeXsecPar[8], inePipBeXsecPar[10], qelPipBeXsecPar[10];
  G4double totPipAlXsecPar[8], inePipAlXsecPar[10], qelPipAlXsecPar[10];
  G4double totPimBeXsecPar[8], inePimBeXsecPar[10], qelPimBeXsecPar[10];
  G4double totPimAlXsecPar[8], inePimAlXsecPar[10], qelPimAlXsecPar[10];
  G4double totEtaBeXsecPar[8], ineEtaBeXsecPar[10], qelEtaBeXsecPar[10];
  G4double totEtaAlXsecPar[8], ineEtaAlXsecPar[10], qelEtaAlXsecPar[10];
  G4double totEtapBeXsecPar[8], ineEtapBeXsecPar[10], qelEtapBeXsecPar[10];
  G4double totEtapAlXsecPar[8], ineEtapAlXsecPar[10], qelEtapAlXsecPar[10];
  G4double totPiZeroBeXsecPar[8], inePiZeroBeXsecPar[10], qelPiZeroBeXsecPar[10];
  G4double totPiZeroAlXsecPar[8], inePiZeroAlXsecPar[10], qelPiZeroAlXsecPar[10];
  
};

#endif
