#ifndef NuBeamTrackingActionMessenger_h
#define NuBeamTrackingActionMessenger_h 1

#include "G4UImessenger.hh"

class NuBeamTrackingAction;
class G4UIdirectory;

class NuBeamTrackingActionMessenger: public G4UImessenger
{
public:
  NuBeamTrackingActionMessenger(NuBeamTrackingAction*);
  ~NuBeamTrackingActionMessenger();
  
  void SetNewValue(G4UIcommand*, G4String );
  
private:
  NuBeamTrackingAction*  fTrackingAction;

  G4UIdirectory* fDirectory;
};

#endif

