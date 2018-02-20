#include "globals.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

#include "NuBeamLocalFieldMessenger.hh"
#include "NuBeamLocalField.hh"

NuBeamLocalFieldMessenger::NuBeamLocalFieldMessenger(NuBeamLocalField* LF)
  :fLocalField(LF)
{ 
  fDirectory = new G4UIdirectory("/boone/field/");
  fDirectory->SetGuidance( "BooNE magnetic field commands.");
  
  fHornCurrentCmd = new G4UIcmdWithADoubleAndUnit("/boone/field/horncurrent",this);
  fHornCurrentCmd->SetGuidance("horn current");
  fHornCurrentCmd->SetGuidance(
			       "Available options: any positive or negative floating-point number, with a valid unit for the current (e.g. kA, A, etc.)");
  fHornCurrentCmd->SetParameterName("hornCur", true, false);
  fHornCurrentCmd->SetDefaultUnit("ampere");
  fHornCurrentCmd->SetDefaultValue(174000.);
}

NuBeamLocalFieldMessenger::~NuBeamLocalFieldMessenger()
{
  delete fHornCurrentCmd;
  delete fDirectory;

}

void NuBeamLocalFieldMessenger::SetNewValue(G4UIcommand * command,G4String newValue)
{ 
  if(command == fHornCurrentCmd){
    fLocalField->
      SetHornCurrent(fHornCurrentCmd->GetNewDoubleValue(newValue));
  }
}

