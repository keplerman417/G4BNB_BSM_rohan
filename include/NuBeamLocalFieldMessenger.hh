#ifndef NuBeamLocalFieldMessenger_h
#define NuBeamLocalFieldMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class NuBeamLocalField;
class NuBeamSkinDepthField;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;

class NuBeamLocalFieldMessenger: public G4UImessenger
{
public:
  NuBeamLocalFieldMessenger(NuBeamLocalField*);
  ~NuBeamLocalFieldMessenger();
  
  void SetNewValue(G4UIcommand*, G4String );
  
private:
  NuBeamLocalField*          fLocalField;
  G4UIdirectory*             fDirectory;
  G4UIcmdWithADoubleAndUnit* fHornCurrentCmd;

};

#endif

