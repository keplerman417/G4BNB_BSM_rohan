#ifndef BooNEHadronInelasticDataSet_h
#define BooNEHadronInelasticDataSet_h 1

#include "G4VCrossSectionDataSet.hh"
#include "BooNEHadronCrossSections.hh"
#include "G4DynamicParticle.hh"
#include "G4Element.hh"


class BooNEHadronInelasticDataSet : public G4VCrossSectionDataSet
{
public:

  BooNEHadronInelasticDataSet():G4VCrossSectionDataSet("BooNEHadronInelastic")
  {
    theHadronCrossSections = BooNEHadronCrossSections::Instance();
  }
  
  ~BooNEHadronInelasticDataSet()
  {
  }
  
  G4bool IsIsoApplicable(const G4DynamicParticle* aParticle, G4int Z, G4int A,    
			 const G4Element*,
			 const G4Material* ) override
  {
    bool ans=theHadronCrossSections->IsApplicable(aParticle, Z, A);
    //    G4cout <<" Iso applicable BooNE XSEC valid for "<<aParticle->GetKineticEnergy()/CLHEP::GeV<<" GeV "<<
    //aParticle->GetDefinition()->GetParticleName()<<" on Z="<<Z<<" A= "<<A<<"\t"<<ans<<G4endl;
    return ans;
  }
  G4bool IsElementApplicable(const G4DynamicParticle* aParticle, 
			     G4int Z, 			     
			     const G4Material* ) override
  {
    //    G4cout<<"Element applicable called first"<<G4endl;
    G4double A=0;
    if (Z==4) A=9;
    else if (Z==13) A=27;
    return IsIsoApplicable(aParticle,Z,A,NULL,NULL);
  }
  
  G4double GetIsoCrossSection(const G4DynamicParticle* aParticle, G4int Z, G4int A,  
			      const G4Isotope*,
			      const G4Element*,
			      const G4Material*)
  {
    return theHadronCrossSections->GetInelasticCrossSection(aParticle, Z, A);
  }

  G4double GetElementCrossSection(const G4DynamicParticle* aParticle, 
			     G4int Z, 			     
			     const G4Material* )
  {
    G4double A=0;
    if (Z==4) A=9;
    else if (Z==13) A=27;
    return theHadronCrossSections->GetInelasticCrossSection(aParticle, Z, A);
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
