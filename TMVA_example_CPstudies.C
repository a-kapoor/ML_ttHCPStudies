void TMVA_example_CPstudies() { 

  TMVA::Tools::Instance();
  auto outputFile = TFile::Open("TMVA_example_studies", "RECREATE");
  TMVA::Factory factory("TMVA_Higgs_Classification", outputFile,
			"!V:ROC:!Silent:Color:!DrawProgressBar:Transformations=I,N:AnalysisType=Classification" ); 
  
//Setup Dataset(s)
  TString CPfile = "TTH_CP_DiLepRegion.root";
  TString SMfile = "ttH_DiLepRegion.root";

  auto CPFile = TFile::Open(CPfile);
  auto SMFile = TFile::Open(SMfile);

  TTree *signalTree     = (TTree*)CPFile->Get("syncTree");
  TTree *backgroundTree = (TTree*)SMFile->Get("syncTree");


  //signalTree->Print();

  /***
## Declare DataLoader(s)
The next step is to declare the DataLoader class that deals with input variables 
Define the input variables that shall be used for the MVA training
note that you may also use variable expressions, which can be parsed by TTree::Draw( "expression" )]
  ***/
  TMVA::DataLoader * loader = new TMVA::DataLoader("dataset");

  loader->AddVariable("avg_dr_jet");
  loader->AddVariable("dr_leps");
  loader->AddVariable("jet1_eta");
  loader->AddVariable("jet1_phi");
  loader->AddVariable("jet2_eta");
  loader->AddVariable("jet2_phi");
  loader->AddVariable("jet3_eta");
  loader->AddVariable("jet3_phi");
  loader->AddVariable("lep1_pt");
  loader->AddVariable("lep2_pt");
  loader->AddVariable("massL");
  loader->AddVariable("mindr_lep1_jet");
  loader->AddVariable("mvaOutput_2lss_ttV");
  loader->AddVariable("acuteangle_bbpp_highest2b");
  loader->AddVariable("deta_highest2b");
  loader->AddVariable("cosa_highest2b");
  loader->AddVariable( "angle_bbpp_highest2b", 'F' );
  loader->AddVariable("hadTop_pt",'F');
  loader->AddVariable("mbb_loose",'F');
  loader->AddVariable("jet1_pt",'F'); 
  loader->AddVariable("jet2_pt",'F'); 
  loader->AddVariable( "PFMET", 'F' ); 
  loader->AddVariable( "mindr_lep2_jet", 'F' );

  for(int i=0;i<6;i++){
    loader->AddSignalTree    ( signalTree,     1.0     );}
  loader->AddBackgroundTree( backgroundTree, 1.0);

  // Set individual event weights (the variables must exist in the original TTree)
  //    for signal    : 
  loader->SetSignalWeightExpression("cpodd_rwgt");
  //    for background: 
  loader->SetBackgroundWeightExpression("cpodd_rwgt");
  //loader->SetBackgroundWeightExpression( "EVENT_genWeight" );

  // Apply additional cuts on the signal and background samples (can be different)
  TCut mycuts = "EVENT_genWeight>0&&angle_bbpp_highest2b>0&&(nLooseJet>=4 && is_tH_like_and_not_ttH_like !=1 && HiggsDecay!=1)";
  TCut mycutb = "EVENT_genWeight>0&&angle_bbpp_highest2b>0&&(nLooseJet>=4 && is_tH_like_and_not_ttH_like !=1 && HiggsDecay!=1)";

  // Tell the factory how to use the training and testing events
  //
  // If no numbers of events are given, half of the events in the tree are used 
  // for training, and the other half for testing:
  //    loader->PrepareTrainingAndTestTree( mycut, "SplitMode=random:!V" );
  // To also specify the number of testing events, use:
  
  loader->PrepareTrainingAndTestTree( mycuts, mycutb,
				      "nTrain_Signal=120000:nTrain_Background=40000:SplitMode=Random:NormMode=NumEvents:!V" );
  
  /***
      ## Booking Methods
  */
  
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
  
  // Define DNN layout
  TString inputLayoutString = "InputLayout=1|1|23"; //23 1d inputs
  TString batchLayoutString= "BatchLayout=1|600|23"; //Batch size=1280
  TString layoutString ("Layout=DENSE|23|RELU,DENSE|23|RELU,DENSE|46|RELU,DENSE|46|RELU,DENSE|100|RELU,DENSE|100|RELU,DENSE|60|RELU,DENSE|1|LINEAR");//7 layers with 60 nodes each (relu activation)
  TString training1("LearningRate=5e-2,Momentum=0.0,Repetitions=2,"
		    "ConvergenceSteps=50,BatchSize=600,TestRepetitions=2,"
		    "MaxEpochs=100,Regularization=L1Regularization,"
		    "Optimizer=SGD,DropConfig=0.0+0.0+0.0+0.0+0.0+0.0+0.0+0.1");
  
  TString trainingStrategyString ("TrainingStrategy=");
  trainingStrategyString += training1; // + "|" + training2;
  
  // General Options.
  
  TString dnnOptions ("!H:V:ErrorStrategy=CROSSENTROPY:VarTransform=N:"
		      "WeightInitialization=XAVIER");
  dnnOptions.Append (":"); dnnOptions.Append (inputLayoutString);
  dnnOptions.Append (":"); dnnOptions.Append (batchLayoutString);
  dnnOptions.Append (":"); dnnOptions.Append (layoutString);
  dnnOptions.Append (":"); dnnOptions.Append (trainingStrategyString);
  dnnOptions += ":Architecture=CPU";
  factory.BookMethod(loader, TMVA::Types::kDL, "DL_CPU", dnnOptions);
  
  
  factory.OptimizeAllMethods("ROCIntegral","FitGA");
  factory.TrainAllMethods();
  factory.TestAllMethods();   
  
  factory.EvaluateAllMethods();
  
  // auto c1 = factory.GetROCCurve(loader);
  // c1->SaveAs("ROC.pdf");
  
  /// at the end we close the output file which contains the evaluation result of all methods and it can be used by TMVAGUI
  /// to display additional plots
  
  outputFile->Close();
  
  
}
