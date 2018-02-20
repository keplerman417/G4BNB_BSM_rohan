#include "NuBeamTrackInformation.hh"
#include "G4ParticleDefinition.hh"
#include "G4ios.hh"

G4Allocator<NuBeamTrackInformation> aTrackInformationAllocator;

NuBeamTrackInformation::NuBeamTrackInformation()
  :
  fCreatorModelName(""),
  fDecayCodeDk2nu(0),
  fIsBiassed(false)
{   
}

NuBeamTrackInformation::NuBeamTrackInformation(const G4Track*)
  :
  fIsBiassed(false)
{
}

NuBeamTrackInformation::NuBeamTrackInformation(const NuBeamTrackInformation* aTrackInfo)
{
  fCreatorModelName = aTrackInfo->fCreatorModelName;
  fDecayCodeDk2nu = aTrackInfo->fDecayCodeDk2nu;
  fIsBiassed = aTrackInfo->fIsBiassed;
}

NuBeamTrackInformation::~NuBeamTrackInformation()
{
}

void NuBeamTrackInformation::Print() const
{
}
