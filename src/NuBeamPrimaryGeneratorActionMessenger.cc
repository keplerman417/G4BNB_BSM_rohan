#include "NuBeamPrimaryGeneratorActionMessenger.hh"
#include "NuBeamPrimaryGeneratorAction.hh"

#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIdirectory.hh"

NuBeamPrimaryGeneratorActionMessenger::NuBeamPrimaryGeneratorActionMessenger(
                                          NuBeamPrimaryGeneratorAction* val):
  fNuBeamAction(val)
{ 
  fPrimaryDirectory = new G4UIdirectory("/boone/primary/");
  fPrimaryDirectory->SetGuidance( "Primary beam particle commands.");

  fPrimaryMeanxCmd = new G4UIcmdWithADoubleAndUnit("/boone/primary/mean_x",this);
  fPrimaryMeanxCmd->SetGuidance("Mean of gaussian x distribution of primary particle");
  fPrimaryMeanxCmd->SetGuidance(
			       "Available options: any floating-point number, with a valid unit for the position (e.g. mm, cm, m, etc.)");
  fPrimaryMeanxCmd->SetParameterName("mx", true, false);
  fPrimaryMeanxCmd->SetDefaultUnit("mm");
  fPrimaryMeanxCmd->SetDefaultValue(0.);

  fPrimaryMeanyCmd = new G4UIcmdWithADoubleAndUnit("/boone/primary/mean_y",this);
  fPrimaryMeanyCmd->SetGuidance("Mean of gaussian y distribution of primary particle");
  fPrimaryMeanyCmd->SetGuidance(
			       "Available options: any floating-point number, with a valid unit for the position (e.g. mm, cm, m, etc.)");
  fPrimaryMeanyCmd->SetParameterName("my", true, false);
  fPrimaryMeanyCmd->SetDefaultUnit("mm");
  fPrimaryMeanyCmd->SetDefaultValue(0.);

  fPrimarySigmaxCmd = new G4UIcmdWithADoubleAndUnit("/boone/primary/sigma_x",this);
  fPrimarySigmaxCmd->SetGuidance("RMS on gaussian x distribution of primary particle");
  fPrimarySigmaxCmd->SetGuidance(
				 "Available options: any positive floating-point number, with a valid unit for the position (e.g. mm, cm, m, etc.)");
  fPrimarySigmaxCmd->SetParameterName("sx", true, false);
  fPrimarySigmaxCmd->SetRange("sx>=0.");
  fPrimarySigmaxCmd->SetDefaultUnit("mm");
  fPrimarySigmaxCmd->SetDefaultValue(1.51);

  fPrimarySigmayCmd = new G4UIcmdWithADoubleAndUnit("/boone/primary/sigma_y",this);
  fPrimarySigmayCmd->SetGuidance("RMS on gaussian y distribution of primary particle");
  fPrimarySigmayCmd->SetGuidance(
			       "Available options: any positive floating-point number, with a valid unit for the position (e.g. mm, cm, m, etc.)");
  fPrimarySigmayCmd->SetParameterName("sy", true, false);
  fPrimarySigmayCmd->SetRange("sy>=0.");
  fPrimarySigmayCmd->SetDefaultUnit("mm");
  fPrimarySigmayCmd->SetDefaultValue(0.75);

  fPrimaryMeanThetaxCmd = new G4UIcmdWithADoubleAndUnit("/boone/primary/mean_thetax",this);
  fPrimaryMeanThetaxCmd->SetGuidance(
				     "Mean of gaussian Thetax=atan(px/pz) distribution of primary particle");
  fPrimaryMeanThetaxCmd->SetGuidance(
			       "Available options: any floating-point number, with a valid unit for the angle (e.g. deg, rad, etc.)");
  fPrimaryMeanThetaxCmd->SetParameterName("mThetax", true, false);
  fPrimaryMeanThetaxCmd->SetDefaultUnit("mrad");
  fPrimaryMeanThetaxCmd->SetDefaultValue(0.);

  fPrimaryMeanThetayCmd = new G4UIcmdWithADoubleAndUnit("/boone/primary/mean_thetay",this);
  fPrimaryMeanThetayCmd->SetGuidance("Mean of gaussian Thetay=atan(py/pz) distribution of primary particle");
  fPrimaryMeanThetayCmd->SetGuidance(
			       "Available options: any floating-point number, with a valid unit for the angle (e.g. deg, rad, etc.)");
  fPrimaryMeanThetayCmd->SetParameterName("mThetay", true, false);
  fPrimaryMeanThetayCmd->SetDefaultUnit("mrad");
  fPrimaryMeanThetayCmd->SetDefaultValue(0.);

  fPrimarySigmaThetaxCmd = new G4UIcmdWithADoubleAndUnit("/boone/primary/sigma_thetax",this);
  fPrimarySigmaThetaxCmd->SetGuidance(
				     "RMS on gaussian Thetax=atan(px/pz) distribution of primary particle");
  fPrimarySigmaThetaxCmd->SetGuidance(
			       "Available options: any positive floating-point number, with a valid unit for the angle (e.g. deg, rad, etc.)");
  fPrimarySigmaThetaxCmd->SetParameterName("sThetax", true, false);
  fPrimarySigmaThetaxCmd->SetRange("sThetax>=0.");
  fPrimarySigmaThetaxCmd->SetDefaultUnit("mrad");
  fPrimarySigmaThetaxCmd->SetDefaultValue(0.66);

  fPrimarySigmaThetayCmd = new G4UIcmdWithADoubleAndUnit("/boone/primary/sigma_thetay",this);
  fPrimarySigmaThetayCmd->SetGuidance("RMS on gaussian Thetay=atan(py/pz) distribution of primary particle");
  fPrimarySigmaThetayCmd->SetGuidance(
			       "Available options: any positive floating-point number, with a valid unit for the angle (e.g. deg, rad, etc.)");
  fPrimarySigmaThetayCmd->SetParameterName("sThetay", true, false);
  fPrimarySigmaThetayCmd->SetRange("sThetay>=0.");
  fPrimarySigmaThetayCmd->SetDefaultUnit("mrad");
  fPrimarySigmaThetayCmd->SetDefaultValue(0.40);

  fPrimaryCorrXThetaxCmd = new G4UIcmdWithADouble("/boone/primary/corr_xthetax",this);
  fPrimaryCorrXThetaxCmd->SetGuidance("Correlation Coefficient between x position and x direction");
  fPrimaryCorrXThetaxCmd->SetGuidance(
			       "Available options: any positive floating-point number, unitless");
  fPrimaryCorrXThetaxCmd->SetParameterName("corrXThetax", true, false);
  fPrimaryCorrXThetaxCmd->SetRange("corrXThetax>=-1.&&corrXThetax<=1.");
  fPrimaryCorrXThetaxCmd->SetDefaultValue(0.0);

  fPrimaryCorrYThetayCmd = new G4UIcmdWithADouble("/boone/primary/corr_ythetay",this);
  fPrimaryCorrYThetayCmd->SetGuidance("Correlation Coefficient between y position and y direction");
  fPrimaryCorrYThetayCmd->SetGuidance(
			       "Available options: any positive floating-point number, unitless");
  fPrimaryCorrYThetayCmd->SetParameterName("corrYThetay", true, false);
  fPrimaryCorrYThetayCmd->SetRange("corrYThetay>=-1.&&corrYThetay<=1.");
  fPrimaryCorrYThetayCmd->SetDefaultValue(0.0);

  fPrimaryZPositionCmd = new G4UIcmdWithADoubleAndUnit("/boone/primary/zPosition",this);
  fPrimaryZPositionCmd->SetGuidance("z position at birth of primary particle");
  fPrimaryZPositionCmd->SetGuidance(
			       "Available options: any floating-point number, with a valid unit for the position (e.g. mm, cm, m, etc.)");
  fPrimaryZPositionCmd->SetGuidance(" Note: the uptream face of the target in the standard MiniBooNE geometry is at approximately z=3.5 cm");
  fPrimaryZPositionCmd->SetParameterName("z", true, false);
  fPrimaryZPositionCmd->SetDefaultUnit("cm");
  fPrimaryZPositionCmd->SetDefaultValue(-1.);
}

