#include "../source/database.C"

void plot_IV();
void plot_DCR();
void plot_gain();
void plot_ThScan();

void IV_plots_for_paper(std::string data_dir = "./Data/", std::string outfile = "out.root", std::string channel = "A1")
{
  //  Load database
  database::basedir = data_dir;
  database::read_database(database::basedir + "/database.txt");

  plot_IV();
  plot_DCR();
  plot_gain();
  plot_ThScan();
}

void plot_gain()
{
  //  States required
  std::vector<std::string> list_of_tifpa_states = {"NEW", "TIFPA-IRR1", "OANN-150-150h"};
  std::vector<std::string> list_of_lnl_states = {"NEW", "LNL-IRR1", "OANN-150-150h"};
  std::map<std::string, std::string> state_to_legend = {{"NEW", "New"}, {"TIFPA-IRR1", "#it{p}-irradiated at 10^{9} 1-MeV n_{eq}"}, {"LNL-IRR1", "#it{n}-irradiated at 10^{9} 1-MeV n_{eq}"}, {"OANN-150-150h", "Annealed 150h at 150C in oven"}};

  //  Output requested
  auto cDrawResults = new TCanvas("cDrawResults", "cDrawResults", 2500, 1000);
  cDrawResults->Divide(2, 1);

  //  TIFPA results
  cDrawResults->cd(1);
  gPad->SetLeftMargin(0.15);
  cDrawResults->cd(1)->DrawFrame(45, 5.e5, 65, 5.e8, ";bias voltage (V);Gain (a.u.)");
  cDrawResults->cd(1)->SetLogy();

  //  Legend
  auto lDrawResults_tifpa = new TLegend(0.42, 0.75, 0.85, 0.9);
  lDrawResults_tifpa->SetFillColorAlpha(0., 0.);
  lDrawResults_tifpa->SetLineColorAlpha(0., 0.);

  //  Loop to plot
  auto iTer = -1;
  for (auto current_state : list_of_tifpa_states)
  {
    iTer++;
    auto current_graph = database::get_gain("2", "A1", current_state, 20, database::status_to_color[current_state]);
    current_graph->SetLineWidth(2);
    current_graph->Draw("samelp");
    lDrawResults_tifpa->AddEntry(current_graph, state_to_legend[current_state].c_str(), "P");
  }
  lDrawResults_tifpa->Draw("same");

  //  LNL results
  cDrawResults->cd(2);
  gPad->SetLeftMargin(0.15);
  cDrawResults->cd(2)->DrawFrame(45, 5.e5, 65, 5.e8, ";bias voltage (V);Gain (a.u.)");
  cDrawResults->cd(2)->SetLogy();

  //  Legend
  auto lDrawResults_lnl = new TLegend(0.42, 0.75, 0.85, 0.9);
  lDrawResults_lnl->SetFillColorAlpha(0., 0.);
  lDrawResults_lnl->SetLineColorAlpha(0., 0.);

  //  Loop to plot
  iTer = -1;
  for (auto current_state : list_of_lnl_states)
  {
    iTer++;
    auto current_graph = database::get_gain("25", "A1", current_state, 20, database::status_to_color[current_state]);
    current_graph->SetLineWidth(2);
    current_graph->Draw("samelp");
    lDrawResults_lnl->AddEntry(current_graph, state_to_legend[current_state].c_str(), "P");
  }
  lDrawResults_lnl->Draw("same");

  //  Save canvas
  cDrawResults->SaveAs("figures/plot_Gain.pdf");
  cDrawResults->SaveAs("figures/plot_Gain.png");
}

