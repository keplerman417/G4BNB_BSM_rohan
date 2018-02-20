#ifndef NuBeamSkinDepthField_H
#define NuBeamSkinDepthField_H 1

#include "globals.hh"
#include "G4MagneticField.hh"
#include "G4UniformMagField.hh"
#include "NuBeamSkinDepthFieldMessenger.hh"

class G4FieldManager;
class G4ChordFinder;
class G4Mag_UsualEqRhs;
class G4MagIntegratorStepper;

class NuBeamSkinDepthField : public G4MagneticField
{
public:
  NuBeamSkinDepthField(); 
  ~NuBeamSkinDepthField();
  
  void GetFieldValue( const  G4double Point[4],
		      G4double *Bfield ) const;


  void SetSkinDepthHornCurrent(G4double val){
    fSkinDepthHornCurrent = val;
  }

  void SetSkinDepth(G4double val){
    fSkinDepth = val;
  }

  void SetSkinDepthModel(G4String val){
    fSkinDepthModel = val;
  }

  G4double GetSkinDepthHornCurrent() const {
    return fSkinDepthHornCurrent;
  }

  G4double GetSkinDepth() const {
    return fSkinDepth;
  }

  const G4String GetSkinDepthModel() const {
    return fSkinDepthModel;
  }

  G4double GetRInnercm(const G4double zcm) const;
  G4double GetROutercm(const G4double zcm) const;
    

private:

  NuBeamSkinDepthFieldMessenger* fMessenger;

  G4double  mu0;
  G4double  fSkinDepthHornCurrent;
  G4double  fSkinDepth;
  G4String  fSkinDepthModel;

  G4int     Npoints;
  G4double  innerZ[100];
  G4double  innerR[100];
  G4double  outerZ[100];
  G4double  outerR[100];


protected:

  G4FieldManager*         fLocalFieldManager;   
  G4Mag_UsualEqRhs*       fEquation ;    
  G4MagIntegratorStepper* fStepper ;
  G4int                   fStepperType;

};

#endif



