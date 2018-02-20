#ifndef NuBeamPrimaryGeneratorAction_h
#define NuBeamPrimaryGeneratorAction_h 1

#include "globals.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"

#include "NuBeamTrajectory.hh"

class NuBeamPrimaryGeneratorActionMessenger;

class NuBeamPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  NuBeamPrimaryGeneratorAction();
  ~NuBeamPrimaryGeneratorAction();

  void SetPrimaryMeanX(G4double val){fMeanXValue = val;}
  void SetPrimaryMeanY(G4double val){fMeanYValue = val;}
  
  G4double GetPrimaryMeanX() const {return fMeanXValue;}
  G4double GetPrimaryMeanY() const {return fMeanYValue;}

  void SetPrimarySigmaX(G4double val){fSigmaXValue = val;}
  void SetPrimarySigmaY(G4double val){fSigmaYValue = val;}

  G4double GetPrimarySigmaX() const {return fSigmaXValue;}
  G4double GetPrimarySigmaY() const {return fSigmaYValue;}

  void SetPrimaryMeanThetaX(G4double val){fMeanThetaXValue = val;}
  void SetPrimaryMeanThetaY(G4double val){fMeanThetaYValue = val;}

  G4double GetPrimaryMeanThetaX() const {return fMeanThetaXValue;}
  G4double GetPrimaryMeanThetaY() const {return fMeanThetaYValue;}

  void SetPrimarySigmaThetaX(G4double val){fSigmaThetaXValue = val;}
  void SetPrimarySigmaThetaY(G4double val){fSigmaThetaYValue = val;}

  G4double GetPrimarySigmaThetaX() const {return fSigmaThetaXValue;}
  G4double GetPrimarySigmaThetaY() const {return fSigmaThetaYValue;}

  void SetPrimaryCorrXThetaX(G4double val){fCorrXThetaXValue = val;}
  void SetPrimaryCorrYThetaY(G4double val){fCorrYThetaYValue = val;}

  G4double GetPrimaryCorrXThetaX() const {return fCorrXThetaXValue;}
  G4double GetPrimaryCorrXThetaY() const {return fCorrYThetaYValue;}

  void SetPrimaryZPosition(G4double val){fZPositionValue = val;}
  G4double GetPrimaryZPosition() const {return fZPositionValue;}

  G4ParticleGun* GetParticleGun() const {return fParticleGun;}

  void GeneratePrimaries(G4Event* anEvent);

private:
  G4ParticleGun* fParticleGun;

  NuBeamPrimaryGeneratorActionMessenger* fMessenger; //messenger of this class
  
  G4ThreeVector GetRandomDirection();
  G4ThreeVector GetGaussianDirection(G4ThreeVector position);
  G4ThreeVector GetGaussianPosition();

  G4double fMeanXValue;
  G4double fMeanYValue;
  G4double fSigmaXValue;
  G4double fSigmaYValue;
  G4double fMeanThetaXValue;
  G4double fMeanThetaYValue;
  G4double fSigmaThetaXValue;
  G4double fSigmaThetaYValue;
  G4double fCorrXThetaXValue;
  G4double fCorrYThetaYValue;

  G4double fZPositionValue;

  G4bool fFirstEvent;

};

#endif
