//include
#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <TH1F.h>
#include <TH2F.h>
#if defined (__MAKECINT__) 
#pragma link C++ class vector<Long64_t>+; 
#endif
#include "/data4/YuSiang/personalLib/RootFile/untuplizerv8_YuSiang.h"
#include "/data4/YuSiang/personalLib/RootFile/untuplizerv8_YuSiang.h"
#include "/data4/YuSiang/personalLib/Math/UnixTranslator.h"

using namespace std;
/*program constant variable don't change!!!*/

const Double_t tcntOneSec  = 2.56E+8;
  //The tcntOneSec tcnt = 1 second
const Int_t    TheFirstUT  = 1600000001;
  //Suppose the first unixtime in data is TheFirstUT
const Int_t    FramIn1Sec  = 1100;
  //suppose in one second contain 1100 frames
const Double_t EdeptoPWit  = 6.55;
  //suppose pwidth(100s) = EdeptoPWit*Edep(GeV)
  
/*Finish program constant variable setting!*/

/*Const of setup */
const int   NumBD = 16;//number of the MTBs
const int   NumCh = 16;//number of the channel on the MTB
const int   NumLY =  4;//number of the board layer
const Int_t BD[NumCh]   = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};//the BID of the MTB
int G4GIDToGID[256] = //Transfer the G4GID to be GID
{
  44,  45,  46,  47,  60,  61,  62,  63,  40,  41,  42,  43,  56,  57,  58,  59,
  36,  37,  38,  39,  52,  53,  54,  55,  32,  33,  34,  35,  48,  49,  50,  51,
  12,  13,  14,  15,  28,  29,  30,  31,  8,  9,  10,  11,  24,  25,  26,  27,
  4,  5,  6,  7,  20,  21,  22,  23,  0,  1,  2,  3,  16,  17,  18,  19,
  108,  109,  110,  111,  124,  125,  126,  127,  104,  105,  106,  107,  120,  121,  122,  123,
  100,  101,  102,  103,  116,  117,  118,  119,  96,  97,  98,  99,  112,  113,  114,  115,
  76,  77,  78,  79,  92,  93,  94,  95,  72,  73,  74,  75,  88,  89,  90,  91,
  68,  69,  70,  71,  84,  85,  86,  87,  64,  65,  66,  67,  80,  81,  82,  83,
  172,  173,  174,  175,  188,  189,  190,  191,  168,  169,  170,  171,  184,  185,  186,  187,
  164,  165,  166,  167,  180,  181,  182,  183,  160,  161,  162,  163,  176,  177,  178,  179,
  140,  141,  142,  143,  156,  157,  158,  159,  136,  137,  138,  139,  152,  153,  154,  155,
  132,  133,  134,  135,  148,  149,  150,  151,  128,  129,  130,  131,  144,  145,  146,  147,
  236,  237,  238,  239,  252,  253,  254,  255,  232,  233,  234,  235,  248,  249,  250,  251,
  228,  229,  230,  231,  244,  245,  246,  247,  224,  225,  226,  227,  240,  241,  242,  243,
  204,  205,  206,  207,  220,  221,  222,  223,  200,  201,  202,  203,  216,  217,  218,  219,
  196,  197,  198,  199,  212,  213,  214,  215,  192,  193,  194,  195,  208,  209,  210,  211
};
/*Finish const of setup */

/*Adjustable variable */
const char VectorInputRootFilesDefault[200] = 
  "/data4/AndyLu/to_cw/4_4_4_starightbeam_merge.root"
;
const char OuputRootFilePathName      [200] = 
  "/data4/YuSiang/TEST/4_4_4_"
;//Format:   Path/Name(GapT*_MC_nHTH*L*.root)
const Int_t MaximumEventGapOfTcnt          = 300;
const Int_t TriggerLessNumberOfHitInEvent  = 1;
const Int_t TriggerLargeNumberOfHitInEvent = 64;
/*Finish adjustable variable */

/*Function of G4GID GID BID CID and Edep Pwidth convert*/
int GetBIDbyG4GID(const int G4GID){
  return int(G4GIDToGID[G4GID])/16+1;
}
int GetCIDbyG4GID(const int G4GID){
  return int(G4GIDToGID[G4GID])/4+(int(G4GIDToGID[G4GID])%4)*4;
}
int ConvertEdeptoPWit(const Double_t Edep){
  return Edep*EdeptoPWit;
}
int BDcheck(const int b){
  int z = -1;
  for(int i=0;i<NumBD;i++){
    if( b == BD[i] ){
      z = i;
      break;
    } 
  }if(z==-1){
    cout<<"Warning: GobelFun.h: Please Enter the true value b into the int BDcheck(int b)!!!   ";
    cout<<"Now the Enter value b is:  "<< b <<endl;
    //throw;
  } 
  return z;
}
/*Finish function of G4GID GID BID CID and Edep Pwidth convert*/

