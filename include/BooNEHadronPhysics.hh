#ifndef BooNEHadronPhysics_h
#define BooNEHadronPhysics_h 1

#include "globals.hh"
#include "G4VPhysicsConstructor.hh"

class BooNEHadronInelasticProcess;
class G4AntiBarionBuilder;
class G4FTFPAntiBarionBuilder;
class G4HyperonFTFPBuilder;

class BooNEHadronPhysics : public G4VPhysicsConstructor
{
public: 
  BooNEHadronPhysics(G4int verbose =1);
  virtual ~BooNEHadronPhysics();
  
public: 
  virtual void ConstructParticle();
  virtual void ConstructProcess();

private:

  BooNEHadronInelasticProcess*     fBooNEHadronInelastic;
  G4AntiBarionBuilder*             fAntiBarionBuilder;
  G4FTFPAntiBarionBuilder*         fFTFPAntiBarion;
  G4HyperonFTFPBuilder*            fHyperonBuilder;
};

#endif
