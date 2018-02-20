#include "globals.hh"
#include "Randomize.hh"

#include "NuBeamPrimaryGeneratorAction.hh"
#include "NuBeamPrimaryGeneratorActionMessenger.hh"
#include "NuBeamGeometryConstruction.hh"
#include "NuBeamRunManager.hh"
#include "NuBeamTrajectory.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleMomentum.hh"
#include "G4Material.hh"
#include "G4UImanager.hh"

NuBeamPrimaryGeneratorAction::NuBeamPrimaryGeneratorAction()
{
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);

  //create a messenger for this class
  fMessenger = new NuBeamPrimaryGeneratorActionMessenger(this);

  G4UImanager* UI = G4UImanager::GetUIpointer();  

  UI->ApplyCommand("/boone/primary/mean_x");
  UI->ApplyCommand("/boone/primary/mean_y");
  UI->ApplyCommand("/boone/primary/z");
  UI->ApplyCommand("/boone/primary/sigma_x");
  UI->ApplyCommand("/boone/primary/sigma_y");
  UI->ApplyCommand("/boone/primary/mean_thetax");
  UI->ApplyCommand("/boone/primary/mean_thetay");
  UI->ApplyCommand("/boone/primary/sigma_thetax");
  UI->ApplyCommand("/boone/primary/sigma_thetay");
  UI->ApplyCommand("/boone/primary/corr_xthetax");
  UI->ApplyCommand("/boone/primary/corr_ythetay"); 

  //need this to be defined when initializing BooNEpBe
  UI->ApplyCommand("/gun/particle");
  UI->ApplyCommand("/gun/energy");

  fFirstEvent = true;
}

NuBeamPrimaryGeneratorAction::~NuBeamPrimaryGeneratorAction()
{
  delete fParticleGun;
  delete fMessenger;
}

void NuBeamPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {

  if ( fFirstEvent ) {  
  
    G4double mass=fParticleGun->GetParticleDefinition()->GetPDGMass();
    G4double totalE=mass+fParticleGun->GetParticleEnergy();
    G4double p=sqrt(totalE*totalE-mass*mass);
    fParticleGun->SetParticleMomentum(p);

    G4cout << "Primary particle "
	   << fParticleGun->GetParticleDefinition()->GetParticleName()<<G4endl;
    G4cout << "Primary Beam Energy: " 
           << fParticleGun->GetParticleEnergy()/CLHEP::GeV << " GeV" << G4endl;
    G4cout << "Primary Beam Momentum: " 
           << fParticleGun->GetParticleMomentum()/CLHEP::GeV << " GeV" << G4endl;
    G4cout << "Primary Beam Charge: " 
           << fParticleGun->GetParticleCharge()<< G4endl;
    fFirstEvent = false;
  }
 
  G4ThreeVector position  = GetGaussianPosition();
  G4ThreeVector direction = GetGaussianDirection(position);
  
  fParticleGun->SetParticleMomentumDirection(direction.unit());
  fParticleGun->SetParticlePosition( position );
  fParticleGun->SetParticlePolarization(G4ThreeVector(0.,0.,0.));

  fParticleGun->GeneratePrimaryVertex(anEvent);
}

G4ThreeVector NuBeamPrimaryGeneratorAction::GetGaussianDirection(G4ThreeVector position) 
{
  G4ThreeVector retval;

  // get generated x and y positions
  G4double xPosition = position.x();
  G4double yPosition = position.y();

  // mean and sigmas after given the drawn X,Y position
  G4double meanThetaX2 = fMeanThetaXValue + fCorrXThetaXValue*(fSigmaThetaXValue/fSigmaXValue)*(xPosition-fMeanXValue);
  G4double meanThetaY2 = fMeanThetaYValue + fCorrYThetaYValue*(fSigmaThetaYValue/fSigmaYValue)*(yPosition-fMeanYValue);
  G4double sigmaThetaX2 = fSigmaThetaXValue*sqrt(1-fCorrXThetaXValue*fCorrXThetaXValue);
  G4double sigmaThetaY2 = fSigmaThetaYValue*sqrt(1-fCorrYThetaYValue*fCorrYThetaYValue);

  // now draw from correlated distribution
  G4double randomThetaX = G4RandGauss::shoot(meanThetaX2, sigmaThetaX2);
  G4double randomThetaY = G4RandGauss::shoot(meanThetaY2, sigmaThetaY2);

  // Real px, py
  G4double ptot = fParticleGun->GetParticleMomentum();  
  G4double px = ptot*randomThetaX;
  G4double py = ptot*randomThetaY;
  
  // keep total momentum constant
  G4double pz = sqrt(ptot*ptot - px*px - py*py);

  retval.setX(px);
  retval.setY(py);
  retval.setZ(pz);

  return retval;
}

G4ThreeVector NuBeamPrimaryGeneratorAction::GetGaussianPosition() 
{
  G4ThreeVector retval;

  G4double x = G4RandGauss::shoot(fMeanXValue,fSigmaXValue);
  G4double y = G4RandGauss::shoot(fMeanYValue,fSigmaYValue);
  G4double z = fZPositionValue;

  retval.setX(x);
  retval.setY(y);
  retval.setZ(z);

  return retval;

}