void plot_DCR()
{
  //  States required
  std::vector<std::string> list_of_tifpa_states = {"NEW", "TIFPA-IRR1", "OANN-150-150h"};
  std::vector<std::string> list_of_lnl_states = {"NEW", "LNL-IRR1", "OANN-150-150h"};
  std::map<std::string, std::string> state_to_legend = {{"NEW", "New"}, {"TIFPA-IRR1", "#it{p}-irradiated at 10^{9} 1-MeV n_{eq}"}, {"LNL-IRR1", "#it{n}-irradiated at 10^{9} 1-MeV n_{eq}"}, {"OANN-150-150h", "Annealed 150h at 150C in oven"}};

  //  Output requested
  auto cDrawResults = new TCanvas("cDrawResults", "cDrawResults", 2500, 1000);
  cDrawResults->Divide(2, 1);

  //  TIFPA results
  cDrawResults->cd(1);
  gPad->SetLeftMargin(0.15);
  cDrawResults->cd(1)->DrawFrame(45, 1.e2, 65, 1.e8, ";bias voltage (V);DCR (Hz)");
  cDrawResults->cd(1)->SetLogy();

  //  Legend
  auto lDrawResults_tifpa = new TLegend(0.17, 0.75, 0.6, 0.9);
  lDrawResults_tifpa->SetFillColorAlpha(0., 0.);
  lDrawResults_tifpa->SetLineColorAlpha(0., 0.);

  //  Loop to plot
  auto iTer = -1;
  for (auto current_state : list_of_tifpa_states)
  {
    iTer++;
    auto current_graph = database::get_dcr_vbias_scan("2", "A1", current_state, 20, database::status_to_color[current_state]);
    current_graph->SetLineWidth(2);
    current_graph->Draw("samelp");
    lDrawResults_tifpa->AddEntry(current_graph, state_to_legend[current_state].c_str(), "P");
  }
  lDrawResults_tifpa->Draw("same");

  //  LNL results
  cDrawResults->cd(2);
  gPad->SetLeftMargin(0.15);
  cDrawResults->cd(2)->DrawFrame(45, 1.e2, 65, 1.e8, ";bias voltage (V);DCR (Hz)");
  cDrawResults->cd(2)->SetLogy();

  //  Legend
  auto lDrawResults_lnl = new TLegend(0.17, 0.75, 0.6, 0.9);
  lDrawResults_lnl->SetFillColorAlpha(0., 0.);
  lDrawResults_lnl->SetLineColorAlpha(0., 0.);

  //  Loop to plot
  iTer = -1;
  for (auto current_state : list_of_lnl_states)
  {
    iTer++;
    auto current_graph = database::get_dcr_vbias_scan("25", "A1", current_state, 20, database::status_to_color[current_state]);
    current_graph->SetLineWidth(2);
    current_graph->Draw("samelp");
    lDrawResults_lnl->AddEntry(current_graph, state_to_legend[current_state].c_str(), "P");
  }
  lDrawResults_lnl->Draw("same");

  //  Save canvas
  cDrawResults->SaveAs("figures/plot_DCR.pdf");
  cDrawResults->SaveAs("figures/plot_DCR.png");
}

void plot_IV()
{
  //  States required
  std::vector<std::string> list_of_tifpa_states = {"NEW", "TIFPA-IRR1", "OANN-150-150h"};
  std::vector<std::string> list_of_lnl_states = {"NEW", "LNL-IRR1", "OANN-150-150h"};
  std::map<std::string, std::string> state_to_legend = {{"NEW", "New"}, {"TIFPA-IRR1", "#it{p}-irradiated at 10^{9} 1-MeV n_{eq}"}, {"LNL-IRR1", "#it{n}-irradiated at 10^{9} 1-MeV n_{eq}"}, {"OANN-150-150h", "Annealed 150h at 150C in oven"}};

  //  Output requested
  auto cDrawResults = new TCanvas("cDrawResults", "cDrawResults", 2500, 1000);
  cDrawResults->Divide(2, 1);

  //  TIFPA results
  cDrawResults->cd(1);
  gPad->SetLeftMargin(0.15);
  cDrawResults->cd(1)->DrawFrame(45, 2.e-11, 65, 2.e-4, ";bias voltage (V);current (A)");
  cDrawResults->cd(1)->SetLogy();

  //  Legend
  auto lDrawResults_tifpa = new TLegend(0.17, 0.75, 0.6, 0.9);
  lDrawResults_tifpa->SetFillColorAlpha(0., 0.);
  lDrawResults_tifpa->SetLineColorAlpha(0., 0.);

  //  Loop to plot
  auto iTer = -1;
  for (auto current_state : list_of_tifpa_states)
  {
    iTer++;
    auto current_graph = database::get_iv_scan("2", "A1", current_state, 20, database::status_to_color[current_state]);
    current_graph->SetLineWidth(2);
    current_graph->Draw("samelp");
    lDrawResults_tifpa->AddEntry(current_graph, state_to_legend[current_state].c_str(), "P");
  }
  lDrawResults_tifpa->Draw("same");

  //  LNL results
  cDrawResults->cd(2);
  gPad->SetLeftMargin(0.15);
  cDrawResults->cd(2)->DrawFrame(45, 2.e-11, 65, 2.e-4, ";bias voltage (V);current (A)");
  cDrawResults->cd(2)->SetLogy();

  //  Legend
  auto lDrawResults_lnl = new TLegend(0.17, 0.75, 0.6, 0.9);
  lDrawResults_lnl->SetFillColorAlpha(0., 0.);
  lDrawResults_lnl->SetLineColorAlpha(0., 0.);

  //  Loop to plot
  iTer = -1;
  for (auto current_state : list_of_lnl_states)
  {
    iTer++;
    auto current_graph = database::get_iv_scan("25", "A1", current_state, 20, database::status_to_color[current_state]);
    current_graph->SetLineWidth(2);
    current_graph->Draw("samelp");
    lDrawResults_lnl->AddEntry(current_graph, state_to_legend[current_state].c_str(), "P");
  }
  lDrawResults_lnl->Draw("same");

  //  Save canvas
  cDrawResults->SaveAs("figures/plot_IV.pdf");
  cDrawResults->SaveAs("figures/plot_IV.png");
}

