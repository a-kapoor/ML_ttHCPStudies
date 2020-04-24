/***
## Declare Factory
Create the Factory class. Later you can choose the methods
whose performance you'd like to investigate. 
The factory is the major TMVA object you have to interact with. Here is the list of parameters you need to pass
 - The first argument is the base of the name of all the output
weightfiles in the directory weight/ that will be created with the 
method parameters 
 - The second argument is the output file for the training results
  
 - The third argument is a string option defining some general configuration for the TMVA session. For example all TMVA output can be suppressed by removing the "!" (not) in front of the "Silent" argument in the option string
**/
#include "Aliases.C"

// import tensorflow as tf
// from keras.models import Sequential
// from keras.layers import Dense, Activation, Dropout
// from keras.regularizers import l2eras.optimizers import SGD

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include "TMVA/Factory.h"
#include "TMVA/Reader.h"
#include "TMVA/DataLoader.h"
#include "TMVA/PyMethodBase.h"


// TString pythonSrc = "\
// from keras.models import Sequential\n\
// from keras.layers.core import Dense, Activation, Dropout\n\
// from keras.optimizers import SGD, Adam\n\
// \n\
// model = Sequential()\n\
// model.add(Dense(32, activation=\"relu\", input_dim=23))\n\
// model.add(Dense(32, activation=\"relu\"))\n\
// model.add(Dense(32, activation=\"relu\"))\n\
// model.add(Dropout(0.5))\n\
// model.add(Dense(16, activation=\"relu\"))\n\
// model.add(Dropout(0.5))\n\
// model.add(Dense(2, activation=\"softmax\"))\n\
// model.compile(loss=\"categorical_crossentropy\", optimizer=SGD(lr=0.01), metrics=[\"accuracy\",])\n\
// model.save(\"kerasModelClassification2.h5\")\n";


TString pythonSrc = "\
from keras.models import Sequential\n\
from keras.layers.core import Dense, Activation, Dropout\n\
from keras.optimizers import SGD, Adam\n\
\n\
model = Sequential()\n\
model.add(Dense(32, activation=\"relu\", input_dim=23))\n\
model.add(Dense(128, activation=\"relu\"))\n\
model.add(Dropout(0.01))\n\
model.add(Dense(64, activation=\"relu\"))\n\
model.add(Dropout(0.01))\n\
model.add(Dense(32, activation=\"relu\"))\n\
model.add(Dropout(0.01))\n\
model.add(Dense(16, activation=\"relu\"))\n\
model.add(Dropout(0.01))\n\
model.add(Dense(8, activation=\"relu\"))\n\
model.add(Dropout(0.1))\n\
model.add(Dense(2, activation=\"softmax\"))\n\
model.compile(loss=\"categorical_crossentropy\", optimizer=Adam(lr=0.00005), metrics=[\"accuracy\",])\n\
model.save(\"kerasModelClassification2.h5\")\n";




