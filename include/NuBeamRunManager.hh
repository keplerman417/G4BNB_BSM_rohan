#ifndef NuBeamRunManager_h
#define NuBeamRunManager_h 1

#include "G4RunManager.hh"

class NuBeamOutput;

class NuBeamRunManager : public G4RunManager
{
public:
  NuBeamRunManager(NuBeamOutput *rec);
  virtual ~NuBeamRunManager();
  
  virtual void InitializeGeometry();
  virtual void InitializePhysics();

  inline NuBeamOutput* GetRecordPtr() const {return fRecord; }

private:
  bool fGeometryInitializedHere;
  bool fPhysicsInitializedHere; 
  NuBeamOutput *fRecord; 
      
};

#endif