NuBeamPrimaryGeneratorActionMessenger::~NuBeamPrimaryGeneratorActionMessenger()
{
  delete fPrimaryMeanxCmd;
  delete fPrimaryMeanyCmd;
  delete fPrimarySigmaxCmd;
  delete fPrimarySigmayCmd;
  delete fPrimaryMeanThetaxCmd;
  delete fPrimaryMeanThetayCmd;
  delete fPrimarySigmaThetaxCmd;
  delete fPrimarySigmaThetayCmd;
  delete fPrimaryCorrXThetaxCmd;
  delete fPrimaryCorrYThetayCmd;
  delete fPrimaryDirectory;
}

void NuBeamPrimaryGeneratorActionMessenger::SetNewValue(
						       G4UIcommand* command,
						       G4String newValue)
{ 
  if(command == fPrimaryMeanxCmd)
    fNuBeamAction->
      SetPrimaryMeanX(fPrimaryMeanxCmd->GetNewDoubleValue(newValue));

  if(command == fPrimaryMeanyCmd)
    fNuBeamAction->
      SetPrimaryMeanY(fPrimaryMeanyCmd->GetNewDoubleValue(newValue));

  if(command == fPrimarySigmaxCmd)
    fNuBeamAction->
      SetPrimarySigmaX(fPrimarySigmaxCmd->GetNewDoubleValue(newValue));

  if(command == fPrimarySigmayCmd)
    fNuBeamAction->
      SetPrimarySigmaY(fPrimarySigmayCmd->GetNewDoubleValue(newValue));

  if(command == fPrimaryMeanThetaxCmd)
    fNuBeamAction->
      SetPrimaryMeanThetaX(fPrimaryMeanThetaxCmd->GetNewDoubleValue(newValue));

  if(command == fPrimaryMeanThetayCmd)
    fNuBeamAction->
      SetPrimaryMeanThetaY(fPrimaryMeanThetayCmd->GetNewDoubleValue(newValue));

  if(command == fPrimarySigmaThetaxCmd)
    fNuBeamAction->
      SetPrimarySigmaThetaX(fPrimarySigmaThetaxCmd->GetNewDoubleValue(newValue));

  if(command == fPrimarySigmaThetayCmd)
    fNuBeamAction->
      SetPrimarySigmaThetaY(fPrimarySigmaThetayCmd->GetNewDoubleValue(newValue));

  if(command == fPrimaryCorrXThetaxCmd)
    fNuBeamAction->
      SetPrimaryCorrXThetaX(fPrimaryCorrXThetaxCmd->GetNewDoubleValue(newValue));

  if(command == fPrimaryCorrYThetayCmd)
    fNuBeamAction->
      SetPrimaryCorrYThetaY(fPrimaryCorrYThetayCmd->GetNewDoubleValue(newValue));

  if(command == fPrimaryZPositionCmd)
    fNuBeamAction->
      SetPrimaryZPosition(fPrimaryZPositionCmd->GetNewDoubleValue(newValue));
}

