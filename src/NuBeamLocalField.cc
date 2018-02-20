#include "NuBeamLocalField.hh"
#include "NuBeamLocalFieldMessenger.hh"
#include "G4UniformMagField.hh"
#include "G4MagneticField.hh"
#include "G4FieldManager.hh"
#include "NuBeamSteppingAction.hh"

#include "G4TransportationManager.hh"
#include "G4ChordFinder.hh"
#include "G4PropagatorInField.hh"
#include "G4FieldTrack.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4ClassicalRK4.hh"
#include "G4UImanager.hh"

NuBeamLocalField::NuBeamLocalField()
  :fMessenger(0)
{
  fMessenger = new NuBeamLocalFieldMessenger(this);

  G4UImanager* UI = G4UImanager::GetUIpointer();

  UI->ApplyCommand("/boone/field/horncurrent");

  fEquation = new G4Mag_UsualEqRhs(this); 
  fStepperType = 4 ;      // ClassicalRK4 is default stepper 

  mu0 = 4.*M_PI*0.0000001;
}


NuBeamLocalField::~NuBeamLocalField()
{
  delete fMessenger;
  fMessenger = 0;
  if (fStepper) delete fStepper;
}

void NuBeamLocalField::GetFieldValue(const G4double Point[4],G4double *Bfield) const
{   
  Bfield[0] =-(mu0/(2.*M_PI))*(fHornCurrent/CLHEP::ampere)*
    (Point[1]/CLHEP::m)/((Point[0]*Point[0]+Point[1]*Point[1])/CLHEP::m2)*CLHEP::tesla;
  Bfield[1] = (mu0/(2.*M_PI))*(fHornCurrent/CLHEP::ampere)*
    (Point[0]/CLHEP::m)/((Point[0]*Point[0]+Point[1]*Point[1])/CLHEP::m2)*CLHEP::tesla;
  Bfield[2] = 0.;
}








