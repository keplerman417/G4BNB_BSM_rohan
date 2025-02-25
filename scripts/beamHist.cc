#include <iostream>
#include <string>
#include <glob.h>
#include <thread>

#include "TFile.h"
#include "TH1F.h"
#include "TH3F.h"
#include "TChain.h"
#include "TTree.h"
#include "TRandom3.h"
#include "TVector3.h"
#include "TThread.h"

#include "dk2nu/tree/dk2nu.h"
#include "dk2nu/tree/dkmeta.h"
#include "dk2nu/tree/calcLocationWeights.h"

#include <boost/program_options.hpp>

using namespace std;
using namespace boost::program_options;

struct histpackage_t
{
  std::vector<string> filelist;
  int NREDECAY;
  std::vector<double> detpos; 
  double RDet;
  int rndSeed;
  bool countPOT;
  TH3F* hxye;
  TH1F* hFlux[4];
  TH1F* hparent[4][4];
  TH1F* hsec[4][5]; 
  double POT;
};


int calcMesonWgt(const bsim::Decay& decay, const TVector3& xyz,
                       double& enu, double& wgt_xy)
{
  // Neutral Meson Energy and Weight at arbitrary point
  // This function is adapted from calcEnuWgt to work for neutral mesons
  // Arguments:
  //    decay    :: contains current decay information
  //    xyz      :: 3-vector of position to evaluate
  //                in *beam* frame coordinates (cm units)
  //    enu      :: resulting energy
  //    wgt_xy   :: resulting weight
  // Return:
  //    (int)    :: error code

  const double kPi0Mass = 0.1349768;     // Neutral pion mass (GeV/c^2)
  const double kEtaMass = 0.547862;      // Eta meson mass (GeV/c^2)
  const double kEtaPrimeMass = 0.95778;  // Eta prime mass (GeV/c^2)
  const double kRDET = 100.0;            // Set to flux per 100 cm radius
  
  double xpos = xyz.X();
  double ypos = xyz.Y();
  double zpos = xyz.Z();
  enu = 0.0;  // Initialize energy
  wgt_xy = 0.0;  // Initialize weight

  // Determine the parent meson type and mass
  double parent_mass = kPi0Mass;
  if(decay.ptype == 2212){
    switch (decay.ntype)
      {
      case 111:  // pi0
	parent_mass = kPi0Mass;
	break;
      case 221:  // eta
	parent_mass = kEtaMass;
	break;
      case 331:  // eta'
	parent_mass = kEtaPrimeMass;
	break;
      default:
	std::cerr << "bsim::calcMesonWgt unknown meson type " << decay.ptype
		  << std::endl;
	enu = 0.0;
	wgt_xy = 0.0;
	return 1;  // Unknown particle type
      }
  }
  double parentp2 = decay.pdpx * decay.pdpx + decay.pdpy * decay.pdpy + decay.pdpz * decay.pdpz;
  double parent_energy = TMath::Sqrt(parentp2 + parent_mass * parent_mass);
  double parentp = TMath::Sqrt(parentp2);
  double gamma = parent_energy / parent_mass;
  double gamma_sqr = gamma * gamma;
  double beta_mag = TMath::Sqrt((gamma_sqr - 1.0) / gamma_sqr);

  // Calculate the energy in the lab frame
  double enuzr = decay.necm;  // Energy in the center of mass frame
  double rad = TMath::Sqrt((xpos - decay.vx) * (xpos - decay.vx) +
                           (ypos - decay.vy) * (ypos - decay.vy) +
                           (zpos - decay.vz) * (zpos - decay.vz));
  double emrat = 1.0;
  double costh_pardet = -999., theta_pardet = -999.;
  if (parentp > 0.0)
    {
      costh_pardet = (decay.pdpx * (xpos - decay.vx) +
			     decay.pdpy * (ypos - decay.vy) +
			     decay.pdpz * (zpos - decay.vz)) /
	(parentp * rad);
      if ( costh_pardet >  1.0 ) costh_pardet =  1.0;
      if ( costh_pardet < -1.0 ) costh_pardet = -1.0;
      theta_pardet = TMath::ACos(costh_pardet);
      //emrat = 1.0 / (gamma * (1.0 - beta_mag * costh_pardet));
    }
  enu = emrat * enuzr;

  // Calculate the solid angle weight
  double sanddetcomp = TMath::Sqrt((xpos - decay.vx) * (xpos - decay.vx) +
                                   (ypos - decay.vy) * (ypos - decay.vy) +
                                   (zpos - decay.vz) * (zpos - decay.vz));
  double sangdet = (1.0 - TMath::Cos(TMath::ATan(kRDET / sanddetcomp))) / 2.0;

  // Weight for solid angle and Lorentz boost
  wgt_xy = sangdet * (emrat * emrat);

  return 0;  // Success
}


