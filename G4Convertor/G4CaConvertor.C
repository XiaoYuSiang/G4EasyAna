#include "TTree.h"
#include "TFile.h"
#include <TH1F.h>

#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <vector>
#include <map>
#if defined (__MAKECINT__) 
#pragma link C++ class vector<Long64_t>+; 
#endif
#include "/data4/YuSiang/personalLib/RootFile/untuplizerv8_YuSiang.h"


using namespace std;
using namespace MuoAna_path_dir;
using namespace MuographAnaVariable;

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


/*Adjustable variable */
const char VectorInputRootFilesDefault[200] = 
  "/home/yusiang/G4/4_4_4HorizontaBeamTest/Data/4_4_4HorizontalBeamTest.root"
;
const char OuputRootFilePathName      [200] = 
  "./"
;//Format:   Path/Name(GapT*_MC_nHTH*L*.root)

const Int_t MaximumEventGapOfTcnt          = 300;
const Int_t TriggerLessNumberOfHitInEvent  = 1;
const Int_t TriggerLargeNumberOfHitInEvent = 64;


int G4GIDToGID[256] = 
{
  44,
  45,
  46,
  47,
  60,
  61,
  62,
  63,
  40,
  41,
  42,
  43,
  56,
  57,
  58,
  59,
  36,
  37,
  38,
  39,
  52,
  53,
  54,
  55,
  32,
  33,
  34,
  35,
  48,
  49,
  50,
  51,
  12,
  13,
  14,
  15,
  28,
  29,
  30,
  31,
  8,
  9,
  10,
  11,
  24,
  25,
  26,
  27,
  4,
  5,
  6,
  7,
  20,
  21,
  22,
  23,
  0,
  1,
  2,
  3,
  16,
  17,
  18,
  19,
  108,
  109,
  110,
  111,
  124,
  125,
  126,
  127,
  104,
  105,
  106,
  107,
  120,
  121,
  122,
  123,
  100,
  101,
  102,
  103,
  116,
  117,
  118,
  119,
  96,
  97,
  98,
  99,
  112,
  113,
  114,
  115,
  76,
  77,
  78,
  79,
  92,
  93,
  94,
  95,
  72,
  73,
  74,
  75,
  88,
  89,
  90,
  91,
  68,
  69,
  70,
  71,
  84,
  85,
  86,
  87,
  64,
  65,
  66,
  67,
  80,
  81,
  82,
  83,
  172,
  173,
  174,
  175,
  188,
  189,
  190,
  191,
  168,
  169,
  170,
  171,
  184,
  185,
  186,
  187,
  164,
  165,
  166,
  167,
  180,
  181,
  182,
  183,
  160,
  161,
  162,
  163,
  176,
  177,
  178,
  179,
  140,
  141,
  142,
  143,
  156,
  157,
  158,
  159,
  136,
  137,
  138,
  139,
  152,
  153,
  154,
  155,
  132,
  133,
  134,
  135,
  148,
  149,
  150,
  151,
  128,
  129,
  130,
  131,
  144,
  145,
  146,
  147,
  236,
  237,
  238,
  239,
  252,
  253,
  254,
  255,
  232,
  233,
  234,
  235,
  248,
  249,
  250,
  251,
  228,
  229,
  230,
  231,
  244,
  245,
  246,
  247,
  224,
  225,
  226,
  227,
  240,
  241,
  242,
  243,
  204,
  205,
  206,
  207,
  220,
  221,
  222,
  223,
  200,
  201,
  202,
  203,
  216,
  217,
  218,
  219,
  196,
  197,
  198,
  199,
  212,
  213,
  214,
  215,
  192,
  193,
  194,
  195,
  208,
  209,
  210,
  211
};


//channel vs rate combine
struct Tmpdata {
  Int_t     gd;
  Int_t     bd;
  Int_t     ch;
  Int_t     se;
  double    dt;
  Long64_t  dtI;
  Int_t     pc;
  Long64_t  tc;
  Int_t     pw;
};

struct Timdata {
  Int_t     fr;
  Int_t     nH;
  Long64_t  ut;
};

bool mycompare(Tmpdata  s1, Tmpdata  s2){
   return s1.dt < s2.dt;
}

const char VectorInputRootFilesDefault[200] = 
  "/data4/AndyLu/to_cw/4_4_4_starightbeam_merge.root"
;
const char OuputRootFilePath      [200] = 
  "/home/yusiang/G4/4_4_4HorizontaBeamTest/Data/"
;
const char OuputRootFileName      [200] = 
  "4_4_4HorizontalBeamTest.root"
;

