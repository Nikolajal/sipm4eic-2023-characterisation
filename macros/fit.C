void fit(std::string outfile_name = "out.root")
{
  TGraphErrors *gPlot = new TGraphErrors();
  gPlot->SetPoint(0, 0.45, 1.25 * 1000);
  gPlot->SetPointError(0, 0.02, 0.2 * 1.25 * 1000);
  gPlot->SetPoint(1, 1.57, 0.11 * 1000);
  gPlot->SetPointError(1, 0.02, 0.2 * 0.11 * 1000);

  TF1 *fDec = new TF1("fDec", "[0]*(1/(x*x))");

  gPlot->Fit(fDec);
  gPlot->Draw("ALP");
}
