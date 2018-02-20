#include "NuBeamSteppingActionMessenger.hh"
#include "NuBeamSteppingAction.hh"

#include "globals.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"

#include <sstream>

NuBeamSteppingActionMessenger::NuBeamSteppingActionMessenger(NuBeamSteppingAction* SA)
:fSteppingAction(SA)
{
  fDirectory = new G4UIdirectory("/boone/step/");
  fDirectory->SetGuidance("NuBeam step control commands.");

  fPerfectFocusingForPositivesCmd = new G4UIcmdWithABool("/boone/step/perfectFocusingForPositives",this);
  fPerfectFocusingForPositivesCmd->SetGuidance("Flag for forcing all positively-charged particles to be on axis and directed along z when crossing a plane located just downstream of the horn.");
  fPerfectFocusingForPositivesCmd->SetGuidance("Available options: 0, 1");
  fPerfectFocusingForPositivesCmd->SetParameterName("perfectFocusingForPositives", true, false);
  fPerfectFocusingForPositivesCmd->SetDefaultValue(false);

  fPerfectFocusingForNegativesCmd = new G4UIcmdWithABool("/boone/step/perfectFocusingForNegatives",this);
  fPerfectFocusingForNegativesCmd->SetGuidance("Flag for forcing all negatively-charged particles to be on axis and directed along z when crossing a plane located just downstream of the horn.");
  fPerfectFocusingForNegativesCmd->SetGuidance("Available options: 0, 1");
  fPerfectFocusingForNegativesCmd->SetParameterName("perfectFocusingForNegatives", true, false);
  fPerfectFocusingForNegativesCmd->SetDefaultValue(false);

}

NuBeamSteppingActionMessenger::~NuBeamSteppingActionMessenger()
{
  delete fPerfectFocusingForPositivesCmd;
  delete fPerfectFocusingForNegativesCmd;
  delete fDirectory;
}

void NuBeamSteppingActionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{

  if(command == fPerfectFocusingForPositivesCmd) {
    G4int vl;
    const char* t = newValue;
    std::istringstream is((char*)t);
    is >> vl;
    fSteppingAction->SetPerfectFocusingForPositives(vl!=0);
  } else if(command == fPerfectFocusingForNegativesCmd) {
    G4int vl;
    const char* t = newValue;
    std::istringstream is((char*)t);
    is >> vl;
    fSteppingAction->SetPerfectFocusingForNegatives(vl!=0);
  } 
}

