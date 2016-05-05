#include "TCanvas.h"
#include "TH2.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TPaletteAxis.h"
#include "TGraphAsymmErrors.h"
#include "TLatex.h"

#define NOVAL_F -9999.0

TCanvas* custom_can_(TH1D* h, std::string canname, int gx = 0, int gy = 0,
		     int histosize_x = 500, int histosize_y = 500,
		     int mar_left = 80, int mar_right = 20, int mar_top = 20, int mar_bottom = 60, 
		     int title_align = 33, float title_x = 1.0, float title_y = 1.0,
		     bool log=false) {
  if (std::string(h->GetTitle()).size()>0&&title_y==1.0) mar_top += 20;
  int titlefontsize = 32;
  // int labelfontsize = 20;
  int yoffset_x = mar_left - titlefontsize - 4;
  int xoffset_y = mar_bottom - titlefontsize - 4;
  int zoffset_x = mar_right - titlefontsize - 4;
  int padsize_x = histosize_x + mar_left + mar_right;
  int padsize_y = histosize_y + mar_top + mar_bottom;
  int padsize = ((padsize_x<=padsize_y) ? padsize_x : padsize_y);
  float padratio_yx = (float)padsize_y/padsize_x > 1 ? 1 : (float)padsize_y/padsize_x;
  float padratio_xy = (float)padsize_x/padsize_y > 1 ? 1 : (float)padsize_x/padsize_y;
  Float_t xoffset = ((Float_t)xoffset_y/titlefontsize+0.5) * padratio_xy /1.6;
  Float_t yoffset = ((Float_t)yoffset_x/titlefontsize+0.5) * padratio_yx /1.6;
  Float_t zoffset = ((Float_t)zoffset_x/titlefontsize+0.5) * padratio_yx /1.6;
  Float_t titlesize = (Float_t)titlefontsize/padsize;
  // Float_t labelsize = (Float_t)labelfontsize/padsize;
  gStyle->SetOptStat(0);
  if (std::string(h->GetTitle()).size()) {
    gStyle->SetOptTitle(1);
    gStyle->SetTitleH(titlefontsize/padsize);
    gStyle->SetTitleFontSize(titlesize*0.8);
    gStyle->SetTitleBorderSize(0);
    gStyle->SetTitleAlign(title_align);
    gStyle->SetTitleX(title_x);
    gStyle->SetTitleY(title_y);
  }
  h->SetTitleFont(42,"xyz");
  h->SetLabelFont(42,"xyz");
  h->SetTitleSize(titlesize,"xyz");
  //h->SetLabelSize(labelsize,"xyz");
  h->GetXaxis()->SetTitleOffset(xoffset);
  h->GetYaxis()->SetTitleOffset(yoffset);
  h->GetZaxis()->SetTitleOffset(zoffset);
  h->GetYaxis()->SetDecimals(1);
  h->GetZaxis()->SetDecimals(1);
  TCanvas* canvas = new TCanvas(canname.c_str(), h->GetTitle(), padsize_x + 4, padsize_y + 26);
  TVirtualPad* pad = canvas->cd(1);
  pad->SetLeftMargin((Float_t)mar_left/padsize_x);
  pad->SetRightMargin((Float_t)mar_right/padsize_x);
  pad->SetTopMargin((Float_t)mar_top/padsize_y);
  pad->SetBottomMargin((Float_t)mar_bottom/padsize_y);
  canvas->SetGrid(gx,gy);
  if (log) canvas->SetLogy(1);
  return canvas;
}

