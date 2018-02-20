#include "NuBeamEventAction.hh"
#include "NuBeamEventActionMessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "globals.hh"

NuBeamEventActionMessenger::NuBeamEventActionMessenger(NuBeamEventAction* EA)
:fEventAction(EA)
{
  fPrintCmd = new G4UIcmdWithAnInteger("/event/printModulo",this);
  fPrintCmd->SetGuidance("Print events modulo n");
  fPrintCmd->SetParameterName("EventNb",false);
  fPrintCmd->SetRange("EventNb>0");
  fPrintCmd->SetDefaultValue(1000);
  fPrintCmd->AvailableForStates(G4State_Idle);     
}

NuBeamEventActionMessenger::~NuBeamEventActionMessenger()
{
  delete fPrintCmd;
}

void NuBeamEventActionMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if(command == fPrintCmd)
    {fEventAction->SetPrintModulo(fPrintCmd->GetNewIntValue(newValue));}
}
