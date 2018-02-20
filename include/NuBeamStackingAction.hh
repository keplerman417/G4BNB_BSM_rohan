#ifndef NuBeamStackingAction_H
#define NuBeamStackingAction_H 1

#include <fstream>
#include <map>
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4UserStackingAction.hh"

class G4Track;
class G4RunManager;

class NuBeamStackingAction : public G4UserStackingAction
{
public:
  NuBeamStackingAction();
  virtual ~NuBeamStackingAction();

  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
  virtual void NewStage();
  virtual void PrepareNewEvent();

  
  void KillEMParticles(G4ClassificationOfNewTrack& classification, 
		       const G4Track* aTrack);
  void KillThresholdParticles(G4ClassificationOfNewTrack& classification, 
			       const G4Track * aTrack);
  
private:
  G4RunManager * fRunManager; 
   
};

#endif

