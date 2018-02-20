#include "NuBeamTrackingActionMessenger.hh"
#include "NuBeamTrackingAction.hh"

#include "G4UIdirectory.hh"

NuBeamTrackingActionMessenger::NuBeamTrackingActionMessenger(NuBeamTrackingAction* TA)
:fTrackingAction(TA)
{ 
  fDirectory = new G4UIdirectory("/boone/tracking/");
  fDirectory->SetGuidance( "BooNE particle biasing commands.");
}

NuBeamTrackingActionMessenger::~NuBeamTrackingActionMessenger()
{
  delete fDirectory;
}

void NuBeamTrackingActionMessenger::SetNewValue(G4UIcommand*, G4String)
{ 
}