void CaConvertor(
  const char vRootFiles[200]= VectorInputRootFilesDefault,
  const char ofilePath [200]= OuputRootFilePath,
  const char ofileName [200]= OuputRootFileName
) {
  
  system(Form("mkdir -p %s",ofilePath));
  
  int eventGap = MaximumEventGapOfTcnt;
  ifstream infile;
  cout<<330<<vRootFiles<<endl;
  infile.open(vRootFiles);
  TreeReader data(vRootFiles,"mu_detector");
  cout<<333<<endl;
  Int_t            frame_= 0 ;
  Long64_t         unixtime_;
  Int_t            tYear_;
  Int_t            tMonth_;
  Int_t            tDate_;
  Int_t            tHour_;
  Int_t            tMinute_;
  Int_t            tSecond_;
  Int_t            nHits_;
  
  Int_t            gid;
  Int_t            board;
  Int_t            channel;
  Int_t            pwidth;
  
  vector<Int_t>      gid_;
  vector<Int_t>      board_;
  vector<Int_t>      channel_;
  vector<Int_t>      seq_;
  vector<Int_t>      pcnt_;
  vector<Long64_t>   tcnt_;
  vector<double>     dtime_;
  vector<Long64_t>   dtimeInt_;
  vector<Int_t>      pwidth_;

  TFile *f = new TFile(Form("%s%s",ofilePath,ofileName), "recreate");

  TTree *t = new TTree("t", "frame data");
  t->Branch("frame",    &frame_);     
  t->Branch("unixtime", &unixtime_);
  t->Branch("tYear",    &tYear_);
  t->Branch("tMonth",   &tMonth_);
  t->Branch("tDate",    &tDate_);
  t->Branch("tHour",    &tHour_);
  t->Branch("tMinute",  &tMinute_);
  t->Branch("tSecond",  &tSecond_);
  t->Branch("nHits",    &nHits_);
  t->Branch("gid",      &gid_);
  t->Branch("board",    &board_);
  t->Branch("channel",  &channel_);
  t->Branch("seq",      &seq_);
  t->Branch("dtime",    &dtime_);
  t->Branch("dtimeInt", &dtimeInt_);
  t->Branch("pcnt",     &pcnt_);
  t->Branch("tcnt",     &tcnt_);
  t->Branch("pwidth",   &pwidth_);     

  nHits_ = 0;
  vector<struct Tmpdata> data1;
  vector<struct Timdata> time1;
  int evs = data.GetEntriesFast();
  Int_t frame=0, frame0=0;
  data.GetEntry(0);
  frame    = data.GetInt("eID");
  frame0 = frame;
  for (Long64_t ev = 0; ev < evs ; ++ev) {//@@@
    data.GetEntry(ev);
    
    
    frame    = data.GetInt("eID");
    gid      = G4GIDToGID[int(data.GetDouble("channel"))];
    pwidth   = data.GetDouble("Edep")*6.55;
    
    
    board   = int(gid/16)+1;
    int chtmp = (int(gid%16)/4+(int(gid%16)%4)*4);
    channel = chtmp;
    // cout<<channel<<endl;

    int       pcnt = (frame/1100);
    Long64_t tcnt = (rand()%eventGap);
    Long64_t dtimeInt = pcnt*2.56E+9+tcnt;
    double   dtime = pcnt*1.+(1.*tcnt)/2.56E+9;
    int      seq = 1;//MC default == 1
    nHits_++;
    
    unixtime_ = 1600000001+pcnt;
    if(ev%1000 == 0) cout<<Form("\r%.5f%%\t%d",(ev*100.)/(1.*evs),unixtime_)<<flush;
    
    Tmpdata dataT = {gid,board,channel,seq,dtime,dtimeInt,pcnt,tcnt,pwidth};
    data1.push_back(dataT);
    
    if(frame!=frame0){
      Timdata Tim_tmp = {frame,nHits_,unixtime_};
      time1.push_back(Tim_tmp);
      if(time1.size()>1){
        time1[time1.size()-2].nH = nHits_;
      }
      nHits_=0;
      frame0 = frame;  
    }
    
  }
  time1[time1.size()-1].nH = nHits_;
  sort(data1.begin(), data1.end(), mycompare);
  cout<<"size:  "<<int(data1.size());


  int totalfr = int(time1.size());
  cout<<"\tNframe:  "<<totalfr<<endl;
  int iHits=0;
  cout<<time1[0].fr<<"\t"<<time1[0].nH<<"\t"<<time1[0].ut<<"\n";
  for(int ifr=0;ifr<totalfr;ifr++){
    frame_    = time1[ifr].fr;
    nHits_    = time1[ifr].nH;
    unixtime_ = time1[ifr].ut;
    unixTimeToHumanReadable(unixtime_,
        tYear_, tMonth_, tDate_, tHour_, tMinute_, tSecond_,timeZone);
    for(int i1=0;i1<nHits_;i1++){
      
      gid_     .push_back(data1[iHits].gd);
      board_   .push_back(data1[iHits].bd);
      channel_ .push_back(data1[iHits].ch);
      seq_     .push_back(data1[iHits].se);
      dtime_   .push_back(data1[iHits].dt);
      dtimeInt_.push_back(data1[iHits].dtI);
      pcnt_    .push_back(data1[iHits].pc);
      tcnt_    .push_back(data1[iHits].tc);
      pwidth_  .push_back(data1[iHits].pw);
      iHits++;
      //cout<<dtimeInt_[i1]<<endl;
    }
    if(nHits_!=0) t->Fill();

    gid_.clear();
    board_.clear();
    channel_.clear();
    seq_.clear();
    dtime_.clear();
    dtimeInt_.clear();
    pcnt_.clear();
    tcnt_.clear();
    pwidth_.clear();
    
  }



  // t->Fill();
  f->Write();
  f->Close();
  
}

void G4CaConvertor() {
  CaConvertor( VectorInputRootFilesDefault , OuputRootFilePath , OuputRootFileName);
  cout<<"Finish convert the G4 raw data:  "<<OuputRootFilePath<<endl;
  cout<<"The G4 MonteCarlo data save as:  "<<OuputRootFilePath<<OuputRootFileName<<endl;
  
}