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
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include "TMVA/Factory.h"
#include "TMVA/Reader.h"
#include "TMVA/DataLoader.h"
#include "TMVA/PyMethodBase.h"


TString pythonSrc = "\
from keras.models import Sequential\n\
from keras.layers.core import Dense, Activation, Dropout\n\
from keras.optimizers import SGD, Adam\n\
\n\
model = Sequential()\n\
model.add(Dense(32, activation=\"relu\", input_dim=17))\n\
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

void ttH_Classification_SimpleExample(const char* fileNameChar="ttH_Classification_SimpleExample", int year=1, int CPmix=59,int repeatS=1, int repeatB=1, int nSTrain=0, int nBTrain=0, int crosstesting=0) { 

  TMVA::Tools::Instance();

  std::string fileName=fileNameChar;
  std::string dataName="dataset";

  auto outputFile = TFile::Open(fileName+"_"+CPmix+"_"+year+"_repeatS_"+repeatS+"_repeatB_"+repeatB+"_TrS_"+nSTrain+"_TrB_"+nBTrain+"_crosstesting_"+crosstesting+".root", "RECREATE");

  TMVA::Factory factory(fileName+"_"+CPmix+"_"+year+"_repeatS_"+repeatS+"_repeatB_"+repeatB+"_TrS_"+nSTrain+"_TrB_"+nBTrain+"_crosstesting_"+\
crosstesting, outputFile,
			"!V:ROC:Silent:Color:DrawProgressBar:Transformations=N:AnalysisType=Classification" ); 

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

  loader->AddVariable("avg_dr_jet");
  loader->AddVariable("dr_leps");
  loader->AddVariable("jet1_pt");
  loader->AddVariable("jet1_eta");
  loader->AddVariable("jet1_phi");
  loader->AddVariable("jet2_pt");
  loader->AddVariable("jet2_eta");
  loader->AddVariable("jet2_phi");
  loader->AddVariable("jet3_pt");
  loader->AddVariable("jet3_eta");
  loader->AddVariable("jet3_phi");
  loader->AddVariable("lep1_pt");
  loader->AddVariable("lep2_pt");

  loader->AddVariable("deta_highest2b");
  loader->AddVariable( "angle_bbpp_highest2b", 'F' );
  loader->AddVariable( "mindr_lep1_jet", 'F' );
  loader->AddVariable( "mindr_lep2_jet", 'F' ); 
 

   
  // You can add an arbitrary number of signal or background trees
  if(year==2016||year==1){
     loader->AddSignalTree    ( signalTree2016,      1.0);
     loader->AddBackgroundTree    ( signalTree2016,     1.0);
  }

  if(year==2017||year==1){
     loader->AddSignalTree    ( signalTree2017,      1.0);
     loader->AddBackgroundTree    ( signalTree2017,     1.0);
  }

  if(year==2018||year==1){
     loader->AddSignalTree    ( signalTree2018,      1.0);
     loader->AddBackgroundTree    ( signalTree2018,     1.0);
  }

  // Set individual event weights (the variables must exist in the original TTree)
  //    for signal    : 


  std::string EV1="(EVENT_rWeights[";
  std::string EV2="])/EVENT_originalXWGTUP";
  loader->SetSignalWeightExpression(EV1+CPmix+EV2);

  loader->SetBackgroundWeightExpression("(EVENT_rWeights[11])/EVENT_originalXWGTUP");

  // Apply additional cuts on the signal and background samples (can be different)
  TCut mycuts = "angle_bbpp_highest2b>0&&(n_presel_jet>=3 && is_tH_like_and_not_ttH_like !=1 && HiggsDecay!=1)";
  TCut mycutb = "angle_bbpp_highest2b>0&&(n_presel_jet>=3 && is_tH_like_and_not_ttH_like !=1 && HiggsDecay!=1)";

  std::string nTr="nTrain_Signal=";
  loader->PrepareTrainingAndTestTree( mycuts, mycutb,
				      nTr+nSTrain+":nTrain_Background="+nBTrain+":SplitMode=Random:NormMode=None:!V" );
  //GenerateKerasModel
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
  factory.BookMethod(loader, TMVA::Types::kPyKeras, "PyKeras",
   		      "!H:!V:VarTransform=N:FilenameModel=kerasModelClassification2.h5:FilenameTrainedModel=trainedKerasModelClassification2.h5:NumEpochs=100:BatchSize=512:SaveBestOnly=true:Verbose=0:Tensorboard=./logs");


  factory.BookMethod(loader,TMVA::Types::kBDT, "MBDT",
  		     "!V:NTrees=150:MinNodeSize=5%:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.6:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=CrossEntropy:nCuts=100" );

  factory.BookMethod(loader,TMVA::Types::kBDT, "NBDT2",
  		     "!V:NTrees=150:MinNodeSize=5%:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.6:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=10" );

  factory.BookMethod(loader,TMVA::Types::kBDT, "NBDT3",
  		     "!V:NTrees=150:MinNodeSize=10%:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.6:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=10" );

  // factory.OptimizeAllMethods("ROCIntegral","FitGA");
  factory.TrainAllMethods();
  factory.TestAllMethods();   
  factory.EvaluateAllMethods();

  // auto c1 = factory.GetROCCurve(loader);
  // c1->SaveAs("ROC.pdf");

  /// at the end we close the output file which contains the evaluation result of all methods and it can be used by TMVAGUI
  /// to display additional plots
  outputFile->Close();

}
