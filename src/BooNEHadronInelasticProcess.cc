#include "BooNEHadronInelasticProcess.hh"
#include "BooNEHadronInelasticDataSet.hh"
#include "BooNEpBeInteraction.hh"
#include "NuBeamRunManager.hh"
#include "NuBeamOutput.hh"
#include "NuBeamTrackInformation.hh"
#include "G4HadronInelasticDataSet.hh"
#include "G4ComponentGGHadronNucleusXsc.hh"
#include "G4ParticleDefinition.hh"
#include "G4CascadeInterface.hh"
#include "G4TheoFSGenerator.hh"
#include "G4FTFModel.hh"
#include "G4ExcitedStringDecay.hh"
#include "G4LundStringFragmentation.hh"
#include "G4CrossSectionInelastic.hh"
#include "G4GeneratorPrecompoundInterface.hh"
#include "G4PreCompoundModel.hh"
#include "G4BGGNucleonInelasticXS.hh"

BooNEHadronInelasticProcess::
BooNEHadronInelasticProcess(const G4ParticleDefinition& aParticleType)
  :  G4HadronicProcess("BooNEHadronInelastic")
{
  fParticle=&aParticleType;
  BooNEHadronInelasticDataSet* booneXsec=new BooNEHadronInelasticDataSet();
  if (aParticleType==*(G4Proton::Proton())) {
    // Note:  Before creating a BooNEpBeInteraction instance, make sure all
    //        data sets have been added to theProtonInelastic.  This is
    //        necessary to ensure that the multiplicities calculated in
    //        BooNEpBeInteraction correspond to the correct p-Be cross-section.
    AddDataSet(new G4HadronInelasticDataSet());
    AddDataSet(booneXsec); //BooNE xsec 
    fLEProtonModel=new G4CascadeInterface();
    fLEProtonModel->SetMaxEnergy(3.5*CLHEP::GeV);
    RegisterMe(fLEProtonModel);
    //lot of pointers here that should be deleted in the end 
    fHEProtonModel = new G4TheoFSGenerator("FTFP");
    G4FTFModel*   theProtonStringModel = new G4FTFModel;
    G4ExcitedStringDecay* theProtonStringDecay = new G4ExcitedStringDecay(new G4LundStringFragmentation);
    theProtonStringModel->SetFragmentationModel(theProtonStringDecay);
    
    G4PreCompoundModel* theProtonPreEquilib = new G4PreCompoundModel(new G4ExcitationHandler);
    G4GeneratorPrecompoundInterface*   theProtonCascade = new G4GeneratorPrecompoundInterface(theProtonPreEquilib);
   
    ((G4TheoFSGenerator*)fHEProtonModel)->SetHighEnergyGenerator(theProtonStringModel);
    G4QuasiElasticChannel*  theProtonQuasiElastic=new G4QuasiElasticChannel;
    ((G4TheoFSGenerator*)fHEProtonModel)->SetQuasiElasticChannel(theProtonQuasiElastic);
  
    ((G4TheoFSGenerator*)fHEProtonModel)->SetTransport(theProtonCascade);
    fHEProtonModel->SetMinEnergy(3.0*CLHEP::GeV);
    fHEProtonModel->SetMaxEnergy(100.*CLHEP::GeV);
    RegisterMe(fHEProtonModel);
    
    fBooNEpBeModel=new BooNEpBeInteraction();
    RegisterMe(fBooNEpBeModel);
  } else if (aParticleType==*(G4Neutron::Neutron())) {
    G4CascadeInterface* theLENeutronModel=new G4CascadeInterface();
    theLENeutronModel->SetMaxEnergy(5.0*CLHEP::GeV);
    RegisterMe(theLENeutronModel);

    G4TheoFSGenerator* theHENeutronModel = new G4TheoFSGenerator("FTFP");
    G4FTFModel*   theNeutronStringModel = new G4FTFModel;
    G4ExcitedStringDecay* theNeutronStringDecay = new G4ExcitedStringDecay(new G4LundStringFragmentation);
    theNeutronStringModel->SetFragmentationModel(theNeutronStringDecay);
    
    G4PreCompoundModel* theNeutronPreEquilib = new G4PreCompoundModel(new G4ExcitationHandler);
    G4GeneratorPrecompoundInterface*   theNeutronCascade = new G4GeneratorPrecompoundInterface(theNeutronPreEquilib);
   
    theHENeutronModel->SetHighEnergyGenerator(theNeutronStringModel);
    G4QuasiElasticChannel*  theNeutronQuasiElastic=new G4QuasiElasticChannel;
    theHENeutronModel->SetQuasiElasticChannel(theNeutronQuasiElastic);
  
    theHENeutronModel->SetTransport(theNeutronCascade);
    theHENeutronModel->SetMinEnergy(4.0*CLHEP::GeV);
    theHENeutronModel->SetMaxEnergy(100.*CLHEP::GeV);
    RegisterMe(theHENeutronModel);
    AddDataSet(new G4HadronInelasticDataSet());
    AddDataSet(booneXsec); //BooNE xsec 
  } else if (aParticleType==*(G4PionPlus::PionPlus()) ||
	     aParticleType==*(G4PionMinus::PionMinus()) ||
	     //aParticleType==*(G4Eta::Eta()) ||
	     aParticleType==*(G4PionZero::PionZero()) ||
	     aParticleType==*(G4KaonPlus::KaonPlus()) ||
	     aParticleType==*(G4KaonMinus::KaonMinus()) ||
	     aParticleType==*(G4KaonZeroLong::KaonZeroLong()) ||
	     aParticleType==*(G4KaonZeroShort::KaonZeroShort())) {
    G4CascadeInterface* theBertiniPiKModel=new G4CascadeInterface;
    theBertiniPiKModel->SetMinEnergy(0.*CLHEP::GeV);
    theBertiniPiKModel->SetMaxEnergy(3.5*CLHEP::GeV);
    G4TheoFSGenerator* theFTFPPiKModel = new G4TheoFSGenerator("FTFP");
    G4FTFModel* thePiKStringModel = new G4FTFModel;
    G4ExcitedStringDecay* thePiKStringDecay = new G4ExcitedStringDecay(new G4LundStringFragmentation);
    thePiKStringModel->SetFragmentationModel(thePiKStringDecay);
    G4GeneratorPrecompoundInterface* thePiKCascade = new G4GeneratorPrecompoundInterface();
    theFTFPPiKModel->SetHighEnergyGenerator(thePiKStringModel);
    G4QuasiElasticChannel* thePiKQuasiElastic=new G4QuasiElasticChannel();
    theFTFPPiKModel->SetQuasiElasticChannel(thePiKQuasiElastic);  
    theFTFPPiKModel->SetTransport(thePiKCascade);
    theFTFPPiKModel->SetMinEnergy(3.0*CLHEP::GeV);

    RegisterMe(theBertiniPiKModel);
    RegisterMe(theFTFPPiKModel);
    /*
      void AddDataSet( G4VCrossSectionDataSet* aDataSet )
      
      This method adds the given cross section data set to the end of the list of data sets in the data store. For the evaluation of cross sections, the list has a LIFO (Last In First Out) priority, meaning that data sets added later to the list will have priority over those added earlier to the list. Another way of saying this, is that the data store, when given a GetCrossSection request, does the IsApplicable queries in the reverse list order, starting with the last data set in the list and proceeding to the first, and the first data set that responds positively is used to calculate the cross section.
     */

    //Old MC used Gheisha xsec outside booneXsec, so we will do the same
    //In future should consider newer better xsec. Note however, since kaons were measured 
    //in SciBooNE with Gheisha xsec, they should be remeasured and reweighted 
    //with new fit once the cross sections are changed
   
    //G4ComponentGGHadronNucleusXsc* theKaonXsec = new G4ComponentGGHadronNucleusXsc();
    //G4VCrossSectionDataSet* theKaonDataSet = new G4CrossSectionInelastic(theKaonXsec);
    //thePiData = new G4PiNuclearCrossSection;
    //AddDataSet(theKaonDataSet);

    AddDataSet(new G4HadronInelasticDataSet());
    AddDataSet(booneXsec); //BooNE xsec 
  }
 
}