TCanvas* custom_can_(TH2D* h, std::string canname, int gx = 0, int gy = 0,
		     int histosize_x = 500, int histosize_y = 500, 
		     int mar_left = 80, int mar_right = 120, int mar_top = 20, int mar_bottom = 60, 
		     int title_align = 33, float title_x = 1.0, float title_y = 1.0,
		     std::string draw="COLZ", bool norm=false, bool log=false) {
  if (std::string(h->GetTitle()).size()>0&&title_y==1.0) mar_top += 20;
  int titlefontsize = 32;
  int labelfontsize = 20;
  int pal_offset_x = 5;
  int pal_width_x = 25;
  int xoffset_y = mar_bottom - titlefontsize - 4;
  int yoffset_x = mar_left - titlefontsize - 4;
  int zoffset_x = mar_right - pal_offset_x - pal_width_x - titlefontsize;
  int padsize_x = histosize_x + mar_left + mar_right;
  int padsize_y = histosize_y + mar_top + mar_bottom;
  int padsize = ((padsize_x<=padsize_y) ? padsize_x : padsize_y);
  float padratio_yx = (Float_t)padsize_y/padsize_x > 1 ? 1 : (Float_t)padsize_y/padsize_x;
  float padratio_xy = (Float_t)padsize_x/padsize_y > 1 ? 1 : (Float_t)padsize_x/padsize_y;
  Float_t xoffset = ((Float_t)xoffset_y/titlefontsize+0.5) * padratio_xy /1.6;
  Float_t yoffset = ((Float_t)yoffset_x/titlefontsize+0.5) * padratio_yx /1.6;
  Float_t zoffset = ((Float_t)zoffset_x/titlefontsize+0.5) * padratio_yx /1.6;
  Float_t titlesize = (Float_t)titlefontsize/padsize;
  Float_t labelsize = (Float_t)labelfontsize/padsize;
  h->SetTitleFont(42,"xyz");
  h->SetLabelFont(42,"xyz");
  h->SetTitleSize(titlesize,"xyz");
  h->SetLabelSize(labelsize,"xyz");
  h->GetXaxis()->SetTitleOffset(xoffset);
  h->GetYaxis()->SetTitleOffset(yoffset);
  h->GetZaxis()->SetTitleOffset(zoffset);
  h->GetZaxis()->RotateTitle(1);
  h->GetYaxis()->SetDecimals(1);
  h->GetZaxis()->SetDecimals(1);
  if (histosize_y<250) h->GetZaxis()->SetNdivisions(505);
  if (std::string(h->GetTitle()).size()) {
    gStyle->SetOptTitle(1);
    gStyle->SetTitleH(titlefontsize/padsize);
    gStyle->SetTitleFontSize(titlesize*0.8);
    gStyle->SetTitleBorderSize(0);
    gStyle->SetTitleAlign(title_align);
    gStyle->SetTitleX(title_x);
    gStyle->SetTitleY(title_y);
  }
  TCanvas* canvas = new TCanvas(canname.c_str(), h->GetTitle(), padsize_x + 4, padsize_y + 26);
  TVirtualPad* pad = canvas->cd(1);
  pad->SetLeftMargin((Float_t)mar_left/padsize_x);
  pad->SetRightMargin((Float_t)mar_right/padsize_x);
  pad->SetTopMargin((Float_t)mar_top/padsize_y);
  pad->SetBottomMargin((Float_t)mar_bottom/padsize_y);
  canvas->SetGrid(gx,gy);
  if (norm&&h->Integral()>0) h = (TH2D*)h->DrawNormalized(draw.c_str());
  else h->Draw(draw.c_str());
  if (h->Integral()>0&&draw=="COLZ") {
    gPad->Update();
    TPaletteAxis* palette = (TPaletteAxis*)h->GetListOfFunctions()->FindObject("palette");
    if (palette) {
      palette->SetX1NDC(1 - (Float_t)(mar_right - pal_offset_x)/padsize_x);
      palette->SetX2NDC(1 - (Float_t)(mar_right - pal_offset_x - pal_width_x)/padsize_x);
      palette->SetY1NDC((Float_t)mar_bottom/padsize_y);
      palette->SetY2NDC(1 - (Float_t)mar_top/padsize_y);
    }
  }
  if (log) canvas->SetLogz(1);
  return canvas;
}

