#ifndef NuBeamTrackingAction_h
#define NuBeamTrackingAction_h 1
#include <fstream>
#include "globals.hh"
#include "G4UserTrackingAction.hh"

class NuBeamTrackingActionMessenger;
class NuBeamOutput;

class NuBeamTrackingAction : public G4UserTrackingAction 
{
public:
  
  NuBeamTrackingAction();
  virtual ~NuBeamTrackingAction();
  
  virtual void PreUserTrackingAction(const G4Track*);
  virtual void PostUserTrackingAction(const G4Track*);

  inline G4bool GetStoreTrajectory() const {return fStoreTrajectory;}

private:
  G4bool fStoreTrajectory;
  NuBeamOutput* fRecords;
  NuBeamTrackingActionMessenger* fMessenger;
};

#endif





