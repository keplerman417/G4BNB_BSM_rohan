#ifndef BooNEHadronQuasiElasticProcess_h
#define BooNEHadronQuasiElasticProcess_h 1
 
#include "globals.hh"
#include "G4HadronicProcess.hh"
#include "G4CrossSectionDataStore.hh"
#include "G4HadronElasticDataSet.hh"
#include "G4Element.hh"
#include "G4ElementVector.hh"
#include "G4VDiscreteProcess.hh"
#include "G4LightMedia.hh"
#include "G4Step.hh"
#include "G4TrackStatus.hh"


class BooNEHadronQuasiElasticProcess : public G4HadronicProcess
{
public:

   BooNEHadronQuasiElasticProcess(const G4String& processName = "BooNEHadronQuasiElastic");

   ~BooNEHadronQuasiElasticProcess();
 
   inline

   G4VParticleChange* PostStepDoIt(const G4Track& aTrack, const G4Step& aStep)
   {
      return G4HadronicProcess::PostStepDoIt(aTrack, aStep);
   }

   G4bool IsApplicable(const G4ParticleDefinition& aParticleType);

   void BuildPhysicsTable(const G4ParticleDefinition& aParticleType);

   void DumpPhysicsTable(const G4ParticleDefinition& aParticleType);

private:

   G4double GetMicroscopicCrossSection(const G4DynamicParticle* aParticle,
                                       const G4Element* anElement,
				       const G4Material* aMaterial);
  
};
#endif
