#ifndef BooNEHadronQuasiElasticModel_h
#define BooNEHadronQuasiElasticModel_h 1

#include <vector>

#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "G4Ions.hh"
#include "G4ParticleTable.hh"
#include "G4NucleiProperties.hh"
#include "G4ParticleChange.hh"
#include "G4Track.hh"
#include "Randomize.hh"
#include "G4Nucleus.hh"
#include "G4IonTable.hh"

#include "G4HadronicInteraction.hh"


class BooNEHadronQuasiElasticModel : public G4HadronicInteraction
{
public:
  
  BooNEHadronQuasiElasticModel();

  ~BooNEHadronQuasiElasticModel() {;}

  G4HadFinalState * ApplyYourself( const G4HadProjectile  &aTrack,
				         G4Nucleus        &aNucleus);

  void SetProtonBetaParameters(G4double* values, G4int nvars)
                               { for(int i = 0; i < nvars; i++)protonBetaParameters.push_back(values[i]);    }
  void SetNeutronBetaParameters(G4double* values, G4int nvars)
                               { for(int i = 0; i < nvars; i++)neutronBetaParameters.push_back(values[i]);   }
  void SetPionPlusBetaParameters(G4double* values, G4int nvars)
                               { for(int i = 0; i < nvars; i++)pionPlusBetaParameters.push_back(values[i]);  }
  void SetPionMinusBetaParameters(G4double* values, G4int nvars)
                               { for(int i = 0; i < nvars; i++)pionMinusBetaParameters.push_back(values[i]); }

  void SetEtaBetaParameters(G4double* values, G4int nvars)
                               { for(int i = 0; i < nvars; i++)etaBetaParameters.push_back(values[i]); }


  void SetEtapBetaParameters(G4double* values, G4int nvars)
                               { for(int i = 0; i < nvars; i++)etapBetaParameters.push_back(values[i]); }


  G4double TanhWithGaussian(G4double p,
			    G4double tNorm, G4double tOffset, G4double tSlope, 
			    G4double gNorm, G4double gMean,   G4double gWidth);

private:

  // parameters describing the evolution of beta with incident momentum
  // protons
  std::vector<G4double>  protonBetaParameters;
  std::vector<G4double>  neutronBetaParameters;
  std::vector<G4double>  pionPlusBetaParameters;
  std::vector<G4double>  pionMinusBetaParameters;
  std::vector<G4double>  etaBetaParameters;
  std::vector<G4double>  etapBetaParameters;
  
  // function that actually returns beta
  G4double Beta(const G4HadProjectile* aTrack);
 
};
#endif
