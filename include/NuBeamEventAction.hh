#ifndef NuBeamEventAction_h
#define NuBeamEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

#include "NuBeamOutput.hh"

class G4Event;
class NuBeamEventActionMessenger;

class NuBeamEventAction : public G4UserEventAction
{
public:

  NuBeamEventAction();
  virtual ~NuBeamEventAction();

  virtual void BeginOfEventAction(const G4Event* );
  virtual void EndOfEventAction(const G4Event* );

  void SetPrintModulo(G4int val) {fPrintModulo = val;};

private:

  NuBeamOutput * fRecords;
  G4int          fPrintModulo;
  NuBeamEventActionMessenger* fMessenger;

};

#endif
