#include "../source/database.C"

void IV_plots_for_paper(std::string data_dir = "./Data/", std::string outfile = "out.root", std::string channel = "A1")
{
  //  Load database
  database::basedir = data_dir;
  database::read_database(database::basedir + "/database.txt");

/*
  //  States required
  std::vector<std::string> list_of_states = {"TEMP313", "TEMP303", "TEMP293", "TEMP283", "TEMP278", "TEMP273", "TEMP263", "TEMP253", "NEW"};
  std::vector<int> list_of_colors = {kRed + 2, kRed, kRed - 4, kRed - 7, kRed - 9, kBlue - 9, kBlue - 7, kBlue - 4, kBlue};

  //  Output requested
  TCanvas *cDrawResults = new TCanvas("cDrawResults", "cDrawResults", 1500, 1000);
  TLegend *lDrawResults = new TLegend(0.12, 0.70, 0.5, 0.9);
  lDrawResults->SetFillColorAlpha(0., 0.);
  lDrawResults->SetLineColorAlpha(0., 0.);
  lDrawResults->SetNColumns(3);
  cDrawResults->cd()->DrawFrame(45, 7.e-11, 65, 2.e-4, ";bias voltage (V);current (A)");
  cDrawResults->cd()->SetLogy();
  auto iTer = -1;
  for (auto current_state : list_of_states)
  {
    iTer++;
    auto current_graph = database::get_iv_scan("31", "A1", current_state, 20, list_of_colors[iTer]);
    current_graph->SetLineWidth(2);
    current_graph->SetMarkerSize(2);
    current_graph->Draw("samelp");
    auto current_temp = database::get_database_info("31", current_state, "temp");
    lDrawResults->AddEntry(current_graph, Form("%s K", current_temp.c_str()), "P");
  }
  lDrawResults->Draw("same");
  cDrawResults->SaveAs("IV_plot_temp.pdf");
  cDrawResults->SaveAs("IV_plot_temp.png");
  */

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
    current_graph->Draw("ALP");
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
  cDrawResults->SaveAs("IV_plot_raddmg.pdf");
}
