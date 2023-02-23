void G4Controller()
{
  
  /*Adjustable variable */
  const char VectorInputG4RootFiles   [200] = 
    "/data4/AndyLu/to_cw/4_4_4_starightbeam_merge.root"
  ;
  const char OuputGapRootFilePathName [200] = 
    "/data4/YuSiang/TEST/4_4_4_"
  ;//Format:   Path/Name(GapT*_MC_nHTH*L*.root)
  
  
  const Int_t TriggerLessNumberOfHitInEvent  = 1;
  const Int_t TriggerLargeNumberOfHitInEvent = 64;
  const Int_t MaximumEventGapOfTcnt          = 300;//Almost don't need to change
  
  TString GapFilePathName = 
    Form("%sGapT%d_%s_nHTH%dL%d.root",
      OuputGapRootFilePathName,MaximumEventGapOfTcnt,"MC",TriggerLessNumberOfHitInEvent,TriggerLargeNumberOfHitInEvent);
    
  const char SaveGraphPathName        [200] = 
  "/home/yusiang/G4/4_4_4HorizontaBeamTest/test"
  ;
  /*Finish adjustable variable */

  /*1. Convert the G4 data to be analysis format: GapT*_MC_nHTH*L*.root*/
  gROOT->LoadMacro(Form("./G4EventAna.C+"));//load macre(need to use .C+)
  G4EventAna(//Convert the G4 data to be GapT*_MC_nHTH*L*.root
    VectorInputG4RootFiles,
    OuputGapRootFilePathName,
    TriggerLessNumberOfHitInEvent, 
    TriggerLargeNumberOfHitInEvent,
    MaximumEventGapOfTcnt
  );
  
  /*2. Use the analysis format file to Draw Graph*/
  gROOT->LoadMacro(Form("./BeamEasyStudy.C++"));//load macre(need to use .C+)
  BeamEasyStudy(//Convert the G4 data to be GapT*_MC_nHTH*L*.root
    GapFilePathName.Data(), SaveGraphPathName
  );
  
}