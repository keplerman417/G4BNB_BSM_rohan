#ifndef BooNEHadronZeroModel_h
#define BooNEHadronZeroModel_h 1

#include <vector>

#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleChange.hh"
#include "G4Track.hh"
#include "G4Nucleus.hh"

#include "G4HadronicInteraction.hh"

class BooNEHadronZeroModel : public G4HadronicInteraction
{
public:
  
  BooNEHadronZeroModel():G4HadronicInteraction("BooNEHadronZeroMode"){};

  ~BooNEHadronZeroModel(){};

  G4HadFinalState * ApplyYourself( const G4HadProjectile  &aTrack,
				   G4Nucleus&)
  {
    theParticleChange.Clear();
    
    // Set outgoing energy and momenum to incoming energy and momentum
    theParticleChange.SetEnergyChange(aTrack.GetKineticEnergy());
    theParticleChange.SetMomentumChange(aTrack.Get4Momentum().vect().unit());

    return &theParticleChange;
  }

private:

};
#endif