/*Main program to convert the G4 data to be analysis format GapT*_MC_nHTH*L*.root*/
void G4EventAna(
  const char vRootFilesin[200] = VectorInputRootFilesDefault,
  const char OORtFN      [200] = OuputRootFilePathName,
  const int  TriggerS          = TriggerLessNumberOfHitInEvent,
  const int  TriggerL          = TriggerLargeNumberOfHitInEvent,
  const int  eventGapTcnt      = MaximumEventGapOfTcnt
) {
  //Input Data Read of TTree by TreeReader in untuplizerv8
  TreeReader data(vRootFilesin,"mu_detector");
  cout<<Form("Root data(Tree) opening from: %s",vRootFilesin)<<endl;
  
  char SaveName[250];
  sprintf(
    SaveName,"%sGapT%d_%s_nHTH%dL%d.root",
    OORtFN,eventGapTcnt,"MC",TriggerS,TriggerL
  );
  //Located Output TTree Data to be GapT*_MC_nHTH*L*.root
  TFile *rotfile = new TFile(SaveName,"RECREATE");
  cout<<Form("Root save to: %s",SaveName)<<endl;
  
  //Data Variable initialize
  Int_t      EvIndex=0; // the index of the event
  Int_t      frame_   ; // the frame number of the event
  Long64_t   unixtime_; // the computer unixtime time of this event(s)
  Int_t      tYear    ; // the year  of this unixtime
  Int_t      tMonth   ; // the Month of this unixtime
  Int_t      tDate    ; // the Date...
  Int_t      tHour    ; // the Hour...
  Int_t      tMinute  ; // the Minute...
  Int_t      tSecond  ; // the Second...
  //PS:2560000000 Hz
  Int_t      nLayers  ;
  Int_t      nH       ;  // number of hit in one event
  Int_t      nH0, nH1, nH2, nH3; // nh"z" = number of hit on the "z" layer
  Int_t      channel_ ;  // Channel ID in G4 setup
  Double_t   Edep_    ;  // Energy deposit
  
  vector<Double_t>  dtime;    // the hit time calculate by tcnt and pcnt(s)
  vector<Long64_t>  dtimeInt; // the hit time calculate by tcnt and pcnt(1/2.56E+8 s)
  vector<Int_t>     g4gid;    // channel ID of hit in G4
  vector<Int_t>     channel;  // channel ID of the hit on the board
  vector<Int_t>     board;    // board ID of hit
  vector<Int_t>     pwidth;   // pwidth = 1 -> signal larger than thershold for 100ms
  vector<Int_t>     pwidthScaleFactor; // unimportant now
  vector<Int_t>     pcnt;     // the number of the board get the PPS signal(s)
  vector<Long64_t>  tcnt;     // the number of the board time clock count(1/2.56E+8 s)
  vector<Double_t>  dtcnt;    // the tcount difference between the last and this hit
  Double_t          dTimeOfEv;// unimportant now
  
  //Setting input TTree read way
  TTree *tree = new TTree("t","data from G4 analyzing file");
  tree->Branch("frame",&frame_);
  tree->Branch("EvIndex",&EvIndex);
  tree->Branch("unixtime",&unixtime_);
  tree->Branch("tYear",&tYear);
  tree->Branch("tMonth",&tMonth);
  tree->Branch("tDate",&tDate);
  tree->Branch("tHour",&tHour);
  tree->Branch("tMinute",&tMinute);
  tree->Branch("tSecond",&tSecond);

  tree->Branch("nH",&nH);
  tree->Branch("nH0",&nH0);
  tree->Branch("nH1",&nH1);
  tree->Branch("nH2",&nH2);
  tree->Branch("nH3",&nH3);
  
  tree->Branch("nLayers",&nLayers);
  tree->Branch("g4gid",&g4gid);
  tree->Branch("channel",&channel);
  tree->Branch("board",&board);
  tree->Branch("pwidth",&pwidth);
  tree->Branch("pwidthScaleFactor",&pwidthScaleFactor);
  tree->Branch("tcnt",&tcnt);
  tree->Branch("pcnt",&pcnt);
  tree->Branch("dtcnt",&dtcnt);
  tree->Branch("dtime",&dtime);
  tree->Branch("dtimeInt",&dtimeInt);
  tree->Branch("dTimeOfEv",&dTimeOfEv);
  
  //Set the histrogram to calculate the bH, nH"z"
  TH1F *hBoard       = new TH1F("hBoard ","",NumBD,0,NumBD);
  
  //Set some temporary variable
  Int_t Index =0, nHit = 0, frame0 = 0;
  double dtime0 = 0;
  dTimeOfEv = 0;
  
  //Set the max index of the input file
  int evs = data.GetEntriesFast();
  
  //Read the input data and output them to be experiment data format
  for (Long64_t ev = 0; ev < evs ; ++ev) {//@@@
    if(ev%(int(evs/100.)) == 0){
      cout<<Form("\r%.5f%%",(ev*100.)/(1.*evs))<<flush; // Scan the progress
    }
    data.GetEntry(ev); // point to the event index "ev"
    
    //read the input hit information
    frame_   = data.GetInt("eID");
    channel_ = data.GetDouble("channel");
    Edep_    = data.GetDouble("Edep");  
    
    //In condition: Prepare to save the data
    if (nHit!=0&&frame0!=frame_){
      unixtime_ = TheFirstUT+pcnt[nHit-1];// create a virtual unixtime for data format.
      
      // get the time information, +8 time zone
      unixTimeToHumanReadable(unixtime_,
        tYear, tMonth, tDate, tHour, tMinute, tSecond,+8);
        
      //total number for hit at "z" layer
      int NPartCut = NumBD/NumLY;
      int VnH[NumLY] = {0,0,0,0};
      for(int iPartCut = 0 ; iPartCut < NPartCut ; iPartCut++){
        for(int iLay = 0 ; iLay < NumLY ; iLay++){
          VnH[iLay] +=hBoard->GetBinContent(BD[iLay*NPartCut+iPartCut]);
        }
      }
      
      nH      = nHit;//total number for hit in a frame/event
      nH0     = VnH[0];
      nH1     = VnH[1];
      nH2     = VnH[2];
      nH3     = VnH[3];
      
      if(nH!=nH0+nH1+nH2+nH3){
        cout<<EvIndex<<"\t"<<nH<<"\t"<<nH0<<"\t"<<nH1<<"\t"<<nH2<<"\t"<<nH3<<endl;//Debug Code
        cout<<frame_<<"\t";
        throw;
      } 
      nLayers=0 ;//reconfigure number of layer of hits
      for(int i1=0; i1<4;i1++){
        if (VnH[i1]!=0) nLayers++;
      }
      
      if (nHit > TriggerS&&nHit < TriggerL){ //Select condition for number of hit
        if(/*unixtime_>TheFirstUT-1*/true){ //Old way to debug and save event
          dTimeOfEv = abs(dtimeInt[nHit-1]-dtime0);
          tree->Fill();
          
          Index++;
          EvIndex = Index;
        }
      }
      //clear the data for new event to storge
      g4gid.clear();
      board.clear();
      channel.clear();
      pwidth.clear();
      pwidthScaleFactor.clear();
      tcnt.clear();
      pcnt.clear();
      dtime.clear();
      dtimeInt.clear();
      dtcnt.clear();
      nHit = 0;
      hBoard->Reset();
      dtcnt.push_back(0);

    }

    //record the hit data in to vector
    g4gid            .push_back((channel_));
    board            .push_back(GetBIDbyG4GID(channel_));
    channel          .push_back(GetCIDbyG4GID(channel_));
    pwidth           .push_back(ConvertEdeptoPWit(Edep_));
      //suppose pwidth(100s) = EdeptoPWit*Edep(GeV)
    pwidthScaleFactor.push_back(1);//default is 1
    tcnt             .push_back(rand()%eventGapTcnt);
    pcnt             .push_back(frame_/FramIn1Sec);
    dtime            .push_back(pcnt[nHit-1]+tcnt[nHit-1]/tcntOneSec);
    dtimeInt         .push_back(pcnt[nHit-1]*tcntOneSec+tcnt[nHit-1]);
    hBoard    ->Fill(GetBIDbyG4GID(channel_)-1);
    frame0    = frame_;
    dtime0    = dtimeInt[nHit-1];
    
    //calculate the hits number
    nHit++;
  }
  // tree->Fill();
  delete hBoard;
  rotfile->Write();
  rotfile->Close();
  cout<<Form("\rFinished root file save: %s",SaveName)<<endl;
  

}