TGraphAsymmErrors* asym_(TH1D* h) {
  int n = 0;
  for (int i=0; i<h->GetNbinsX(); ++i) if (h->GetBinContent(i+1)>0) n++;
  TGraphAsymmErrors* tgae = new TGraphAsymmErrors(n);
  n = 0;
  for (int i=0; i<h->GetNbinsX(); ++i) {
    double y = h->GetBinContent(i+1);
    double x = h->GetBinCenter(i+1);
    if (y>0) {
      n++;
      tgae->SetPoint(n-1,x,y);
    }
  }
  // Calculate the asymmetric wilson score interval
  // double z = 1; // 1 Sigma confidence
  for (int bin=1; bin<=h->GetNbinsX(); ++bin) {
    // double p = h->GetBinContent(bin);
    double err = h->GetBinError(bin);
    tgae->SetPointEYhigh(bin-1,err);
    tgae->SetPointEYlow (bin-1,err);
  }
  // Set same ranges/settings etc
  double xlow = h->GetXaxis()->GetBinLowEdge(h->GetXaxis()->GetFirst());
  double xup  = h->GetXaxis()->GetBinUpEdge(h->GetXaxis()->GetLast());
  double ylow = h->GetMinimum();
  double yup  = h->GetMaximum();
  tgae->GetXaxis()->SetRangeUser(xlow,xup);
  tgae->GetYaxis()->SetRangeUser(ylow,yup);
  tgae->SetTitle(h->GetTitle());
  tgae->GetXaxis()->SetTitle(h->GetXaxis()->GetTitle());
  tgae->GetYaxis()->SetTitle(h->GetYaxis()->GetTitle());
  tgae->GetXaxis()->SetTitleSize(h->GetXaxis()->GetTitleSize());
  tgae->GetYaxis()->SetTitleSize(h->GetYaxis()->GetTitleSize());
  tgae->GetXaxis()->SetTitleOffset(h->GetXaxis()->GetTitleOffset());
  tgae->GetYaxis()->SetTitleOffset(h->GetYaxis()->GetTitleOffset());
  tgae->GetXaxis()->SetTitleFont(h->GetXaxis()->GetTitleFont());
  tgae->GetYaxis()->SetTitleFont(h->GetYaxis()->GetTitleFont());
  tgae->GetXaxis()->SetNdivisions(h->GetNdivisions("X"));
  tgae->GetYaxis()->SetNdivisions(h->GetNdivisions("Y"));
  tgae->GetYaxis()->SetDecimals(1);
  tgae->SetMarkerStyle(h->GetMarkerStyle());
  tgae->SetMarkerColor(h->GetMarkerColor());
  tgae->SetMarkerSize(h->GetMarkerSize());
  return tgae;
}

void asym_labels_(TH1D *orig, TGraphAsymmErrors* tgae, int hor_vert_up_down = 1) {
  int angle = hor_vert_up_down==0 ? 0 : hor_vert_up_down==1 ? 90 : hor_vert_up_down==2 ? 20 : -20 ;
  int align = hor_vert_up_down==0 ? 23 : hor_vert_up_down==1 ? 32 : hor_vert_up_down==2 ? 33 : 13;
  if (orig->GetNbinsX()>0) tgae->GetXaxis()->SetLabelColor(0);
  double labelsize = orig->GetXaxis()->GetLabelSize();
  double offset = (gPad->GetY2()-gPad->GetY1()) * orig->GetXaxis()->GetLabelOffset()*0.16;
  for (size_t i=0; i<(size_t)(orig->GetNbinsX()); ++i) {
    TLatex *lat = new TLatex(orig->GetBinCenter(i+1), orig->GetMinimum()-offset, orig->GetXaxis()->GetBinLabel(i+1));
    //std::cout<<i+1<<" "<<orig->GetXaxis()->GetBinLabel(i+1)<<" x "<<orig->GetBinCenter(i+1)<<" y "<<orig->GetMinimum()-offset<<std::endl;
    lat->SetTextAlign(align);
    lat->SetTextAngle(angle);
    lat->SetTextFont(orig->GetXaxis()->GetLabelFont());
    lat->SetTextSize(labelsize*0.65);
    lat->Draw();
  }
}

