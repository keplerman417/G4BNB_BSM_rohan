#ifndef NuBeamPrimaryGeneratorActionMessenger_h
#define NuBeamPrimaryGeneratorActionMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class G4UIdirectory;
class NuBeamPrimaryGeneratorAction;
class G4UIcmdWithAString;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWith3VectorAndUnit;

class NuBeamPrimaryGeneratorActionMessenger: public G4UImessenger
{
  public:
    NuBeamPrimaryGeneratorActionMessenger(NuBeamPrimaryGeneratorAction*);
   ~NuBeamPrimaryGeneratorActionMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
  NuBeamPrimaryGeneratorAction* fNuBeamAction; 

  G4UIdirectory*             fPrimaryDirectory;
  G4UIcmdWithADoubleAndUnit* fPrimaryMeanxCmd;
  G4UIcmdWithADoubleAndUnit* fPrimaryMeanyCmd;
  G4UIcmdWithADoubleAndUnit* fPrimarySigmaxCmd;
  G4UIcmdWithADoubleAndUnit* fPrimarySigmayCmd;
  G4UIcmdWithADoubleAndUnit* fPrimaryMeanThetaxCmd;
  G4UIcmdWithADoubleAndUnit* fPrimaryMeanThetayCmd;
  G4UIcmdWithADoubleAndUnit* fPrimarySigmaThetaxCmd;
  G4UIcmdWithADoubleAndUnit* fPrimarySigmaThetayCmd;
  G4UIcmdWithADouble*        fPrimaryCorrXThetaxCmd;
  G4UIcmdWithADouble*        fPrimaryCorrYThetayCmd;
  G4UIcmdWithADoubleAndUnit* fPrimaryZPositionCmd;
 
};

#endif

