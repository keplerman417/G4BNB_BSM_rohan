#ifndef NuBeamRunAction_h
#define NuBeamRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

#include "NuBeamOutput.hh"

class G4Run;
class NuBeamRunActionMessenger;

class NuBeamRunAction : public G4UserRunAction
{
  public:
  NuBeamRunAction();
  ~NuBeamRunAction();

  virtual void BeginOfRunAction(const G4Run* aRun);
  virtual void EndOfRunAction(const G4Run* aRun);

  private:
  NuBeamRunActionMessenger *fRunActionMessenger; 
  NuBeamOutput * fRecord;

};

#endif

