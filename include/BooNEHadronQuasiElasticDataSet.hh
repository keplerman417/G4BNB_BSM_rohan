#ifndef BooNEHadronQuasiElasticDataSet_h
#define BooNEHadronQuasiElasticDataSet_h 1

#include "G4VCrossSectionDataSet.hh"
#include "BooNEHadronCrossSections.hh"
#include "G4DynamicParticle.hh"
#include "G4Element.hh"


class BooNEHadronQuasiElasticDataSet : public G4VCrossSectionDataSet
{
public:

  BooNEHadronQuasiElasticDataSet():G4VCrossSectionDataSet("BooNEHadronQuasiElastic")
  {
    theHadronCrossSections = BooNEHadronCrossSections::Instance();
  }
  
  ~BooNEHadronQuasiElasticDataSet()
  {
  }

  G4bool IsIsoApplicable(const G4DynamicParticle* aParticle, G4int Z, G4int A,    
			 const G4Element* elm = 0,
			 const G4Material* mat = 0)
  {
    return theHadronCrossSections->IsApplicable(aParticle, Z, A);
  }

  G4double GetIsoCrossSection(const G4DynamicParticle* aParticle, G4int Z, G4int A,  
			      const G4Isotope* iso = 0,
			      const G4Element* elm = 0,
			      const G4Material* mat = 0)
  {
    return theHadronCrossSections->GetQuasiElasticCrossSection(aParticle, Z, A);
  }

  void BuildPhysicsTable(const G4ParticleDefinition&)
  {
  }
  
  void DumpPhysicsTable(const G4ParticleDefinition&)
  {
  }

  void DumpPhysicsTable2();
 

private:

   BooNEHadronCrossSections* theHadronCrossSections;
};

#endif