BooNEHadronInelasticProcess::~BooNEHadronInelasticProcess()
{
}
 
void BooNEHadronInelasticProcess::
BuildPhysicsTable(const G4ParticleDefinition& aParticleType)
{
  if (!G4HadronicProcess::GetCrossSectionDataStore()) {
    return;
  }
  G4HadronicProcess::GetCrossSectionDataStore()->BuildPhysicsTable(aParticleType);
}

G4bool
BooNEHadronInelasticProcess::
IsApplicable(const G4ParticleDefinition& aParticleType)
{
  return (aParticleType == *(G4PionPlus::PionPlus()  )  ||
	  aParticleType == *(G4PionMinus::PionMinus())  ||
	  aParticleType == *(G4PionZero::PionZero()  )  ||
	  //aParticleType == *(G4Eta::Eta())  ||
	  aParticleType == *(G4KaonPlus::KaonPlus()  )  ||
	  aParticleType == *(G4KaonMinus::KaonMinus())  ||
	  aParticleType == *(G4KaonZeroLong::KaonZeroLong()) || 
	  aParticleType == *(G4KaonZeroShort::KaonZeroShort()) ||
	  aParticleType == *(G4Proton::Proton()      )  ||
	  aParticleType == *(G4Neutron::Neutron()    ) 
	  );
}

