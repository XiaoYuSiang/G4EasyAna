#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <TPDF.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TGaxis.h>
#include <TText.h>
#include <TTree.h>
#include <TLatex.h>
#include <TStyle.h>
#include <TPaveStats.h>
#include <TGraphErrors.h>
#include "/data4/YuSiang/personalLib/Style/DBMTStyle.h"
#include "/data4/YuSiang/personalLib/RootFile/untuplizerv8_YuSiang.h"


TH2F *setTH2Model(const char* name, const char* title="title", const char* xtitle="X", const char* ytitle="Y", const char* ztitle="Z"){
  TH2F *th2fObj = new TH2F(name,title,40,0,40,8,0,8);
  th2fObj->GetXaxis()->SetTitle(xtitle);
  th2fObj->GetYaxis()->SetTitle(ytitle);
  th2fObj->GetXaxis()->CenterTitle(true);
  th2fObj->GetYaxis()->CenterTitle(true);
  th2fObj->GetXaxis()->CenterLabels(true);
  th2fObj->GetYaxis()->CenterLabels(true);
  th2fObj->GetZaxis()->SetTitle(ztitle);
  th2fObj->GetXaxis()->SetLabelSize(0.05);
  th2fObj->GetYaxis()->SetLabelSize(0.05);
  th2fObj->GetXaxis()->SetTitleSize(0.05);
  th2fObj->GetYaxis()->SetTitleSize(0.05);
  th2fObj->GetXaxis()->SetTitleOffset(1.0);
  th2fObj->GetYaxis()->SetTitleOffset(1.0);
  th2fObj->GetXaxis()->SetNdivisions(40);
  th2fObj->GetYaxis()->SetNdivisions(8);
  th2fObj->GetXaxis()->SetTickLength(1);
  th2fObj->GetYaxis()->SetTickLength(1);
  return th2fObj;
}

const char TargetRotFile[200] = 
  "/home/yusiang/G4/4_4_4ConvertTest/GapT300_MC_nHTH1L64.root"
;
const char SaveGraphName[200] = 
  "/home/yusiang/G4/4_4_4HorizontaBeamTest/test"
;


void BeamEasyStudy(
  const char FileSource[200]= TargetRotFile,
  const char SGraphPath[200]= SaveGraphName
){

  TFile *_file0 = TFile::Open(FileSource);
  TTree *t  = (TTree*) _file0 ->Get("t");
  gStyle-> SetStatX(0.08);
  gStyle-> SetStatY(0.8);
  gStyle-> SetStatW(0.08);
  gStyle-> SetStatH(0.5); 
  TCanvas *c1 = new TCanvas("c1","c1",1125,300*6);
  c1->Divide(1,6);
  
  TCanvas *cs = new TCanvas("cs","cs",1125,300);
  cs->Divide(1,2);
  
  cs->cd(1)->SetLogy(0);
  TH1F *H4 = new TH1F("H4","pwidth",100,0,100);
  t->Draw("pwidth>>H4","1","box");
  cs->cd(2)->SetLogy(1);
  H4->Draw("box");
  
  c1->cd(4);
  cs->DrawClonePad();
  cs->Close();
  
  gStyle-> SetStatX(0.08);
  gStyle-> SetStatY(0.3);
  gStyle-> SetStatW(0.08);
  gStyle-> SetStatH(0.2); 
  
  string LayVote
  = string("nH==4&&(")
  +   string("int(g4gid[0]/64)!=int(g4gid[1]/64)&&int(g4gid[0]/64)!=int(g4gid[2]/64)&&int(g4gid[0]/64)!=int(g4gid[3]/64)&&")
  +   string("int(g4gid[1]/64)!=int(g4gid[2]/64)&&int(g4gid[1]/64)!=int(g4gid[3]/64)&&")
  +   string("int(g4gid[2]/64)!=int(g4gid[3]/64)")
  + string(")");
  
  c1->cd(1);
  c1->cd(1)->SetLogz(1);
  TH2F *H1 = setTH2Model("H1","Fire position","iX+10*iZ","iY","Number");
  t->Draw("int((int(g4gid%64))/8):(int(g4gid%64))%8+10*int(g4gid/64)>>H1","1","colz");
  
  c1->cd(2);
  c1->cd(2)->SetLogz(1);
  TH2F *H2 = setTH2Model("H2","Fire position(nH==4)","iX+10*iZ","iY","Number");
  t->Draw("int((int(g4gid%64))/8):(int(g4gid%64))%8+10*int(g4gid/64)>>H2",Form("%s",LayVote.data()),"colz");
  
  c1->cd(3);
  c1->cd(3)->SetLogz(1);
  TH2F *H3 = setTH2Model("H3","Fire position(nH==4&&pwidth<10)","iX+10*iZ","iY","Number");
  t->Draw("int((int(g4gid%64))/8):(int(g4gid%64))%8+10*int(g4gid/64)>>H3",Form("%s&&pwidth<10",LayVote.data()),"colz");
  
  c1->cd(5);
  c1->cd(5)->SetLogz(1);
  TH2F *H5 = setTH2Model("H5","Fire position(pwidth>13)","iX+10*iZ","iY","Number");
  t->Draw("int((int(g4gid%64))/8):(int(g4gid%64))%8+10*int(g4gid/64)>>H5","pwidth>13","colz");
  
  c1->cd(6);
  c1->cd(6)->SetLogz(1);
  TH2F *H6 = setTH2Model("H6","Fire position(nH==4&&pwidth>13)","iX+10*iZ","iY","Number");
  t->Draw("int((int(g4gid%64))/8):(int(g4gid%64))%8+10*int(g4gid/64)>>H6",Form("%s&&pwidth>13",LayVote.data()),"colz");
  
  c1->Print("./All&4.png");
  c1->Print("./All&4.pdf");
  
  TCanvas *c2 = new TCanvas("c2","c2",1125,300*5);
  c2->Divide(1,5);
  
  c2->cd(1);
  c2->cd(1)->SetLogz(1);
  t->Draw("frame","1","colz");
  
  c2->cd(2);
  c2->cd(2)->SetLogz(1);
  t->Draw("frame",Form("%s",LayVote.data()),"colz");
  
  c2->cd(3);
  c2->cd(3)->SetLogz(1);
  t->Draw("frame",Form("%s&&pwidth<10",LayVote.data()),"colz");
  
  c2->cd(4);
  c2->cd(4)->SetLogz(1);
  t->Draw("frame","pwidth>13","colz");
  
  c2->cd(5);
  c2->cd(5)->SetLogz(1);
  t->Draw("frame",Form("%s&&pwidth>13",LayVote.data()),"colz");
  
  c2->Print(Form("%s.png",SGraphPath));
  c2->Print(Form("%s.pdf",SGraphPath));
  
}