void ttH_Classification(const char* fileNameChar="ttH_Classification", int year=1, int CPmix=59,int repeatS=1, int repeatB=1, int nSTrain=0, int nBTrain=0, int crosstesting=0) { 

  TMVA::Tools::Instance();

  std::string fileName=fileNameChar;
  std::string dataName="dataset";


  auto outputFile = TFile::Open(fileName+"_"+CPmix+"_"+year+"_repeatS_"+repeatS+"_repeatB_"+repeatB+"_TrS_"+nSTrain+"_TrB_"+nBTrain+"_crosstesting_"+crosstesting+".root", "RECREATE");

  TMVA::Factory factory(fileName+"_"+CPmix+"_"+year+"_repeatS_"+repeatS+"_repeatB_"+repeatB+"_TrS_"+nSTrain+"_TrB_"+nBTrain+"_crosstesting_"+\
crosstesting, outputFile,
			"!V:ROC:!Silent:Color:!DrawProgressBar:Transformations=N:AnalysisType=Classification" ); 

  /**
## Setup Dataset(s)
Define now input data file and signal and background trees
  **/

  TString SMfile2016 = "newSamples/DiLepRegion/2016/DiLepRegion/TTH_DiLepRegion.root";
  TString CPfile2016 = "newSamples/DiLepRegion/2016/DiLepRegion/ttH_DiLepRegion.root";

  TString SMfile2017 = "newSamples/DiLepRegion/2017/DiLepRegion/TTH_DiLepRegion.root";
  TString CPfile2017 = "newSamples/DiLepRegion/2017/DiLepRegion/ttH_DiLepRegion.root";

  TString SMfile2018 = "newSamples/DiLepRegion/2018/DiLepRegion/TTH_DiLepRegion.root";
  TString CPfile2018 = "newSamples/DiLepRegion/2018/DiLepRegion/ttH_DiLepRegion.root";
  

  //TString inputFileName = "tmva_class_example.root";

  auto CPFile2016 = TFile::Open(CPfile2016);
  auto SMFile2016 = TFile::Open(SMfile2016);

  auto CPFile2017 = TFile::Open(CPfile2017);
  auto SMFile2017 = TFile::Open(SMfile2017);

  auto CPFile2018 = TFile::Open(CPfile2018);
  auto SMFile2018 = TFile::Open(SMfile2018);

  // --- Register the training and test trees

  
  TTree *signalTree2016     = (TTree*)CPFile2016->Get("syncTree");
  TTree *backgroundTree2016 = (TTree*)SMFile2016->Get("syncTree");

  TTree *signalTree2017     = (TTree*)CPFile2017->Get("syncTree");
  TTree *backgroundTree2017 = (TTree*)SMFile2017->Get("syncTree");

  TTree *signalTree2018     = (TTree*)CPFile2018->Get("syncTree");
  TTree *backgroundTree2018 = (TTree*)SMFile2018->Get("syncTree");

  TString CPfile = "TTH_CP_DiLepRegion.root";
  TString SMfile = "ttH_DiLepRegion.root";


  //TString inputFileName = "tmva_class_example.root";                                                                                                

  auto CPFile = TFile::Open(CPfile);
  auto SMFile = TFile::Open(SMfile);

  // --- Register the training and test trees                                                                                                         
  TTree *signalTree     = (TTree*)CPFile->Get("syncTree");
  TTree *backgroundTree = (TTree*)SMFile->Get("syncTree");


  // signalTree2016->SetAlias("CPmixI","CPmix");

  // signalTree2017->SetAlias("CPmixI","CPmix");

  // signalTree2018->SetAlias("CPmixI","CPmix");
  
  //backgroundTree2016->SetAlias("Wta","EVENT_rWeights[60]/EVENT_originalXWGTUP");
  // backgroundTree2017->SetAlias("Wta","Aliases::Wt(EVENT_rWeights[60],EVENT_originalXWGTUP)");
  // backgroundTree2018->SetAlias("Wta","Aliases::Wt(EVENT_rWeights[60],EVENT_originalXWGTUP)");


  /***
## Declare DataLoader(s)
The next step is to declare the DataLoader class that deals with input variables 
Define the input variables that shall be used for the MVA training
note that you may also use variable expressions, which can be parsed by TTree::Draw( "expression" )]
  ***/
  TMVA::DataLoader * loader = new TMVA::DataLoader(dataName+CPmix+"_"+year+"_repeatS_"+repeatS+"_repeatB_"+repeatB+"_TrS_"+nSTrain+"_TrB_"+nBTrain+"_crosstesting_"+crosstesting);

  //loader->AddVariable("Bin2l");


// loader->AddVariable("Dilep_bestMVA");
// loader->AddVariable("Dilep_htllv");
// loader->AddVariable("Dilep_mtWmin");
// loader->AddVariable("Dilep_nTight");
// loader->AddVariable("Dilep_pdgId");
// loader->AddVariable("Dilep_worseIso");
// loader->AddVariable("Dilep_worseMVA");
// loader->AddVariable("Dilep_worseSip");
// loader->AddVariable("Dilep_worsedz");

// loader->AddVariable("Hj_tagger_hadTop");
// loader->AddVariable("Hj_tagger_resTop");

 // loader->AddVariable("truth_H_eta");                                                                                                                
 // loader->AddVariable("truth_H_pt");   

//  loader->AddVariable("PFMET");
//  loader->AddVariable("PFMETphi");
  loader->AddVariable("avg_dr_jet");
  loader->AddVariable("dr_leps");
  loader->AddVariable("jet1_pt");
  loader->AddVariable("jet1_eta");
  loader->AddVariable("jet1_phi");
  //loader->AddVariable("jet1_CSV");
  //loader->AddVariable("jet1_DeepCSV");
  loader->AddVariable("jet2_pt");
  loader->AddVariable("jet2_eta");
  loader->AddVariable("jet2_phi");
  //loader->AddVariable("jet2_CSV");
  //loader->AddVariable("jet2_DeepCSV");
  loader->AddVariable("jet3_pt");
  loader->AddVariable("jet3_eta");
  loader->AddVariable("jet3_phi");
  //loader->AddVariable("jet3_CSV");
  //loader->AddVariable("jet3_DeepCSV");

   loader->AddVariable("lep1_pt");
  // loader->AddVariable("lep1_eta");
  // loader->AddVariable("lep1_phi");
   loader->AddVariable("lep2_pt");
  // loader->AddVariable("lep2_eta");
  // loader->AddVariable("lep2_phi");

  // loader->AddVariable("jet1_eta-jet2_eta");
  // loader->AddVariable("jet1_eta-jet3_eta");
  // loader->AddVariable("jet2_eta-jet3_eta");


  //  loader->AddVariable("lep1_eta-lep2_eta");


 //loader->AddVariable("massL");
//loader->AddVariable("massll");
//loader->AddVariable("metLD");
//loader->AddVariable("minMllAFAS");
//loader->AddVariable("mvaOutput_2lss_ttV");
//loader->AddVariable("mvaOutput_2lss_ttbar");


//loader->AddVariable("acuteangle_bbpp_highest2b");
loader->AddVariable("deta_highest2b");
//loader->AddVariable("cosa_highest2b");
loader->AddVariable( "angle_bbpp_highest2b", 'F' );
loader->AddVariable( "mindr_lep1_jet", 'F' );
loader->AddVariable( "mindr_lep2_jet", 'F' ); 
 
 
TCut CutSTrain_Sig = "Entry$%2==0&&(angle_bbpp_highest2b>0&&(n_presel_jet>=3 && is_tH_like_and_not_ttH_like !=1 && HiggsDecay!=1))";
TCut CutSTest_Sig = "Entry$%2!=0&&(angle_bbpp_highest2b>0&&(n_presel_jet>=3 && is_tH_like_and_not_ttH_like !=1 && HiggsDecay!=1))";


TCut CutSTrain_Bkg = "Entry$%2==0&&(angle_bbpp_highest2b>0&&(n_presel_jet>=3 && is_tH_like_and_not_ttH_like !=1 && HiggsDecay!=1))";
TCut CutSTest_Bkg = "Entry$%2!=0&&(angle_bbpp_highest2b>0&&(n_presel_jet>=3 && is_tH_like_and_not_ttH_like !=1 && HiggsDecay!=1))";


TCut CutBTrain_Bkg = "Entry$%2==0&&(angle_bbpp_highest2b>0&&(n_presel_jet>=3 && is_tH_like_and_not_ttH_like !=1 && HiggsDecay!=1))";
TCut CutBTest_Bkg = "Entry$%2!=0&&(angle_bbpp_highest2b>0&&(n_presel_jet>=3 && is_tH_like_and_not_ttH_like !=1 && HiggsDecay!=1))";


   
  // You can add an arbitrary number of signal or background trees
 if(crosstesting==0){  
 for(int i=0;i<repeatS;i++){
   
   if(year==2016){
     loader->AddSignalTree    ( signalTree2016,      1.0);}
   


   if(year==0){
     loader->AddSignalTree    ( signalTree,      1.0);}
   
     // loader->AddTree( signalTree2016,     "Signal", 1.0, CutSTrain_Sig,"train");
     // loader->AddTree( signalTree2016,     "Signal", 1.0, CutSTest_Sig,"test");}
 
   if(year==2017)loader->AddSignalTree    ( signalTree2017,      1.0);
   if(year==2018)loader->AddSignalTree    ( signalTree2018,      1.0);}


 // for(int i=0;i<repeatS;i++){
 //   if(year==2016)loader->AddSignalTree    ( signalTree2016,      1.0, "train");
 //   if(year==2017)loader->AddSignalTree    ( signalTree2017,      1.0, "train");
 //   if(year==2018)loader->AddSignalTree    ( signalTree2018,      1.0, "train");
 // }
 //   if(year==2016)loader->AddSignalTree    ( signalTree2017,      1.0, "test");
 //   if(year==2017)loader->AddSignalTree    ( signalTree2017,      1.0, "test"    );
 //   if(year==2018)loader->AddSignalTree    ( signalTree2017,      1.0, "test");
  
   if(year==1){
     loader->AddSignalTree    ( signalTree2016,     1.0     );
     loader->AddSignalTree    ( signalTree2017,     1.0     );
     loader->AddSignalTree    ( signalTree2018,     1.0     );
   }
  
 for(int i=0;i<repeatB;i++){
   if(year==2016){
     loader->AddBackgroundTree    ( signalTree2016,     1.0);}
   //loader->AddBackgroundTree    ( backgroundTree2016,     1.0     );}
     // loader->AddTree    ( signalTree2016,     "Background", 1.0, CutSTrain_Bkg,"train");
     // loader->AddTree    ( backgroundTree2016,     "Background", 1.0, CutBTrain_Bkg,"train");
     // loader->AddTree    ( signalTree2016,     "Background", 1.0, CutSTest_Bkg,"test");
     // loader->AddTree    ( backgroundTree2016,     "Background", 1.0, CutBTest_Bkg,"test");}
   if(year==2017){
     loader->AddBackgroundTree    ( signalTree2017,     1.0     );}
     //loader->AddBackgroundTree    ( backgroundTree2017,     1.0     );}
   if(year==2018){
     loader->AddBackgroundTree    ( signalTree2018,     1.0     );}
     //loader->AddBackgroundTree    ( backgroundTree2018,     1.0     );}


   if(year==0){
     loader->AddBackgroundTree    ( backgroundTree,      1.0);}
   
   if(year==1){
     loader->AddBackgroundTree    ( signalTree2016,     1.0     );
     loader->AddBackgroundTree    ( signalTree2017,     1.0     );
     loader->AddBackgroundTree    ( signalTree2018,     1.0     );}
 }
 }

 if(crosstesting==1){  
   
 for(int i=0;i<repeatS;i++){
     loader->AddSignalTree( signalTree2016,     1.0, "train");
     loader->AddBackgroundTree( signalTree2016,     1.0, "train");}

     loader->AddSignalTree( signalTree2017,     1.0, "test");
     loader->AddBackgroundTree( signalTree2017,     1.0, "test");}

  // Set individual event weights (the variables must exist in the original TTree)
  //    for signal    : 
 if(year!=0){
 if(CPmix==0)loader->SetSignalWeightExpression("(EVENT_rWeights[0])/EVENT_originalXWGTUP");
 if(CPmix==1)loader->SetSignalWeightExpression("(EVENT_rWeights[1])/EVENT_originalXWGTUP");
 if(CPmix==2)loader->SetSignalWeightExpression("(EVENT_rWeights[2])/EVENT_originalXWGTUP");
 if(CPmix==3)loader->SetSignalWeightExpression("(EVENT_rWeights[3])/EVENT_originalXWGTUP");
 if(CPmix==4)loader->SetSignalWeightExpression("(EVENT_rWeights[4])/EVENT_originalXWGTUP");
 if(CPmix==5)loader->SetSignalWeightExpression("(EVENT_rWeights[5])/EVENT_originalXWGTUP");
 if(CPmix==6)loader->SetSignalWeightExpression("(EVENT_rWeights[6])/EVENT_originalXWGTUP");
 if(CPmix==7)loader->SetSignalWeightExpression("(EVENT_rWeights[7])/EVENT_originalXWGTUP");
 if(CPmix==8)loader->SetSignalWeightExpression("(EVENT_rWeights[8])/EVENT_originalXWGTUP");
 if(CPmix==9)loader->SetSignalWeightExpression("(EVENT_rWeights[9])/EVENT_originalXWGTUP");
 if(CPmix==10)loader->SetSignalWeightExpression("(EVENT_rWeights[10])/EVENT_originalXWGTUP");
 if(CPmix==11)loader->SetSignalWeightExpression("(EVENT_rWeights[11])/EVENT_originalXWGTUP");
 if(CPmix==12)loader->SetSignalWeightExpression("(EVENT_rWeights[12])/EVENT_originalXWGTUP");
 if(CPmix==13)loader->SetSignalWeightExpression("(EVENT_rWeights[13])/EVENT_originalXWGTUP");
 if(CPmix==14)loader->SetSignalWeightExpression("(EVENT_rWeights[14])/EVENT_originalXWGTUP");
 if(CPmix==15)loader->SetSignalWeightExpression("(EVENT_rWeights[15])/EVENT_originalXWGTUP");
 if(CPmix==16)loader->SetSignalWeightExpression("(EVENT_rWeights[16])/EVENT_originalXWGTUP");
 if(CPmix==17)loader->SetSignalWeightExpression("(EVENT_rWeights[17])/EVENT_originalXWGTUP");
 if(CPmix==18)loader->SetSignalWeightExpression("(EVENT_rWeights[18])/EVENT_originalXWGTUP");
 if(CPmix==19)loader->SetSignalWeightExpression("(EVENT_rWeights[19])/EVENT_originalXWGTUP");
 if(CPmix==20)loader->SetSignalWeightExpression("(EVENT_rWeights[20])/EVENT_originalXWGTUP");
 if(CPmix==21)loader->SetSignalWeightExpression("(EVENT_rWeights[21])/EVENT_originalXWGTUP");
 if(CPmix==22)loader->SetSignalWeightExpression("(EVENT_rWeights[22])/EVENT_originalXWGTUP");
 if(CPmix==23)loader->SetSignalWeightExpression("(EVENT_rWeights[23])/EVENT_originalXWGTUP");
 if(CPmix==24)loader->SetSignalWeightExpression("(EVENT_rWeights[24])/EVENT_originalXWGTUP");
 if(CPmix==25)loader->SetSignalWeightExpression("(EVENT_rWeights[25])/EVENT_originalXWGTUP");
 if(CPmix==26)loader->SetSignalWeightExpression("(EVENT_rWeights[26])/EVENT_originalXWGTUP");
 if(CPmix==27)loader->SetSignalWeightExpression("(EVENT_rWeights[27])/EVENT_originalXWGTUP");
 if(CPmix==28)loader->SetSignalWeightExpression("(EVENT_rWeights[28])/EVENT_originalXWGTUP");
 if(CPmix==29)loader->SetSignalWeightExpression("(EVENT_rWeights[29])/EVENT_originalXWGTUP");
 if(CPmix==30)loader->SetSignalWeightExpression("(EVENT_rWeights[30])/EVENT_originalXWGTUP");
 if(CPmix==31)loader->SetSignalWeightExpression("(EVENT_rWeights[31])/EVENT_originalXWGTUP");
 if(CPmix==32)loader->SetSignalWeightExpression("(EVENT_rWeights[32])/EVENT_originalXWGTUP");
 if(CPmix==33)loader->SetSignalWeightExpression("(EVENT_rWeights[33])/EVENT_originalXWGTUP");
 if(CPmix==34)loader->SetSignalWeightExpression("(EVENT_rWeights[34])/EVENT_originalXWGTUP");
 if(CPmix==35)loader->SetSignalWeightExpression("(EVENT_rWeights[35])/EVENT_originalXWGTUP");
 if(CPmix==36)loader->SetSignalWeightExpression("(EVENT_rWeights[36])/EVENT_originalXWGTUP");
 if(CPmix==37)loader->SetSignalWeightExpression("(EVENT_rWeights[37])/EVENT_originalXWGTUP");
 if(CPmix==38)loader->SetSignalWeightExpression("(EVENT_rWeights[38])/EVENT_originalXWGTUP");
 if(CPmix==39)loader->SetSignalWeightExpression("(EVENT_rWeights[39])/EVENT_originalXWGTUP");
 if(CPmix==40)loader->SetSignalWeightExpression("(EVENT_rWeights[40])/EVENT_originalXWGTUP");
 if(CPmix==41)loader->SetSignalWeightExpression("(EVENT_rWeights[41])/EVENT_originalXWGTUP");
 if(CPmix==42)loader->SetSignalWeightExpression("(EVENT_rWeights[42])/EVENT_originalXWGTUP");
 if(CPmix==43)loader->SetSignalWeightExpression("(EVENT_rWeights[43])/EVENT_originalXWGTUP");
 if(CPmix==44)loader->SetSignalWeightExpression("(EVENT_rWeights[44])/EVENT_originalXWGTUP");
 if(CPmix==45)loader->SetSignalWeightExpression("(EVENT_rWeights[45])/EVENT_originalXWGTUP");
 if(CPmix==46)loader->SetSignalWeightExpression("(EVENT_rWeights[46])/EVENT_originalXWGTUP");
 if(CPmix==47)loader->SetSignalWeightExpression("(EVENT_rWeights[47])/EVENT_originalXWGTUP");
 if(CPmix==48)loader->SetSignalWeightExpression("(EVENT_rWeights[48])/EVENT_originalXWGTUP");
 if(CPmix==49)loader->SetSignalWeightExpression("(EVENT_rWeights[49])/EVENT_originalXWGTUP");
 if(CPmix==50)loader->SetSignalWeightExpression("(EVENT_rWeights[50])/EVENT_originalXWGTUP");
 if(CPmix==51)loader->SetSignalWeightExpression("(EVENT_rWeights[51])/EVENT_originalXWGTUP");
 if(CPmix==52)loader->SetSignalWeightExpression("(EVENT_rWeights[52])/EVENT_originalXWGTUP");
 if(CPmix==53)loader->SetSignalWeightExpression("(EVENT_rWeights[53])/EVENT_originalXWGTUP");
 if(CPmix==54)loader->SetSignalWeightExpression("(EVENT_rWeights[54])/EVENT_originalXWGTUP");
 if(CPmix==55)loader->SetSignalWeightExpression("(EVENT_rWeights[55])/EVENT_originalXWGTUP");
 if(CPmix==56)loader->SetSignalWeightExpression("(EVENT_rWeights[56])/EVENT_originalXWGTUP");
 if(CPmix==57)loader->SetSignalWeightExpression("(EVENT_rWeights[57])/EVENT_originalXWGTUP");
 if(CPmix==58)loader->SetSignalWeightExpression("(EVENT_rWeights[58])/EVENT_originalXWGTUP");
 if(CPmix==59)loader->SetSignalWeightExpression("(EVENT_rWeights[59])/EVENT_originalXWGTUP");
 if(CPmix==60)loader->SetSignalWeightExpression("(EVENT_rWeights[60])/EVENT_originalXWGTUP");
 if(CPmix==61)loader->SetSignalWeightExpression("(EVENT_rWeights[61])/EVENT_originalXWGTUP");
 if(CPmix==62)loader->SetSignalWeightExpression("(EVENT_rWeights[62])/EVENT_originalXWGTUP");
 if(CPmix==63)loader->SetSignalWeightExpression("(EVENT_rWeights[63])/EVENT_originalXWGTUP");
 if(CPmix==64)loader->SetSignalWeightExpression("(EVENT_rWeights[64])/EVENT_originalXWGTUP");
 if(CPmix==65)loader->SetSignalWeightExpression("(EVENT_rWeights[65])/EVENT_originalXWGTUP");
 if(CPmix==66)loader->SetSignalWeightExpression("(EVENT_rWeights[66])/EVENT_originalXWGTUP");
 if(CPmix==67)loader->SetSignalWeightExpression("(EVENT_rWeights[67])/EVENT_originalXWGTUP");
 if(CPmix==68)loader->SetSignalWeightExpression("(EVENT_rWeights[68])/EVENT_originalXWGTUP");
 if(CPmix==69)loader->SetSignalWeightExpression("(EVENT_rWeights[69])/EVENT_originalXWGTUP");}




     //loader->SetSignalWeightExpression("Wta");
  //    for background: 
 if(year==0)loader->SetSignalWeightExpression("cpodd_rwgt");
 if(year!=0)loader->SetBackgroundWeightExpression("(EVENT_rWeights[11])/EVENT_originalXWGTUP");

  // Apply additional cuts on the signal and background samples (can be different)
  TCut mycuts = "angle_bbpp_highest2b>0&&(n_presel_jet>=3 && is_tH_like_and_not_ttH_like !=1 && HiggsDecay!=1)";
  TCut mycutb = "angle_bbpp_highest2b>0&&(n_presel_jet>=3 && is_tH_like_and_not_ttH_like !=1 && HiggsDecay!=1)";

  // Tell the factory how to use the training and testing events
  //
  // If no numbers of events are given, half of the events in the tree are used 
  // for training, and the other half for testing:
  //    loader->PrepareTrainingAndTestTree( mycut, "SplitMode=random:!V" );
  // To also specify the number of testing events, use:

  std::string nTr="nTrain_Signal=";


  loader->PrepareTrainingAndTestTree( mycuts, mycutb,
				      nTr+nSTrain+":nTrain_Background="+nBTrain+":SplitMode=Random:NormMode=None:!V" );

  /***
## Booking Methods
Here we book the TMVA methods. We book firat a Likelihood based on KDE (Kernel Density Estimation), a Fischer discriminant, a BDT
and a shallow neural network 
  */

  std::cout << "Generate keras model..." << std::endl;
  UInt_t ret;
  ret = gSystem->Exec("echo '"+pythonSrc+"' > generateKerasModelClassification2.py");
  if(ret!=0){
    std::cout << "[ERROR] Failed to write python code to file" << std::endl;
    return 1;
  }
  ret = gSystem->Exec("python generateKerasModelClassification2.py");
  if(ret!=0){
    std::cout << "[ERROR] Failed to generate model using python" << std::endl;
    return 1;
   }

  TMVA::PyMethodBase::PyInitialize();

  // factory.BookMethod(loader, TMVA::Types::kPyRandomForest, "PyRandomForest1",
  // 		      "!V:NEstimators=50:Criterion=gini:MaxFeatures=auto:MaxDepth=5:MinSamplesLeaf=200:MinWeightFractionLeaf=0:Bootstrap=kTRUE");

  // factory.BookMethod(loader, TMVA::Types::kPyRandomForest, "PyRandomForest2",
  // 		      "!V:NEstimators=50:Criterion=gini:MaxFeatures=auto:MaxDepth=5:MinSamplesLeaf=200:MinWeightFractionLeaf=0:Bootstrap=kTRUE");

  //factory.BookMethod(loader, TMVA::Types::kPyRandomForest, "PyRandomForest3",
  // 		      "!V:NEstimators=50:Criterion=gini:MaxFeatures=auto:MaxDepth=5:MinSamplesLeaf=200:MinWeightFractionLeaf=0:Bootstrap=kTRUE");

  // factory.BookMethod(loader, TMVA::Types::kPyAdaBoost, "PyAdaBoost",
  // 		      "!V:BaseEstimator=None:NEstimators=100:LearningRate=1:Algorithm=SAMME.R:RandomState=None");
  // factory.BookMethod(loader, TMVA::Types::kPyGTB, "PyGTB",
  // 		      "!V:NEstimators=150:Loss=deviance:LearningRate=0.1:Subsample=1:MaxDepth=6:MaxFeatures='auto'");

   factory.BookMethod(loader, TMVA::Types::kPyKeras, "PyKeras",
   		      "!H:!V:VarTransform=N:FilenameModel=kerasModelClassification2.h5:FilenameTrainedModel=trainedKerasModelClassification2.h5:NumEpochs=100:BatchSize=512:SaveBestOnly=true:Verbose=0:Tensorboard=./logs");
  // std::cout << "Train model..." << std::endl;

  // Likelihood ("naive Bayes estimator")

  //  factory.BookMethod(loader, TMVA::Types::kLikelihood, "Likelihood",
  //                         "H:!V:TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmoothBkg[1]=10:NSmooth=1:NAvEvtPerBin=50" );

  // Use a kernel density estimator to approximate the PDFs
  // factory.BookMethod(loader, TMVA::Types::kLikelihood, "LikelihoodKDE",
  // 		     "!H:!V:!TransformOutput:PDFInterpol=KDE:KDEtype=Gauss:KDEiter=Adaptive:KDEFineFactor=0.3:KDEborder=None:NAvEvtPerBin=50" ); 


  // Fisher discriminant (same as LD)
  //factory.BookMethod(loader, TMVA::Types::kFisher, "Fisher", "H:!V:Fisher:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10" );


  //  Boosted Decision Trees
  // factory.BookMethod(loader,TMVA::Types::kBDT, "BDT"
  // 		     "!V:NTrees=50:MinNodeSize=5%:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.6:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=CrossEntropy:nCuts=100" );

  // factory.BookMethod(loader,TMVA::Types::kBDT, "BDT1",
  // 		     "!V:NTrees=50:MinNodeSize=5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.6:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=100" );


  // factory.BookMethod(loader,TMVA::Types::kBDT, "BDT2",
  // 		     "!V:NTrees=50:MinNodeSize=5%:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.6:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=100" );


  factory.BookMethod(loader,TMVA::Types::kBDT, "MBDT",
  		     "!V:NTrees=150:MinNodeSize=5%:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.6:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=CrossEntropy:nCuts=100" );

  // factory.BookMethod(loader,TMVA::Types::kBDT, "MBDT1",
  // 		     "!V:NTrees=50:MinNodeSize=5%:MaxDepth=6:BoostType=AdaBoost:AdaBoostBeta=0.6:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=100" );


  // factory.BookMethod(loader,TMVA::Types::kBDT, "MBDT2",
  // 		     "!V:NTrees=50:MinNodeSize=5%:MaxDepth=7:BoostType=AdaBoost:AdaBoostBeta=0.6:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=100" );


  // factory.BookMethod(loader,TMVA::Types::kBDT, "NBDT",
  // 		     "!V:NTrees=50:MinNodeSize=5%:MaxDepth=8:BoostType=AdaBoost:AdaBoostBeta=0.6:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=100" );

  // factory.BookMethod(loader,TMVA::Types::kBDT, "NBDT1",
  // 		     "!V:NTrees=50:MinNodeSize=5%:MaxDepth=9:BoostType=AdaBoost:AdaBoostBeta=0.6:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=100" );


  factory.BookMethod(loader,TMVA::Types::kBDT, "NBDT2",
  		     "!V:NTrees=150:MinNodeSize=5%:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.6:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=10" );



  factory.BookMethod(loader,TMVA::Types::kBDT, "NBDT3",
  		     "!V:NTrees=150:MinNodeSize=10%:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.6:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=10" );


  // factory.BookMethod(loader,TMVA::Types::kBDT, "BBDT",
  // 		     "!V:NTrees=50:MinNodeSize=5%:MaxDepth=11:BoostType=AdaBoost:AdaBoostBeta=0.6:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=100" );

  // factory.BookMethod(loader,TMVA::Types::kBDT, "BBDT1",
  // 		     "!V:NTrees=50:MinNodeSize=5%:MaxDepth=12:BoostType=AdaBoost:AdaBoostBeta=0.6:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=100" );


  // factory.BookMethod(loader,TMVA::Types::kBDT, "BBDT2",
  // 		     "!V:NTrees=50:MinNodeSize=5%:MaxDepth=13:BoostType=AdaBoost:AdaBoostBeta=0.6:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=100" );


  // factory.BookMethod(loader,TMVA::Types::kBDT, "BMBDT",
  // 		     "!V:NTrees=50:MinNodeSize=5%:MaxDepth=14:BoostType=AdaBoost:AdaBoostBeta=0.6:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=100" );

  // factory.BookMethod(loader,TMVA::Types::kBDT, "BMBDT1",
  // 		     "!V:NTrees=50:MinNodeSize=5%:MaxDepth=15:BoostType=AdaBoost:AdaBoostBeta=0.6:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=100" );


  // factory.BookMethod(loader,TMVA::Types::kBDT, "BMBDT2",
  // 		     "!V:NTrees=50:MinNodeSize=5%:MaxDepth=16:BoostType=AdaBoost:AdaBoostBeta=0.6:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=100" );


  // factory.BookMethod(loader,TMVA::Types::kBDT, "BNBDT",
  // 		     "!V:NTrees=50:MinNodeSize=5%:MaxDepth=17:BoostType=AdaBoost:AdaBoostBeta=0.6:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=100" );

  // factory.BookMethod(loader,TMVA::Types::kBDT, "BNBDT1",
  // 		     "!V:NTrees=50:MinNodeSize=5%:MaxDepth=18:BoostType=AdaBoost:AdaBoostBeta=0.6:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=100" );


  // factory.BookMethod(loader,TMVA::Types::kBDT, "BNBDT2",
  // 		     "!V:NTrees=50:MinNodeSize=5%:MaxDepth=19:BoostType=AdaBoost:AdaBoostBeta=0.6:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=100" );

  // factory.BookMethod(loader,TMVA::Types::kBDT, "xgb",
  // 		     "!V:NTrees=50:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.7:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=-1" );
  //factory.BookMethod(loader,TMVA::Types::kRXBG,"RXGB", "!V:NRounds=80:MaxDepth=2:Eta=1");




  //Multi-Layer Perceptron (Neural Network)
  //  factory.BookMethod(loader, TMVA::Types::kMLP, "MLP",
  // 	     "!H:!V:NeuronType=tanh:VarTransform=N:NCycles=200:HiddenLayers=N+10:TestRate=5:!UseRegulator" );



  /// Here we book the new DNN of TMVA if we have support in ROOT. We will use GPU version if ROOT is enabled with GPU

  bool useDLCPU = false;
  bool useDLGPU = false;
#ifdef R__HAS_TMVACPU
  useDLCPU = true;
#endif
#ifdef R__HAS_TMVAGPU
  useDLGPU = true;
#endif

  /***
## Booking Deep Neural Network
Here we define the option string for builfing the Deep Neural network model. 
#### 1. Define DNN layout
The DNN configuration os defined using a string. Note that whitespaces between characters are not allowed. 
We define first the DNN layout:
- **input layout** :   this defines the input data format for the DNN as  ``input depth | height | width``. 
   In case of a dense layer as first layer the input layout should be  ``1 | 1 | number of input variables`` (features)
- **batch layout**  : this defines how are the input batch. It is related to input layout but not the same. 
   If the first layer is dense it should be ``1 | batch size ! number of variables`` (fetures)
   
   *(note the use of the character `|` as  separator of  input parameters for DNN layout)*
note that in case of only dense layer the input layout could be omitted but it is required when defining more 
complex architectures
- **layer layout** string defining the layer architecture. The syntax is  
   - layer type (e.g. DENSE, CONV, RNN)
   - layer parameters (e.g. number of units)
   - activation function (e.g  TANH, RELU,...)
   
   *the different layers are separated by the ``","`` *
                
#### 2. Define Trainining Strategy
We define here the training strategy parameters for the DNN. The parameters are separated by the ``","`` separator. 
One can then concatenate different training strategy with different parameters. The training strategy are separated by 
the ``"|"`` separator. 
 - Optimizer
 - Learning rate
							  - Momentum (valid for SGD and RMSPROP)
 - Regularization and Weight Decay 
 - Dropout 
 - Max number of epochs 
 - Convergence steps. if the test error will not decrease after that value the training will stop
			       - Batch size (This value must be the same specified in the input layout)
			       - Test Repetitions (the interval when the test error will be computed) 
#### 3. Define general DNN options
We define the general DNN options concateneting in the final string the previously defined layout and training strategy.
			       Note we use the ``":"`` separator to separate the different higher level options, as in the other TMVA methods. 
			       In addition to input layout, batch layout and training strategy we add now: 

- Type of Loss function (e.g. cross entropy)
- Weight Initizalization (e.g XAVIER, XAVIERUNIFORM, NORMAL )
- Variable Transformation
- Type of Architecture (e.g. CPU, GPU, Standard)
We can then book the DL method using the built otion string
  ***/

  if (useDLCPU || useDLGPU) {  
    // Define DNN layout
    TString inputLayoutString = "InputLayout=1|1|30"; 
    TString batchLayoutString= "BatchLayout=1|128|30";
    TString layoutString ("Layout=DENSE|100|RELU,DENSE|100|RELU,DENSE|100|RELU,DENSE|100|RELU,DENSE|50|RELU,DENSE|1|LINEAR");
    TString training1("LearningRate=4e-1,Momentum=0.0,Repetitions=2,"
                        "ConvergenceSteps=50,BatchSize=128,TestRepetitions=2,"
                        "MaxEpochs=50,Regularization=L1Regularization,"
		      "Optimizer=SGD,DropConfig=0.0+0.0+0.2+0.5+0.0");

    TString training2("LearningRate=2e-1,Momentum=0.0,Repetitions=2,"
                        "ConvergenceSteps=50,BatchSize=128,TestRepetitions=2,"
                        "MaxEpochs=100,Regularization=L1Regularization,"
    		      "Optimizer=SGD,DropConfig=0.0+0.0+0.2+0.5+0.0");

  
    TString trainingStrategyString ("TrainingStrategy=");
    trainingStrategyString += training1 + "|" + training2;

    // General Options.

    TString dnnOptions ("!H:V:ErrorStrategy=CROSSENTROPY:VarTransform=N:"
			"WeightInitialization=XAVIER");
    dnnOptions.Append (":"); dnnOptions.Append (inputLayoutString);
    dnnOptions.Append (":"); dnnOptions.Append (batchLayoutString);
    dnnOptions.Append (":"); dnnOptions.Append (layoutString);
    dnnOptions.Append (":"); dnnOptions.Append (trainingStrategyString);

    if (useDLGPU)
      dnnOptions += ":Architecture=GPU";
    else
      dnnOptions += ":Architecture=CPU";
    //    factory.BookMethod(loader, TMVA::Types::kDL, "DL_CPU", dnnOptions);

  }


  if (useDLCPU || useDLGPU) {  
    // Define DNN layout
    TString inputLayoutString2 = "InputLayout=1|1|33"; 
    TString batchLayoutString2= "BatchLayout=1|1280|33";
    //TString layoutString2 ("Layout=DENSE|120|RELU,DENSE|100|RELU,DENSE|80|RELU,DENSE|40|RELU,DENSE|1|LINEAR");
    TString layoutString2 ("Layout=DENSE|100|RELU,DENSE|60|RELU,DENSE|60|RELU,DENSE|60|RELU,DENSE|60|RELU,DENSE|60|RELU,DENSE|60|RELU,DENSE|60|RELU,DENSE|60|RELU,DENSE|60|RELU,DENSE|60|RELU,DENSE|1|SIGMOID");
    //TString layoutString2 ("Layout=DENSE|120|TANH,DENSE|60|TANH,DENSE|60|TANH,DENSE|60|TANH,DENSE|15|TANH,DENSE|15|TANH,DENSE|1|LINEAR");
    //TString layoutString2 ("Layout=DENSE|22|TANH,DENSE|22|TANH,DENSE|22|TANH,DENSE|22|TANH,DENSE|1|SIGMOID");
    // Define Training strategies 
    // one can catenate several training strategies 
    TString training1_2("LearningRate=8e-2,Momentum=0.1,Repetitions=2,"
                        "ConvergenceSteps=10,BatchSize=1280,TestRepetitions=2,"
                        "MaxEpochs=30,Regularization=L1Regularization,"
		      "Optimizer=SGD,DropConfig=0.0+0.0+0.1");
    //     TString training2("LearningRate=1e-3,Momentum=0.9,Repetitions=1,"
    //                       "ConvergenceSteps=10,BatchSize=128,TestRepetitions=1,"
    //                       "MaxEpochs=20,WeightDecay=1e-4,Regularization=None,"
    //                       "Optimizer=SGD,DropConfig=0.0+0.0+0.0+0.");
  
    TString trainingStrategyString2 ("TrainingStrategy=");
    trainingStrategyString2 += training1_2; // + "|" + training2;

    // General Options.

    TString dnnOptions2 ("!H:V:ErrorStrategy=CROSSENTROPY:VarTransform=N:"
			"WeightInitialization=XAVIER");
    dnnOptions2.Append (":"); dnnOptions2.Append (inputLayoutString2);
    dnnOptions2.Append (":"); dnnOptions2.Append (batchLayoutString2);
    dnnOptions2.Append (":"); dnnOptions2.Append (layoutString2);
    dnnOptions2.Append (":"); dnnOptions2.Append (trainingStrategyString2);

    if (useDLGPU)
      dnnOptions2 += ":Architecture=GPU";
    else
      dnnOptions2 += ":Architecture=CPU";
    //factory.BookMethod(loader, TMVA::Types::kDL, "DL_CPU_2", dnnOptions2);

  }

  if (useDLCPU || useDLGPU) {  
    // Define DNN layout
    TString inputLayoutString3 = "InputLayout=1|1|29"; 
    TString batchLayoutString3= "BatchLayout=1|1280|29";

    TString layoutString3 ("Layout=DENSE|60|RELU,DENSE|60|RELU,DENSE|60|RELU,DENSE|60|RELU,DENSE|60|RELU,DENSE|60|RELU,DENSE|60|RELU,DENSE|60|RELU,DENSE|60|RELU,DENSE|60|RELU,DENSE|60|RELU,DENSE|1|LINEAR");
    //TString layoutString2 ("Layout=DENSE|120|TANH,DENSE|60|TANH,DENSE|30|TANH,DENSE|30|TANH,DENSE|15|TANH,DENSE|15|TANH,DENSE|1|LINEAR");
    //TString layoutString2 ("Layout=DENSE|22|TANH,DENSE|22|TANH,DENSE|22|TANH,DENSE|22|TANH,DENSE|1|SIGMOID");
    // Define Training strategies 
    // one can catenate several training strategies 
    TString training1_3("LearningRate=5e-2,Momentum=0.8,Repetitions=2,"
                        "ConvergenceSteps=100,BatchSize=1280,TestRepetitions=2,"
                        "MaxEpochs=100,Regularization=L1Regularization,"
		      "Optimizer=SGD,DropConfig=0.0+0.0+0.0+0.0+0.0+0.0+0.0+0.0+0.0+0.0+0.3");
    //     TString training2("LearningRate=1e-3,Momentum=0.9,Repetitions=1,"
    //                       "ConvergenceSteps=10,BatchSize=128,TestRepetitions=1,"
    //                       "MaxEpochs=20,WeightDecay=1e-4,Regularization=None,"
    //                       "Optimizer=SGD,DropConfig=0.0+0.0+0.0+0.");
  
    TString trainingStrategyString3 ("TrainingStrategy=");
    trainingStrategyString3 += training1_3; // + "|" + training2;

    // General Options.

    TString dnnOptions3 ("!H:V:ErrorStrategy=CROSSENTROPY:VarTransform=N:"
			"WeightInitialization=XAVIER");
    dnnOptions3.Append (":"); dnnOptions3.Append (inputLayoutString3);
    dnnOptions3.Append (":"); dnnOptions3.Append (batchLayoutString3);
    dnnOptions3.Append (":"); dnnOptions3.Append (layoutString3);
    dnnOptions3.Append (":"); dnnOptions3.Append (trainingStrategyString3);

    if (useDLGPU)
      dnnOptions3 += ":Architecture=GPU";
    else
      dnnOptions3 += ":Architecture=CPU";
    //factory.BookMethod(loader, TMVA::Types::kDL, "DL_CPU_3", dnnOptions3);

  }

  /**
## Train Methods
Here we train all the previously booked methods. 
  */

  /**
   ## Test  all methods
 Now we test and evaluate all methods using the test data set
  */

  // factory.OptimizeAllMethods("ROCIntegral","FitGA");
  factory.TrainAllMethods();
  factory.TestAllMethods();   

   factory.EvaluateAllMethods();

  /// after we get the ROC curve and we display 

  // auto c1 = factory.GetROCCurve(loader);
  // c1->SaveAs("ROC.pdf");

  /// at the end we close the output file which contains the evaluation result of all methods and it can be used by TMVAGUI
  /// to display additional plots
   
  outputFile->Close();


}
