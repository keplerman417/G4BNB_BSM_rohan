#ifndef BooNEHadronElasticPhysics_h
#define BooNEHadronElasticPhysics_h 1

#include "BooNEHadronElasticDataSet.hh"
#include "BooNEHadronElasticProcess.hh"

#include "G4VPhysicsConstructor.hh"
#include "G4HadronElastic.hh"

class BooNEHadronElasticPhysics : public G4VPhysicsConstructor
{
public: 
  BooNEHadronElasticPhysics(G4int verbose =1);
  virtual ~BooNEHadronElasticPhysics();
  
public: 
  virtual void ConstructParticle();
  virtual void ConstructProcess();

private:
  
  BooNEHadronElasticProcess fBooNEHadronElasticProcess;
  G4HadronElastic           fBooNEHadronElasticModel;
  BooNEHadronElasticDataSet fBooNEHadronElasticData;

};

#endif
