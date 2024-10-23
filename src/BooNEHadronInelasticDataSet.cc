#include "BooNEHadronInelasticDataSet.hh"

void BooNEHadronInelasticDataSet::DumpPhysicsTable2()
{
  G4cout << "Dumping out Inelastic Cross sections on Be " << G4endl;


  G4Element*  Be          = new G4Element("Beryllium", "Be", 4, 9.01*(CLHEP::g/CLHEP::mole));
  G4int ZBe=4;
  G4int ABe=9;

  G4double    protonMass    = G4Proton::Proton()->GetPDGMass()/CLHEP::GeV;
  G4double    neutronMass   = G4Neutron::Neutron()->GetPDGMass()/CLHEP::GeV;
  G4double    pionPlusMass  = G4PionPlus::PionPlus()->GetPDGMass()/CLHEP::GeV;
  G4double    pionMinusMass = G4PionMinus::PionMinus()->GetPDGMass()/CLHEP::GeV;
  //G4double    etaMass = G4Eta::Eta()->GetPDGMass()/CLHEP::GeV;

  G4cout << "Mom (GeV/c)           "  
	 << "p Valid    "  
	 << "p (mb)     "  
	 << "n Valid    "  
	 << "n (mb)     "  
	 << "pi+ Valid  "  
	 << "pi+ (mb)   "  
	 << "pi- Valid  "  
	 << "pi-        " 
    //	 << "eta        "  
	 << G4endl; 

    
  for(int i = 1; i < 100; i++){
    G4double momentum = 0.10 * (G4double)i * CLHEP::GeV;
    G4double protonEnergy    = sqrt(momentum*momentum + protonMass*protonMass      );
    G4double neutronEnergy   = sqrt(momentum*momentum + neutronMass*neutronMass    );
    G4double pionPlusEnergy  = sqrt(momentum*momentum + pionPlusMass*pionPlusMass  );
    G4double pionMinusEnergy = sqrt(momentum*momentum + pionMinusMass*pionMinusMass);
    //G4double etaEnergy = sqrt(momentum*momentum + etaMass*etaMass);

    G4LorentzVector pProton   (0, 0, momentum, protonEnergy   );
    G4LorentzVector pNeutron  (0, 0, momentum, neutronEnergy  );
    G4LorentzVector pPionPlus (0, 0, momentum, pionPlusEnergy );
    G4LorentzVector pPionMinus(0, 0, momentum, pionMinusEnergy);
    //G4LorentzVector pEta(0, 0, momentum, etaEnergy);

    G4DynamicParticle *aProton    =   new G4DynamicParticle(G4Proton::Proton(),       pProton);
    G4DynamicParticle *aNeutron   =   new G4DynamicParticle(G4Neutron::Neutron(),     pNeutron);
    G4DynamicParticle *aPionPlus  =   new G4DynamicParticle(G4PionPlus::PionPlus(),   pPionPlus);
    G4DynamicParticle *aPionMinus =   new G4DynamicParticle(G4PionMinus::PionMinus(), pPionMinus);
    //G4DynamicParticle *aEta =   new G4DynamicParticle(G4Eta::Eta(), pEta);

    G4cout << momentum/CLHEP::GeV                                        << "\t" 
	   << this->IsIsoApplicable(aProton, ZBe, ABe, NULL,NULL)        << "\t" 
	   << this->GetCrossSection(aProton, Be, 0)/CLHEP::millibarn     << "\t" 
	   << this->IsIsoApplicable(aNeutron, ZBe, ABe, NULL,NULL)       << "\t" 
	   << this->GetCrossSection(aNeutron, Be, 0)/CLHEP::millibarn    << "\t" 
	   << this->IsIsoApplicable(aPionPlus, ZBe, ABe, NULL,NULL)      << "\t" 
	   << this->GetCrossSection(aPionPlus, Be, 0)/CLHEP::millibarn   << "\t" 
	   << this->IsIsoApplicable(aPionMinus, ZBe, ABe, NULL,NULL)     << "\t" 
	   << this->GetCrossSection(aPionMinus, Be, 0)/CLHEP::millibarn  << "\t"
      //<< this->IsIsoApplicable(aEta, ZBe, ABe, NULL,NULL)     << "\t"
      //<< this->GetCrossSection(aEta, Be, 0)/CLHEP::millibarn  << "\t"
	   << G4endl;

    delete aProton;    aProton    = 0;
    delete aNeutron;   aNeutron   = 0;
    delete aPionPlus;  aPionPlus  = 0;
    delete aPionMinus; aPionMinus = 0;
    //delete aEta; aEta = 0;

  }


  G4cout << "Dumping out Inelastic Cross sections on Al " << G4endl;


  G4Element*  Al          = new G4Element("Aluminum", "Al", 13, 27.01*(CLHEP::g/CLHEP::mole));
  G4int ZAl=13;
  G4int AAl=27;

  G4cout << "Mom (GeV/c)           "  
	 << "p Valid    "  
	 << "p (mb)     "  
	 << "n Valid    "  
	 << "n (mb)     "  
	 << "pi+ Valid  "  
	 << "pi+ (mb)   "  
	 << "pi- Valid  "  
	 << "pi-        " 
	 << "eta        "  
	 << "eta_prime  "
	 << G4endl; 

    
  for(int i = 1; i < 100; i++){
    G4double momentum = 0.10 * (G4double)i * CLHEP::GeV;
    G4double protonEnergy    = sqrt(momentum*momentum + protonMass*protonMass      );
    G4double neutronEnergy   = sqrt(momentum*momentum + neutronMass*neutronMass    );
    G4double pionPlusEnergy  = sqrt(momentum*momentum + pionPlusMass*pionPlusMass  );
    G4double pionMinusEnergy = sqrt(momentum*momentum + pionMinusMass*pionMinusMass);
    //G4double etaEnergy = sqrt(momentum*momentum + etaMass*etaMass);

    G4LorentzVector pProton   (0, 0, momentum, protonEnergy   );
    G4LorentzVector pNeutron  (0, 0, momentum, neutronEnergy  );
    G4LorentzVector pPionPlus (0, 0, momentum, pionPlusEnergy );
    G4LorentzVector pPionMinus(0, 0, momentum, pionMinusEnergy);
    //G4LorentzVector pEta(0, 0, momentum, etaEnergy);

    G4DynamicParticle *aProton    =   new G4DynamicParticle(G4Proton::Proton(),       pProton);
    G4DynamicParticle *aNeutron   =   new G4DynamicParticle(G4Neutron::Neutron(),     pNeutron);
    G4DynamicParticle *aPionPlus  =   new G4DynamicParticle(G4PionPlus::PionPlus(),   pPionPlus);
    G4DynamicParticle *aPionMinus =   new G4DynamicParticle(G4PionMinus::PionMinus(), pPionMinus);
    //G4DynamicParticle *aEta =   new G4DynamicParticle(G4Eta::Eta(), pEta);

    G4cout << momentum/CLHEP::GeV                                        << "\t" 
	   << this->IsIsoApplicable(aProton, ZAl, AAl, NULL,NULL)        << "\t" 
	   << this->GetCrossSection(aProton, Al, 0)/CLHEP::millibarn     << "\t" 
	   << this->IsIsoApplicable(aNeutron, ZAl, AAl, NULL,NULL)       << "\t"  
	   << this->GetCrossSection(aNeutron, Al, 0)/CLHEP::millibarn    << "\t" 
	   << this->IsIsoApplicable(aPionPlus, ZAl, AAl, NULL,NULL)      << "\t"  
	   << this->GetCrossSection(aPionPlus, Al, 0)/CLHEP::millibarn   << "\t" 
	   << this->IsIsoApplicable(aPionMinus, ZAl, AAl, NULL,NULL)     << "\t" 
	   << this->GetCrossSection(aPionMinus, Al, 0)/CLHEP::millibarn  << "\t"
      //<< this->IsIsoApplicable(aEta, ZAl, AAl, NULL,NULL)     << "\t"
      //<< this->GetCrossSection(aEta, Al, 0)/CLHEP::millibarn  << "\t"
	   << G4endl;

    delete aProton;    aProton    = 0;
    delete aNeutron;   aNeutron   = 0;
    delete aPionPlus;  aPionPlus  = 0;
    delete aPionMinus; aPionMinus = 0;
    //delete aEta; aEta = 0;

  }

}


