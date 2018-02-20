#ifndef NuBeamEventActionMessenger_h
#define NuBeamEventActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class NuBeamEventAction;
class G4UIcmdWithAnInteger;

class NuBeamEventActionMessenger: public G4UImessenger
{
  public:
    NuBeamEventActionMessenger(NuBeamEventAction*);
   ~NuBeamEventActionMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    NuBeamEventAction*    fEventAction;
    G4UIcmdWithAnInteger* fPrintCmd;    
};

#endif