void plot_ThScan()
{
  //  States required
  std::vector<std::string> list_of_tifpa_states = {"NEW", "TIFPA-IRR1", "OANN-150-150h"};
  std::vector<std::string> list_of_lnl_states = {"NEW", "LNL-IRR1", "OANN-150-150h"};
  std::map<std::string, std::string> state_to_legend = {{"NEW", "New"}, {"TIFPA-IRR1", "#it{p}-irradiated at 10^{9} 1-MeV n_{eq}"}, {"LNL-IRR1", "#it{n}-irradiated at 10^{9} 1-MeV n_{eq}"}, {"OANN-150-150h", "Annealed 150h at 150C in oven"}};

  //  Output requested
  auto cDrawResults = new TCanvas("cDrawResults", "cDrawResults", 2500, 1000);
  cDrawResults->Divide(2, 1);

  //  TIFPA results
  cDrawResults->cd(1);
  gPad->SetLeftMargin(0.15);
  cDrawResults->cd(1)->DrawFrame(0, 1.e0, 65, 1.e8, ";Threshold (a.u.);DCR (Hz)");
  cDrawResults->cd(1)->SetLogy();

  //  Legend
  auto lDrawResults_tifpa = new TLegend(0.17, 0.75, 0.6, 0.9);
  lDrawResults_tifpa->SetFillColorAlpha(0., 0.);
  lDrawResults_tifpa->SetLineColorAlpha(0., 0.);

  //  Loop to plot
  auto iTer = -1;
  for (auto current_state : list_of_tifpa_states)
  {
    iTer++;
    auto current_graph = database::get_dcr_threshold_scan("2", "A1", current_state, 20, database::status_to_color[current_state]);
    current_graph->SetLineWidth(2);
    current_graph->Draw("samelp");
    lDrawResults_tifpa->AddEntry(current_graph, state_to_legend[current_state].c_str(), "P");
  }
  lDrawResults_tifpa->Draw("same");

  //  LNL results
  cDrawResults->cd(2);
  gPad->SetLeftMargin(0.15);
  cDrawResults->cd(2)->DrawFrame(0, 1.e0, 65, 1.e8, ";Threshold (a.u.);DCR (Hz)");
  cDrawResults->cd(2)->SetLogy();

  //  Legend
  auto lDrawResults_lnl = new TLegend(0.17, 0.75, 0.6, 0.9);
  lDrawResults_lnl->SetFillColorAlpha(0., 0.);
  lDrawResults_lnl->SetLineColorAlpha(0., 0.);

  //  Loop to plot
  iTer = -1;
  for (auto current_state : list_of_lnl_states)
  {
    iTer++;
    auto current_graph = database::get_dcr_threshold_scan("25", "A1", current_state, 20, database::status_to_color[current_state]);
    current_graph->SetLineWidth(2);
    current_graph->Draw("samelp");
    lDrawResults_lnl->AddEntry(current_graph, state_to_legend[current_state].c_str(), "P");
  }
  lDrawResults_lnl->Draw("same");

  //  Save canvas
  cDrawResults->SaveAs("figures/plot_ThScan.pdf");
  cDrawResults->SaveAs("figures/plot_ThScan.png");

  for (auto current_state : list_of_tifpa_states)
    database::get_cross_talk("2", "A1", current_state, Form("figures/plot_ThScan_check_%i_%s", 2, current_state.c_str()));
  for (auto current_state : list_of_lnl_states)
    database::get_cross_talk("25", "A1", current_state, Form("figures/plot_ThScan_check_%i_%s", 25, current_state.c_str()));
  for (auto current_state : list_of_tifpa_states)
    database::get_sig_amp("2", "A1", current_state, Form("figures/plot_ThScanSigAmp_check_%i_%s", 2, current_state.c_str()));

  //  Test for calculations
  auto graphs_lnl_irr1_postirrd = database::get_general_TGraphs_iv(database::sensors_full[0],
                                                                   {{"LNL-IRR1", {"22"}}, {"SANN-175-10m", {"22"}}, {"SANN-175-10m-2", {"22"}}, {"SANN-175-10m-3", {"22"}}},
                                                                   4., database::status_to_marker["LNL-IRR1"], database::status_to_color["LNL-IRR1"]);
  TCanvas *c1 = new TCanvas();
  std::vector<std::array<float, 2>> x_axis_lnl_irr = {{0., 0.}, {10. / 60., 0.}, {20. / 60., 0.}, {30. / 60., 0.}};
  TF1 *ftest = new TF1("ftest", "[0]*TMath::Exp(-[1]*x)*(TMath::Exp(-[2]*x)-1)");
  ftest->SetParameters(1., 1., 1.);
  graphutils::reassign_x_values(graphs_lnl_irr1_postirrd[0], x_axis_lnl_irr);
  graphutils::reassign_x_values(graphs_lnl_irr1_postirrd[1], x_axis_lnl_irr);
  graphs_lnl_irr1_postirrd[0]->Fit(ftest);
  graphs_lnl_irr1_postirrd[0]->Draw("ALP");
}