//thread function (has to be void* since root uses this to determine if it is detached or non-detached thread)  
void* FillHist(void* hp);


int main(int ac, char* av[])
{
  vector<double> detpos;
  detpos.resize(3);
  double RDet;
  string searchpath;
  string outputfn;
  int NREDECAY;
  double userPOT=0;
  int nthread=1;
  options_description opt("Options");
  opt.add_options()
    ("help", "Print help message")
    ("input",value<string>(&searchpath),"Path pattern for input files. Put it in quotes or escape *.")
    ("output",value<string>(&outputfn)->default_value("hist.root"),"Output file name.")
    ("pot",value<double>(&userPOT),"POT used for normalization (overides counting using info in meta tree and speeds up process). \nTotal POT should be given (number of files X POT per file).")
    ("nredecays",value<int>(&NREDECAY)->default_value(1.),"Number of redecays.")
    ("detector-radius",value<double>(&RDet)->default_value(100.),"Detector radius (in cm).")
    ("detector-position",value<vector<double> >(&detpos)->multitoken(),"Detector position (in cm).")
    ("thread",value<int>(&nthread)->default_value(1),"Number of threads to run. (max set to 8)");
    
  variables_map vm;
  
  try {
    store(parse_command_line(ac,av,opt, command_line_style::unix_style ^ command_line_style::allow_short),vm);
    notify(vm);
    if (vm.count("help")) {
      cerr<<opt<<endl;
      return 1;
    } 
    if (!vm.count("input")) {
      cerr<<"Need to provide input pattern."<<endl;
      cerr<<opt<<endl;
      return 1;
    }
    if (!vm.count("detector-position")) {
      //assume it is uboone
      detpos[0]=0;
      detpos[1]=0;
      detpos[2]=0;
    }
  } catch (error& e) {
    cerr << e.what()<<endl<<endl;
    cerr << opt <<endl;
    return 1;
  }
  if (nthread>8) nthread=8;

  glob_t glob_result;
  cout<<"Searching "<<searchpath<<endl;
  glob(searchpath.c_str(),GLOB_TILDE,NULL,&glob_result);
  vector<string> filelist;
  for(unsigned int i=0;i<glob_result.gl_pathc;++i){
    filelist.push_back(string(glob_result.gl_pathv[i]));
  }
  globfree(&glob_result);
  cout <<"Found "<<filelist.size()<<" files"<<endl;

  //  cout<<"Total POT: "<<hp->POT<<endl;
  //cout<<"(with redecaying "<<hp->NREDECAY<<" times)"<<endl;
  //if (dkmetaTree) {
  // cout <<"POT calculated using meta data"<<endl;
  //} else {
  // cout <<"POT set using --pot option."<<endl;
  //}
  cout <<"Making histograms for detector at r=("
       <<detpos[0]<<", "
       <<detpos[1]<<", "
       <<detpos[2]<<") cm and smearing over RDet="<<RDet<<" cm"<<endl;
  cout <<"Redecaying "<<NREDECAY<<" times."<<endl;

  cout<<"Starting "<<nthread<<" threads"<<endl;
  TThread::Initialize();
  TThread* t[nthread];
  histpackage_t* hp[nthread];

  //prepare filelists
  std::vector<string> tfl[nthread];
  while (filelist.size()>0) 
    for (int i=0;i<nthread;i++) {
      if (filelist.size()>0) {
	tfl[i].push_back(filelist.back());
	filelist.pop_back();
      }
    }

  for (int i=0;i<nthread;i++) {
    hp[i]=new histpackage_t();
    hp[i]->filelist=tfl[i];
    hp[i]->NREDECAY=NREDECAY;
    hp[i]->detpos=detpos;
    hp[i]->RDet=RDet;
    hp[i]->rndSeed=i;
    if (!vm.count("pot"))
      hp[i]->countPOT=true;
    else 
      hp[i]->countPOT=false;
    t[i]=new TThread(Form("Thread_%i",i),FillHist, (void*) hp[i]);
    t[i]->Run();
  }
  TThread::Ps();
  for (int i=0;i<nthread;i++) {
    t[i]->Join();
  }

  //add histograms from all threads
  for (int i=1;i<nthread;i++) {
    hp[0]->hxye->Add(hp[i]->hxye);
    for (int inu=0;inu<4;inu++) {
      hp[0]->hFlux[inu]->Add(hp[i]->hFlux[inu]);
      for (int ipar=0;ipar<4;ipar++) {
	hp[0]->hparent[inu][ipar]->Add(hp[i]->hparent[inu][ipar]);
      }
      for (int isec=0;isec<5;isec++) {
	hp[0]->hsec[inu][isec]->Add(hp[i]->hsec[inu][isec]);
      } 
    }
    hp[0]->POT+=hp[i]->POT;
  }

  double totPOT=hp[0]->POT;
  if (vm.count("pot")) {
    totPOT=userPOT;
    cout <<"POT set using --pot option to "<<totPOT<<endl; 
  } else {
    cout <<"Total POT summed using meta data= "<<totPOT<<endl;
  }

  for (int inu=0;inu<4;inu++) {
    hp[0]->hFlux[inu]->Scale(1./totPOT);
    for (int ipar=0;ipar<4;ipar++) {
      hp[0]->hparent[inu][ipar]->Scale(1./totPOT);
    }
    for (int isec=0;isec<5;isec++) {
      hp[0]->hsec[inu][isec]->Scale(1./totPOT);
    } 
  }

  //write histograms to file
  TFile fout(outputfn.c_str(),"RECREATE");
  //hp[0]->hxye->Write();
  for (int inu=0;inu<4;inu++) {
    hp[0]->hFlux[inu]->Write();
    for (int ipar=0;ipar<4;ipar++) {
      //hp[0]->hparent[inu][ipar]->Write();
    }
  }  
  for (int inu=0;inu<4;inu++) {
    hp[0]->hFlux[inu]->Write(Form("h70%i",inu+1)); //same as h50x, keeping copy 
    //to be consistent with MB files
    for (int isec=0;isec<5;isec++) {
      hp[0]->hsec[inu][isec]->Write();
    } 
  }
  fout.Close();

  return 0;
}

