#ifndef BooNEpBeInteractionMessenger_h
#define BooNEpBeInteractionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class BooNEpBeInteraction;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

class BooNEpBeInteractionMessenger: public G4UImessenger
{
  public:
    BooNEpBeInteractionMessenger(BooNEpBeInteraction*);
   ~BooNEpBeInteractionMessenger();

    void SetNewValue(G4UIcommand* command, G4String newValues);

  private:
  BooNEpBeInteraction*   BooNEProtonModel;

  // quasi elastic scattering
  // See comment in BooNEpBeInteraction.hh
  // In new MC we only keep this for 8GeV protons on Be
  G4UIdirectory* booneQuasiElasticDirectory;
  G4UIcommand*   proQuasiElasticParCmd;
  G4UIcommand*   neuQuasiElasticParCmd;
  G4UIcommand*   pipQuasiElasticParCmd;
  G4UIcommand*   pimQuasiElasticParCmd;

  G4double proQuasiElasticPar[6];
  G4double neuQuasiElasticPar[6];
  G4double pipQuasiElasticPar[6];
  G4double pimQuasiElasticPar[6];

  G4UIcommand*fModelEnergyRangeCmd;

  G4UIcmdWithAString* PiPlusPhysicsModelCmd;
  G4UIcmdWithAString* PiMinusPhysicsModelCmd;
  G4UIcmdWithAString* KaonPlusPhysicsModelCmd;
  G4UIcmdWithAString* KaonMinusPhysicsModelCmd;
  G4UIcmdWithAString* KaonZeroLongPhysicsModelCmd;
  G4UIcmdWithAString* ProtonPhysicsModelCmd;
  G4UIcmdWithAString* NeutronPhysicsModelCmd;

  G4UIcmdWithAnInteger* randomSeedCmd;

  G4UIcommand* SWPiPlusParCmd;
  G4UIcommand* SWPiMinusParCmd;
  G4UIcommand* SWKaonPlusParCmd;
  G4UIcommand* SWKaonZeroLongParCmd;
  G4UIcommand* FSKaonPlusParCmd;
  G4UIcommand* FSKaonZeroLongParCmd;

  G4UIcmdWithABool* BeToAScalingCmd;
  G4UIcommand* BeToAScalingParCmd;

  G4UIcmdWithABool* PhysicsVerboseCmd;
  G4UIcmdWithABool* NoBeamPionsCmd;

  G4UIcmdWithAString* PionPlusRwgtFuncCmd;
  G4UIcmdWithAString* PionMinusRwgtFuncCmd;
  G4UIcmdWithAString* KaonPlusRwgtFuncCmd;
  G4UIcmdWithAString* KaonMinusRwgtFuncCmd;
  G4UIcmdWithAString* KaonZeroLongRwgtFuncCmd;
  G4UIcmdWithAString* ProtonRwgtFuncCmd;
  G4UIcmdWithAString* NeutronRwgtFuncCmd;

  G4UIcommand* PionPlusRwgtParamsCmd;
  G4UIcommand* PionMinusRwgtParamsCmd;
  G4UIcommand* KaonPlusRwgtParamsCmd;
  G4UIcommand* KaonMinusRwgtParamsCmd;
  G4UIcommand* KaonZeroLongRwgtParamsCmd;
  G4UIcommand* ProtonRwgtParamsCmd;
  G4UIcommand* NeutronRwgtParamsCmd;

  G4int iRandomSeed;

  G4double fSWPiPlusPar1Value;
  G4double fSWPiPlusPar2Value;
  G4double fSWPiPlusPar3Value;
  G4double fSWPiPlusPar4Value;
  G4double fSWPiPlusPar5Value;
  G4double fSWPiPlusPar6Value;
  G4double fSWPiPlusPar7Value;
  G4double fSWPiPlusPar8Value;

  G4double fSWPiMinusPar1Value;
  G4double fSWPiMinusPar2Value;
  G4double fSWPiMinusPar3Value;
  G4double fSWPiMinusPar4Value;
  G4double fSWPiMinusPar5Value;
  G4double fSWPiMinusPar6Value;
  G4double fSWPiMinusPar7Value;
  G4double fSWPiMinusPar8Value;

  G4double fSWKaonPlusPar1Value;
  G4double fSWKaonPlusPar2Value;
  G4double fSWKaonPlusPar3Value;
  G4double fSWKaonPlusPar4Value;
  G4double fSWKaonPlusPar5Value;
  G4double fSWKaonPlusPar6Value;
  G4double fSWKaonPlusPar7Value;
  G4double fSWKaonPlusPar8Value;
  G4double fSWKaonPlusPar9Value;

  G4double fSWKaonZeroLongPar1Value;
  G4double fSWKaonZeroLongPar2Value;
  G4double fSWKaonZeroLongPar3Value;
  G4double fSWKaonZeroLongPar4Value;
  G4double fSWKaonZeroLongPar5Value;
  G4double fSWKaonZeroLongPar6Value;
  G4double fSWKaonZeroLongPar7Value;
  G4double fSWKaonZeroLongPar8Value;
  G4double fSWKaonZeroLongPar9Value;

  G4double fFSKaonPlusPar1Value;
  G4double fFSKaonPlusPar2Value;
  G4double fFSKaonPlusPar3Value;
  G4double fFSKaonPlusPar4Value;
  G4double fFSKaonPlusPar5Value;
  G4double fFSKaonPlusPar6Value;
  G4double fFSKaonPlusPar7Value;
  G4double fFSKaonPlusPar8Value;

  G4double fFSKaonZeroLongPar1Value;
  G4double fFSKaonZeroLongPar2Value;
  G4double fFSKaonZeroLongPar3Value;
  G4double fFSKaonZeroLongPar4Value;
  G4double fFSKaonZeroLongPar5Value;
  G4double fFSKaonZeroLongPar6Value;
  G4double fFSKaonZeroLongPar7Value;
  G4double fFSKaonZeroLongPar8Value;

  G4double fBeToAScalingPar0Value;
  G4double fBeToAScalingPar1Value;
  G4double fBeToAScalingPar2Value;
};

#endif
