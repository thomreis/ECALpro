#define calibAnaEcalEB_cxx

#include "calibAnaEcalEB.h"

#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TProfile.h>
#include <TPaletteAxis.h>

#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib> //as stdlib.h                                                                                                                                   
#include <cstdio>
#include <cmath>
#include <sstream> //to use ostringstream to convert numbers to string in c++                                                                             

using namespace std;

#ifdef calibAnaEcalEB_cxx

calibAnaEcalEB::calibAnaEcalEB(TTree *tree) : calibAnaEcal(tree) {

  ////////////////////////////                                                                                  
  //initializing data members                                                                                                                                         
  ///////////////////////////                                                                                                                                         
  //////////////////////////                                                                                                                                         
  // protected data members                                                                                                                                           
  EBorEE = "EB";
  ////////////////////////////////                                                                                                                            
  // public data members                    

  // ieta and iphi are integer, but we create bins for histograms such that, e.g., the bin with (ieta = 2 ) goes from 1.5 to 2.5
  // note that ieta!=0 and iphi!=0 --> 0 < |ieta| <= 85 and 0 < iphi <= 360 
  // we add 2 more bins to both axis (e.g., we have 362 bins in iphi) in order to create a white margin in the plots 
  NbinsX_2Dmap = 362;
  lowerX_2Dmap = -0.5;
  upperX_2Dmap = 361.5;
  NbinsY_2Dmap = 173;
  lowerY_2Dmap = -86.5;
  upperY_2Dmap = 86.5;

  NbinsX_etaProfile = 170;
  lowerX_etaProfile = -1.48;
  upperX_etaProfile = 1.48;

  Init(tree);

}

//===============================================                                                                                                
void calibAnaEcalEB::setHistograms() {

  calibAnaEcal::setHistograms();

  //cout << "[DEBUG] calibAnaEcalEB::setHistograms() : iter " << this->getIterNumber() << endl; 

  // X in 2D map is iphi for EB
  mean_iphiProfile = new TProfile("mean_iphiProfile",Form("fit_mean profile in %s",EBorEE.c_str()),NbinsX_2Dmap,lowerX_2Dmap,upperX_2Dmap);

  // I was using these values starting for SoverB, SoverSqrtSplusB, SigmaMeanOverMean, mean,                                                                          
  // set only some of them with h->GetBinContent(h->GetMinimumBin()) and h->GetBinContent(h->GetMaximumBin())                                               
  th2dMinZaxisVector.push_back(0.0);  // was set to hSignal->GetBinContent(hSignal->GetMinimumBin())                                                                  
  th2dMinZaxisVector.push_back(0.0);  // hBackground->GetBinContent(hBackground->GetMinimumBin())                                                           
  th2dMinZaxisVector.push_back(0.0);
  th2dMinZaxisVector.push_back(0.0);
  th2dMinZaxisVector.push_back(0.0);//0.0                                                                 
  if (Pi0orEta == "Pi0") {
    if (this->getIterNumber() == "iter_0" && this->getDirName().find("_ext") == string::npos) th2dMinZaxisVector.push_back(0.130);
    else {
      string match = "iter_";
      string iterN_str = this->getIterNumber().substr(this->getIterNumber().find(match) + match.size());
      Int_t iterN_int = atoi(iterN_str.c_str());
      if (iterN_int > 2 || this->getDirName().find("_ext") != string::npos) 
	th2dMinZaxisVector.push_back(0.134);
      else 
	th2dMinZaxisVector.push_back(0.130);
    }
    th2dMinZaxisVector.push_back(0.005);
  } else {
    th2dMinZaxisVector.push_back(0.500);
    th2dMinZaxisVector.push_back(0.008);
  }
  th2dMinZaxisVector.push_back(0.0);//0.0
  th2dMinZaxisVector.push_back(0.02);//0.0

}

//===============================================                                                                                                         

