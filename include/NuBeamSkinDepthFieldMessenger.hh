#ifndef NuBeamSkinDepthFieldMessenger_h
#define NuBeamSkinDepthFieldMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class NuBeamSkinDepthField;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;

class NuBeamSkinDepthFieldMessenger: public G4UImessenger
{

public:
  NuBeamSkinDepthFieldMessenger(NuBeamSkinDepthField*);
  ~NuBeamSkinDepthFieldMessenger();
  
  void SetNewValue(G4UIcommand*, G4String );
  
private:
  NuBeamSkinDepthField*      fSkinDepthField;

  G4UIdirectory*             fDirectory;
  G4UIcmdWithADoubleAndUnit* fSkinDepthHornCurrentCmd;
  G4UIcmdWithAString*        fSkinDepthModelCmd;
  G4UIcmdWithADoubleAndUnit* fSkinDepthCmd;


};

#endif

