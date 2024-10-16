#include "BooNEpBeInteractionMessenger.hh"
#include "BooNEpBeInteraction.hh"

#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithABool.hh"
#include "G4ios.hh"
#include "globals.hh"
#include "G4UIdirectory.hh"
#include "G4Tokenizer.hh"

#include <sstream>

BooNEpBeInteractionMessenger::BooNEpBeInteractionMessenger(BooNEpBeInteraction* model)
{ 
  BooNEProtonModel = model;

  fModelEnergyRangeCmd = 
    new G4UIcommand("/boone/physics/BooNEpBeModelEnergyRange",this);
  fModelEnergyRangeCmd->SetGuidance("Set energy range where BooNE pBe model is applicable");
  fModelEnergyRangeCmd->SetGuidance("Usage: /boone/physics/BooNEpBeModelEnergyRange Material Elow Ehigh");
  fModelEnergyRangeCmd->SetGuidance("Description:  Elow is lower kinetic energy limit and Ehigh is high kinetic energy limit in GeV");
  
  G4UIparameter* epar;
  epar = new G4UIparameter("BooNEpBeElow",'d',true);
  epar->SetParameterRange("BooNEpBeElow>=0.");
  epar->SetDefaultValue(7.5);
  fModelEnergyRangeCmd->SetParameter(epar);
  epar = new G4UIparameter("BooNEpBeEhigh",'d',true);
  epar->SetParameterRange("BooNEpBeEhigh>=0.");
  epar->SetDefaultValue(8.5);
  fModelEnergyRangeCmd->SetParameter(epar);

  NoBeamPionsCmd = new G4UIcmdWithABool("/boone/physics/noBeamPions",this);
  NoBeamPionsCmd->SetGuidance("This flag prevents the production of charged pions from p-Be interactions.");
  NoBeamPionsCmd->SetParameterName("nobeampions",true,false);
  NoBeamPionsCmd->SetDefaultValue(false);

  //pi+ physics model
  PiPlusPhysicsModelCmd = new G4UIcmdWithAString("/boone/physics/piPlusModel",this);
  PiPlusPhysicsModelCmd->SetGuidance("physics model for pi+ production in p-Be interaction");
  PiPlusPhysicsModelCmd->SetGuidance("Description:");
  PiPlusPhysicsModelCmd->SetGuidance("MARS: pi+ production in p-target interactions according to MARS ( http://waldo.fnal.gov/MARS/ )");
  PiPlusPhysicsModelCmd->SetGuidance("GFLUKA: pi+ production in p-target interactions according to GFLUKA ( http://alisoft.cern.ch/offline/geant3.html )");
  PiPlusPhysicsModelCmd->SetGuidance("ZGS: pi+ production in p-target interactions according to a Sanford-Wang parametrization of Argonne's ZGS data ( http://cornell.mirror.aps.org/abstract/PRD/v4/i7/p1967_1 )");
  PiPlusPhysicsModelCmd->SetGuidance("SWPar: pi+ production in p-target interactions according to a Sanford-Wang parametrization, chosen by the user via the command SWPiPlusPar.");
  PiPlusPhysicsModelCmd->SetGuidance("G4DEFAULT: pi+ production in p-target interactions according to G4 Default model");
  PiPlusPhysicsModelCmd->SetParameterName("physics", true, false);
  PiPlusPhysicsModelCmd->SetDefaultValue("SWPar");
  PiPlusPhysicsModelCmd->SetCandidates("MARS GFLUKA ZGS SWPar G4DEFAULT");
  //
  //pi- physics model
  PiMinusPhysicsModelCmd = new G4UIcmdWithAString("/boone/physics/piMinusModel",this);
  PiMinusPhysicsModelCmd->SetGuidance("physics model for pi- production in p-Be interaction");
  PiMinusPhysicsModelCmd->SetGuidance("Description:");
  PiMinusPhysicsModelCmd->SetGuidance("MARS: pi- production in p-target interactions according to MARS");
  PiMinusPhysicsModelCmd->SetGuidance("GFLUKA: pi- production in p-target interactions according to GFLUKA");
  PiMinusPhysicsModelCmd->SetGuidance("ZGS: pi- production in p-target interactions according to a Sanford-Wang parametrization of Argonne's ZGS data");
  PiMinusPhysicsModelCmd->SetGuidance("SWPar: pi- production in p-target interactions according to a Sanford-Wang parametrization, chosen by the user via the command SWPiMinusPar.");
  PiMinusPhysicsModelCmd->SetGuidance("G4DEFALT: pi- production in p-target interactions according to G4 default model");
  PiMinusPhysicsModelCmd->SetParameterName("physics", true, false);
  PiMinusPhysicsModelCmd->SetDefaultValue("SWPar");
  PiMinusPhysicsModelCmd->SetCandidates("MARS GFLUKA ZGS SWPar G4DEFAULT");
  //
  // K+ physics model
  KaonPlusPhysicsModelCmd = new G4UIcmdWithAString("/boone/physics/kaonPlusModel",this);
  KaonPlusPhysicsModelCmd->SetGuidance("physics model for K+ production in p-Be interaction");
  KaonPlusPhysicsModelCmd->SetGuidance("Description:");
  KaonPlusPhysicsModelCmd->SetGuidance("MARS: K+ production in p-target interactions according to MARS");
  KaonPlusPhysicsModelCmd->SetGuidance("GFLUKA: K+ production in p-target interactions according to GFLUKA");
  KaonPlusPhysicsModelCmd->SetGuidance("SWPar: K+ production in p-target interactions according to a Sanford-Wang parametrization, chosen by the user via the command SWKaonPlusPar.");
  KaonPlusPhysicsModelCmd->SetGuidance("FScal: K+ production in p-target interactions according to a Feynman Scaling parametrization (as described in Tech Note 209), chosen by the user via the command FSKaonPlusPar.");
  KaonPlusPhysicsModelCmd->SetGuidance("G4DEFAULT: K+ production in p-target interactions according to G4 Default model");
  KaonPlusPhysicsModelCmd->SetParameterName("physics", true, false);
  KaonPlusPhysicsModelCmd->SetDefaultValue("FScal");
  KaonPlusPhysicsModelCmd->SetCandidates("MARS GFLUKA SWPar FScal G4DEFAULT");
  //
  // K- physics model
  KaonMinusPhysicsModelCmd = new G4UIcmdWithAString("/boone/physics/kaonMinusModel",this);
  KaonMinusPhysicsModelCmd->SetGuidance("physics model for K- production in p-Be interaction");
  KaonMinusPhysicsModelCmd->SetGuidance("Description:");
  KaonMinusPhysicsModelCmd->SetGuidance("MARS: K- production in p-target interactions according to MARS");
  KaonMinusPhysicsModelCmd->SetGuidance("GFLUKA: K- production in p-target interactions according to GFLUKA");
  KaonMinusPhysicsModelCmd->SetGuidance("G4DEFAULT: K- production in p-target interactions according to G4 default model");
  KaonMinusPhysicsModelCmd->SetParameterName("physics", true, false);
  KaonMinusPhysicsModelCmd->SetDefaultValue("MARS");
  KaonMinusPhysicsModelCmd->SetCandidates("MARS GFLUKA G4DEFAULT");
  //
  // K0L physics model
  KaonZeroLongPhysicsModelCmd = new G4UIcmdWithAString("/boone/physics/kaonZeroLongModel",this);
  KaonZeroLongPhysicsModelCmd->SetGuidance("physics model for K0L production in p-Be interaction");
  KaonZeroLongPhysicsModelCmd->SetGuidance("Description:");
  KaonZeroLongPhysicsModelCmd->SetGuidance("MARS: K0L production in p-target interactions according to MARS and GFLUKA");
  KaonZeroLongPhysicsModelCmd->SetGuidance("GFLUKA: K0L production in p-target interactions according to GFLUKA");
  KaonZeroLongPhysicsModelCmd->SetGuidance("SWPar: K0L production in p-target interactions according to a Sanford-Wang parametrization, chosen by the user via the command SWKaonZeroLongPar.");
  KaonZeroLongPhysicsModelCmd->SetGuidance("FScal: K0L production in p-target interactions according to a Feynman Scaling parametrization (as described in Tech Note 209), chosen by the user via the command FSKaonZeroLongPar.");
  KaonZeroLongPhysicsModelCmd->SetGuidance("G4DEFAULT: K0L production in p-target interactions according to G4 default model");
  KaonZeroLongPhysicsModelCmd->SetParameterName("physics", true, false);
  KaonZeroLongPhysicsModelCmd->SetDefaultValue("SWPar");
  KaonZeroLongPhysicsModelCmd->SetCandidates("MARS GFLUKA SWPar FScal G4DEFAULT");
  //
  // secondary protons physics model
  ProtonPhysicsModelCmd = new G4UIcmdWithAString("/boone/physics/protonModel",this);
  ProtonPhysicsModelCmd->SetGuidance("physics model for proton production in p-Be interaction");
  ProtonPhysicsModelCmd->SetGuidance("Description:");
  ProtonPhysicsModelCmd->SetGuidance("MARS: secondary proton production in p-target interactions according to MARS");
  ProtonPhysicsModelCmd->SetGuidance("GFLUKA: secondary proton production in p-target interactions according to GFLUKA");
  ProtonPhysicsModelCmd->SetGuidance("G4DEFAULT: secondary proton production in p-target interactions according to G4 Default model");
  ProtonPhysicsModelCmd->SetParameterName("physics", true, false);
  ProtonPhysicsModelCmd->SetDefaultValue("MARS");
  ProtonPhysicsModelCmd->SetCandidates("MARS GFLUKA G4DEFAULT");
  //
  // neutron physics model
  NeutronPhysicsModelCmd = new G4UIcmdWithAString("/boone/physics/neutronModel",this);
  NeutronPhysicsModelCmd->SetGuidance("physics model for neutron production in p-Be interaction");
  NeutronPhysicsModelCmd->SetGuidance("Description:");
  NeutronPhysicsModelCmd->SetGuidance("MARS: neutron production in p-target interactions according to MARS");
  NeutronPhysicsModelCmd->SetGuidance("GFLUKA: neutron production in p-target interactions according to GFLUKA");
  NeutronPhysicsModelCmd->SetGuidance("G4DEFAULT: neutron production in p-target interactions according to G4 Default model");
  NeutronPhysicsModelCmd->SetParameterName("physics", true, false);
  NeutronPhysicsModelCmd->SetDefaultValue("MARS");
  NeutronPhysicsModelCmd->SetCandidates("MARS GFLUKA G4DEFAULT");
  //
  //eta physics model
  EtaPhysicsModelCmd = new G4UIcmdWithAString("/boone/physics/etaModel",this);
  EtaPhysicsModelCmd->SetGuidance("physics model for pi- production in p-Be interaction");
  EtaPhysicsModelCmd->SetGuidance("Description:");
  EtaPhysicsModelCmd->SetGuidance("MARS: eta production in p-target interactions according to MARS");
  EtaPhysicsModelCmd->SetGuidance("GFLUKA: eta production in p-target interactions according to GFLUKA");
  EtaPhysicsModelCmd->SetGuidance("ZGS: eta production in p-target interactions according to a Sanford-Wang parametrization of Argonne's ZGS data");
  EtaPhysicsModelCmd->SetGuidance("SWPar: eta production in p-target interactions according to a Sanford-Wang parametrization, chosen by the user via the command SWEtaPar.");
  EtaPhysicsModelCmd->SetGuidance("G4DEFALT: eta production in p-target interactions according to G4 default model");
  EtaPhysicsModelCmd->SetParameterName("physics", true, false);
  EtaPhysicsModelCmd->SetDefaultValue("SWPar");
  EtaPhysicsModelCmd->SetCandidates("MARS GFLUKA ZGS SWPar G4DEFAULT");
  //




  
  // scaling option for scaling cross sections from Be to materials of atomic number A
  BeToAScalingCmd = new G4UIcmdWithABool("/boone/physics/BeToAScaling",this);
  BeToAScalingCmd->SetGuidance("Flag for using the Be to A scaling factor for the cross sections");
  BeToAScalingCmd->SetParameterName("physics", true, false);
  BeToAScalingCmd->SetDefaultValue(false);

  BeToAScalingParCmd = new G4UIcommand("/boone/physics/BeToAScalingPar",this);
  BeToAScalingParCmd->SetGuidance("Set coefficients for scaling factor c0 + c1*x + c2*x^2");
  BeToAScalingParCmd->SetGuidance("Usage: /boone/physics/BeToAScalingPar c0 c1 c2");

  G4UIparameter* scalingParam;
  scalingParam = new G4UIparameter("BeToAScalingPar0Value",'d',true);
  scalingParam->SetDefaultValue(0.74);
  BeToAScalingParCmd->SetParameter(scalingParam);
  scalingParam = new G4UIparameter("BeToAScalingPar1Value",'d',true);
  scalingParam->SetDefaultValue(-0.55);
  BeToAScalingParCmd->SetParameter(scalingParam);
  scalingParam = new G4UIparameter("BeToAScalingPar2Value",'d',true);
  scalingParam->SetDefaultValue(0.26);
  BeToAScalingParCmd->SetParameter(scalingParam);

  // runtime verbosity
  PhysicsVerboseCmd = new G4UIcmdWithABool("/boone/physics/verbose",this);
  PhysicsVerboseCmd->SetGuidance("Flag for dumping extra-information on screen from p-Be interaction");
  PhysicsVerboseCmd->SetParameterName("physicsverbose", true, false);
  PhysicsVerboseCmd->SetDefaultValue(false);


  // Specify Sanford-Wang parameters for pi+, pi-, and k+
  // pi+
  SWPiPlusParCmd = 
    new G4UIcommand("/boone/physics/SWPiPlusPar",this);
  SWPiPlusParCmd->SetGuidance("Set values of SW parameters for pi+");
  SWPiPlusParCmd->SetGuidance("Usage: /boone/physics/SWPiPlusPar c1 c2 c3 c4 c5 c6 c7 c8");
  SWPiPlusParCmd->SetGuidance("Description: c1 through c8 are the real SW parameters for pi+ production (see http://cornell.mirror.aps.org/abstract/PRD/v4/i7/p1967_1 )");

  G4UIparameter* param;
  param = new G4UIparameter("SWPiPlusPar1Value",'d',true);
  param->SetParameterRange("SWPiPlusPar1Value>=0.");
  param->SetDefaultValue(206.4);
  SWPiPlusParCmd->SetParameter(param);
  param = new G4UIparameter("SWPiPlusPar2Value",'d',true);
  param->SetParameterRange("SWPiPlusPar2Value>=0.");
  param->SetDefaultValue(1.030);
  SWPiPlusParCmd->SetParameter(param);
  param = new G4UIparameter("SWPiPlusPar3Value",'d',true);
  param->SetParameterRange("SWPiPlusPar3Value>=0.");
  param->SetDefaultValue(5.902);
  SWPiPlusParCmd->SetParameter(param);
  param = new G4UIparameter("SWPiPlusPar4Value",'d',true);
  param->SetParameterRange("SWPiPlusPar4Value>=0.");
  param->SetDefaultValue(2.012);
  SWPiPlusParCmd->SetParameter(param);
  param = new G4UIparameter("SWPiPlusPar5Value",'d',true);
  param->SetParameterRange("SWPiPlusPar5Value>=0.");
  param->SetDefaultValue(2.127);
  SWPiPlusParCmd->SetParameter(param);
  param = new G4UIparameter("SWPiPlusPar6Value",'d',true);
  param->SetParameterRange("SWPiPlusPar6Value>=0.");
  param->SetDefaultValue(5.510);
  SWPiPlusParCmd->SetParameter(param);
  param = new G4UIparameter("SWPiPlusPar7Value",'d',true);
  param->SetParameterRange("SWPiPlusPar7Value>=0.");
  param->SetDefaultValue(0.9958E-01);
  SWPiPlusParCmd->SetParameter(param);
  param = new G4UIparameter("SWPiPlusPar8Value",'d',true);
  param->SetParameterRange("SWPiPlusPar8Value>=0.");
  param->SetDefaultValue(12.03);
  SWPiPlusParCmd->SetParameter(param);

  // pi-
  SWPiMinusParCmd = 
    new G4UIcommand("/boone/physics/SWPiMinusPar",this);
  SWPiMinusParCmd->SetGuidance("Set values of SW parameters for pi-");
  SWPiMinusParCmd->SetGuidance("Usage: /boone/physics/SWPiMinusPar c1 c2 c3 c4 c5 c6 c7 c8");
  SWPiMinusParCmd->SetGuidance("Description: c1 through c8 are the real SW parameters for pi- production.");

  param = new G4UIparameter("SWPiMinusPar1Value",'d',true);
  param->SetParameterRange("SWPiMinusPar1Value>=0.");
  param->SetDefaultValue(184.1);
  SWPiMinusParCmd->SetParameter(param);
  param = new G4UIparameter("SWPiMinusPar2Value",'d',true);
  param->SetParameterRange("SWPiMinusPar2Value>=0.");
  param->SetDefaultValue(1.052);
  SWPiMinusParCmd->SetParameter(param);
  param = new G4UIparameter("SWPiMinusPar3Value",'d',true);
  param->SetParameterRange("SWPiMinusPar3Value>=0.");
  param->SetDefaultValue(6.706);
  SWPiMinusParCmd->SetParameter(param);
  param = new G4UIparameter("SWPiMinusPar4Value",'d',true);
  param->SetParameterRange("SWPiMinusPar4Value>=0.");
  param->SetDefaultValue(1.275);
  SWPiMinusParCmd->SetParameter(param);
  param = new G4UIparameter("SWPiMinusPar5Value",'d',true);
  param->SetParameterRange("SWPiMinusPar5Value>=0.");
  param->SetDefaultValue(1.424);
  SWPiMinusParCmd->SetParameter(param);
  param = new G4UIparameter("SWPiMinusPar6Value",'d',true);
  param->SetParameterRange("SWPiMinusPar6Value>=0.");
  param->SetDefaultValue(5.225);
  SWPiMinusParCmd->SetParameter(param);
  param = new G4UIparameter("SWPiMinusPar7Value",'d',true);
  param->SetParameterRange("SWPiMinusPar7Value>=0.");
  param->SetDefaultValue(0.9439E-01);
  SWPiMinusParCmd->SetParameter(param);
  param = new G4UIparameter("SWPiMinusPar8Value",'d',true);
  param->SetParameterRange("SWPiMinusPar8Value>=0.");
  param->SetDefaultValue(10.74);
  SWPiMinusParCmd->SetParameter(param);

  // K+
  SWKaonPlusParCmd = 
    new G4UIcommand("/boone/physics/SWKaonPlusPar",this);
  SWKaonPlusParCmd->SetGuidance("Set values of SW parameters for k+");
  SWKaonPlusParCmd->SetGuidance("Usage: /boone/physics/SWKaonPlusPar c1 c2 c3 c4 c5 c6 c7 c8 c9");
  SWKaonPlusParCmd->SetGuidance("Description: c1 through c9 are the real SW parameters for K+ production (see http://cornell.mirror.aps.org/abstract/PRD/v4/i7/p1967_1 )");

  param = new G4UIparameter("SWKaonPlusPar1Value",'d',true);
  param->SetParameterRange("SWKaonPlusPar1Value>=0.");
  param->SetDefaultValue(6.083);
  SWKaonPlusParCmd->SetParameter(param);
  param = new G4UIparameter("SWKaonPlusPar2Value",'d',true);
  param->SetParameterRange("SWKaonPlusPar2Value>=0.");
  param->SetDefaultValue(1.468);
  SWKaonPlusParCmd->SetParameter(param);
  param = new G4UIparameter("SWKaonPlusPar3Value",'d',true);
  param->SetParameterRange("SWKaonPlusPar3Value>=0.");
  param->SetDefaultValue(10.04);
  SWKaonPlusParCmd->SetParameter(param);
  param = new G4UIparameter("SWKaonPlusPar4Value",'d',true);
  param->SetParameterRange("SWKaonPlusPar4Value>=0.");
  param->SetDefaultValue(3.086);
  SWKaonPlusParCmd->SetParameter(param);
  param = new G4UIparameter("SWKaonPlusPar5Value",'d',true);
  param->SetParameterRange("SWKaonPlusPar5Value>=0.");
  param->SetDefaultValue(3.356);
  SWKaonPlusParCmd->SetParameter(param);
  param = new G4UIparameter("SWKaonPlusPar6Value",'d',true);
  param->SetParameterRange("SWKaonPlusPar6Value>=0.");
  param->SetDefaultValue(5.718);
  SWKaonPlusParCmd->SetParameter(param);
  param = new G4UIparameter("SWKaonPlusPar7Value",'d',true);
  param->SetParameterRange("SWKaonPlusPar7Value>=0.");
  param->SetDefaultValue(0.1274);
  SWKaonPlusParCmd->SetParameter(param);
  param = new G4UIparameter("SWKaonPlusPar8Value",'d',true);
  param->SetParameterRange("SWKaonPlusPar8Value>=0.");
  param->SetDefaultValue(10.36);
  SWKaonPlusParCmd->SetParameter(param);
  param = new G4UIparameter("SWKaonPlusPar9Value",'d',true);
  param->SetParameterRange("SWKaonPlusPar9Value>=0.");
  param->SetDefaultValue(0.0);
  SWKaonPlusParCmd->SetParameter(param);

  FSKaonPlusParCmd = 
    new G4UIcommand("/boone/physics/FSKaonPlusPar",this);
  FSKaonPlusParCmd->SetGuidance("Set values of FS parameters for k+");
  FSKaonPlusParCmd->SetGuidance("Usage: /boone/physics/FSKaonPlusPar c1 c2 c3 c4 c5 c6 c7 c8");
  FSKaonPlusParCmd->SetGuidance("Description: c1 through c8 are the real FS parameters for K+ production (see MiniBooNE Tech Note 209)");

  param = new G4UIparameter("FSKaonPlusPar1Value",'d',true);
  param->SetParameterRange("FSKaonPlusPar1Value>=0.");
  param->SetDefaultValue(11.70);
  FSKaonPlusParCmd->SetParameter(param);
  param = new G4UIparameter("FSKaonPlusPar2Value",'d',true);
  param->SetParameterRange("FSKaonPlusPar2Value>=0.");
  param->SetDefaultValue(0.88);
  FSKaonPlusParCmd->SetParameter(param);
  param = new G4UIparameter("FSKaonPlusPar3Value",'d',true);
  param->SetParameterRange("FSKaonPlusPar3Value>=0.");
  param->SetDefaultValue(4.77);
  FSKaonPlusParCmd->SetParameter(param);
  param = new G4UIparameter("FSKaonPlusPar4Value",'d',true);
  param->SetParameterRange("FSKaonPlusPar4Value>=0.");
  param->SetDefaultValue(1.51);
  FSKaonPlusParCmd->SetParameter(param);
  param = new G4UIparameter("FSKaonPlusPar5Value",'d',true);
  param->SetParameterRange("FSKaonPlusPar5Value>=0.");
  param->SetDefaultValue(2.21);
  FSKaonPlusParCmd->SetParameter(param);
  param = new G4UIparameter("FSKaonPlusPar6Value",'d',true);
  param->SetParameterRange("FSKaonPlusPar6Value>=0.");
  param->SetDefaultValue(2.17);
  FSKaonPlusParCmd->SetParameter(param);
  param = new G4UIparameter("FSKaonPlusPar7Value",'d',true);
  param->SetParameterRange("FSKaonPlusPar7Value>=0.");
  param->SetDefaultValue(1.51);
  FSKaonPlusParCmd->SetParameter(param);
  param = new G4UIparameter("FSKaonPlusPar8Value",'d',true);
  param->SetParameterRange("FSKaonPlusPar8Value>=0.");
  param->SetDefaultValue(0.0);
  FSKaonPlusParCmd->SetParameter(param);

  // K0L
  SWKaonZeroLongParCmd = 
    new G4UIcommand("/boone/physics/SWKaonZeroLongPar",this);
  SWKaonZeroLongParCmd->SetGuidance("Set values of SW parameters for K0L");
  SWKaonZeroLongParCmd->SetGuidance("Usage: /boone/physics/SWKaonZeroLongPar c1 c2 c3 c4 c5 c6 c7 c8 c9");
  SWKaonZeroLongParCmd->SetGuidance("Description: c1 through c9 are the real SW parameters for K0L production (see http://cornell.mirror.aps.org/abstract/PRD/v4/i7/p1967_1 )");

  param = new G4UIparameter("SWKaonZeroLongPar1Value",'d',true);
  param->SetParameterRange("SWKaonZeroLongPar1Value>=0.");
  param->SetDefaultValue(15.13);
  SWKaonZeroLongParCmd->SetParameter(param);
  param = new G4UIparameter("SWKaonZeroLongPar2Value",'d',true);
  param->SetParameterRange("SWKaonZeroLongPar2Value>=0.");
  param->SetDefaultValue(1.975);
  SWKaonZeroLongParCmd->SetParameter(param);
  param = new G4UIparameter("SWKaonZeroLongPar3Value",'d',true);
  param->SetParameterRange("SWKaonZeroLongPar3Value>=0.");
  param->SetDefaultValue(4.084);
  SWKaonZeroLongParCmd->SetParameter(param);
  param = new G4UIparameter("SWKaonZeroLongPar4Value",'d',true);
  param->SetParameterRange("SWKaonZeroLongPar4Value>=0.");
  param->SetDefaultValue(0.9277);
  SWKaonZeroLongParCmd->SetParameter(param);
  param = new G4UIparameter("SWKaonZeroLongPar5Value",'d',true);
  param->SetParameterRange("SWKaonZeroLongPar5Value>=0.");
  param->SetDefaultValue(0.7306);
  SWKaonZeroLongParCmd->SetParameter(param);
  param = new G4UIparameter("SWKaonZeroLongPar6Value",'d',true);
  param->SetParameterRange("SWKaonZeroLongPar6Value>=0.");
  param->SetDefaultValue(4.362);
  SWKaonZeroLongParCmd->SetParameter(param);
  param = new G4UIparameter("SWKaonZeroLongPar7Value",'d',true);
  param->SetParameterRange("SWKaonZeroLongPar7Value>=0.");
  param->SetDefaultValue(0.04789);
  SWKaonZeroLongParCmd->SetParameter(param);
  param = new G4UIparameter("SWKaonZeroLongPar8Value",'d',true);
  param->SetParameterRange("SWKaonZeroLongPar8Value>=0.");
  param->SetDefaultValue(13.3);
  SWKaonZeroLongParCmd->SetParameter(param);
  param = new G4UIparameter("SWKaonZeroLongPar9Value",'d',true);
  param->SetParameterRange("SWKaonZeroLongPar9Value>=0.");
  param->SetDefaultValue(1.278);
  SWKaonZeroLongParCmd->SetParameter(param);

  FSKaonZeroLongParCmd = 
    new G4UIcommand("/boone/physics/FSKaonZeroLongPar",this);
  FSKaonZeroLongParCmd->SetGuidance("Set values of FS parameters for K0L");
  FSKaonZeroLongParCmd->SetGuidance("Usage: /boone/physics/FSKaonZeroLongPar c1 c2 c3 c4 c5 c6 c7 c8");
  FSKaonZeroLongParCmd->SetGuidance("Description: c1 through c8 are the real FS parameters for K0L production (see Tech Note 209)");

  param = new G4UIparameter("FSKaonZeroLongPar1Value",'d',true);
  param->SetParameterRange("FSKaonZeroLongPar1Value>=0.");
  param->SetDefaultValue(11.70);
  FSKaonZeroLongParCmd->SetParameter(param);
  param = new G4UIparameter("FSKaonZeroLongPar2Value",'d',true);
  param->SetParameterRange("FSKaonZeroLongPar2Value>=0.");
  param->SetDefaultValue(0.88);
  FSKaonZeroLongParCmd->SetParameter(param);
  param = new G4UIparameter("FSKaonZeroLongPar3Value",'d',true);
  param->SetParameterRange("FSKaonZeroLongPar3Value>=0.");
  param->SetDefaultValue(4.77);
  FSKaonZeroLongParCmd->SetParameter(param);
  param = new G4UIparameter("FSKaonZeroLongPar4Value",'d',true);
  param->SetParameterRange("FSKaonZeroLongPar4Value>=0.");
  param->SetDefaultValue(1.51);
  FSKaonZeroLongParCmd->SetParameter(param);
  param = new G4UIparameter("FSKaonZeroLongPar5Value",'d',true);
  param->SetParameterRange("FSKaonZeroLongPar5Value>=0.");
  param->SetDefaultValue(2.21);
  FSKaonZeroLongParCmd->SetParameter(param);
  param = new G4UIparameter("FSKaonZeroLongPar6Value",'d',true);
  param->SetParameterRange("FSKaonZeroLongPar6Value>=0.");
  param->SetDefaultValue(2.17);
  FSKaonZeroLongParCmd->SetParameter(param);
  param = new G4UIparameter("FSKaonZeroLongPar7Value",'d',true);
  param->SetParameterRange("FSKaonZeroLongPar7Value>=0.");
  param->SetDefaultValue(1.51);
  FSKaonZeroLongParCmd->SetParameter(param);
  param = new G4UIparameter("FSKaonZeroLongPar8Value",'d',true);
  param->SetParameterRange("FSKaonZeroLongPar8Value>=0.");
  param->SetDefaultValue(0.0);
  FSKaonZeroLongParCmd->SetParameter(param);

  // eta
  SWEtaParCmd = 
    new G4UIcommand("/boone/physics/SWEtaPar",this);
  SWEtaParCmd->SetGuidance("Set values of SW parameters for eta");
  SWEtaParCmd->SetGuidance("Usage: /boone/physics/SWEtaPar c1 c2 c3 c4 c5 c6 c7 c8");
  SWEtaParCmd->SetGuidance("Description: c1 through c8 are the real SW parameters for pi- production.");

  param = new G4UIparameter("SWEtaPar1Value",'d',true);
  param->SetParameterRange("SWEtaPar1Value>=0.");
  param->SetDefaultValue(184.1);
  SWEtaParCmd->SetParameter(param);
  param = new G4UIparameter("SWEtaPar2Value",'d',true);
  param->SetParameterRange("SWEtaPar2Value>=0.");
  param->SetDefaultValue(1.052);
  SWEtaParCmd->SetParameter(param);
  param = new G4UIparameter("SWEtaPar3Value",'d',true);
  param->SetParameterRange("SWEtaPar3Value>=0.");
  param->SetDefaultValue(6.706);
  SWEtaParCmd->SetParameter(param);
  param = new G4UIparameter("SWEtaPar4Value",'d',true);
  param->SetParameterRange("SWEtaPar4Value>=0.");
  param->SetDefaultValue(1.275);
  SWEtaParCmd->SetParameter(param);
  param = new G4UIparameter("SWEtaPar5Value",'d',true);
  param->SetParameterRange("SWEtaPar5Value>=0.");
  param->SetDefaultValue(1.424);
  SWEtaParCmd->SetParameter(param);
  param = new G4UIparameter("SWEtaPar6Value",'d',true);
  param->SetParameterRange("SWEtaPar6Value>=0.");
  param->SetDefaultValue(5.225);
  SWEtaParCmd->SetParameter(param);
  param = new G4UIparameter("SWEtaPar7Value",'d',true);
  param->SetParameterRange("SWEtaPar7Value>=0.");
  param->SetDefaultValue(0.9439E-01);
  SWEtaParCmd->SetParameter(param);
  param = new G4UIparameter("SWEtaPar8Value",'d',true);
  param->SetParameterRange("SWEtaPar8Value>=0.");
  param->SetDefaultValue(10.74);
  SWEtaParCmd->SetParameter(param);


  
  // Proton Reweighting Function Command
  ProtonRwgtFuncCmd = new G4UIcmdWithAString("/boone/physics/protonRwgtFunc",this);
  ProtonRwgtFuncCmd->SetGuidance("Controls the functional form of the proton cross-section reweighting function");
  ProtonRwgtFuncCmd->SetGuidance("Options:");
  ProtonRwgtFuncCmd->SetGuidance("NONE  - no reweighting function is used");
  ProtonRwgtFuncCmd->SetGuidance("POLY  - reweighting is a polynomial function of the daughter's pz");
  ProtonRwgtFuncCmd->SetGuidance("EXP   - reweighting is an exponential function of the daughter's pz");
  ProtonRwgtFuncCmd->SetGuidance("FLAT1 - reweighting makes the daughter cross-section tables flat in p and theta");
  ProtonRwgtFuncCmd->SetGuidance("FLAT2 - reweighting makes the daughter cross-section tables flat in pt and pz");
  ProtonRwgtFuncCmd->SetParameterName("prwgtfunc", true, false);
  ProtonRwgtFuncCmd->SetDefaultValue("NONE");
  ProtonRwgtFuncCmd->SetCandidates("NONE POLY EXP FLAT1 FLAT2");

  // Neutron Reweighting Function Command
  NeutronRwgtFuncCmd = new G4UIcmdWithAString("/boone/physics/neutronRwgtFunc",this);
  NeutronRwgtFuncCmd->SetGuidance("Controls the functional form of the neutron cross-section reweighting function");
  NeutronRwgtFuncCmd->SetGuidance("Options:");
  NeutronRwgtFuncCmd->SetGuidance("NONE  - no reweighting function is used");
  NeutronRwgtFuncCmd->SetGuidance("POLY  - reweighting is a polynomial function of the daughter's pz");
  NeutronRwgtFuncCmd->SetGuidance("EXP   - reweighting is an exponential function of the daughter's pz");
  NeutronRwgtFuncCmd->SetGuidance("FLAT1 - reweighting makes the daughter cross-section tables flat in p and theta");
  NeutronRwgtFuncCmd->SetGuidance("FLAT2 - reweighting makes the daughter cross-section tables flat in pt and pz");
  NeutronRwgtFuncCmd->SetParameterName("nrwgtfunc", true, false);
  NeutronRwgtFuncCmd->SetDefaultValue("NONE");
  NeutronRwgtFuncCmd->SetCandidates("NONE POLY EXP FLAT1 FLAT2");

  // PionPlus Reweighting Function Command
  PionPlusRwgtFuncCmd = new G4UIcmdWithAString("/boone/physics/pionPlusRwgtFunc",this);
  PionPlusRwgtFuncCmd->SetGuidance("Controls the functional form of the pionPlus cross-section reweighting function");
  PionPlusRwgtFuncCmd->SetGuidance("Options:");
  PionPlusRwgtFuncCmd->SetGuidance("NONE  - no reweighting function is used");
  PionPlusRwgtFuncCmd->SetGuidance("POLY  - reweighting is a polynomial function of the daughter's pz");
  PionPlusRwgtFuncCmd->SetGuidance("EXP   - reweighting is an exponential function of the daughter's pz");
  PionPlusRwgtFuncCmd->SetGuidance("FLAT1 - reweighting makes the daughter cross-section tables flat in p and theta");
  PionPlusRwgtFuncCmd->SetGuidance("FLAT2 - reweighting makes the daughter cross-section tables flat in pt and pz");
  PionPlusRwgtFuncCmd->SetParameterName("piplrwgtfunc", true, false);
  PionPlusRwgtFuncCmd->SetDefaultValue("NONE");
  PionPlusRwgtFuncCmd->SetCandidates("NONE POLY EXP FLAT1 FLAT2");

  // PionMinus Reweighting Function Command
  PionMinusRwgtFuncCmd = new G4UIcmdWithAString("/boone/physics/pionMinusRwgtFunc",this);
  PionMinusRwgtFuncCmd->SetGuidance("Controls the functional form of the pionMinus cross-section reweighting function");
  PionMinusRwgtFuncCmd->SetGuidance("Options:");
  PionMinusRwgtFuncCmd->SetGuidance("NONE  - no reweighting function is used");
  PionMinusRwgtFuncCmd->SetGuidance("POLY  - reweighting is a polynomial function of the daughter's pz");
  PionMinusRwgtFuncCmd->SetGuidance("EXP   - reweighting is an exponential function of the daughter's pz");
  PionMinusRwgtFuncCmd->SetGuidance("FLAT1 - reweighting makes the daughter cross-section tables flat in p and theta");
  PionMinusRwgtFuncCmd->SetGuidance("FLAT2 - reweighting makes the daughter cross-section tables flat in pt and pz");
  PionMinusRwgtFuncCmd->SetParameterName("pimirwgtfunc", true, false);
  PionMinusRwgtFuncCmd->SetDefaultValue("NONE");
  PionMinusRwgtFuncCmd->SetCandidates("NONE POLY EXP FLAT1 FLAT2");

  // KaonPlus Reweighting Function Command
  KaonPlusRwgtFuncCmd = new G4UIcmdWithAString("/boone/physics/kaonPlusRwgtFunc",this);
  KaonPlusRwgtFuncCmd->SetGuidance("Controls the functional form of the kaonPlus cross-section reweighting function");
  KaonPlusRwgtFuncCmd->SetGuidance("Options:");
  KaonPlusRwgtFuncCmd->SetGuidance("NONE  - no reweighting function is used");
  KaonPlusRwgtFuncCmd->SetGuidance("POLY  - reweighting is a polynomial function of the daughter's pz");
  KaonPlusRwgtFuncCmd->SetGuidance("EXP   - reweighting is an exponential function of the daughter's pz");
  KaonPlusRwgtFuncCmd->SetGuidance("FLAT1 - reweighting makes the daughter cross-section tables flat in p and theta");
  KaonPlusRwgtFuncCmd->SetGuidance("FLAT2 - reweighting makes the daughter cross-section tables flat in pt and pz");
  KaonPlusRwgtFuncCmd->SetParameterName("kplrwgtfunc", true, false);
  KaonPlusRwgtFuncCmd->SetDefaultValue("NONE");
  KaonPlusRwgtFuncCmd->SetCandidates("NONE POLY EXP FLAT1 FLAT2");

  // KaonMinus Reweighting Function Command
  KaonMinusRwgtFuncCmd = new G4UIcmdWithAString("/boone/physics/kaonMinusRwgtFunc",this);
  KaonMinusRwgtFuncCmd->SetGuidance("Controls the functional form of the kaonMinus cross-section reweighting function");
  KaonMinusRwgtFuncCmd->SetGuidance("Options:");
  KaonMinusRwgtFuncCmd->SetGuidance("NONE  - no reweighting function is used");
  KaonMinusRwgtFuncCmd->SetGuidance("POLY  - reweighting is a polynomial function of the daughter's pz");
  KaonMinusRwgtFuncCmd->SetGuidance("EXP   - reweighting is an exponential function of the daughter's pz");
  KaonMinusRwgtFuncCmd->SetGuidance("FLAT1 - reweighting makes the daughter cross-section tables flat in p and theta");
  KaonMinusRwgtFuncCmd->SetGuidance("FLAT2 - reweighting makes the daughter cross-section tables flat in pt and pz");
  KaonMinusRwgtFuncCmd->SetParameterName("kmirwgtfunc", true, false);
  KaonMinusRwgtFuncCmd->SetDefaultValue("NONE");
  KaonMinusRwgtFuncCmd->SetCandidates("NONE POLY EXP FLAT1 FLAT2");

  // KaonZeroLong Reweighting Function Command
  KaonZeroLongRwgtFuncCmd = new G4UIcmdWithAString("/boone/physics/kaonZeroLongRwgtFunc",this);
  KaonZeroLongRwgtFuncCmd->SetGuidance("Controls the functional form of the kaonZeroLong cross-section reweighting function");
  KaonZeroLongRwgtFuncCmd->SetGuidance("Options:");
  KaonZeroLongRwgtFuncCmd->SetGuidance("NONE  - no reweighting function is used");
  KaonZeroLongRwgtFuncCmd->SetGuidance("POLY  - reweighting is a polynomial function of the daughter's pz");
  KaonZeroLongRwgtFuncCmd->SetGuidance("EXP   - reweighting is an exponential function of the daughter's pz");
  KaonZeroLongRwgtFuncCmd->SetGuidance("FLAT1 - reweighting makes the daughter cross-section tables flat in p and theta");
  KaonZeroLongRwgtFuncCmd->SetGuidance("FLAT2 - reweighting makes the daughter cross-section tables flat in pt and pz");
  KaonZeroLongRwgtFuncCmd->SetParameterName("klrwgtfunc", true, false);
  KaonZeroLongRwgtFuncCmd->SetDefaultValue("NONE");
  KaonZeroLongRwgtFuncCmd->SetCandidates("NONE POLY EXP FLAT1 FLAT2");


  // Eta Reweighting Function Command
  EtaRwgtFuncCmd = new G4UIcmdWithAString("/boone/physics/etaRwgtFunc",this);
  EtaRwgtFuncCmd->SetGuidance("Controls the functional form of the eta cross-section reweighting function");
  EtaRwgtFuncCmd->SetGuidance("Options:");
  EtaRwgtFuncCmd->SetGuidance("NONE  - no reweighting function is used");
  EtaRwgtFuncCmd->SetGuidance("POLY  - reweighting is a polynomial function of the daughter's pz");
  EtaRwgtFuncCmd->SetGuidance("EXP   - reweighting is an exponential function of the daughter's pz");
  EtaRwgtFuncCmd->SetGuidance("FLAT1 - reweighting makes the daughter cross-section tables flat in p and theta");
  EtaRwgtFuncCmd->SetGuidance("FLAT2 - reweighting makes the daughter cross-section tables flat in pt and pz");
  EtaRwgtFuncCmd->SetParameterName("etarwgtfunc", true, false);
  EtaRwgtFuncCmd->SetDefaultValue("NONE");
  EtaRwgtFuncCmd->SetCandidates("NONE POLY EXP FLAT1 FLAT2");


  
  // Proton Reweighting Parameters Command
  ProtonRwgtParamsCmd = 
    new G4UIcommand("/boone/physics/protonRwgtParams",this);
  ProtonRwgtParamsCmd->SetGuidance("Set the parameters of the chosen cross-section reweighting function");
  ProtonRwgtParamsCmd->SetGuidance("NONE:  parameters will be ignored");
  ProtonRwgtParamsCmd->SetGuidance("POLY:  c0 + c1*pz + c2*pz^2 + ... + c9*pz^9");
  ProtonRwgtParamsCmd->SetGuidance("       Usage:  /boone/physics/protonRwgtParams c0 c1 c2 c3 c4 c5 c6 c7 c8 c9");
  ProtonRwgtParamsCmd->SetGuidance("EXP:   c0 + c1*exp(c2*(pz-c3))");
  ProtonRwgtParamsCmd->SetGuidance("       Usage:  /boone/physics/protonRwgtParams c0 c1 c2 c3");
  ProtonRwgtParamsCmd->SetGuidance("FLAT1: parameters will be ignored");
  ProtonRwgtParamsCmd->SetGuidance("FLAT2: parameters will be ignored");

  param = new G4UIparameter("protonRwgtParam0",'d',true);
  param->SetDefaultValue(1.);
  ProtonRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("protonRwgtParam1",'d',true);
  param->SetDefaultValue(0.);
  ProtonRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("protonRwgtParam2",'d',true);
  param->SetDefaultValue(0.);
  ProtonRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("protonRwgtParam3",'d',true);
  param->SetDefaultValue(0.);
  ProtonRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("protonRwgtParam4",'d',true);
  param->SetDefaultValue(0.);
  ProtonRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("protonRwgtParam5",'d',true);
  param->SetDefaultValue(0.);
  ProtonRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("protonRwgtParam6",'d',true);
  param->SetDefaultValue(0.);
  ProtonRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("protonRwgtParam7",'d',true);
  param->SetDefaultValue(0.);
  ProtonRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("protonRwgtParam8",'d',true);
  param->SetDefaultValue(0.);
  ProtonRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("protonRwgtParam9",'d',true);
  param->SetDefaultValue(0.);
  ProtonRwgtParamsCmd->SetParameter(param);

  // Neutron Reweighting Parameters Command
  NeutronRwgtParamsCmd = 
    new G4UIcommand("/boone/physics/neutronRwgtParams",this);
  NeutronRwgtParamsCmd->SetGuidance("Set the parameters of the chosen cross-section reweighting function");
  NeutronRwgtParamsCmd->SetGuidance("NONE:  parameters will be ignored");
  NeutronRwgtParamsCmd->SetGuidance("POLY:  c0 + c1*pz + c2*pz^2 + ... + c9*pz^9");
  NeutronRwgtParamsCmd->SetGuidance("       Usage:  /boone/physics/neutronRwgtParams c0 c1 c2 c3 c4 c5 c6 c7 c8 c9");
  NeutronRwgtParamsCmd->SetGuidance("EXP:   c0 + c1*exp(c2*(pz-c3))");
  NeutronRwgtParamsCmd->SetGuidance("       Usage:  /boone/physics/neutronRwgtParams c0 c1 c2 c3");
  NeutronRwgtParamsCmd->SetGuidance("FLAT1: parameters will be ignored");
  NeutronRwgtParamsCmd->SetGuidance("FLAT2: parameters will be ignored");

  param = new G4UIparameter("neutronRwgtParam0",'d',true);
  param->SetDefaultValue(1.);
  NeutronRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("neutronRwgtParam1",'d',true);
  param->SetDefaultValue(0.);
  NeutronRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("neutronRwgtParam2",'d',true);
  param->SetDefaultValue(0.);
  NeutronRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("neutronRwgtParam3",'d',true);
  param->SetDefaultValue(0.);
  NeutronRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("neutronRwgtParam4",'d',true);
  param->SetDefaultValue(0.);
  NeutronRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("neutronRwgtParam5",'d',true);
  param->SetDefaultValue(0.);
  NeutronRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("neutronRwgtParam6",'d',true);
  param->SetDefaultValue(0.);
  NeutronRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("neutronRwgtParam7",'d',true);
  param->SetDefaultValue(0.);
  NeutronRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("neutronRwgtParam8",'d',true);
  param->SetDefaultValue(0.);
  NeutronRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("neutronRwgtParam9",'d',true);
  param->SetDefaultValue(0.);
  NeutronRwgtParamsCmd->SetParameter(param);

  // PionPlus Reweighting Parameters Command
  PionPlusRwgtParamsCmd = 
    new G4UIcommand("/boone/physics/pionPlusRwgtParams",this);
  PionPlusRwgtParamsCmd->SetGuidance("Set the parameters of the chosen cross-section reweighting function");
  PionPlusRwgtParamsCmd->SetGuidance("NONE:  parameters will be ignored");
  PionPlusRwgtParamsCmd->SetGuidance("POLY:  c0 + c1*pz + c2*pz^2 + ... + c9*pz^9");
  PionPlusRwgtParamsCmd->SetGuidance("       Usage:  /boone/physics/pionPlusRwgtParams c0 c1 c2 c3 c4 c5 c6 c7 c8 c9");
  PionPlusRwgtParamsCmd->SetGuidance("EXP:   c0 + c1*exp(c2*(pz-c3))");
  PionPlusRwgtParamsCmd->SetGuidance("       Usage:  /boone/physics/pionPlusRwgtParams c0 c1 c2 c3");
  PionPlusRwgtParamsCmd->SetGuidance("FLAT1: parameters will be ignored");
  PionPlusRwgtParamsCmd->SetGuidance("FLAT2: parameters will be ignored");

  param = new G4UIparameter("pionPlusRwgtParam0",'d',true);
  param->SetDefaultValue(1.);
  PionPlusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("pionPlusRwgtParam1",'d',true);
  param->SetDefaultValue(0.);
  PionPlusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("pionPlusRwgtParam2",'d',true);
  param->SetDefaultValue(0.);
  PionPlusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("pionPlusRwgtParam3",'d',true);
  param->SetDefaultValue(0.);
  PionPlusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("pionPlusRwgtParam4",'d',true);
  param->SetDefaultValue(0.);
  PionPlusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("pionPlusRwgtParam5",'d',true);
  param->SetDefaultValue(0.);
  PionPlusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("pionPlusRwgtParam6",'d',true);
  param->SetDefaultValue(0.);
  PionPlusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("pionPlusRwgtParam7",'d',true);
  param->SetDefaultValue(0.);
  PionPlusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("pionPlusRwgtParam8",'d',true);
  param->SetDefaultValue(0.);
  PionPlusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("pionPlusRwgtParam9",'d',true);
  param->SetDefaultValue(0.);
  PionPlusRwgtParamsCmd->SetParameter(param);

  // PionMinus Reweighting Parameters Command
  PionMinusRwgtParamsCmd = 
    new G4UIcommand("/boone/physics/pionMinusRwgtParams",this);
  PionMinusRwgtParamsCmd->SetGuidance("Set the parameters of the chosen cross-section reweighting function");
  PionMinusRwgtParamsCmd->SetGuidance("NONE:  parameters will be ignored");
  PionMinusRwgtParamsCmd->SetGuidance("POLY:  c0 + c1*pz + c2*pz^2 + ... + c9*pz^9");
  PionMinusRwgtParamsCmd->SetGuidance("       Usage:  /boone/physics/pionMinusRwgtParams c0 c1 c2 c3 c4 c5 c6 c7 c8 c9");
  PionMinusRwgtParamsCmd->SetGuidance("EXP:   c0 + c1*exp(c2*(pz-c3))");
  PionMinusRwgtParamsCmd->SetGuidance("       Usage:  /boone/physics/pionMinusRwgtParams c0 c1 c2 c3");
  PionMinusRwgtParamsCmd->SetGuidance("FLAT1: parameters will be ignored");
  PionMinusRwgtParamsCmd->SetGuidance("FLAT2: parameters will be ignored");

  param = new G4UIparameter("pionMinusRwgtParam0",'d',true);
  param->SetDefaultValue(1.);
  PionMinusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("pionMinusRwgtParam1",'d',true);
  param->SetDefaultValue(0.);
  PionMinusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("pionMinusRwgtParam2",'d',true);
  param->SetDefaultValue(0.);
  PionMinusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("pionMinusRwgtParam3",'d',true);
  param->SetDefaultValue(0.);
  PionMinusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("pionMinusRwgtParam4",'d',true);
  param->SetDefaultValue(0.);
  PionMinusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("pionMinusRwgtParam5",'d',true);
  param->SetDefaultValue(0.);
  PionMinusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("pionMinusRwgtParam6",'d',true);
  param->SetDefaultValue(0.);
  PionMinusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("pionMinusRwgtParam7",'d',true);
  param->SetDefaultValue(0.);
  PionMinusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("pionMinusRwgtParam8",'d',true);
  param->SetDefaultValue(0.);
  PionMinusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("pionMinusRwgtParam9",'d',true);
  param->SetDefaultValue(0.);
  PionMinusRwgtParamsCmd->SetParameter(param);

  // KaonPlus Reweighting Parameters Command
  KaonPlusRwgtParamsCmd = 
    new G4UIcommand("/boone/physics/kaonPlusRwgtParams",this);
  KaonPlusRwgtParamsCmd->SetGuidance("Set the parameters of the chosen cross-section reweighting function");
  KaonPlusRwgtParamsCmd->SetGuidance("NONE:  parameters will be ignored");
  KaonPlusRwgtParamsCmd->SetGuidance("POLY:  c0 + c1*pz + c2*pz^2 + ... + c9*pz^9");
  KaonPlusRwgtParamsCmd->SetGuidance("       Usage:  /boone/physics/kaonPlusRwgtParams c0 c1 c2 c3 c4 c5 c6 c7 c8 c9");
  KaonPlusRwgtParamsCmd->SetGuidance("EXP:   c0 + c1*exp(c2*(pz-c3))");
  KaonPlusRwgtParamsCmd->SetGuidance("       Usage:  /boone/physics/kaonPlusRwgtParams c0 c1 c2 c3");
  KaonPlusRwgtParamsCmd->SetGuidance("FLAT1: parameters will be ignored");
  KaonPlusRwgtParamsCmd->SetGuidance("FLAT2: parameters will be ignored");

  param = new G4UIparameter("kaonPlusRwgtParam0",'d',true);
  param->SetDefaultValue(1.);
  KaonPlusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("kaonPlusRwgtParam1",'d',true);
  param->SetDefaultValue(0.);
  KaonPlusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("kaonPlusRwgtParam2",'d',true);
  param->SetDefaultValue(0.);
  KaonPlusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("kaonPlusRwgtParam3",'d',true);
  param->SetDefaultValue(0.);
  KaonPlusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("kaonPlusRwgtParam4",'d',true);
  param->SetDefaultValue(0.);
  KaonPlusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("kaonPlusRwgtParam5",'d',true);
  param->SetDefaultValue(0.);
  KaonPlusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("kaonPlusRwgtParam6",'d',true);
  param->SetDefaultValue(0.);
  KaonPlusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("kaonPlusRwgtParam7",'d',true);
  param->SetDefaultValue(0.);
  KaonPlusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("kaonPlusRwgtParam8",'d',true);
  param->SetDefaultValue(0.);
  KaonPlusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("kaonPlusRwgtParam9",'d',true);
  param->SetDefaultValue(0.);
  KaonPlusRwgtParamsCmd->SetParameter(param);

  // KaonMinus Reweighting Parameters Command
  KaonMinusRwgtParamsCmd = 
    new G4UIcommand("/boone/physics/kaonMinusRwgtParams",this);
  KaonMinusRwgtParamsCmd->SetGuidance("Set the parameters of the chosen cross-section reweighting function");
  KaonMinusRwgtParamsCmd->SetGuidance("NONE:  parameters will be ignored");
  KaonMinusRwgtParamsCmd->SetGuidance("POLY:  c0 + c1*pz + c2*pz^2 + ... + c9*pz^9");
  KaonMinusRwgtParamsCmd->SetGuidance("       Usage:  /boone/physics/kaonMinusRwgtParams c0 c1 c2 c3 c4 c5 c6 c7 c8 c9");
  KaonMinusRwgtParamsCmd->SetGuidance("EXP:   c0 + c1*exp(c2*(pz-c3))");
  KaonMinusRwgtParamsCmd->SetGuidance("       Usage:  /boone/physics/kaonMinusRwgtParams c0 c1 c2 c3");
  KaonMinusRwgtParamsCmd->SetGuidance("FLAT1: parameters will be ignored");
  KaonMinusRwgtParamsCmd->SetGuidance("FLAT2: parameters will be ignored");

  param = new G4UIparameter("kaonMinusRwgtParam0",'d',true);
  param->SetDefaultValue(1.);
  KaonMinusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("kaonMinusRwgtParam1",'d',true);
  param->SetDefaultValue(0.);
  KaonMinusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("kaonMinusRwgtParam2",'d',true);
  param->SetDefaultValue(0.);
  KaonMinusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("kaonMinusRwgtParam3",'d',true);
  param->SetDefaultValue(0.);
  KaonMinusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("kaonMinusRwgtParam4",'d',true);
  param->SetDefaultValue(0.);
  KaonMinusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("kaonMinusRwgtParam5",'d',true);
  param->SetDefaultValue(0.);
  KaonMinusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("kaonMinusRwgtParam6",'d',true);
  param->SetDefaultValue(0.);
  KaonMinusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("kaonMinusRwgtParam7",'d',true);
  param->SetDefaultValue(0.);
  KaonMinusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("kaonMinusRwgtParam8",'d',true);
  param->SetDefaultValue(0.);
  KaonMinusRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("kaonMinusRwgtParam9",'d',true);
  param->SetDefaultValue(0.);
  KaonMinusRwgtParamsCmd->SetParameter(param);

  // KaonZeroLong Reweighting Parameters Command
  KaonZeroLongRwgtParamsCmd = 
    new G4UIcommand("/boone/physics/kaonZeroLongRwgtParams",this);
  KaonZeroLongRwgtParamsCmd->SetGuidance("Set the parameters of the chosen cross-section reweighting function");
  KaonZeroLongRwgtParamsCmd->SetGuidance("NONE:  parameters will be ignored");
  KaonZeroLongRwgtParamsCmd->SetGuidance("POLY:  c0 + c1*pz + c2*pz^2 + ... + c9*pz^9");
  KaonZeroLongRwgtParamsCmd->SetGuidance("       Usage:  /boone/physics/kaonZeroLongRwgtParams c0 c1 c2 c3 c4 c5 c6 c7 c8 c9");
  KaonZeroLongRwgtParamsCmd->SetGuidance("EXP:   c0 + c1*exp(c2*(pz-c3))");
  KaonZeroLongRwgtParamsCmd->SetGuidance("       Usage:  /boone/physics/kaonZeroLongRwgtParams c0 c1 c2 c3");
  KaonZeroLongRwgtParamsCmd->SetGuidance("FLAT1: parameters will be ignored");
  KaonZeroLongRwgtParamsCmd->SetGuidance("FLAT2: parameters will be ignored");

  param = new G4UIparameter("kaonZeroLongRwgtParam0",'d',true);
  param->SetDefaultValue(1.);
  KaonZeroLongRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("kaonZeroLongRwgtParam1",'d',true);
  param->SetDefaultValue(0.);
  KaonZeroLongRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("kaonZeroLongRwgtParam2",'d',true);
  param->SetDefaultValue(0.);
  KaonZeroLongRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("kaonZeroLongRwgtParam3",'d',true);
  param->SetDefaultValue(0.);
  KaonZeroLongRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("kaonZeroLongRwgtParam4",'d',true);
  param->SetDefaultValue(0.);
  KaonZeroLongRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("kaonZeroLongRwgtParam5",'d',true);
  param->SetDefaultValue(0.);
  KaonZeroLongRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("kaonZeroLongRwgtParam6",'d',true);
  param->SetDefaultValue(0.);
  KaonZeroLongRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("kaonZeroLongRwgtParam7",'d',true);
  param->SetDefaultValue(0.);
  KaonZeroLongRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("kaonZeroLongRwgtParam8",'d',true);
  param->SetDefaultValue(0.);
  KaonZeroLongRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("kaonZeroLongRwgtParam9",'d',true);
  param->SetDefaultValue(0.);
  KaonZeroLongRwgtParamsCmd->SetParameter(param);

  // Eta Reweighting Parameters Command
  EtaRwgtParamsCmd = 
    new G4UIcommand("/boone/physics/etaRwgtParams",this);
  EtaRwgtParamsCmd->SetGuidance("Set the parameters of the chosen cross-section reweighting function");
  EtaRwgtParamsCmd->SetGuidance("NONE:  parameters will be ignored");
  EtaRwgtParamsCmd->SetGuidance("POLY:  c0 + c1*pz + c2*pz^2 + ... + c9*pz^9");
  EtaRwgtParamsCmd->SetGuidance("       Usage:  /boone/physics/etaRwgtParams c0 c1 c2 c3 c4 c5 c6 c7 c8 c9");
  EtaRwgtParamsCmd->SetGuidance("EXP:   c0 + c1*exp(c2*(pz-c3))");
  EtaRwgtParamsCmd->SetGuidance("       Usage:  /boone/physics/etaRwgtParams c0 c1 c2 c3");
  EtaRwgtParamsCmd->SetGuidance("FLAT1: parameters will be ignored");
  EtaRwgtParamsCmd->SetGuidance("FLAT2: parameters will be ignored");

  param = new G4UIparameter("etaRwgtParam0",'d',true);
  param->SetDefaultValue(1.);
  EtaRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("etaRwgtParam1",'d',true);
  param->SetDefaultValue(0.);
  EtaRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("etaRwgtParam2",'d',true);
  param->SetDefaultValue(0.);
  EtaRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("etaRwgtParam3",'d',true);
  param->SetDefaultValue(0.);
  EtaRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("etaRwgtParam4",'d',true);
  param->SetDefaultValue(0.);
  EtaRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("etaRwgtParam5",'d',true);
  param->SetDefaultValue(0.);
  EtaRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("etaRwgtParam6",'d',true);
  param->SetDefaultValue(0.);
  EtaRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("etaRwgtParam7",'d',true);
  param->SetDefaultValue(0.);
  EtaRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("etaRwgtParam8",'d',true);
  param->SetDefaultValue(0.);
  EtaRwgtParamsCmd->SetParameter(param);
  param = new G4UIparameter("etaRwgtParam9",'d',true);
  param->SetDefaultValue(0.);
  EtaRwgtParamsCmd->SetParameter(param);


  
  // Quasi elastic scattering parameters:
  // Only protons at 8GeV use this in new MC, but keeping all QE parameters
  // for now
 //  Define the quasielastic parameters directory
  booneQuasiElasticDirectory = new G4UIdirectory("/boone/physics/quasielastic/");
  booneQuasiElasticDirectory->SetGuidance( "BooNE quasielastic scattering parameters control commands.");

  proQuasiElasticParCmd =  new G4UIcommand("/boone/physics/quasielastic/proQuasiElasticPar",this);
  proQuasiElasticParCmd->SetGuidance("Set values of parameters for proton quasielastic");
  proQuasiElasticParCmd->SetGuidance("Usage: /boone/physics/quasielastic/proQuasiElasticPar c0 c1 c2 c3 c4 c5");
  proQuasiElasticParCmd->SetGuidance("Description: parametrization according to c0 + c1*p + c2*p^2 + c3*p^3 + c4*p^4 + c5*p^5");

  param = new G4UIparameter("proQuasiElasticPar0",'d',true);  param->SetDefaultValue( 5.20996   ); proQuasiElasticParCmd->SetParameter(param);
  param = new G4UIparameter("proQuasiElasticPar1",'d',true);  param->SetDefaultValue( 0.814526  ); proQuasiElasticParCmd->SetParameter(param);
  param = new G4UIparameter("proQuasiElasticPar2",'d',true);  param->SetDefaultValue(-0.0376766 ); proQuasiElasticParCmd->SetParameter(param);
  param = new G4UIparameter("proQuasiElasticPar3",'d',true);  param->SetDefaultValue( 0.0       ); proQuasiElasticParCmd->SetParameter(param);
  param = new G4UIparameter("proQuasiElasticPar4",'d',true);  param->SetDefaultValue( 0.0       ); proQuasiElasticParCmd->SetParameter(param);
  param = new G4UIparameter("proQuasiElasticPar5",'d',true);  param->SetDefaultValue( 0.0       ); proQuasiElasticParCmd->SetParameter(param);

  neuQuasiElasticParCmd =  new G4UIcommand("/boone/physics/quasielastic/neuQuasiElasticPar",this);
  neuQuasiElasticParCmd->SetGuidance("Set values of parameters for neutron quasielastic");
  neuQuasiElasticParCmd->SetGuidance("Usage: /boone/physics/quasielastic/neuQuasiElasticPar c0 c1 c2 c3 c4 c5");
  neuQuasiElasticParCmd->SetGuidance("Description: parametrization according to c0 + c1*p + c2*p^2 + c3*p^3 + c4*p^4 + c5*p^5");
  
  param = new G4UIparameter("neuQuasiElasticPar0",'d',true);  param->SetDefaultValue( 2.56793   ); neuQuasiElasticParCmd->SetParameter(param);
  param = new G4UIparameter("neuQuasiElasticPar1",'d',true);  param->SetDefaultValue( 2.87091   ); neuQuasiElasticParCmd->SetParameter(param);
  param = new G4UIparameter("neuQuasiElasticPar2",'d',true);  param->SetDefaultValue(-0.535132  ); neuQuasiElasticParCmd->SetParameter(param);
  param = new G4UIparameter("neuQuasiElasticPar3",'d',true);  param->SetDefaultValue( 0.0429920 ); neuQuasiElasticParCmd->SetParameter(param);
  param = new G4UIparameter("neuQuasiElasticPar4",'d',true);  param->SetDefaultValue(-0.00126008); neuQuasiElasticParCmd->SetParameter(param);
  param = new G4UIparameter("neuQuasiElasticPar5",'d',true);  param->SetDefaultValue( 0.0       ); neuQuasiElasticParCmd->SetParameter(param);

  pipQuasiElasticParCmd =  new G4UIcommand("/boone/physics/quasielastic/pipQuasiElasticPar",this);
  pipQuasiElasticParCmd->SetGuidance("Set values of parameters for piplus quasielastic");
  pipQuasiElasticParCmd->SetGuidance("Usage: /boone/physics/quasielastic/pipQuasiElasticcPar c0 c1 c2 c3 c4 c5");
  pipQuasiElasticParCmd->SetGuidance("Description: parametrization according to c0 * tanh((p-c1)*c2) + c3 Gauss(c4, c5)");

  param = new G4UIparameter("pipQuasiElasticPar0",'d',true);  param->SetDefaultValue( 4.09816 ); pipQuasiElasticParCmd->SetParameter(param);
  param = new G4UIparameter("pipQuasiElasticPar1",'d',true);  param->SetDefaultValue( 1.73419 ); pipQuasiElasticParCmd->SetParameter(param);
  param = new G4UIparameter("pipQuasiElasticPar2",'d',true);  param->SetDefaultValue( 1.15718 ); pipQuasiElasticParCmd->SetParameter(param);
  param = new G4UIparameter("pipQuasiElasticPar3",'d',true);  param->SetDefaultValue( 2.97006 ); pipQuasiElasticParCmd->SetParameter(param);
  param = new G4UIparameter("pipQuasiElasticPar4",'d',true);  param->SetDefaultValue( 1.52589 ); pipQuasiElasticParCmd->SetParameter(param);
  param = new G4UIparameter("pipQuasiElasticPar5",'d',true);  param->SetDefaultValue( 0.217029); pipQuasiElasticParCmd->SetParameter(param);

  pimQuasiElasticParCmd =  new G4UIcommand("/boone/physics/quasielastic/pimQuasiElasticPar",this);
  pimQuasiElasticParCmd->SetGuidance("Set values of parameters for pimlus quasielastic");
  pimQuasiElasticParCmd->SetGuidance("Usage: /boone/physics/quasielastic/pimQuasiElasticcPar c0 c1 c2 c3 c4 c5");
  pimQuasiElasticParCmd->SetGuidance("Description: parametrization according to c0 * tanh((p-c1)*c2) + c3 Gauss(c4, c5)");

  param = new G4UIparameter("pimQuasiElasticPar0",'d',true);  param->SetDefaultValue(  4.16121  ); pimQuasiElasticParCmd->SetParameter(param);
  param = new G4UIparameter("pimQuasiElasticPar1",'d',true);  param->SetDefaultValue(  0.583495 ); pimQuasiElasticParCmd->SetParameter(param);
  param = new G4UIparameter("pimQuasiElasticPar2",'d',true);  param->SetDefaultValue(  1.21394  ); pimQuasiElasticParCmd->SetParameter(param);
  param = new G4UIparameter("pimQuasiElasticPar3",'d',true);  param->SetDefaultValue(  2.17844  ); pimQuasiElasticParCmd->SetParameter(param);
  param = new G4UIparameter("pimQuasiElasticPar4",'d',true);  param->SetDefaultValue(  1.07658  ); pimQuasiElasticParCmd->SetParameter(param);
  param = new G4UIparameter("pimQuasiElasticPar5",'d',true);  param->SetDefaultValue( -0.0891735); pimQuasiElasticParCmd->SetParameter(param);

  etaQuasiElasticParCmd =  new G4UIcommand("/boone/physics/quasielastic/etaQuasiElasticPar",this);
  etaQuasiElasticParCmd->SetGuidance("Set values of parameters for eta quasielastic");
  etaQuasiElasticParCmd->SetGuidance("Usage: /boone/physics/quasielastic/etaQuasiElasticcPar c0 c1 c2 c3 c4 c5");
  etaQuasiElasticParCmd->SetGuidance("Description: parametrization according to c0 * tanh((p-c1)*c2) + c3 Gauss(c4, c5)");

  param = new G4UIparameter("etaQuasiElasticPar0",'d',true);  param->SetDefaultValue(  4.16121  ); etaQuasiElasticParCmd->SetParameter(param);
  param = new G4UIparameter("etaQuasiElasticPar1",'d',true);  param->SetDefaultValue(  0.583495 ); etaQuasiElasticParCmd->SetParameter(param);
  param = new G4UIparameter("etaQuasiElasticPar2",'d',true);  param->SetDefaultValue(  1.21394  ); etaQuasiElasticParCmd->SetParameter(param);
  param = new G4UIparameter("etaQuasiElasticPar3",'d',true);  param->SetDefaultValue(  2.17844  ); etaQuasiElasticParCmd->SetParameter(param);
  param = new G4UIparameter("etaQuasiElasticPar4",'d',true);  param->SetDefaultValue(  1.07658  ); etaQuasiElasticParCmd->SetParameter(param);
  param = new G4UIparameter("etaQuasiElasticPar5",'d',true);  param->SetDefaultValue( -0.0891735); etaQuasiElasticParCmd->SetParameter(param);
  
}