G4VParticleChange*
BooNEHadronInelasticProcess::PostStepDoIt(const G4Track& aTrack, const G4Step&)
{
  // if primary is not Alive then do nothing
  theTotalResult->Clear();
  theTotalResult->Initialize(aTrack);
  theTotalResult->ProposeWeight(aTrack.GetWeight());
  if(aTrack.GetTrackStatus() != fAlive) { return theTotalResult; }

  // Find cross section at end of step and check if <= 0
  //
  const G4DynamicParticle* aParticle = aTrack.GetDynamicParticle();
  G4Material* aMaterial = aTrack.GetMaterial();

  G4Element* anElement = 0;
  try
  {
    anElement = GetCrossSectionDataStore()->SampleZandA(aParticle,
							 aMaterial,
							 *GetTargetNucleusPointer());
  }
  catch(G4HadronicException & aR)
  {
    G4ExceptionDescription ed;
    aR.Report(ed);
    DumpState(aTrack,"SampleZandA",ed);
    ed << " PostStepDoIt failed on element selection" << G4endl;
    G4Exception("G4HadronicProcess::PostStepDoIt", "had003", FatalException,
		ed);
  }

  // check only for charged particles
  if(aParticle->GetDefinition()->GetPDGCharge() != 0.0) {
    if (GetElementCrossSection(aParticle, anElement, aMaterial) <= 0.0) {
      // No interaction
      return theTotalResult;
    }    
  }

  // Next check for illegal track status
  //
  if (aTrack.GetTrackStatus() != fAlive && 
      aTrack.GetTrackStatus() != fSuspend) {
    if (aTrack.GetTrackStatus() == fStopAndKill ||
        aTrack.GetTrackStatus() == fKillTrackAndSecondaries ||
        aTrack.GetTrackStatus() == fPostponeToNextEvent) {
      G4ExceptionDescription ed;
      ed << "G4HadronicProcess: track in unusable state - "
	 << aTrack.GetTrackStatus() << G4endl;
      ed << "G4HadronicProcess: returning unchanged track " << G4endl;
      DumpState(aTrack,"PostStepDoIt",ed);
      G4Exception("G4HadronicProcess::PostStepDoIt", "had004", JustWarning, ed);
    }
    // No warning for fStopButAlive which is a legal status here
    return theTotalResult;
  }

  // Initialize the hadronic projectile from the track
  thePro.Initialise(aTrack);

  try
  {
    fInteraction =
      this->ChooseHadronicInteraction( thePro, *GetTargetNucleusPointer(), aMaterial, anElement );
  }
  catch(G4HadronicException & aE)
  {
    G4ExceptionDescription ed;
    aE.Report(ed);
    ed << "Target element "<<anElement->GetName()<<"  Z= "
       << GetTargetNucleus()->GetZ_asInt() << "  A= "
       << GetTargetNucleus()->GetA_asInt() << G4endl;
    DumpState(aTrack,"ChooseHadronicInteraction",ed);
    ed << " No HadronicInteraction found out" << G4endl;
    G4Exception("G4HadronicProcess::PostStepDoIt", "had005", FatalException,
		ed);
  }

  G4HadFinalState* result = 0;
  G4int reentryCount = 0;

  do
  {
    try
    {
      // Call the interaction
      result = fInteraction->ApplyYourself( thePro, *GetTargetNucleusPointer());
      ++reentryCount;
    }
    catch(G4HadronicException aR)
    {
      G4ExceptionDescription ed;
      aR.Report(ed);
      ed << "Call for " << fInteraction->GetModelName() << G4endl;
      ed << "Target element "<<anElement->GetName()<<"  Z= "
	 << GetTargetNucleus()->GetZ_asInt()
	 << "  A= " << GetTargetNucleus()->GetA_asInt() << G4endl;
      DumpState(aTrack,"ApplyYourself",ed);
      ed << " ApplyYourself failed" << G4endl;
      G4Exception("G4HadronicProcess::PostStepDoIt", "had006", FatalException,
		  ed);
    }
    
    // Check the result for catastrophic energy non-conservation
    CheckResult(thePro, *GetTargetNucleusPointer(), result);

    if(reentryCount>100) {
      G4ExceptionDescription ed;
      ed << "Call for " << fInteraction->GetModelName() << G4endl;
      ed << "Target element "<<anElement->GetName()<<"  Z= "
	 << GetTargetNucleus()->GetZ_asInt()
	 << "  A= " <<GetTargetNucleus()->GetA_asInt() << G4endl;
      DumpState(aTrack,"ApplyYourself",ed);
      ed << " ApplyYourself does not completed after 100 attempts" << G4endl;
      G4Exception("G4HadronicProcess::PostStepDoIt", "had006", FatalException,
		  ed);
    }
  }
  while(!result);

  //record primary proton TGT interactions
  //Will do this in BooNEpBe to be consistent with old MC
  /*
  if (aTrack.GetParticleDefinition()==G4Proton::Proton()&&
      aTrack.GetKineticEnergy()>7.5*CLHEP::GeV &&
      aTrack.GetKineticEnergy()<=8.01*CLHEP::GeV &&
      GetTargetNucleus()->GetZ_asInt()==4) {
    const NuBeamRunManager *pRunManager=
      reinterpret_cast<const NuBeamRunManager*>(G4RunManager::GetRunManager());
    NuBeamOutput* records = pRunManager->GetRecordPtr();
    if (records != NULL) records->RecordpBeInteraction(result);
  }
  */
  result->SetTrafoToLab(thePro.GetTrafoToLab());

  ClearNumberOfInteractionLengthLeft();

  FillResult(result, aTrack);

  if (epReportLevel != 0) {
    CheckEnergyMomentumConservation(aTrack, *GetTargetNucleusPointer());
  }
  
  for (int i=0;i<theTotalResult->GetNumberOfSecondaries();i++) {
    G4Track* secTrack=theTotalResult->GetSecondary(i);
    NuBeamTrackInformation* tInfo=dynamic_cast<NuBeamTrackInformation*>
      (secTrack->GetUserInformation());
    if (tInfo==NULL) {
      tInfo=new NuBeamTrackInformation();
      secTrack->SetUserInformation(tInfo);
    }
    tInfo->SetCreatorModelName(GetHadronicInteraction()->GetModelName()); 
  }

  return theTotalResult;
}