void draw_with_right_yaxis_(TH1D* hleft, TH1D* hright, std::string opt, std::string ytitle_right,
      		      Double_t xmin = NOVAL_F, Double_t xmax = NOVAL_F,
      		      Double_t left_ymin = NOVAL_F, Double_t left_ymax = NOVAL_F,
      		      Double_t right_ymin = NOVAL_F, Double_t right_ymax = NOVAL_F,
      		      Double_t left_localymax_x = NOVAL_F, Double_t shift_maximum = 1.0) {
  if (hleft->GetEntries()>0&&hleft->GetMaximum()!=0&&hright->GetEntries()>0&&hright->GetMaximum()!=0) {
    // Set X axis range
    Int_t binx_min = hleft->GetXaxis()->FindBin(xmin);
    Int_t binx_max = hleft->GetXaxis()->FindBin(xmax);
    xmin = (xmin == NOVAL_F) ? hleft->GetXaxis()->GetXmin()
      : hleft->GetXaxis()->GetBinLowEdge(binx_min);
    xmax = xmax == NOVAL_F ? hleft->GetXaxis()->GetXmax()
      : hleft->GetXaxis()->GetBinUpEdge(binx_max);
    binx_min = hleft->GetXaxis()->FindBin(xmin);
    binx_max = hleft->GetXaxis()->FindBin(xmax);
    hleft->GetXaxis()->SetRange(binx_min,binx_max-1);
    hright->GetXaxis()->SetRange(binx_min,binx_max-1);
    // Set Styles and Draw First histogram with left y axis
    if (opt.find("P")!=std::string::npos) {
      hleft->SetMarkerStyle(21);
      hright->SetMarkerStyle(20);
      hright->SetMarkerColor(2);
    } else hright->SetLineColor(2);
    hleft->Draw(opt.c_str());
    // Rescale Right Histogram according to specified Right Y axis ranges
    // If ranges unspecified, will set histogram to same minimum/maximum
    // (On both X and right Y axis)
    Double_t left_maximum = left_localymax_x==NOVAL_F ? hleft->GetMaximum() :
      hleft->GetBinContent(hleft->GetXaxis()->FindBin(left_localymax_x));
    Double_t right_maximum = hright->GetMaximum();
    left_ymin = (left_ymin==NOVAL_F) ? hleft->GetMinimum() : left_ymin;
    left_ymax = (left_ymax==NOVAL_F) ? left_maximum *1.05 : left_ymax;
    hleft->GetYaxis()->SetRangeUser(left_ymin,left_ymax);
    // Find Minimum Plotted value
    Double_t left_minimum = left_maximum;
    Double_t right_minimum = right_maximum;
    for (int binx = binx_min; binx<binx_max; binx++) {
      Double_t value_left = hleft->GetBinContent(binx);
      if (value_left<left_minimum&&value_left>xmin) left_minimum = value_left;
      Double_t value_right = hright->GetBinContent(binx);
      if (value_right<right_maximum&&value_right>0) right_minimum = value_right;
    }
    // Rescale Right Histogram
    if (right_ymin==NOVAL_F&&right_ymax==NOVAL_F) {
      // No ranges specified 
      // Minimums, Maximums forced to same height (or shifted by factor)
      Double_t rescale_factor = (left_maximum - left_minimum) / (right_maximum - right_minimum) * shift_maximum;
      for (int binx = 1; binx<=hright->GetNbinsX(); binx++) {
        Double_t value = hright->GetBinContent(binx);
        if (value) hright->SetBinContent(binx,left_minimum + (value - right_minimum) * rescale_factor);
      }
      right_ymin = right_minimum - (left_minimum - left_ymin) / rescale_factor;
      right_ymax = right_minimum + (left_ymax - left_minimum) / rescale_factor;
    } else if (right_ymax==NOVAL_F) {
      // Right Y-axis low range Specified
      // Maximums forced to same height (or shifted by factor)
      Double_t rescale_factor = (left_maximum - left_ymin) / (right_maximum - right_ymin) * shift_maximum;
      for (int binx = 1; binx<=hright->GetNbinsX(); binx++) {
        Double_t value = hright->GetBinContent(binx);
        if (value) hright->SetBinContent(binx,left_ymin + (value - right_ymin) * rescale_factor);
      }
      right_ymax = right_ymin + (left_ymax-left_ymin) / rescale_factor;
    } else if (right_ymin==NOVAL_F) {
      // Right Y-axis upper range Specified 
      // Maximums forced to same height (or shifted by factor)
      Double_t rescale_factor = (left_ymax - left_maximum) / (right_ymax - right_maximum) / shift_maximum;
      for (int binx = 1; binx<=hright->GetNbinsX(); binx++) {
        Double_t value = hright->GetBinContent(binx);
        if (value) hright->SetBinContent(binx,left_ymax - (right_ymax - value) * rescale_factor);
      }
      right_ymin = right_ymax - (left_ymax-left_ymin) / rescale_factor;
    } else {
      // Right Y-axis ranges Specified (Manual mode)
      // Rescale accordingl, no shift possible
      Double_t rescale_factor = (left_ymax - left_ymin) / (right_ymax - right_ymin);
      for (int binx = 1; binx<=hright->GetNbinsX(); binx++) {
        Double_t value = hright->GetBinContent(binx);
        if (value) hright->SetBinContent(binx,left_ymin + (value - right_ymin) * rescale_factor);
      }
    }
    // Draw Rescaled histo and Paint Right axis
    hright->Draw("SAMEPE1X0");
    Double_t smallshift = hleft->GetBinWidth(binx_max)/1000;
    TGaxis* axis = new TGaxis(xmax+smallshift,left_ymin,xmax+smallshift,left_ymax,right_ymin,right_ymax,510,"+L");
    axis->SetDecimals(hleft->GetZaxis()->GetDecimals());
    axis->SetLabelFont(hleft->GetZaxis()->GetLabelFont());
    axis->SetLabelSize(hleft->GetZaxis()->GetLabelSize());
    axis->SetLabelColor(2);
    axis->SetLineColor(2);
    axis->SetTitleColor(2);
    axis->SetTitle(ytitle_right.c_str());
    axis->SetTitleFont(hleft->GetZaxis()->GetTitleFont());
    axis->SetTitleSize(hleft->GetZaxis()->GetTitleSize());
    axis->SetTitleOffset(hleft->GetZaxis()->GetTitleOffset());
    axis->Draw();
  }
}