BooNEpBeInteractionMessenger::~BooNEpBeInteractionMessenger()
{
  delete fModelEnergyRangeCmd;
  delete PiPlusPhysicsModelCmd;
  delete PiMinusPhysicsModelCmd;
  delete KaonPlusPhysicsModelCmd;
  delete KaonMinusPhysicsModelCmd;
  delete KaonZeroLongPhysicsModelCmd;
  delete ProtonPhysicsModelCmd;
  delete NeutronPhysicsModelCmd;
  delete EtaPhysicsModelCmd;
  // delete randomSeedCmd;
  delete SWPiPlusParCmd;
  delete SWPiMinusParCmd;
  delete SWKaonPlusParCmd;
  delete SWKaonZeroLongParCmd;
  delete FSKaonPlusParCmd;
  delete FSKaonZeroLongParCmd;
  delete BeToAScalingCmd;
  delete BeToAScalingParCmd;
  delete PhysicsVerboseCmd;
  delete NoBeamPionsCmd;
  delete SWEtaParCmd;
  
  delete ProtonRwgtFuncCmd;
  delete NeutronRwgtFuncCmd;
  delete PionPlusRwgtFuncCmd;
  delete PionMinusRwgtFuncCmd;
  delete KaonPlusRwgtFuncCmd;
  delete KaonMinusRwgtFuncCmd;
  delete KaonZeroLongRwgtFuncCmd;
  delete EtaRwgtFuncCmd;
  
  delete ProtonRwgtParamsCmd;
  delete NeutronRwgtParamsCmd;
  delete PionPlusRwgtParamsCmd;
  delete PionMinusRwgtParamsCmd;
  delete KaonPlusRwgtParamsCmd;
  delete KaonMinusRwgtParamsCmd;
  delete KaonZeroLongRwgtParamsCmd;
  delete EtaRwgtParamsCmd;
  
}

