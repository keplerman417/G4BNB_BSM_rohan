#ifndef NuBeamPhysicsList_h
#define NuBeamPhysicsList_h 1

#include "globals.hh"
#include "G4VModularPhysicsList.hh"

class NuBeamPhysicsList: public G4VModularPhysicsList
{
public:
  NuBeamPhysicsList();
  virtual ~NuBeamPhysicsList();

  void CorrectDecayTables();

public:
  virtual void SetCuts();
};

#endif



