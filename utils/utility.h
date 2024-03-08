struct TObjects
{
  std::map<std::string, TH1F *> _TH1F;
  std::map<std::string, TH1D *> _TH1D;
  std::map<std::string, TH2F *> _TH2F;
  std::map<std::string, TH2I *> _TH2I;
  std::map<std::string, TH3F *> _TH3F;
  std::map<std::string, TProfile *> _TProfile;
  std::map<std::string, TGraph *> _TGraph;
  std::map<std::string, TGraphErrors *> _TGraphErrors;
};

void write_all(TObjects target)
{
  for (auto [label, object] : target._TProfile)
    object->Write(label.c_str());
  for (auto [label, object] : target._TH1F)
    object->Write(label.c_str());
  for (auto [label, object] : target._TH1D)
    object->Write(label.c_str());
  for (auto [label, object] : target._TH2F)
    object->Write(label.c_str());
  for (auto [label, object] : target._TH2I)
    object->Write(label.c_str());
  for (auto [label, object] : target._TH3F)
    object->Write(label.c_str());
  for (auto [label, object] : target._TGraph)
    object->Write(label.c_str());
  for (auto [label, object] : target._TGraphErrors)
    object->Write(label.c_str());
}

template<bool logx = true, bool logy = true>
TCanvas *standard_canvas(std::string name, std::string title, float nXpixels = 1000, float nYpixels = 1000)
{
  TCanvas *cResult = new TCanvas(name.c_str(), title.c_str(), nXpixels, nYpixels);
  gStyle->SetOptStat(0);
  gPad->SetGridx(logx);
  gPad->SetGridy(logy);
  gPad->SetRightMargin(0.05);
  gPad->SetTopMargin(0.05);
  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.15);
  return cResult;
}

template<bool logx = true, bool logy = true>
TCanvas *standard_canvas_2D(std::string name, std::string title, float nXpixels = 1145, float nYpixels = 1000)
{
  TCanvas *cResult = new TCanvas(name.c_str(), title.c_str(), nXpixels, nYpixels);
  gStyle->SetOptStat(0);
  gPad->SetGridx(logx);
  gPad->SetGridy(logy);
  gPad->SetRightMargin(0.15);
  gPad->SetTopMargin(0.05);
  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.15);
  return cResult;
}
