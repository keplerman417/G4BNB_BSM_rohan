#include "NuBeamPhysicsList.hh"
#include "BooNEDecayPhysics.hh"
#include "BooNEHadronPhysics.hh"
#include "BooNEHadronElasticPhysics.hh"

#include "G4ios.hh"
#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4HadronPhysicsNuBeam.hh"

#include "G4DecayTable.hh"
#include "G4VDecayChannel.hh"
#include "G4KL3DecayChannel.hh"
#include "G4PhaseSpaceDecayChannel.hh"

NuBeamPhysicsList::NuBeamPhysicsList():  G4VModularPhysicsList()
{
  // default cut value  (0.7mm) 
  this->defaultCutValue = 0.7*CLHEP::mm;  

  int ver=1; //verbosity
  this->SetVerboseLevel(ver);
 
  // EM Physics
  this->RegisterPhysics( new G4EmStandardPhysics(ver));
  
  // Synchroton Radiation & GN Physics
  this->RegisterPhysics( new G4EmExtraPhysics(ver) );
  
  // Decays 
  this->RegisterPhysics( new BooNEDecayPhysics(ver) );

  /* 
  //GEANT4 models
  // Hadron Elastic scattering
  this->RegisterPhysics( new G4HadronElasticPhysics(ver) );
  
  // Hadron Physics
  this->RegisterPhysics( new G4HadronPhysicsNuBeam(ver));
  */

  //BooNE elastic&inelastic models
  this->RegisterPhysics( new BooNEHadronElasticPhysics(ver) );

  this->RegisterPhysics( new BooNEHadronPhysics(ver));

  // Stopping Physics
  this->RegisterPhysics( new G4StoppingPhysics(ver) );

  // Ion Physics
  this->RegisterPhysics( new G4IonPhysics(ver));
  
  // Neutron tracking cut
  this->RegisterPhysics( new G4NeutronTrackingCut(ver));

}

NuBeamPhysicsList::~NuBeamPhysicsList()
{
  G4cout << "Destroy physics list." << G4endl;
}

void NuBeamPhysicsList::SetCuts()
{
  this->SetCutsWithDefault();   
  
  if (this->verboseLevel>1)
    G4VUserPhysicsList::DumpCutValuesTable();
}