void calibAnaEcalEB::set2DmapMaxZaxisVector() {

  th2dMaxZaxisVector.push_back(hSignal->GetBinContent(hSignal->GetMaximumBin()));
  th2dMaxZaxisVector.push_back(hBackground->GetBinContent(hBackground->GetMaximumBin()));
  th2dMaxZaxisVector.push_back(10e9);
  th2dMaxZaxisVector.push_back(10e9); // when this value is very large (bigger than the default) use the default to plot axis                  
  th2dMaxZaxisVector.push_back(0.005);//0.02                                                  
  if (Pi0orEta == "Pi0") {
    if (this->getIterNumber() == "iter_0" && this->getDirName().find("_ext") == string::npos) th2dMaxZaxisVector.push_back(0.150);
    else {
      string match = "iter_";
      string iterN_str = this->getIterNumber().substr(this->getIterNumber().find(match) + match.size());
      Int_t iterN_int = atoi(iterN_str.c_str());
      if (iterN_int > 2 || this->getDirName().find("_ext") != string::npos) 
	th2dMaxZaxisVector.push_back(0.136);
      else 
	th2dMaxZaxisVector.push_back(0.140);
    }
    th2dMaxZaxisVector.push_back(0.015);
  } else {
    th2dMaxZaxisVector.push_back(0.600);
    th2dMaxZaxisVector.push_back(0.025);
  }
  th2dMaxZaxisVector.push_back(70);
  th2dMaxZaxisVector.push_back(0.2);

}

//===============================================                                                                                                          

void calibAnaEcalEB::draw2Dmap(TH2D* hist2d, const Bool_t saveHistoAsRoot = false) {

  calibAnaEcal::draw2Dmap(hist2d, saveHistoAsRoot);

}

//===============================================                                                                                                           

void calibAnaEcalEB::drawProfile(TProfile *profile, const string& yAxisName, const Bool_t saveHistoAsRoot = false) {
 
  calibAnaEcal::drawProfile(profile, yAxisName, saveHistoAsRoot);
   
}

//===============================================                                                                                            

void calibAnaEcalEB::Init(TTree* tree) {

  calibAnaEcal::Init(tree);

  fChain->SetBranchAddress("rawId_", &rawId, &b_rawId);                                                                                                             
  fChain->SetBranchAddress("ieta_", &ieta, &b_ieta);                                                                                                                
  fChain->SetBranchAddress("iphi_", &iphi, &b_iphi);                                                                                                                
  fChain->SetBranchAddress("iSM_", &iSM, &b_iSM);                                                                                                                   
  fChain->SetBranchAddress("iMod_", &iMod, &b_iMod);                                                                                                                
  fChain->SetBranchAddress("iTT_", &iTT, &b_iTT);                                                                                                                   
  fChain->SetBranchAddress("iTTeta_", &iTTeta, &b_iTTeta);                                                                                                          
  fChain->SetBranchAddress("iTTphi_", &iTTphi, &b_iTTphi);                                                                                                          
  Notify();    

}

//===============================================                                                                                                 

