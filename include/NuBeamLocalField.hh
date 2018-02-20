#ifndef NuBeamLocalField_H
#define NuBeamLocalField_H 1

#include "globals.hh"
#include "G4MagneticField.hh"
#include "G4UniformMagField.hh"

// #define  mu0  4.*M_PI*0.0000001;

class G4FieldManager;
class G4ChordFinder;
class G4Mag_UsualEqRhs;
class G4MagIntegratorStepper;
class NuBeamLocalFieldMessenger;

class NuBeamLocalField : public G4MagneticField
{
public:
  NuBeamLocalField(); 
  ~NuBeamLocalField();

  void GetFieldValue( const  G4double Point[4],
		      G4double *Bfield ) const;

  G4FieldManager*  GetLocalFieldManager() { return fLocalFieldManager ;}  

  void SetHornCurrent(G4double val){
    fHornCurrent = val;
  }

  // modifiers
  G4double GetHornCurrent() const {
    return fHornCurrent;
  }

private:

  G4double mu0;
  G4double fHornCurrent;

  NuBeamLocalFieldMessenger* fMessenger;

protected:

  G4FieldManager*         fLocalFieldManager;   
  G4Mag_UsualEqRhs*       fEquation ;    
  G4MagIntegratorStepper* fStepper ;
  G4int                   fStepperType ;

};

#endif



