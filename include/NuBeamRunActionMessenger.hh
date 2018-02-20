#ifndef NuBeamRunActionMessenger_h
#define NuBeamRunActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4ios.hh"

class NuBeamRunAction;
class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;

class NuBeamRunActionMessenger: public G4UImessenger
{
public:
  NuBeamRunActionMessenger(NuBeamRunAction* );
  ~NuBeamRunActionMessenger();
  
  void SetNewValue(G4UIcommand* ,G4String );
 
private:

  NuBeamRunAction*            fRunAction;
  G4UIdirectory*              fRndmDir;
  G4UIcmdWithAString*         fReadRndmCmd;  
  G4UIcmdWithoutParameter*    fShowRndmCmd;
  G4UIcmdWithAnInteger*       fSetRndmSeedCmd;
      
};

#endif
