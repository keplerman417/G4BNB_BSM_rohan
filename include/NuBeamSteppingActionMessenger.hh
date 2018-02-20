#ifndef NuBeamSteppingActionMessenger_h
#define NuBeamSteppingActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UIcommand.hh"

class NuBeamSteppingAction;
class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;


class NuBeamSteppingActionMessenger: public G4UImessenger
{
public:

  NuBeamSteppingActionMessenger(NuBeamSteppingAction*);
  ~NuBeamSteppingActionMessenger();
  void SetNewValue(G4UIcommand* command, G4String newValues);

private:
  NuBeamSteppingAction* fSteppingAction;
  G4UIdirectory*    fDirectory;
  G4UIcmdWithABool* fPerfectFocusingForPositivesCmd;
  G4UIcmdWithABool* fPerfectFocusingForNegativesCmd;
};

#endif

