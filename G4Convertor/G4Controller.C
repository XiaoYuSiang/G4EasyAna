#include <G4EventAna.C+>
#include <BeamEasyStudy.C+>
void G4Controller()
{
  
  /*Adjustable variable */
  const char VectorInputG4RootFiles   [200] = 
    "/data4/AndyLu/to_cw/4_4_4_starightbeam_merge.root"
  ;
  const char OuputGapRootFilePath [200] = 
    "/data4/YuSiang/TEST/"
  ;//Format:   Path/Name(GapT*_MC_nHTH*L*.root)
  const char OuputGapRootFileName [200] = 
    "4_4_4_"
  ;//Format:   Path/Name(GapT*_MC_nHTH*L*.root)
  
  char OuputGapRootFilePathName[400];
  sprintf(OuputGapRootFilePathName,"%s%s",OuputGapRootFilePath,OuputGapRootFileName);
  // cout<<OuputGapRootFilePathName<<endl;
  system(Form("mkdir -p %s",OuputGapRootFilePath));
  /*Finish adjustable variable */
  
  const Int_t TriggerLessNumberOfHitInEvent  = 1;
  const Int_t TriggerLargeNumberOfHitInEvent = 64;
  const Int_t MaximumEventGapOfTcnt          = 300;//Almost don't need to change
  
  TString GapFilePathName = 
    Form("%sGapT%d_%s_nHTH%dL%d.root",
      OuputGapRootFilePathName,MaximumEventGapOfTcnt,"MC",TriggerLessNumberOfHitInEvent,TriggerLargeNumberOfHitInEvent);
    
  const char SaveGraphPath        [200] = 
  "/home/yusiang/G4/4_4_4HorizontaBeamTest/"
  ;
  const char SaveGraphName        [200] = 
  "test"
  ;
  
  char SaveGraphPathName[400];
  sprintf(SaveGraphPathName,"%s%s",SaveGraphPath,SaveGraphName);
  // cout<<OuputGapRootFilePathName<<endl;
  system(Form("mkdir -p %s",SaveGraphPath));
  /*Finish adjustable variable */
  
  /*1. Convert the G4 data to be analysis format: GapT*_MC_nHTH*L*.root*/
  G4EventAna(//Convert the G4 data to be GapT*_MC_nHTH*L*.root
    VectorInputG4RootFiles,
    OuputGapRootFilePathName,
    TriggerLessNumberOfHitInEvent, 
    TriggerLargeNumberOfHitInEvent,
    MaximumEventGapOfTcnt
  );
  
  /*2. Use the analysis format file to Draw Graph*/
  BeamEasyStudy(//Convert the G4 data to be GapT*_MC_nHTH*L*.root
    GapFilePathName.Data(), SaveGraphPathName
  );
  
}