G4HadronicInteraction* BooNEHadronInelasticProcess::ChooseHadronicInteraction(G4HadProjectile& hadProj, G4Nucleus& aNucl, G4Material*& aMat, G4Element*& anElement)
{
  //returns BooNpBe for protons in valid energy range on Be
  //otherwise picks the LE proton model or HE proton model
  //for any other particle uses standard G4 ChooseHadronicInteraction
  G4HadronicInteraction* hadInt;
  if (hadProj.GetDefinition()==G4Proton::Proton() && 
      hadProj.GetKineticEnergy()>fBooNEpBeModel->GetMinEnergy() && 
      hadProj.GetKineticEnergy()<fBooNEpBeModel->GetMaxEnergy() &&
      anElement==G4Element::GetElement("Be"))
    hadInt=fBooNEpBeModel;
  else if (hadProj.GetDefinition()==G4Proton::Proton()) {
    G4double energy=hadProj.GetKineticEnergy();
    if (energy<fHEProtonModel->GetMinEnergy())
      hadInt=fLEProtonModel;
    else if (energy>fLEProtonModel->GetMaxEnergy())
      hadInt=fHEProtonModel;
    else if ((energy>fHEProtonModel->GetMinEnergy()) &&
	     (energy<fLEProtonModel->GetMaxEnergy())) {
      G4double frac=(energy-fHEProtonModel->GetMinEnergy())/
	(fLEProtonModel->GetMaxEnergy()-fHEProtonModel->GetMinEnergy());
      if (G4UniformRand()>frac)
	hadInt=fLEProtonModel;
      else
	hadInt=fHEProtonModel;
    } else {
      throw G4HadronicException(__FILE__, __LINE__,
				"GetHadronicInteraction: No Model found");
      hadInt=0;
    }
  } else 
    hadInt=G4HadronicProcess::ChooseHadronicInteraction(hadProj, aNucl, aMat ,anElement);
  
  return hadInt;
}