void BooNEpBeInteractionMessenger::SetNewValue(G4UIcommand * command,G4String newValues)
{ 
  if(command == fModelEnergyRangeCmd) {
    G4Tokenizer next( newValues );
    G4double Elow = StoD(next());
    G4double Ehigh = StoD(next());
    BooNEProtonModel->SetMinEnergy(Elow*CLHEP::GeV);
    BooNEProtonModel->SetMaxEnergy(Ehigh*CLHEP::GeV);
  }

  if(command == PiPlusPhysicsModelCmd)
    BooNEProtonModel->SetPiPlusPhysicsModel(newValues);

  if(command == PiMinusPhysicsModelCmd)
    BooNEProtonModel->SetPiMinusPhysicsModel(newValues);

  if(command == KaonPlusPhysicsModelCmd)
    BooNEProtonModel->SetKaonPlusPhysicsModel(newValues);

  if(command == KaonMinusPhysicsModelCmd)
    BooNEProtonModel->SetKaonMinusPhysicsModel(newValues);

  if(command == KaonZeroLongPhysicsModelCmd)
    BooNEProtonModel->SetKaonZeroLongPhysicsModel(newValues);

  if(command == ProtonPhysicsModelCmd)
    BooNEProtonModel->SetProtonPhysicsModel(newValues);

  if(command == NeutronPhysicsModelCmd)
    BooNEProtonModel->SetNeutronPhysicsModel(newValues);

  
  if(command == EtaPhysicsModelCmd)
    BooNEProtonModel->SetEtaPhysicsModel(newValues);

  
  if (command == SWPiPlusParCmd) {
    G4Tokenizer next( newValues );
    fSWPiPlusPar1Value = StoD(next());
    fSWPiPlusPar2Value = StoD(next());
    fSWPiPlusPar3Value = StoD(next());
    fSWPiPlusPar4Value = StoD(next());
    fSWPiPlusPar5Value = StoD(next());
    fSWPiPlusPar6Value = StoD(next());
    fSWPiPlusPar7Value = StoD(next());
    fSWPiPlusPar8Value = StoD(next());
    BooNEProtonModel->SetSWPiPlusPar(
				     fSWPiPlusPar1Value, fSWPiPlusPar2Value,
				     fSWPiPlusPar3Value, fSWPiPlusPar4Value,
				     fSWPiPlusPar5Value, fSWPiPlusPar6Value,
				     fSWPiPlusPar7Value, fSWPiPlusPar8Value);
  }

  if (command == SWPiMinusParCmd) {
    G4Tokenizer next( newValues );
    fSWPiMinusPar1Value = StoD(next());
    fSWPiMinusPar2Value = StoD(next());
    fSWPiMinusPar3Value = StoD(next());
    fSWPiMinusPar4Value = StoD(next());
    fSWPiMinusPar5Value = StoD(next());
    fSWPiMinusPar6Value = StoD(next());
    fSWPiMinusPar7Value = StoD(next());
    fSWPiMinusPar8Value = StoD(next());
    BooNEProtonModel->SetSWPiMinusPar(
				     fSWPiMinusPar1Value, fSWPiMinusPar2Value,
				     fSWPiMinusPar3Value, fSWPiMinusPar4Value,
				     fSWPiMinusPar5Value, fSWPiMinusPar6Value,
				     fSWPiMinusPar7Value, fSWPiMinusPar8Value);
  }

  if (command == SWKaonPlusParCmd) {
    G4Tokenizer next( newValues );
    fSWKaonPlusPar1Value = StoD(next());
    fSWKaonPlusPar2Value = StoD(next());
    fSWKaonPlusPar3Value = StoD(next());
    fSWKaonPlusPar4Value = StoD(next());
    fSWKaonPlusPar5Value = StoD(next());
    fSWKaonPlusPar6Value = StoD(next());
    fSWKaonPlusPar7Value = StoD(next());
    fSWKaonPlusPar8Value = StoD(next());
    fSWKaonPlusPar9Value = StoD(next());
    BooNEProtonModel->SetSWKaonPlusPar(
				     fSWKaonPlusPar1Value, fSWKaonPlusPar2Value,
				     fSWKaonPlusPar3Value, fSWKaonPlusPar4Value,
				     fSWKaonPlusPar5Value, fSWKaonPlusPar6Value,
				     fSWKaonPlusPar7Value, fSWKaonPlusPar8Value,
				     fSWKaonPlusPar9Value);
  }

  if (command == SWKaonZeroLongParCmd) {
    G4Tokenizer next( newValues );
    fSWKaonZeroLongPar1Value = StoD(next());
    fSWKaonZeroLongPar2Value = StoD(next());
    fSWKaonZeroLongPar3Value = StoD(next());
    fSWKaonZeroLongPar4Value = StoD(next());
    fSWKaonZeroLongPar5Value = StoD(next());
    fSWKaonZeroLongPar6Value = StoD(next());
    fSWKaonZeroLongPar7Value = StoD(next());
    fSWKaonZeroLongPar8Value = StoD(next());
    fSWKaonZeroLongPar9Value = StoD(next());
    BooNEProtonModel->SetSWKaonZeroLongPar(
				     fSWKaonZeroLongPar1Value, fSWKaonZeroLongPar2Value,
				     fSWKaonZeroLongPar3Value, fSWKaonZeroLongPar4Value,
				     fSWKaonZeroLongPar5Value, fSWKaonZeroLongPar6Value,
				     fSWKaonZeroLongPar7Value, fSWKaonZeroLongPar8Value,
				     fSWKaonZeroLongPar9Value);
  }

  if (command == FSKaonPlusParCmd) {
    G4Tokenizer next( newValues );
    fFSKaonPlusPar1Value = StoD(next());
    fFSKaonPlusPar2Value = StoD(next());
    fFSKaonPlusPar3Value = StoD(next());
    fFSKaonPlusPar4Value = StoD(next());
    fFSKaonPlusPar5Value = StoD(next());
    fFSKaonPlusPar6Value = StoD(next());
    fFSKaonPlusPar7Value = StoD(next());
    fFSKaonPlusPar8Value = StoD(next());
    BooNEProtonModel->SetFSKaonPlusPar(
				     fFSKaonPlusPar1Value, fFSKaonPlusPar2Value,
				     fFSKaonPlusPar3Value, fFSKaonPlusPar4Value,
				     fFSKaonPlusPar5Value, fFSKaonPlusPar6Value,
				     fFSKaonPlusPar7Value, fFSKaonPlusPar8Value);
  }

  if (command == FSKaonZeroLongParCmd) {
    G4Tokenizer next( newValues );
    fFSKaonZeroLongPar1Value = StoD(next());
    fFSKaonZeroLongPar2Value = StoD(next());
    fFSKaonZeroLongPar3Value = StoD(next());
    fFSKaonZeroLongPar4Value = StoD(next());
    fFSKaonZeroLongPar5Value = StoD(next());
    fFSKaonZeroLongPar6Value = StoD(next());
    fFSKaonZeroLongPar7Value = StoD(next());
    fFSKaonZeroLongPar8Value = StoD(next());
    BooNEProtonModel->SetFSKaonZeroLongPar(
				     fFSKaonZeroLongPar1Value, fFSKaonZeroLongPar2Value,
				     fFSKaonZeroLongPar3Value, fFSKaonZeroLongPar4Value,
				     fFSKaonZeroLongPar5Value, fFSKaonZeroLongPar6Value,
				     fFSKaonZeroLongPar7Value, fFSKaonZeroLongPar8Value);
  }


  if (command == SWEtaParCmd) {
    G4Tokenizer next( newValues );
    fSWEtaPar1Value = StoD(next());
    fSWEtaPar2Value = StoD(next());
    fSWEtaPar3Value = StoD(next());
    fSWEtaPar4Value = StoD(next());
    fSWEtaPar5Value = StoD(next());
    fSWEtaPar6Value = StoD(next());
    fSWEtaPar7Value = StoD(next());
    fSWEtaPar8Value = StoD(next());
    BooNEProtonModel->SetSWEtaPar(
				     fSWEtaPar1Value, fSWEtaPar2Value,
				     fSWEtaPar3Value, fSWEtaPar4Value,
				     fSWEtaPar5Value, fSWEtaPar6Value,
				     fSWEtaPar7Value, fSWEtaPar8Value);
  }

  
  if(command == BeToAScalingCmd) {
    G4int vl;
    const char* t = newValues;
    std::istringstream is((char*)t);
    is >> vl;
    BooNEProtonModel->SetUseBeToAScaling(vl!=0);
  }

  if (command == BeToAScalingParCmd) {
    G4Tokenizer next( newValues );
    fBeToAScalingPar0Value = StoD(next());
    fBeToAScalingPar1Value = StoD(next());
    fBeToAScalingPar2Value = StoD(next());
    BooNEProtonModel->SetBeToAScalingPar(
					 fBeToAScalingPar0Value, 
					 fBeToAScalingPar1Value,
					 fBeToAScalingPar2Value);
  }

  if(command == PhysicsVerboseCmd) {
    G4int vl;
    const char* t = newValues;
    std::istringstream is((char*)t);
    is >> vl;
    BooNEProtonModel->SetPhysicsVerbose(vl!=0);
  }

  if(command == NoBeamPionsCmd) {
    G4int vl;
    const char* t = newValues;
    std::istringstream is((char*)t);
    is >> vl;
    BooNEProtonModel->SetNoBeamPions(vl!=0);
  }

  // set quasi elastic scattering parameters:  
  // in new mc this is used only for 8GeV protons, but keeping all
  // parameters for now
  // protons
  if (command == proQuasiElasticParCmd){
    G4Tokenizer next( newValues );
    for(int i = 0; i < 6; i++)proQuasiElasticPar[i] = StoD(next());
    BooNEProtonModel->SetProtonQuasiElasticParameters(proQuasiElasticPar, 6);
  }

  // neutrons
  if (command == neuQuasiElasticParCmd){
    G4Tokenizer next( newValues );
    for(int i = 0; i < 6; i++)neuQuasiElasticPar[i] = StoD(next());
    BooNEProtonModel->SetNeutronQuasiElasticParameters(neuQuasiElasticPar, 6);
  }
  
  // pion plus
  if (command == pipQuasiElasticParCmd){
    G4Tokenizer next( newValues );
    for(int i = 0; i < 6; i++)pipQuasiElasticPar[i] = StoD(next());
    BooNEProtonModel->SetPionPlusQuasiElasticParameters(pipQuasiElasticPar, 6);
  }

  // pion minus
  if (command == pimQuasiElasticParCmd){
    G4Tokenizer next( newValues );
    for(int i = 0; i < 6; i++)pimQuasiElasticPar[i] = StoD(next());
    BooNEProtonModel->SetPionMinusQuasiElasticParameters(pimQuasiElasticPar, 6);
  }

  
  // eta
  if (command == etaQuasiElasticParCmd){
    G4Tokenizer next( newValues );
    for(int i = 0; i < 6; i++)etaQuasiElasticPar[i] = StoD(next());
    BooNEProtonModel->SetEtaQuasiElasticParameters(etaQuasiElasticPar, 6);
  }

  
  if(command == ProtonRwgtFuncCmd)
    BooNEProtonModel->SetProtonRwgtFunc(newValues);

  if(command == NeutronRwgtFuncCmd)
    BooNEProtonModel->SetNeutronRwgtFunc(newValues);

  if(command == PionPlusRwgtFuncCmd)
    BooNEProtonModel->SetPionPlusRwgtFunc(newValues);

  if(command == PionMinusRwgtFuncCmd)
    BooNEProtonModel->SetPionMinusRwgtFunc(newValues);

  if(command == KaonPlusRwgtFuncCmd)
    BooNEProtonModel->SetKaonPlusRwgtFunc(newValues);

  if(command == KaonMinusRwgtFuncCmd)
    BooNEProtonModel->SetKaonMinusRwgtFunc(newValues);

  if(command == KaonZeroLongRwgtFuncCmd)
    BooNEProtonModel->SetKaonZeroLongRwgtFunc(newValues);

  
  if(command == EtaRwgtFuncCmd)
    BooNEProtonModel->SetEtaRwgtFunc(newValues);

  
  if (command == ProtonRwgtParamsCmd) {

    G4Tokenizer next( newValues );
    G4double protonRwgtParam0 = StoD(next());
    G4double protonRwgtParam1 = StoD(next());
    G4double protonRwgtParam2 = StoD(next());
    G4double protonRwgtParam3 = StoD(next());
    G4double protonRwgtParam4 = StoD(next());
    G4double protonRwgtParam5 = StoD(next());
    G4double protonRwgtParam6 = StoD(next());
    G4double protonRwgtParam7 = StoD(next());
    G4double protonRwgtParam8 = StoD(next());
    G4double protonRwgtParam9 = StoD(next());

    BooNEProtonModel->SetProtonRwgtParams(protonRwgtParam0,
					  protonRwgtParam1,
					  protonRwgtParam2,
					  protonRwgtParam3,
					  protonRwgtParam4,
					  protonRwgtParam5,
					  protonRwgtParam6,
					  protonRwgtParam7,
					  protonRwgtParam8,
					  protonRwgtParam9);

  }

  if (command == NeutronRwgtParamsCmd) {

    G4Tokenizer next( newValues );
    G4double neutronRwgtParam0 = StoD(next());
    G4double neutronRwgtParam1 = StoD(next());
    G4double neutronRwgtParam2 = StoD(next());
    G4double neutronRwgtParam3 = StoD(next());
    G4double neutronRwgtParam4 = StoD(next());
    G4double neutronRwgtParam5 = StoD(next());
    G4double neutronRwgtParam6 = StoD(next());
    G4double neutronRwgtParam7 = StoD(next());
    G4double neutronRwgtParam8 = StoD(next());
    G4double neutronRwgtParam9 = StoD(next());

    BooNEProtonModel->SetNeutronRwgtParams(neutronRwgtParam0,
					   neutronRwgtParam1,
					   neutronRwgtParam2,
					   neutronRwgtParam3,
					   neutronRwgtParam4,
					   neutronRwgtParam5,
					   neutronRwgtParam6,
					   neutronRwgtParam7,
					   neutronRwgtParam8,
					   neutronRwgtParam9);

  }

  if (command == PionPlusRwgtParamsCmd) {

    G4Tokenizer next( newValues );
    G4double pionPlusRwgtParam0 = StoD(next());
    G4double pionPlusRwgtParam1 = StoD(next());
    G4double pionPlusRwgtParam2 = StoD(next());
    G4double pionPlusRwgtParam3 = StoD(next());
    G4double pionPlusRwgtParam4 = StoD(next());
    G4double pionPlusRwgtParam5 = StoD(next());
    G4double pionPlusRwgtParam6 = StoD(next());
    G4double pionPlusRwgtParam7 = StoD(next());
    G4double pionPlusRwgtParam8 = StoD(next());
    G4double pionPlusRwgtParam9 = StoD(next());

    BooNEProtonModel->SetPionPlusRwgtParams(pionPlusRwgtParam0,
					    pionPlusRwgtParam1,
					    pionPlusRwgtParam2,
					    pionPlusRwgtParam3,
					    pionPlusRwgtParam4,
					    pionPlusRwgtParam5,
					    pionPlusRwgtParam6,
					    pionPlusRwgtParam7,
					    pionPlusRwgtParam8,
					    pionPlusRwgtParam9);

  }

  if (command == PionMinusRwgtParamsCmd) {

    G4Tokenizer next( newValues );
    G4double pionMinusRwgtParam0 = StoD(next());
    G4double pionMinusRwgtParam1 = StoD(next());
    G4double pionMinusRwgtParam2 = StoD(next());
    G4double pionMinusRwgtParam3 = StoD(next());
    G4double pionMinusRwgtParam4 = StoD(next());
    G4double pionMinusRwgtParam5 = StoD(next());
    G4double pionMinusRwgtParam6 = StoD(next());
    G4double pionMinusRwgtParam7 = StoD(next());
    G4double pionMinusRwgtParam8 = StoD(next());
    G4double pionMinusRwgtParam9 = StoD(next());

    BooNEProtonModel->SetPionMinusRwgtParams(pionMinusRwgtParam0,
					     pionMinusRwgtParam1,
					     pionMinusRwgtParam2,
					     pionMinusRwgtParam3,
					     pionMinusRwgtParam4,
					     pionMinusRwgtParam5,
					     pionMinusRwgtParam6,
					     pionMinusRwgtParam7,
					     pionMinusRwgtParam8,
					     pionMinusRwgtParam9);

  }

  if (command == KaonPlusRwgtParamsCmd) {

    G4Tokenizer next( newValues );
    G4double kaonPlusRwgtParam0 = StoD(next());
    G4double kaonPlusRwgtParam1 = StoD(next());
    G4double kaonPlusRwgtParam2 = StoD(next());
    G4double kaonPlusRwgtParam3 = StoD(next());
    G4double kaonPlusRwgtParam4 = StoD(next());
    G4double kaonPlusRwgtParam5 = StoD(next());
    G4double kaonPlusRwgtParam6 = StoD(next());
    G4double kaonPlusRwgtParam7 = StoD(next());
    G4double kaonPlusRwgtParam8 = StoD(next());
    G4double kaonPlusRwgtParam9 = StoD(next());

    BooNEProtonModel->SetKaonPlusRwgtParams(kaonPlusRwgtParam0,
					    kaonPlusRwgtParam1,
					    kaonPlusRwgtParam2,
					    kaonPlusRwgtParam3,
					    kaonPlusRwgtParam4,
					    kaonPlusRwgtParam5,
					    kaonPlusRwgtParam6,
					    kaonPlusRwgtParam7,
					    kaonPlusRwgtParam8,
					    kaonPlusRwgtParam9);

  }

  if (command == KaonMinusRwgtParamsCmd) {

    G4Tokenizer next( newValues );
    G4double kaonMinusRwgtParam0 = StoD(next());
    G4double kaonMinusRwgtParam1 = StoD(next());
    G4double kaonMinusRwgtParam2 = StoD(next());
    G4double kaonMinusRwgtParam3 = StoD(next());
    G4double kaonMinusRwgtParam4 = StoD(next());
    G4double kaonMinusRwgtParam5 = StoD(next());
    G4double kaonMinusRwgtParam6 = StoD(next());
    G4double kaonMinusRwgtParam7 = StoD(next());
    G4double kaonMinusRwgtParam8 = StoD(next());
    G4double kaonMinusRwgtParam9 = StoD(next());

    BooNEProtonModel->SetKaonMinusRwgtParams(kaonMinusRwgtParam0,
					     kaonMinusRwgtParam1,
					     kaonMinusRwgtParam2,
					     kaonMinusRwgtParam3,
					     kaonMinusRwgtParam4,
					     kaonMinusRwgtParam5,
					     kaonMinusRwgtParam6,
					     kaonMinusRwgtParam7,
					     kaonMinusRwgtParam8,
					     kaonMinusRwgtParam9);

  }

  if (command == KaonZeroLongRwgtParamsCmd) {

    G4Tokenizer next( newValues );
    G4double kaonZeroLongRwgtParam0 = StoD(next());
    G4double kaonZeroLongRwgtParam1 = StoD(next());
    G4double kaonZeroLongRwgtParam2 = StoD(next());
    G4double kaonZeroLongRwgtParam3 = StoD(next());
    G4double kaonZeroLongRwgtParam4 = StoD(next());
    G4double kaonZeroLongRwgtParam5 = StoD(next());
    G4double kaonZeroLongRwgtParam6 = StoD(next());
    G4double kaonZeroLongRwgtParam7 = StoD(next());
    G4double kaonZeroLongRwgtParam8 = StoD(next());
    G4double kaonZeroLongRwgtParam9 = StoD(next());

    BooNEProtonModel->SetKaonZeroLongRwgtParams(kaonZeroLongRwgtParam0,
						kaonZeroLongRwgtParam1,
						kaonZeroLongRwgtParam2,
						kaonZeroLongRwgtParam3,
						kaonZeroLongRwgtParam4,
						kaonZeroLongRwgtParam5,
						kaonZeroLongRwgtParam6,
						kaonZeroLongRwgtParam7,
						kaonZeroLongRwgtParam8,
						kaonZeroLongRwgtParam9);

  }


  if (command == EtaRwgtParamsCmd) {

    G4Tokenizer next( newValues );
    G4double etaRwgtParam0 = StoD(next());
    G4double etaRwgtParam1 = StoD(next());
    G4double etaRwgtParam2 = StoD(next());
    G4double etaRwgtParam3 = StoD(next());
    G4double etaRwgtParam4 = StoD(next());
    G4double etaRwgtParam5 = StoD(next());
    G4double etaRwgtParam6 = StoD(next());
    G4double etaRwgtParam7 = StoD(next());
    G4double etaRwgtParam8 = StoD(next());
    G4double etaRwgtParam9 = StoD(next());

    BooNEProtonModel->SetEtaRwgtParams(etaRwgtParam0,
					    etaRwgtParam1,
					    etaRwgtParam2,
					    etaRwgtParam3,
					    etaRwgtParam4,
					    etaRwgtParam5,
					    etaRwgtParam6,
					    etaRwgtParam7,
					    etaRwgtParam8,
					    etaRwgtParam9);

  }


  
}