void draw_lat_(double x, double y, const char* text, Int_t col, double size=0.04, double angle=0.0, short al=11) { 
  TLatex* l = new TLatex(x,y,text); l->SetTextColor(col); l->SetTextSize(size); l->SetTextAngle(angle); l->SetTextAlign(al); l->Draw(); 
}

void prelim_lat_(double xmin, double xmax, double ymin, double ymax, bool in) {
  std::string prelim = "CMS #scale[0.8]{#font[52]{Preliminary 2015}}";// #font[22]{Times bold} and #font[12]{Times Italic}
  if (in) { TLatex* cms_lat = new TLatex(xmin+(xmax-xmin)/20.0, ymax-(ymax-ymin)/10.0, prelim.c_str()); cms_lat->SetLineWidth(2); cms_lat->Draw(); }
  else { TLatex* cms_lat = new TLatex(xmin, ymax+(ymax-ymin)/40.0, prelim.c_str()); cms_lat->SetLineWidth(2); cms_lat->Draw(); }
}

void era_lat_(double xmin, double xmax, double ymin, double ymax) {
  std::string prelim = "#sqrt{s}=13 TeV (25ns)";// #font[22]{Times bold} and #font[12]{Times Italic}
  TLatex* era_lat = new TLatex(xmax, ymax+(ymax-ymin)/25.0, prelim.c_str()); era_lat->SetTextAlign(32); era_lat->SetTextSize(0.04); era_lat->SetTextFont(42); era_lat->SetLineWidth(2); era_lat->Draw();
}