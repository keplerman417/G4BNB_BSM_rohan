#include "G4Run.hh"
#include "G4ios.hh"

#include "NuBeamRunAction.hh"
#include "NuBeamRunActionMessenger.hh"
#include "NuBeamRunManager.hh"
#include "NuBeamOutput.hh"

NuBeamRunAction::NuBeamRunAction() 
{
  const NuBeamRunManager *runManager=
    reinterpret_cast<const NuBeamRunManager*>(G4RunManager::GetRunManager());
  
  fRecord = runManager->GetRecordPtr();
  
  fRunActionMessenger = new NuBeamRunActionMessenger(this);
}

NuBeamRunAction::~NuBeamRunAction() 
{
}

void NuBeamRunAction::BeginOfRunAction(const G4Run* aRun)
{
  // Initiate record-keeping for this run.
  G4cout<< "NuBeamRunAction::BeginOfRunAction" << G4endl;
  fRecord->RecordBeginOfRun(aRun);
}

void NuBeamRunAction::EndOfRunAction(const G4Run* aRun)
{
  // Terminate record-keeping for this run.
  G4cout<< "NuBeamRunAction::EndOfRunAction" << G4endl;
  fRecord->RecordEndOfRun(aRun);
}