void* FillHist(void* hpvoid)
{
  histpackage_t* hp=(histpackage_t*) hpvoid;
  TThread::Lock();
  TChain* dk2nuTree=new TChain("dk2nuTree");
  TChain* dkmetaTree=NULL;
  if (hp->countPOT)
    dkmetaTree=new TChain("dkmetaTree");
  for (auto ifile : hp->filelist) {
    dk2nuTree->Add(ifile.c_str());
    if (dkmetaTree)
      dkmetaTree->Add(ifile.c_str());
  }

  bsim::Dk2Nu*  dk2nu  = new bsim::Dk2Nu;
  dk2nuTree->SetBranchAddress("dk2nu",&dk2nu);
  bsim::DkMeta* dkmeta  = new bsim::DkMeta;
  if (dkmetaTree)
    dkmetaTree->SetBranchAddress("dkmeta",&dkmeta);

  TThread::UnLock();

  Long64_t ientry=0;
  hp->POT=0;
  if (dkmetaTree) {
    while (dkmetaTree->GetEntry(ientry++)) {
      hp->POT+=dkmeta->pots;
    }
  }
  hp->POT*=hp->NREDECAY;

  string nutype[]={    "pi0",        "eta",      "etaprime",         "numubar"};
  string nultx[] ={"#pi_{0}", "#eta", "#eta_{prime}", "#bar{#nu}_{#mu}"};
  int pdgcode[]  ={       111,             221,         331,               -14};
  string pltx[]  ={"#mu^{#pm}","#pi^{#pm}","K^{0}_{L}","K^{#pm}"};
  string secltx[]  ={"pBe->#pi^{#pm}->...->#mu^{#pm}",
		     "pBe->#pi^{#pm}->..(not #mu^{#pm})..",
		     "pBe->K^{0}_{L}->...",
		     "pBe->K^{#pm}->...",
		     "pBe->(p or n)->..."};


  TRandom3 rndmno;
  rndmno.SetSeed(hp->rndSeed);  
  std::string suffix="";
  if (hp->rndSeed>0) 
    suffix=Form("_%i",hp->rndSeed);
  TThread::Lock();
  hp->hxye=new TH3F(Form("h_xyE%s",suffix.c_str()),Form("Neutrino vertices at r=(%f,%f,%f)cm;x (cm);y (cm);E (GeV)",hp->detpos[0],hp->detpos[1],hp->detpos[2]),100,-hp->RDet,hp->RDet,100,-hp->RDet,hp->RDet,200,0,10);

  for (int i=0;i<4;i++) {
    hp->hFlux[i]=new TH1F(Form("h50%i%s",i+1,suffix.c_str()),Form("%s (all);Energy %s (GeV);#phi(%s)/50MeV/POT",nultx[i].c_str(),nultx[i].c_str(),nultx[i].c_str()),200,0,10);
    hp->hFlux[i]->Sumw2();
  }
 
  for (int inu=0;inu<4;inu++) {
    for (int ipar=0;ipar<4;ipar++) {
      hp->hparent[inu][ipar]=new TH1F(Form("h5%i%i%s",ipar+1,inu+1,suffix.c_str()),
				      Form("...->%s->%s;Energy %s (GeV);#phi(%s)/50MeV/POT",pltx[ipar].c_str(),nultx[inu].c_str(),nultx[inu].c_str(),nultx[inu].c_str()),
				      200,0,10);
      hp->hparent[inu][ipar]->Sumw2();
    }
    for (int isec=0;isec<5;isec++) {
      hp->hsec[inu][isec]=new TH1F(Form("h7%i%i%s",isec+1,inu+1,suffix.c_str()),
				   Form("%s->%s;Energy %s (GeV);#phi(%s)/50MeV/POT",secltx[isec].c_str(),nultx[inu].c_str(),nultx[inu].c_str(),nultx[inu].c_str()),
				   200,0,10);
      hp->hsec[inu][isec]->Sumw2();
    }
  }
  TThread::UnLock();
  ientry=0;
  cout<<"Thread "<<hp->rndSeed<<" starting to process "<<dk2nuTree->GetNtrees()<<" files."<<endl;
  while (dk2nuTree->GetEntry(ientry++)) {
    //    if (ientry%100000==0) cout<<"Thread "<<hp->rndSeed<<" on entry "<<ientry<<endl;
    for (int ipdg=0;ipdg<4;ipdg++) {
      if (dk2nu->decay.ntype!=pdgcode[ipdg]) continue;
      
      for (int iredecay=0;iredecay<hp->NREDECAY;iredecay++) {
	double enu,wgt_xy;
	double xx=rndmno.Uniform(-hp->RDet,hp->RDet);
	double yy=rndmno.Uniform(-hp->RDet,hp->RDet);
	while (sqrt(xx*xx+yy*yy)>hp->RDet) {
	  xx=rndmno.Uniform(-hp->RDet,hp->RDet);
	  yy=rndmno.Uniform(-hp->RDet,hp->RDet);
	}
	TVector3 xyz(xx+hp->detpos[0],yy+hp->detpos[1],hp->detpos[2]);
	calcMesonWgt(dk2nu->decay,xyz,enu,wgt_xy);
	
	//to compare with FluxForNuance output (MiniBooNE files)
	//normalize through whole detector area in m2
	double totwgh=/*wgt_xy*/dk2nu->decay.nimpwt;//*3.14159*hp->RDet*hp->RDet*3.14159*1e-4;
	/*double px_temp = dk2nu->ancestor.startpx;
	double py_temp = dk2nu->ancestor.startpy;
	double pz_temp = dk2nu->ancestor.startpz;
	double p2_temp = px_temp * px_temp + py_temp * py_temp + pz_temp * pz_temp;
	double kPi0Mass2 = 0.1349768 * 0.1349768;     // Pi0 mass (GeV/c^2)
	double kEtaMass2 = 0.547862 * 0.547862;      // Eta  mass (GeV/c^2)
	double kEtaPrimeMass2 = 0.95778 * 0.95778; // Eta prime mass (GeV/c^2)
	double e_temp = 0;
	if (ipdg == 0){
	  e_temp = TMath::Sqrt(p2_temp+kPi0Mass2);
	}
	else if (ipdg == 1){
	  e_temp = TMath::Sqrt(p2_temp+kEtaMass2);
	}
	else if (ipdg == 2){
	  e_temp = TMath::Sqrt(p2_temp+kEtaPrimeMass2);
	  }*/
	/*
	if(ipdg==0){
	  cout<<"enu: "<<enu<<endl;
	  cout<<"totwgh: "<<totwgh<<endl;
	  cout<<"nimpwt: "<<dk2nu->decay.nimpwt<<endl;
	  cout<<"rdet: "<<hp->RDet<<endl;
	  cout<<"======"<<endl;
	}
	*/
	hp->hxye->Fill(xx,yy,enu,totwgh);
	//if(enu>=0.1349768){
	hp->hFlux[ipdg]->Fill(enu,totwgh);
	  //}
	if (dk2nu->decay.ptype==13 || dk2nu->decay.ptype==-13) //mu+-
	  hp->hparent[ipdg][0]->Fill(enu,totwgh);
	else if (dk2nu->decay.ptype==211 || dk2nu->decay.ptype==-211) //pi+-
	  hp->hparent[ipdg][1]->Fill(enu,totwgh);
	else if (dk2nu->decay.ptype==130) //K0L
	  hp->hparent[ipdg][2]->Fill(enu,totwgh);
	else if (dk2nu->decay.ptype==321 || dk2nu->decay.ptype==-321) //K+-
	  hp->hparent[ipdg][3]->Fill(enu,totwgh);
	
	int firstInelastic=0;
	while (dk2nu->ancestor[firstInelastic].proc.find("HadronInelastic")==string::npos) firstInelastic++;
      
	if (fabs(dk2nu->ancestor[firstInelastic].pdg)==211 && fabs(dk2nu->decay.ptype)==13)
	  hp->hsec[ipdg][0]->Fill(enu,totwgh);
	else if (fabs(dk2nu->ancestor[firstInelastic].pdg)==211)
	  hp->hsec[ipdg][1]->Fill(enu,totwgh);
	else if (fabs(dk2nu->ancestor[firstInelastic].pdg)==130)
	  hp->hsec[ipdg][2]->Fill(enu,totwgh);
	else if (fabs(dk2nu->ancestor[firstInelastic].pdg)==321)
	  hp->hsec[ipdg][3]->Fill(enu,totwgh);
	else if (dk2nu->ancestor[firstInelastic].pdg==2212 || dk2nu->ancestor[firstInelastic].pdg==2112)
	  hp->hsec[ipdg][4]->Fill(enu,totwgh);
      }
    }
  }
 
  cout<<"Thread "<<hp->rndSeed<<" processed "<<ientry<<" entries. POT = "<<hp->POT<<endl;

  return NULL;
}
