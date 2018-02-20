#include "NuBeamRunActionMessenger.hh"
#include "NuBeamRunAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4ios.hh"
#include "globals.hh"
#include "Randomize.hh"
#include "NuBeamRunManager.hh"
#include "NuBeamSteppingAction.hh"

NuBeamRunActionMessenger::NuBeamRunActionMessenger(NuBeamRunAction* RA)
  :fRunAction (RA)
{
  fRndmDir = new G4UIdirectory("/boone/rndm/");
  fRndmDir->SetGuidance("Rndm status control.");
  
  fReadRndmCmd = new G4UIcmdWithAString("/boone/rndm/read",this);
  fReadRndmCmd->SetGuidance("get rndm status from an external file.");
  fReadRndmCmd->SetParameterName("fileName",true);
  fReadRndmCmd->SetDefaultValue ("");
  fReadRndmCmd->AvailableForStates(G4State_PreInit,G4State_Idle);  

  fShowRndmCmd = new G4UIcmdWithoutParameter("/boone/rndm/show",this);
  fShowRndmCmd->SetGuidance("show rndm status.");
  fShowRndmCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fSetRndmSeedCmd = new G4UIcmdWithAnInteger("/boone/rndm/setRndmSeed",this);
  fSetRndmSeedCmd->SetGuidance("set rndm seed.");
  fSetRndmSeedCmd->SetParameterName("rndmSeed",true);
  fSetRndmSeedCmd->SetDefaultValue (0);
  fSetRndmSeedCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
 
}

NuBeamRunActionMessenger::~NuBeamRunActionMessenger()
{
   delete         fRndmDir;
   delete         fReadRndmCmd;  
   delete         fShowRndmCmd;
   delete         fSetRndmSeedCmd;
}

void NuBeamRunActionMessenger::SetNewValue(G4UIcommand* command,G4String newValues)
{
  if (command == fReadRndmCmd) { 
    G4cout << "\n---> rndm status restored from file: " << newValues << G4endl;
    G4String rndmFile="rndm/";
    rndmFile.append(newValues);
    CLHEP::HepRandom::restoreEngineStatus(rndmFile);
  }   
  if (command == fShowRndmCmd) { 
    CLHEP::HepRandom::showEngineStatus();
  }  
   
  if (command == fSetRndmSeedCmd) {
    G4cout << " Setting the seed to " <<  fSetRndmSeedCmd->GetNewIntValue(newValues) << G4endl;
    CLHEP::HepRandom::setTheSeed(fSetRndmSeedCmd->GetNewIntValue(newValues));
  }
  
}

