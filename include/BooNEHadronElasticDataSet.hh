#ifndef BooNEHadronElasticDataSet_h
#define BooNEHadronElasticDataSet_h 1

#include "G4VCrossSectionDataSet.hh"
#include "BooNEHadronCrossSections.hh"
#include "G4DynamicParticle.hh"
#include "G4Element.hh"


class BooNEHadronElasticDataSet : public G4VCrossSectionDataSet
{
public:

  BooNEHadronElasticDataSet():G4VCrossSectionDataSet("BooNEHadronElastic")
  {
    theHadronCrossSections = BooNEHadronCrossSections::Instance();
  }
  
  ~BooNEHadronElasticDataSet()
  {
  }

  G4bool IsIsoApplicable(const G4DynamicParticle* aParticle, G4int Z, G4int A,
			 const G4Element*,
			 const G4Material*)
  {
    return theHadronCrossSections->IsApplicable(aParticle, Z, A);
  }
  
  G4double GetIsoCrossSection(const G4DynamicParticle* aParticle, G4int Z, G4int A,  
			      const G4Isotope*,
			      const G4Element*,
			      const G4Material*)
  {
    return theHadronCrossSections->GetElasticCrossSection(aParticle, Z, A);
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
