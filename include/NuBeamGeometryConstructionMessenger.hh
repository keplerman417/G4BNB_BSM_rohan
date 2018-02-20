#ifndef NuBeamGeometryConstructionMessenger_h
#define NuBeamGeometryConstructionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

class NuBeamGeometryConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWithoutParameter;

class NuBeamGeometryConstructionMessenger: public G4UImessenger
{
public:
  NuBeamGeometryConstructionMessenger(NuBeamGeometryConstruction*);
  ~NuBeamGeometryConstructionMessenger();
  
  void SetNewValue(G4UIcommand*, G4String );
  
private:
  NuBeamGeometryConstruction* fGeometryConstruction;
  G4UIdirectory*              fDirectory;
  G4UIcmdWithAString*         fGeometryFileCmd;
  G4UIcmdWithoutParameter*    fConstructCmd;
};

#endif