void calibAnaEcalEB::Loop()
{  

  if (fChain == 0) return;

  Double_t resolution_fromFit = 0.0;

  this->setHistograms();

  Long64_t nentries = fChain->GetEntriesFast();

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;                                                                                                                              
      
    if (jentry % 100000 == 0) cout << jentry << endl;

    if (ieta == 0) continue;
    if ((abs(Backgr) > 0.00001) && (abs(Signal) > 0.00001)) { // avoid empty crystals due to masked towers or whatever                                              

      normalizedS = Signal * fit_Snorm;
      normalizedB = Backgr * fit_Bnorm;

      // to avoid that in 2D maps points below lower threshold in z axis are drawn white (as if they are empty), fill with the maximum between threshold and value  
      resolution_fromFit = ((Double_t)fit_mean > 0.0) ? ((Double_t)fit_sigma/(Double_t)fit_mean) : 0.0;
      cout << "Resolution =" << resolution_fromFit<< endl;

      hSignal->Fill((Double_t)iphi,(Double_t)ieta,max(th2dMinZaxisVector[0],(Double_t)normalizedS));
      hBackground->Fill((Double_t)iphi,(Double_t)ieta,max(th2dMinZaxisVector[1],(Double_t)normalizedB));
      SoverB->Fill((Double_t)iphi,(Double_t)ieta,max(th2dMinZaxisVector[2],(Double_t)normalizedS/normalizedB));
      SoverSqrtSplusB->Fill((Double_t)iphi,(Double_t)ieta, max(th2dMinZaxisVector[3],(Double_t)normalizedS/sqrt(normalizedS + normalizedB)));
      SigmaMeanOverMean->Fill((Double_t)iphi,(Double_t)ieta, max(th2dMinZaxisVector[4],(Double_t)fit_mean_err/fit_mean));
      mean->Fill((Double_t)iphi,(Double_t)ieta, max(th2dMinZaxisVector[5],(Double_t)fit_mean));
      sigma->Fill((Double_t)iphi,(Double_t)ieta, max(th2dMinZaxisVector[6],(Double_t)fit_sigma));
      chisquare->Fill((Double_t)iphi,(Double_t)ieta, max(th2dMinZaxisVector[7],(Double_t)Chisqu));
      resolution->Fill((Double_t)iphi,(Double_t)ieta, max(th2dMinZaxisVector[8],resolution_fromFit));

      chisquare_vs_etaring->Fill((Double_t)ieta,(Double_t)Chisqu * (Double_t)Ndof);

      // 0.0174532925 = pi/180; in the barrel eta is simply proportional to ieta
      // also, put the eta value in the middle of the bin
      // 170 bins from -1.48 to 1.48 (so the width is not exactly 0.0174532925, but ok
      Double_t offsetEta = (ieta < 0) ? (0.5*0.0174532925) : (-0.5*0.0174532925);
      Double_t eta = ieta * 0.0174532925 + offsetEta;     

      hSignal_etaProfile->Fill((Double_t)eta,normalizedS);
      hBackground_etaProfile->Fill((Double_t)eta,normalizedB);
      SoverB_etaProfile->Fill((Double_t)eta,normalizedS/normalizedB);
      SoverSqrtSplusB_etaProfile->Fill((Double_t)eta, normalizedS/sqrt(normalizedS + normalizedB));
      SigmaMeanOverMean_etaProfile->Fill((Double_t)eta, fit_mean_err/fit_mean);
      mean_etaProfile->Fill((Double_t)eta, fit_mean);
      sigma_etaProfile->Fill((Double_t)eta, fit_sigma);
      chisquare_etaProfile->Fill((Double_t)eta, Chisqu);
      resolution_etaProfile->Fill((Double_t)eta, resolution_fromFit);

      mean_iphiProfile->Fill((Double_t)iphi, fit_mean);

    }

  }

  // set preference for max value in the vertical scale
  set2DmapMaxZaxisVector();  
  // now set the vertical axis maximum value based on user input (will use the least between the default and the user input). 
  setVerticalRangeInHisto();

  for ( UInt_t i = 0; i < th2dVector.size(); i++ ) {

    Bool_t saveHistoAsRoot = false;
    std::string hname = th2dVector[i]->GetName(); 
    if (hname == "resolution") saveHistoAsRoot = true;
    draw2Dmap(th2dVector[i], saveHistoAsRoot);
    drawProfile(profileEtaVector[i], profileYaxisTitle[i], saveHistoAsRoot);

  }

  TCanvas *c_mean_iphi_prof = new TCanvas("c_mean_iphi_prof",("c_" + string(mean_iphiProfile->GetName())).c_str());
  c_mean_iphi_prof->SetTickx(1);
  c_mean_iphi_prof->SetTicky(1);
  c_mean_iphi_prof->SetGrid();
  mean_iphiProfile->Draw("HE");
  mean_iphiProfile->GetXaxis()->SetTitle("i #phi");
  mean_iphiProfile->GetXaxis()->SetTitleSize(0.06);
  mean_iphiProfile->GetXaxis()->SetTitleOffset(0.7);
  mean_iphiProfile->GetYaxis()->SetTitle("mean [GeV]");
  if (Pi0orEta == "Pi0") {
    if (this->getIterNumber() == "iter_0" && this->getDirName().find("_ext") == string::npos) mean_iphiProfile->GetYaxis()->SetRangeUser(0.13,0.14);
    else {
      string match = "iter_";
      string iterN_str = this->getIterNumber().substr(this->getIterNumber().find(match) + match.size());
      Int_t iterN_int = atoi(iterN_str.c_str());
      if (iterN_int > 2 || this->getDirName().find("_ext") != string::npos) 
	mean_iphiProfile->GetYaxis()->SetRangeUser(0.134,0.136);
      else 
	mean_iphiProfile->GetYaxis()->SetRangeUser(0.13,0.14);
    }
  } else mean_iphiProfile->GetYaxis()->SetRangeUser(0.5,0.6);
  mean_iphiProfile->GetYaxis()->SetTitleSize(0.055);
  mean_iphiProfile->GetYaxis()->SetTitleOffset(0.8);
  mean_iphiProfile->SetStats(0);
  mean_iphiProfile->Draw("HE");
  c_mean_iphi_prof->SaveAs( (wwwPath + mean_iphiProfile->GetName() + "_EB.pdf").c_str() );
  c_mean_iphi_prof->SaveAs( (wwwPath + mean_iphiProfile->GetName() + "_EB.png").c_str() );

  drawChisquare(chisquare_vs_etaring, true);

}



#endif
