// ===================================================================== //
//                                                                       //
//       Skim QCD training files for 20k events per pt/eta bin           //
//                                                                       //
// ===================================================================== //   




#include <string>         // std::string

void Filter() {
  string filename[54]={"CombinedSVNoVertexNoSoftLepton_B.root","CombinedSVNoVertexNoSoftLepton_C.root","CombinedSVNoVertexNoSoftLepton_DUSG.root","CombinedSVPseudoVertexNoSoftLepton_B.root","CombinedSVPseudoVertexNoSoftLepton_C.root","CombinedSVPseudoVertexNoSoftLepton_DUSG.root","CombinedSVRecoVertexNoSoftLepton_B.root","CombinedSVRecoVertexNoSoftLepton_C.root","CombinedSVRecoVertexNoSoftLepton_DUSG.root","CombinedSVNoVertexSoftElectron_B.root","CombinedSVNoVertexSoftElectron_C.root","CombinedSVNoVertexSoftElectron_DUSG.root","CombinedSVPseudoVertexSoftElectron_B.root","CombinedSVPseudoVertexSoftElectron_C.root","CombinedSVPseudoVertexSoftElectron_DUSG.root","CombinedSVRecoVertexSoftElectron_B.root","CombinedSVRecoVertexSoftElectron_C.root","CombinedSVRecoVertexSoftElectron_DUSG.root","CombinedSVNoVertexSoftMuon_B.root","CombinedSVNoVertexSoftMuon_C.root","CombinedSVNoVertexSoftMuon_DUSG.root","CombinedSVPseudoVertexSoftMuon_B.root","CombinedSVPseudoVertexSoftMuon_C.root","CombinedSVPseudoVertexSoftMuon_DUSG.root","CombinedSVRecoVertexSoftMuon_B.root","CombinedSVRecoVertexSoftMuon_C.root","CombinedSVRecoVertexSoftMuon_DUSG.root"};
  TString treename[54]={"CombinedSVNoVertexNoSoftLepton","CombinedSVNoVertexNoSoftLepton","CombinedSVNoVertexNoSoftLepton","CombinedSVPseudoVertexNoSoftLepton","CombinedSVPseudoVertexNoSoftLepton","CombinedSVPseudoVertexNoSoftLepton","CombinedSVRecoVertexNoSoftLepton","CombinedSVRecoVertexNoSoftLepton","CombinedSVRecoVertexNoSoftLepton","CombinedSVNoVertexSoftElectron","CombinedSVNoVertexSoftElectron","CombinedSVNoVertexSoftElectron","CombinedSVPseudoVertexSoftElectron","CombinedSVPseudoVertexSoftElectron","CombinedSVPseudoVertexSoftElectron","CombinedSVRecoVertexSoftElectron","CombinedSVRecoVertexSoftElectron","CombinedSVRecoVertexSoftElectron","CombinedSVNoVertexSoftMuon","CombinedSVNoVertexSoftMuon","CombinedSVNoVertexSoftMuon","CombinedSVPseudoVertexSoftMuon","CombinedSVPseudoVertexSoftMuon","CombinedSVPseudoVertexSoftMuon","CombinedSVRecoVertexSoftMuon","CombinedSVRecoVertexSoftMuon","CombinedSVRecoVertexSoftMuon"};

  Int_t max_nb_ofjets	=20000;
  Float_t ptbins[] = {15,40,60,90,150,400,600,10000};
  Float_t etabins[] = {0,1.2,2.1,2.4};
  
  for(int k = 0; k<54; k++){
    
    //Get old file, old tree and set top branch address
    TString name = filename[k].c_str();
    TFile *oldfile = new TFile(name);
    TTree *oldtree = (TTree*)oldfile->Get(treename[k]); //CombinedSVNoVertex, CombinedSVRecoVertex, CombinedSVPseudoVertex
    Int_t nentries = (Int_t)oldtree->GetEntries();
    
    cout << "There are " << nentries << " jets in the file " << filename[k] << " will select " << max_nb_ofjets << " in each pt/eta bin" << endl;
   
    Int_t flavour;
    Double_t jetpt, jeteta, trackSumJetEtRatio, trackSumJetDeltaR;
    std::vector <double>  *trackSip3dSig, *trackSip2dSig, *trackPtRel, *trackDeltaR, *trackPtRatio, *trackJetDist, *trackDecayLenVal, *trackSip2dSigAboveCharm, *trackSip3dSigAboveCharm, *leptonPtRel, *leptonSip3d, *leptonDeltaR, *leptonRatioRel;
    oldtree->SetBranchAddress("flavour",&flavour);
    oldtree->SetBranchAddress("jetPt",&jetpt);
    oldtree->SetBranchAddress("jetEta",&jeteta);
    oldtree->SetBranchAddress("trackPtRel",&trackPtRel);
    //Create a new file + a clone of old tree in new file
    TFile *newfile = new TFile("skimmed_20k_eachptetabin_"+name,"recreate");
    TTree *newtree = oldtree->CloneTree(0);
    
    Int_t nbOfjets[19],nbOfjetsKept[19];
    Int_t nbOfjets_DUS[19],nbOfjetsKept_DUS[19];
    Int_t nbOfjets_G[19],nbOfjetsKept_G[19];
    for(int j = 0 ; j<19 ; j++){
      nbOfjets[j]=0;
      nbOfjetsKept[j]=0;
      nbOfjets_DUS[j]=0;
      nbOfjetsKept_DUS[j]=0;
      nbOfjets_G[j]=0;
      nbOfjetsKept_G[j]=0;
    }
		
    
    if(filename[k].find("DUSG") != std::string::npos )// DUSG file found!!!
      {
	for (Int_t i=0;i<nentries; i++) {
	  oldtree->GetEntry(i);

	  bool Skipevent = false;
	  if(trackPtRel != 0){
	    for(Int_t iVect = 0; iVect < trackPtRel->size(); iVect ++){
	      if(trackPtRel->at(iVect) != trackPtRel->at(iVect) )
		{
		  cout << "trackPtRel[" << iVect << "]: " << trackPtRel->at(iVect) << endl; 
		  Skipevent = true;
		}
	    }		
	  }
	  if(Skipevent == true) continue;
	  // Light jets, not gluons
	  if(flavour < 21 ){ 
	    if((jetpt>=ptbins[0] && jetpt<ptbins[1]) && (fabs(jeteta)>=etabins[0] && fabs(jeteta)<etabins[1])){
	      nbOfjets_DUS[0]++;
	      if(nbOfjets_DUS[0]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_DUS[0]++;}
	    } else if((jetpt>=ptbins[0] && jetpt<ptbins[1]) && (fabs(jeteta)>=etabins[1] && fabs(jeteta)<etabins[2])){
	      nbOfjets_DUS[1]++;
	      if(nbOfjets_DUS[1]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_DUS[1]++;}
	    } else if((jetpt>=ptbins[0] && jetpt<ptbins[1]) && (fabs(jeteta)>=etabins[2] && fabs(jeteta)<etabins[3])){
	      nbOfjets_DUS[2]++;
	      if(nbOfjets_DUS[2]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_DUS[2]++;}
	    } else if((jetpt>=ptbins[1] && jetpt<ptbins[2]) && (fabs(jeteta)>=etabins[0] && fabs(jeteta)<etabins[1])){
	      nbOfjets_DUS[3]++;
	      if(nbOfjets_DUS[3]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_DUS[3]++;}
	    } else if((jetpt>=ptbins[1] && jetpt<ptbins[2]) && (fabs(jeteta)>=etabins[1] && fabs(jeteta)<etabins[2])){
	      nbOfjets_DUS[4]++;
	      if(nbOfjets_DUS[4]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_DUS[4]++;}
	    } else if((jetpt>=ptbins[1] && jetpt<ptbins[2]) && (fabs(jeteta)>=etabins[2] && fabs(jeteta)<etabins[3])){
	      nbOfjets_DUS[5]++;
	      if(nbOfjets_DUS[5]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_DUS[5]++;}
	    } else if((jetpt>=ptbins[2] && jetpt<ptbins[3]) && (fabs(jeteta)>=etabins[0] && fabs(jeteta)<etabins[1])){
	      nbOfjets_DUS[6]++;
	      if(nbOfjets_DUS[6]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_DUS[6]++;}
	    } else if((jetpt>=ptbins[2] && jetpt<ptbins[3]) && (fabs(jeteta)>=etabins[1] && fabs(jeteta)<etabins[2])){
	      nbOfjets_DUS[7]++;
	      if(nbOfjets_DUS[7]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_DUS[7]++;}
	    } else if((jetpt>=ptbins[2] && jetpt<ptbins[3]) && (fabs(jeteta)>=etabins[2] && fabs(jeteta)<etabins[3])){
	      nbOfjets_DUS[8]++;
	      if(nbOfjets_DUS[8]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_DUS[8]++;}
	    } else if((jetpt>=ptbins[3] && jetpt<ptbins[4]) && (fabs(jeteta)>=etabins[0] && fabs(jeteta)<etabins[1])){
	      nbOfjets_DUS[9]++;
	      if(nbOfjets_DUS[9]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_DUS[9]++;}
	    } else if((jetpt>=ptbins[3] && jetpt<ptbins[4]) && (fabs(jeteta)>=etabins[1] && fabs(jeteta)<etabins[2])){
	      nbOfjets_DUS[10]++;
	      if(nbOfjets_DUS[10]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_DUS[10]++;}
	    } else if((jetpt>=ptbins[3] && jetpt<ptbins[4]) && (fabs(jeteta)>=etabins[2] && fabs(jeteta)<etabins[3])){
	      nbOfjets_DUS[11]++;
	      if(nbOfjets_DUS[11]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_DUS[11]++;}
	    } else if((jetpt>=ptbins[4] && jetpt<ptbins[5]) && (fabs(jeteta)>=etabins[0] && fabs(jeteta)<etabins[1])){
	      nbOfjets_DUS[12]++;
	      if(nbOfjets_DUS[12]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_DUS[12]++;}
	    } else if((jetpt>=ptbins[4] && jetpt<ptbins[5]) && (fabs(jeteta)>=etabins[1] && fabs(jeteta)<etabins[2])){
	      nbOfjets_DUS[13]++;
	      if(nbOfjets_DUS[13]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_DUS[13]++;}
	    } else if((jetpt>=ptbins[4] && jetpt<ptbins[5]) && (fabs(jeteta)>=etabins[2] && fabs(jeteta)<etabins[3])){
	      nbOfjets_DUS[14]++;
	      if(nbOfjets_DUS[14]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_DUS[14]++;}
	    } else if((jetpt>=ptbins[5] && jetpt<ptbins[6]) && (fabs(jeteta)>=etabins[0] && fabs(jeteta)<etabins[1])){
	      nbOfjets_DUS[15]++;
	      if(nbOfjets_DUS[15]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_DUS[15]++;}
	    } else if((jetpt>=ptbins[5] && jetpt<ptbins[6]) && (fabs(jeteta)>=etabins[1] && fabs(jeteta)<etabins[3])){
	      nbOfjets_DUS[16]++;
	      if(nbOfjets_DUS[16]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_DUS[16]++;}
	    } else if((jetpt>=ptbins[6] && jetpt<ptbins[7]) && (fabs(jeteta)>=etabins[0] && fabs(jeteta)<etabins[1])){
	      nbOfjets_DUS[17]++;
	      if(nbOfjets_DUS[17]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_DUS[17]++;}
	    } else if((jetpt>=ptbins[6] && jetpt<ptbins[7]) && (fabs(jeteta)>=etabins[1] && fabs(jeteta)<etabins[3])){
	      nbOfjets_DUS[18]++;
	      if(nbOfjets_DUS[18]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_DUS[18]++;}
	    }				
	  }else{
	    // Gluons
	    if((jetpt>=ptbins[0] && jetpt<ptbins[1]) && (fabs(jeteta)>=etabins[0] && fabs(jeteta)<etabins[1])){
	      nbOfjets_G[0]++;
	      if(nbOfjets_G[0]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_G[0]++;}
	    } else if((jetpt>=ptbins[0] && jetpt<ptbins[1]) && (fabs(jeteta)>=etabins[1] && fabs(jeteta)<etabins[2])){
	      nbOfjets_G[1]++;
	      if(nbOfjets_G[1]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_G[1]++;}
	    } else if((jetpt>=ptbins[0] && jetpt<ptbins[1]) && (fabs(jeteta)>=etabins[2] && fabs(jeteta)<etabins[3])){
	      nbOfjets_G[2]++;
	      if(nbOfjets_G[2]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_G[2]++;}
	    } else if((jetpt>=ptbins[1] && jetpt<ptbins[2]) && (fabs(jeteta)>=etabins[0] && fabs(jeteta)<etabins[1])){
	      nbOfjets_G[3]++;
	      if(nbOfjets_G[3]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_G[3]++;}
	    } else if((jetpt>=ptbins[1] && jetpt<ptbins[2]) && (fabs(jeteta)>=etabins[1] && fabs(jeteta)<etabins[2])){
	      nbOfjets_G[4]++;
	      if(nbOfjets_G[4]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_G[4]++;}
	    } else if((jetpt>=ptbins[1] && jetpt<ptbins[2]) && (fabs(jeteta)>=etabins[2] && fabs(jeteta)<etabins[3])){
	      nbOfjets_G[5]++;
	      if(nbOfjets_G[5]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_G[5]++;}
	    } else if((jetpt>=ptbins[2] && jetpt<ptbins[3]) && (fabs(jeteta)>=etabins[0] && fabs(jeteta)<etabins[1])){
	      nbOfjets_G[6]++;
	      if(nbOfjets_G[6]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_G[6]++;}
	    } else if((jetpt>=ptbins[2] && jetpt<ptbins[3]) && (fabs(jeteta)>=etabins[1] && fabs(jeteta)<etabins[2])){
	      nbOfjets_G[7]++;
	      if(nbOfjets_G[7]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_G[7]++;}
	    } else if((jetpt>=ptbins[2] && jetpt<ptbins[3]) && (fabs(jeteta)>=etabins[2] && fabs(jeteta)<etabins[3])){
	      nbOfjets_G[8]++;
	      if(nbOfjets_G[8]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_G[8]++;}
	    } else if((jetpt>=ptbins[3] && jetpt<ptbins[4]) && (fabs(jeteta)>=etabins[0] && fabs(jeteta)<etabins[1])){
	      nbOfjets_G[9]++;
	      if(nbOfjets_G[9]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_G[9]++;}
	    } else if((jetpt>=ptbins[3] && jetpt<ptbins[4]) && (fabs(jeteta)>=etabins[1] && fabs(jeteta)<etabins[2])){
	      nbOfjets_G[10]++;
	      if(nbOfjets_G[10]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_G[10]++;}
	    } else if((jetpt>=ptbins[3] && jetpt<ptbins[4]) && (fabs(jeteta)>=etabins[2] && fabs(jeteta)<etabins[3])){
	      nbOfjets_G[11]++;
	      if(nbOfjets_G[11]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_G[11]++;}
	    } else if((jetpt>=ptbins[4] && jetpt<ptbins[5]) && (fabs(jeteta)>=etabins[0] && fabs(jeteta)<etabins[1])){
	      nbOfjets_G[12]++;
	      if(nbOfjets_G[12]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_G[12]++;}
	    } else if((jetpt>=ptbins[4] && jetpt<ptbins[5]) && (fabs(jeteta)>=etabins[1] && fabs(jeteta)<etabins[2])){
	      nbOfjets_G[13]++;
	      if(nbOfjets_G[13]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_G[13]++;}
	    } else if((jetpt>=ptbins[4] && jetpt<ptbins[5]) && (fabs(jeteta)>=etabins[2] && fabs(jeteta)<etabins[3])){
	      nbOfjets_G[14]++;
	      if(nbOfjets_G[14]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_G[14]++;}
	    } else if((jetpt>=ptbins[5] && jetpt<ptbins[6]) && (fabs(jeteta)>=etabins[0] && fabs(jeteta)<etabins[1])){
	      nbOfjets_G[15]++;
	      if(nbOfjets_G[15]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_G[15]++;}
	    } else if((jetpt>=ptbins[5] && jetpt<ptbins[6]) && (fabs(jeteta)>=etabins[1] && fabs(jeteta)<etabins[3])){
	      nbOfjets_G[16]++;
	      if(nbOfjets_G[16]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_G[16]++;}
	    } else if((jetpt>=ptbins[6] && jetpt<ptbins[7]) && (fabs(jeteta)>=etabins[0] && fabs(jeteta)<etabins[1])){
	      nbOfjets_G[17]++;
	      if(nbOfjets_G[17]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_G[17]++;}
	    } else if((jetpt>=ptbins[6] && jetpt<ptbins[7]) && (fabs(jeteta)>=etabins[1] && fabs(jeteta)<etabins[3])){
	      nbOfjets_G[18]++;
	      if(nbOfjets_G[18]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept_G[18]++;}
	    }				
	  }
	}
	
	for(int j = 0 ; j<19 ; j++)
	  {
	    cout << "Tree " << filename[k] << " has " << nbOfjets_DUS[j] << " in pt/eta-bin " << j << " and we keep " << nbOfjetsKept_DUS[j] << endl;
	    cout << "Tree " << filename[k] << " has " << nbOfjets_G[j] << " in pt/eta-bin " << j << " and we keep " << nbOfjetsKept_G[j] << endl;
	  }
	// Store output
	newtree->Print();
	newtree->AutoSave();
      }
    else
      // B and C, already separated in their files by flavour
      {
	for (Int_t i=0;i<nentries; i++) {
	  oldtree->GetEntry(i);
	  bool Skipevent = false;
	  if(trackPtRel != 0){
	    for(Int_t iVect = 0; iVect < trackPtRel->size(); iVect ++){
	      if(trackPtRel->at(iVect) != trackPtRel->at(iVect))
		{
		  cout << "trackPtRel[" << iVect << "]: " << trackPtRel->at(iVect) << endl; 
		  Skipevent = true;
		}
	    }
	  }
	  
	  if(Skipevent == true) continue;

	  if((jetpt>=ptbins[0] && jetpt<ptbins[1]) && (fabs(jeteta)>=etabins[0] && fabs(jeteta)<etabins[1])){
	    nbOfjets[0]++;

	    if(nbOfjets[0]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept[0]++;}
	  } else if((jetpt>=ptbins[0] && jetpt<ptbins[1]) && (fabs(jeteta)>=etabins[1] && fabs(jeteta)<etabins[2])){
	    nbOfjets[1]++;

	    if(nbOfjets[1]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept[1]++;}
	  } else if((jetpt>=ptbins[0] && jetpt<ptbins[1]) && (fabs(jeteta)>=etabins[2] && fabs(jeteta)<etabins[3])){
	    nbOfjets[2]++;

	    if(nbOfjets[2]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept[2]++;}
	  } else if((jetpt>=ptbins[1] && jetpt<ptbins[2]) && (fabs(jeteta)>=etabins[0] && fabs(jeteta)<etabins[1])){
	    nbOfjets[3]++;

	    if(nbOfjets[3]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept[3]++;}
	  } else if((jetpt>=ptbins[1] && jetpt<ptbins[2]) && (fabs(jeteta)>=etabins[1] && fabs(jeteta)<etabins[2])){
	    nbOfjets[4]++;

	    if(nbOfjets[4]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept[4]++;}
	  } else if((jetpt>=ptbins[1] && jetpt<ptbins[2]) && (fabs(jeteta)>=etabins[2] && fabs(jeteta)<etabins[3])){
	    nbOfjets[5]++;

	    if(nbOfjets[5]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept[5]++;}
	  } else if((jetpt>=ptbins[2] && jetpt<ptbins[3]) && (fabs(jeteta)>=etabins[0] && fabs(jeteta)<etabins[1])){
	    nbOfjets[6]++;
	    
	    if(nbOfjets[6]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept[6]++;}
	  } else if((jetpt>=ptbins[2] && jetpt<ptbins[3]) && (fabs(jeteta)>=etabins[1] && fabs(jeteta)<etabins[2])){
	    nbOfjets[7]++;

	    if(nbOfjets[7]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept[7]++;}
	  } else if((jetpt>=ptbins[2] && jetpt<ptbins[3]) && (fabs(jeteta)>=etabins[2] && fabs(jeteta)<etabins[3])){
	    nbOfjets[8]++;

	    if(nbOfjets[8]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept[8]++;}
	  } else if((jetpt>=ptbins[3] && jetpt<ptbins[4]) && (fabs(jeteta)>=etabins[0] && fabs(jeteta)<etabins[1])){
	    nbOfjets[9]++;

	    if(nbOfjets[9]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept[9]++;}
	  } else if((jetpt>=ptbins[3] && jetpt<ptbins[4]) && (fabs(jeteta)>=etabins[1] && fabs(jeteta)<etabins[2])){
	    nbOfjets[10]++;
	    
	    if(nbOfjets[10]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept[10]++;}
	  } else if((jetpt>=ptbins[3] && jetpt<ptbins[4]) && (fabs(jeteta)>=etabins[2] && fabs(jeteta)<etabins[3])){
	    nbOfjets[11]++;

	    if(nbOfjets[11]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept[11]++;}
	  } else if((jetpt>=ptbins[4] && jetpt<ptbins[5]) && (fabs(jeteta)>=etabins[0] && fabs(jeteta)<etabins[1])){
	    nbOfjets[12]++;
	    
	    if(nbOfjets[12]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept[12]++;}
	  } else if((jetpt>=ptbins[4] && jetpt<ptbins[5]) && (fabs(jeteta)>=etabins[1] && fabs(jeteta)<etabins[2])){
	    nbOfjets[13]++;

	    if(nbOfjets[13]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept[13]++;}
	  } else if((jetpt>=ptbins[4] && jetpt<ptbins[5]) && (fabs(jeteta)>=etabins[2] && fabs(jeteta)<etabins[3])){
	    nbOfjets[14]++;

	    if(nbOfjets[14]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept[14]++;}
	  } else if((jetpt>=ptbins[5] && jetpt<ptbins[6]) && (fabs(jeteta)>=etabins[0] && fabs(jeteta)<etabins[1])){
	    nbOfjets[15]++;

	    if(nbOfjets[15]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept[15]++;}
	  } else if((jetpt>=ptbins[5] && jetpt<ptbins[6]) && (fabs(jeteta)>=etabins[1] && fabs(jeteta)<etabins[3])){
	    nbOfjets[16]++;

	    if(nbOfjets[16]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept[16]++;}
	  } else if((jetpt>=ptbins[6] && jetpt<ptbins[7]) && (fabs(jeteta)>=etabins[0] && fabs(jeteta)<etabins[1])){
	    nbOfjets[17]++;

	    if(nbOfjets[17]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept[17]++;}
	  } else if((jetpt>=ptbins[6] && jetpt<ptbins[7]) && (fabs(jeteta)>=etabins[1] && fabs(jeteta)<etabins[3])){
	    nbOfjets[18]++;

	    if(nbOfjets[18]<max_nb_ofjets){ newtree->Fill(); nbOfjetsKept[18]++;}
	  }
	}
	
	for(int j = 0 ; j<19 ; j++)
	  {
	    cout << "Tree " << filename[k] << " has " << nbOfjets[j] << " in pt/eta-bin " << j << " and we keep " << nbOfjetsKept[j] << endl;
	  }
	// Store output
	newtree->Print();
	newtree->AutoSave();
	
      }
    
    // Clean up
    delete oldfile;
    delete newfile;
  }
}
