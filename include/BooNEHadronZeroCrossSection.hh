#ifndef BooNEHadronZeroCrossSection_h
#define BooNEHadronZeroCrossSection_h 1

#include "globals.hh"
#include "G4VCrossSectionDataSet.hh"
#include "G4DynamicParticle.hh"
#include "G4Element.hh"
#include "G4ios.hh"
#include "G4Proton.hh"
#include "G4Neutron.hh"
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"
#include "G4KaonPlus.hh"
#include "G4KaonMinus.hh"
#include "G4KaonZeroLong.hh"

class BooNEHadronZeroCrossSection : public G4VCrossSectionDataSet
{
public:

  BooNEHadronZeroCrossSection():G4VCrossSectionDataSet("BooNEZeroCrossSection"){};
  ~BooNEHadronZeroCrossSection(){};

  G4bool IsIsoApplicable(const G4DynamicParticle*, G4int, G4int,    
			 const G4Element*,
			 const G4Material*)
  {return true;};
  
  G4double GetIsoCrossSection(const G4DynamicParticle*, G4int, G4int,  
			      const G4Isotope*,
			      const G4Element*,
			      const G4Material*)
  {return 0;};

  void BuildPhysicsTable(const G4ParticleDefinition&){};
  void DumpPhysicsTable(const G4ParticleDefinition&){};

};

#endif
