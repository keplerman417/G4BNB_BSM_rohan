#include "NuBeamGeometryConstructionMessenger.hh"
#include "NuBeamGeometryConstruction.hh"

#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4ios.hh"
#include "globals.hh"
#include "G4UIdirectory.hh"

NuBeamGeometryConstructionMessenger::NuBeamGeometryConstructionMessenger(NuBeamGeometryConstruction* GC)
:fGeometryConstruction(GC)
{ 
  fDirectory = new G4UIdirectory("/boone/geometry/");
  fDirectory->SetGuidance("NuBeam geometry control commands.");

  fGeometryFileCmd = new G4UIcmdWithAString("/boone/geometry/geometryfile",this);
  fGeometryFileCmd->SetGuidance("GDML geometry file");
  fGeometryFileCmd->SetGuidance("BooNE_50m.gdml: MiniBooNE geometry from target to far (50m) absorber, without the 25m absorber in place.");
  fGeometryFileCmd->SetDefaultValue("BooNE_50m.gdml");
  fGeometryFileCmd->AvailableForStates(G4State_PreInit);

  fConstructCmd = new G4UIcmdWithoutParameter("/boone/geometry/construct",this);
  fConstructCmd->SetGuidance("Must be called after pre-init stage (after ack above cmds) ");
  fConstructCmd->AvailableForStates(G4State_PreInit);
}

NuBeamGeometryConstructionMessenger::~NuBeamGeometryConstructionMessenger()
{
  delete fDirectory;  
  delete fGeometryFileCmd;  
  delete fConstructCmd;
}

void NuBeamGeometryConstructionMessenger::SetNewValue(G4UIcommand * command,G4String newValue)
{
  if(command == fGeometryFileCmd) {
    fGeometryConstruction->SetGeometryFile(newValue);
  }
  if(command == fConstructCmd) {
    fGeometryConstruction->Construct();
  }
}





