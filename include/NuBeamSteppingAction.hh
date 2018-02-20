#ifndef NuBeamSteppingAction_h
#define NuBeamSteppingAction_h 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"
#include "G4HadronicProcess.hh"

class NuBeamSteppingActionMessenger;
class NuBeamOutput;

class NuBeamSteppingAction : public G4UserSteppingAction 
{
public:
  NuBeamSteppingAction();
  virtual ~NuBeamSteppingAction();
  
  virtual void UserSteppingAction(const G4Step*);

  void SetPerfectFocusingForPositives(G4bool valb) {fPerfectFocusingForPositives = valb;};
  G4bool GetPerfectFocusingForPositives() {return fPerfectFocusingForPositives;};

  void SetPerfectFocusingForNegatives(G4bool valb) {fPerfectFocusingForNegatives = valb;};
  G4bool GetPerfectFocusingForNegatives() {return fPerfectFocusingForNegatives;};

private:
  // Save the BooNEOutput object to be called after each step
  NuBeamOutput * fRecords;
  NuBeamSteppingActionMessenger* fMessenger;
  G4bool fPerfectFocusingForPositives;
  G4bool fPerfectFocusingForNegatives;

  struct G4HPExposer: G4HadronicProcess {
    using G4HadronicProcess::GetHadronicInteraction;
  };
};

#endif

