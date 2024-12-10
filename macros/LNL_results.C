#include "../source/database.C"

void LNL_results(std::string outfile_name = "out.root")
{
  //  Load database
  database::basedir = "/Users/nrubini/Analysis/ePIC/memory/sipm4eic-2023-characterisation/Data/";
  database::read_database(database::basedir + "database.txt");

  //  General set-up
  TLatex *l_latex = new TLatex();
  std::vector<std::vector<TGraphErrors *>> memory_lnl_irr1;
  std::vector<std::vector<TGraphErrors *>> memory_lnl_irr2;

  //  --- --- --- Overcurrent
  //  --- --- --- --- Baseline sub. overcurrent at 4 ov
  //  Frame
  TH1F *h_frame_ov_fluences = new TH1F("h_frame", ";Irradiation level (1-MeV n_{eq});Damage current @ 4V ov. (A)", 1, 5.e7, 3.e10);
  h_frame_ov_fluences->GetXaxis()->SetTitleOffset(1.5);
  h_frame_ov_fluences->SetMinimum(5.e-10);
  h_frame_ov_fluences->SetMaximum(1.e-4);
  auto i_sensor = -1;
  for (auto current_sensor : database::sensors_full)
  {
    i_sensor++;

    //  Canvas set-up
    TCanvas *LNL_ov_results = new TCanvas("", "", 800, 800);
    gStyle->SetOptStat(0);
    gPad->SetLogy();
    gPad->SetLogx();
    gPad->SetMargin(0.15, 0.05, 0.15, 0.05);
    h_frame_ov_fluences->Draw();

    //  Generate TGraphErrors
    std::vector<std::array<float, 2>> x_axis_lnl_irr = {{1.e8, 0}, {1.e9, 0}, {5.e9, 0}, {1.e10, 0}};
    auto graphs_lnl_irr1_postirrd = database::get_general_TGraphs_iv(current_sensor, {{"LNL-IRR1", {"26"}}, {"LNL-IRR1", {"21", "22", "23", "24", "25"}}, {"LNL-IRR1", {"20"}}, {"LNL-IRR1", {"19"}}}, 4., database::status_to_marker["LNL-IRR1"], database::status_to_color["LNL-IRR1"]);
    auto graphs_lnl_irr1_baseline = database::get_general_TGraphs_iv(current_sensor, {{"NEW", {"26"}}, {"NEW", {"21", "22", "23", "24", "25"}}, {"NEW", {"20"}}, {"NEW", {"19"}}}, 4., database::status_to_marker["LNL-IRR1"], database::status_to_color["LNL-IRR1"]);
    std::vector<TGraphErrors *> graphs_lnl_irr1 = {graphutils::diff(graphs_lnl_irr1_postirrd[0], graphs_lnl_irr1_baseline[0]), graphutils::diff(graphs_lnl_irr1_postirrd[1], graphs_lnl_irr1_baseline[1])};
    memory_lnl_irr1.push_back(graphs_lnl_irr1);
    graphutils::reassign_x_values(graphs_lnl_irr1[0], x_axis_lnl_irr);
    graphutils::reassign_x_values(graphs_lnl_irr1[1], x_axis_lnl_irr);
    graphs_lnl_irr1[0]->SetMarkerSize(2);
    graphs_lnl_irr1[1]->SetMarkerSize(2);
    graphs_lnl_irr1[0]->Draw("SAME EP");
    graphs_lnl_irr1[1]->Draw("SAME E[]P");

    auto graphs_lnl_irr2_postirrd = database::get_general_TGraphs_iv(current_sensor, {{"LNL-IRR2", {"26"}}, {"LNL-IRR2", {"25"}}, {"LNL-IRR2", {"20"}}, {"LNL-IRR2", {"19"}}}, 4., database::status_to_marker["LNL-IRR2"], database::status_to_color["LNL-IRR2"]);
    auto graphs_lnl_irr2_baseline = database::get_general_TGraphs_iv(current_sensor, {{"OANN-150-450h", {"26"}}, {"OANN-150-450h", {"25"}}, {"OANN-150-450h-2", {"20"}}, {"OANN-150-450h-2", {"19"}}}, 4., database::status_to_marker["LNL-IRR2"], database::status_to_color["LNL-IRR2"]);
    std::vector<TGraphErrors *> graphs_lnl_irr2 = {graphutils::diff(graphs_lnl_irr2_postirrd[0], graphs_lnl_irr2_baseline[0]), graphutils::diff(graphs_lnl_irr2_postirrd[1], graphs_lnl_irr2_baseline[1])};
    memory_lnl_irr2.push_back(graphs_lnl_irr2);
    graphutils::reassign_x_values(graphs_lnl_irr2[0], x_axis_lnl_irr);
    graphutils::reassign_x_values(graphs_lnl_irr2[1], x_axis_lnl_irr);
    graphs_lnl_irr2[0]->SetMarkerSize(2);
    graphs_lnl_irr2[1]->SetMarkerSize(2);
    //graphs_lnl_irr2[0]->Draw("SAME EP");
    //graphs_lnl_irr2[1]->Draw("SAME E[]P");

    std::vector<std::array<float, 2>> x_axis_tifpa_irr1 = {{1.e9, 0}};
    auto graphs_tifpa_irr1_postirrd = database::get_general_TGraphs_iv(current_sensor, {{"TIFPA-IRR1", {"1", "2", "13", "14", "15", "16"}}}, 4., database::status_to_marker["TIFPA-IRR1"], database::status_to_color["TIFPA-IRR1"]);
    auto graphs_tifpa_irr1_baseline = database::get_general_TGraphs_iv(current_sensor, {{"NEW", {"1", "2", "13", "14", "15", "16"}}}, 4., database::status_to_marker["TIFPA-IRR1"], database::status_to_color["TIFPA-IRR1"]);
    std::vector<TGraphErrors *> graphs_tifpa_irr1 = {graphutils::diff(graphs_tifpa_irr1_postirrd[0], graphs_tifpa_irr1_baseline[0]), graphutils::diff(graphs_tifpa_irr1_postirrd[1], graphs_tifpa_irr1_baseline[1])};
    graphutils::reassign_x_values(graphs_tifpa_irr1[0], x_axis_tifpa_irr1);
    graphutils::reassign_x_values(graphs_tifpa_irr1[1], x_axis_tifpa_irr1);
    graphs_tifpa_irr1[0]->SetMarkerSize(2);
    graphs_tifpa_irr1[1]->SetMarkerSize(2);
    graphs_tifpa_irr1[0]->Draw("SAME EP");
    graphs_tifpa_irr1[1]->Draw("SAME E[]P");

    auto graphs_lnl_irr1_ann_postirrd = database::get_general_TGraphs_iv(current_sensor, {{"OANN-150-150h", {"26"}}, {"OANN-150-150h", {"25"}}, {"OANN-150-150h", {"20"}}, {"OANN-150-150h", {"19"}}}, 4., database::status_to_marker["LNL-IRR1"], database::status_to_color["OANN-150-150h"]);
    auto graphs_lnl_irr1_ann_baseline = database::get_general_TGraphs_iv(current_sensor, {{"NEW", {"26"}}, {"NEW", {"25"}}, {"NEW", {"20"}}, {"NEW", {"19"}}}, 4., database::status_to_marker["LNL-IRR1"], database::status_to_color["LNL-IRR1"]);
    std::vector<TGraphErrors *> graphs_lnl_irr1_ann = {graphutils::diff(graphs_lnl_irr1_ann_postirrd[0], graphs_lnl_irr1_ann_baseline[0]), graphutils::diff(graphs_lnl_irr1_ann_postirrd[1], graphs_lnl_irr1_ann_baseline[1])};
    graphutils::reassign_x_values(graphs_lnl_irr1_ann[0], x_axis_lnl_irr);
    graphutils::reassign_x_values(graphs_lnl_irr1_ann[1], x_axis_lnl_irr);
    graphs_lnl_irr1_ann[0]->SetMarkerSize(2);
    graphs_lnl_irr1_ann[1]->SetMarkerSize(2);
    graphs_lnl_irr1_ann[0]->Draw("SAME EP");
    graphs_lnl_irr1_ann[1]->Draw("SAME E[]P");

    auto graphs_tifpa_irr1_ann_postirrd = database::get_general_TGraphs_iv(current_sensor, {{"OANN-150-150h", {"2"}}}, 4., database::status_to_marker["TIFPA-IRR1"], database::status_to_color["OANN-150-150h"]);
    auto graphs_tifpa_irr1_ann_baseline = database::get_general_TGraphs_iv(current_sensor, {{"NEW", {"2"}}}, 4., database::status_to_marker["TIFPA-IRR1"], database::status_to_color["TIFPA-IRR1"]);
    std::vector<TGraphErrors *> graphs_tifpa_irr1_ann = {graphutils::diff(graphs_tifpa_irr1_ann_postirrd[0], graphs_tifpa_irr1_ann_baseline[0]), graphutils::diff(graphs_tifpa_irr1_ann_postirrd[1], graphs_tifpa_irr1_ann_baseline[1])};
    graphutils::reassign_x_values(graphs_tifpa_irr1_ann[0], x_axis_tifpa_irr1);
    graphutils::reassign_x_values(graphs_tifpa_irr1_ann[1], x_axis_tifpa_irr1);
    graphs_tifpa_irr1_ann[0]->SetMarkerSize(2);
    graphs_tifpa_irr1_ann[1]->SetMarkerSize(2);
    graphs_tifpa_irr1_ann[0]->Draw("SAME EP");
    graphs_tifpa_irr1_ann[1]->Draw("SAME E[]P");

    //  TLegend
    TLegend *l_legend_lnl_ov = new TLegend(0.175, 0.925 - 6 * 0.04, 0.500, 0.925);
    l_legend_lnl_ov->SetLineColorAlpha(0., 0.);
    l_legend_lnl_ov->SetFillStyle(0.);
    l_legend_lnl_ov->AddEntry(graphs_lnl_irr1[1], "#it{n}-irradiated", "P");
    //l_legend_lnl_ov->AddEntry(graphs_lnl_irr2[1], "#it{n}-irradiated (2nd)", "P");
    l_legend_lnl_ov->AddEntry(graphs_tifpa_irr1[1], "#it{p}-irradiated", "P");
    l_legend_lnl_ov->AddEntry(graphs_lnl_irr1_ann[1], "#it{n}-irr., annealed", "P");
    l_legend_lnl_ov->AddEntry(graphs_tifpa_irr1_ann[1], "#it{p}-irr., annealed", "P");
    l_legend_lnl_ov->Draw("SAME");

    //  Save canvas
    l_latex->SetTextFont(42);
    l_latex->DrawLatexNDC(0.475, 0.9, current_sensor.c_str());
    LNL_ov_results->SaveAs(Form("figures/LNL_ov_results_%s.png", database::sensors_short[i_sensor].c_str()));
    delete LNL_ov_results;
  }

  //  --- --- --- --- Baseline sub. overcurrent at 4 ov
  //  Canvas set-up
  auto LNL_ov_results_close_up = new TCanvas("", "", 800, 800);
  gStyle->SetOptStat(0);
  gPad->SetGridy();
  gPad->SetMargin(0.15, 0.05, 0.15, 0.05);
  //  Frame
  auto h_frame = new TH1F("h_frame", ";;Ratio of damage current @ 4 ov.", 3, 0, 3);
  h_frame->GetXaxis()->SetBinLabel(1, "#it{p}-irrdiated");
  h_frame->GetXaxis()->SetBinLabel(2, "#it{n}-irrdiated");
  h_frame->GetXaxis()->SetBinLabel(3, "#it{n}-irrdiated (2nd)");
  h_frame->SetMinimum(0.5);
  h_frame->SetMaximum(2.5);
  h_frame->Draw();

  //  Generate TGraphErrors
  auto graphs_tifpa_irr1_closeup_postirrd_sens1 = database::get_general_TGraphs_iv("HPK S13360-3050VS", {{"TIFPA-IRR1", {"1", "2", "13", "14", "15", "16"}}, {"LNL-IRR1", {"21", "22", "23", "24", "25"}}, {"LNL-IRR2", {"25"}}}, 4., 20, database::status_to_color["TIFPA-IRR1"]);
  auto graphs_tifpa_irr1_closeup_baseline_sens1 = database::get_general_TGraphs_iv("HPK S13360-3050VS", {{"NEW", {"1", "2", "13", "14", "15", "16"}}, {"NEW", {"21", "22", "23", "24", "25"}}, {"OANN-150-450h", {"25"}}}, 4., database::status_to_marker["TIFPA-IRR1"], database::status_to_color["TIFPA-IRR1"]);
  std::vector<TGraphErrors *> graphs_tifpa_irr1_closeup_sens1 = {graphutils::diff(graphs_tifpa_irr1_closeup_postirrd_sens1[0], graphs_tifpa_irr1_closeup_baseline_sens1[0]), graphutils::diff(graphs_tifpa_irr1_closeup_postirrd_sens1[1], graphs_tifpa_irr1_closeup_baseline_sens1[1])};
  auto norm_val = graphs_tifpa_irr1_closeup_sens1[0]->GetPointY(0);
  graphutils::y_scale(graphs_tifpa_irr1_closeup_sens1[0], 1. / norm_val);
  graphutils::y_scale(graphs_tifpa_irr1_closeup_sens1[1], 1. / norm_val);
  graphs_tifpa_irr1_closeup_sens1[0]->SetMarkerSize(2);
  graphs_tifpa_irr1_closeup_sens1[1]->SetMarkerSize(2);
  graphs_tifpa_irr1_closeup_sens1[0]->Draw("SAME EP");
  graphs_tifpa_irr1_closeup_sens1[1]->Draw("SAME E[]P");

  auto graphs_tifpa_irr1_closeup_postirrd_sens2 = database::get_general_TGraphs_iv("HPK S13360-3075VS", {{"TIFPA-IRR1", {"1", "2", "13", "14", "15", "16"}}, {"LNL-IRR1", {"21", "22", "23", "24", "25"}}, {"LNL-IRR2", {"25"}}}, 4., 21, database::status_to_color["LNL-IRR1"]);
  auto graphs_tifpa_irr1_closeup_baseline_sens2 = database::get_general_TGraphs_iv("HPK S13360-3075VS", {{"NEW", {"1", "2", "13", "14", "15", "16"}}, {"NEW", {"21", "22", "23", "24", "25"}}, {"OANN-150-450h", {"25"}}}, 4., database::status_to_marker["TIFPA-IRR1"], database::status_to_color["TIFPA-IRR1"]);
  std::vector<TGraphErrors *> graphs_tifpa_irr1_closeup_sens2 = {graphutils::diff(graphs_tifpa_irr1_closeup_postirrd_sens2[0], graphs_tifpa_irr1_closeup_baseline_sens2[0]), graphutils::diff(graphs_tifpa_irr1_closeup_postirrd_sens2[1], graphs_tifpa_irr1_closeup_baseline_sens2[1])};
  norm_val = graphs_tifpa_irr1_closeup_sens2[0]->GetPointY(0);
  graphutils::y_scale(graphs_tifpa_irr1_closeup_sens2[0], 1. / norm_val);
  graphutils::y_scale(graphs_tifpa_irr1_closeup_sens2[1], 1. / norm_val);
  graphs_tifpa_irr1_closeup_sens2[0]->SetMarkerSize(2);
  graphs_tifpa_irr1_closeup_sens2[1]->SetMarkerSize(2);
  graphs_tifpa_irr1_closeup_sens2[0]->Draw("SAME EP");
  graphs_tifpa_irr1_closeup_sens2[1]->Draw("SAME E[]P");

  auto graphs_tifpa_irr1_closeup_postirrd_sens3 = database::get_general_TGraphs_iv("HPK S14161-3050HS", {{"TIFPA-IRR1", {"1", "2", "13", "14", "15", "16"}}, {"LNL-IRR1", {"21", "22", "23", "24", "25"}}, {"LNL-IRR2", {"25"}}}, 4., 33, database::status_to_color["TIFPA-IRR1"]);
  auto graphs_tifpa_irr1_closeup_baseline_sens3 = database::get_general_TGraphs_iv("HPK S14161-3050HS", {{"NEW", {"1", "2", "13", "14", "15", "16"}}, {"NEW", {"21", "22", "23", "24", "25"}}, {"OANN-150-450h", {"25"}}}, 4., database::status_to_marker["TIFPA-IRR1"], database::status_to_color["LNL-IRR2"]);
  std::vector<TGraphErrors *> graphs_tifpa_irr1_closeup_sens3 = {graphutils::diff(graphs_tifpa_irr1_closeup_postirrd_sens3[0], graphs_tifpa_irr1_closeup_baseline_sens3[0]), graphutils::diff(graphs_tifpa_irr1_closeup_postirrd_sens3[1], graphs_tifpa_irr1_closeup_baseline_sens3[1])};
  norm_val = graphs_tifpa_irr1_closeup_sens3[0]->GetPointY(0);
  graphutils::y_scale(graphs_tifpa_irr1_closeup_sens3[0], 1. / norm_val);
  graphutils::y_scale(graphs_tifpa_irr1_closeup_sens3[1], 1. / norm_val);
  graphs_tifpa_irr1_closeup_sens3[0]->SetMarkerSize(2);
  graphs_tifpa_irr1_closeup_sens3[1]->SetMarkerSize(2);
  graphs_tifpa_irr1_closeup_sens3[0]->Draw("SAME EP");
  graphs_tifpa_irr1_closeup_sens3[1]->Draw("SAME E[]P");

  //  Legend
  TLegend *l_legend_lnl_irr_closeup = new TLegend(0.175, 0.825, 0.500, 0.925);
  l_legend_lnl_irr_closeup->SetLineColorAlpha(0., 0.);
  l_legend_lnl_irr_closeup->SetFillStyle(0.);
  l_legend_lnl_irr_closeup->AddEntry(graphs_tifpa_irr1_closeup_sens1[0], "HPK S13360-3050VS", "P");
  l_legend_lnl_irr_closeup->AddEntry(graphs_tifpa_irr1_closeup_sens2[0], "HPK S13360-3075VS", "P");
  l_legend_lnl_irr_closeup->AddEntry(graphs_tifpa_irr1_closeup_sens3[0], "HPK S14161-3050HS", "P");
  l_legend_lnl_irr_closeup->Draw("SAME");

  //  Save canvas
  l_latex->SetTextFont(42);
  l_latex->SetTextSize(0.025);
  l_latex->DrawLatexNDC(0.620, 0.915, "normalised to #it{p}-irradiated eq.");
  l_latex->DrawLatexNDC(0.620, 0.885, "all irradiations at 10^{9} 1-MeV n_{eq}");
  LNL_ov_results_close_up->SaveAs("figures/LNL_ov_results_close_up.png");

  //  --- --- --- --- Baseline sub. ratio of different fluences
  //  Canvas set-up
  auto LNL_ov_results_ratio = new TCanvas("", "", 800, 800);
  gStyle->SetOptStat(0);
  gPad->SetGridy();
  gPad->SetLogx();
  gPad->SetMargin(0.15, 0.05, 0.15, 0.05);
  //  Frame
  h_frame_ov_fluences->GetYaxis()->SetTitle("Ratio of damage current @ 4 ov.");
  h_frame_ov_fluences->SetMinimum(0.25);
  h_frame_ov_fluences->SetMaximum(1.25);
  h_frame_ov_fluences->Draw();
  //  Legend
  TLegend *l_legend_lnl_irr_ratio = new TLegend(0.175, 0.825, 0.500, 0.925);
  l_legend_lnl_irr_ratio->SetLineColorAlpha(0., 0.);
  l_legend_lnl_irr_ratio->SetFillStyle(0.);

  std::vector<int> marker = {20, 21, 33};
  std::vector<std::string> mrkcolor = {"TIFPA-IRR1", "LNL-IRR1", "LNL-IRR2"};
  for (auto i_sensor = 0; i_sensor < 3; i_sensor++)
  {
    auto current_graph_0 = graphutils::ratio(memory_lnl_irr2[i_sensor][0], memory_lnl_irr1[i_sensor][0], false);
    auto current_graph_1 = graphutils::ratio(memory_lnl_irr2[i_sensor][1], memory_lnl_irr1[i_sensor][1], false);
    current_graph_0->SetMarkerStyle(marker[i_sensor]);
    current_graph_1->SetMarkerStyle(marker[i_sensor]);
    current_graph_0->SetMarkerColor(database::status_to_color[mrkcolor[i_sensor]]);
    current_graph_1->SetMarkerColor(database::status_to_color[mrkcolor[i_sensor]]);
    l_legend_lnl_irr_ratio->AddEntry(current_graph_1, database::sensors_full[i_sensor].c_str(), "P");
    current_graph_0->Draw("SAME EPL");
    current_graph_1->Draw("SAME E[]P");
  }

  l_legend_lnl_irr_ratio->Draw("SAME");
  l_latex->SetTextFont(42);
  l_latex->SetTextSize(0.025);
  l_latex->DrawLatexNDC(0.620, 0.915, "#it{n}-irradiated (2nd)");
  l_latex->DrawLatexNDC(0.620, 0.885, "normalised to #it{n}-irradiated eq.");
  LNL_ov_results_ratio->SaveAs("figures/LNL_ov_results_ratio.png");
  return;

  // memory_lnl_irr1
  // memory_lnl_irr2

  /*
  delete LNL_ov_results_close_up;
  delete h_frame;

  return;

  LNL_ov_results = new TCanvas("", "", 800, 800);
  gStyle->SetOptStat(0);
  gPad->SetLogy();
  gPad->SetMargin(0.15, 0.05, 0.15, 0.05);
  h_frame = new TH1F("h_frame", ";V_{bias} (V);Current (A)", 100, 0, 100);
  h_frame->SetMinimum(5.e-12);
  h_frame->SetMaximum(1.e-7);
  h_frame->Draw();
  auto graph_1 = database::get_iv_scan("23", "C1", "NEW", kFullCircle, kBlue);
  auto graph_2 = database::get_iv_scan("23", "C1", "OANN-150-15h-3-again", kFullCircle, kRed);
  auto graph_3 = database::get_iv_scan("23", "C1", "LNL-IRR2", kFullCircle, kOrange);
  graph_1->Draw("SAME");
  graph_2->Draw("SAME");
  graph_3->Draw("SAME");
  database::get_iv_scan("23", "C2", "NEW", kFullCircle, kBlue)->Draw("SAME");
  database::get_iv_scan("23", "C2", "OANN-150-15h-3-again", kFullCircle, kRed)->Draw("SAME");
  database::get_iv_scan("23", "C2", "LNL-IRR2", kFullCircle, kOrange)->Draw("SAME");
  database::get_iv_scan("23", "C3", "NEW", kFullCircle, kBlue)->Draw("SAME");
  database::get_iv_scan("23", "C3", "OANN-150-15h-3-again", kFullCircle, kRed)->Draw("SAME");
  database::get_iv_scan("23", "C3", "LNL-IRR2", kFullCircle, kOrange)->Draw("SAME");
  database::get_iv_scan("23", "C4", "NEW", kFullCircle, kBlue)->Draw("SAME");
  database::get_iv_scan("23", "C4", "OANN-150-15h-3-again", kFullCircle, kRed)->Draw("SAME");
  database::get_iv_scan("23", "C4", "LNL-IRR2", kFullCircle, kOrange)->Draw("SAME");
  TLegend *l_wlegend = new TLegend(0.175, 0.75, 0.45, 0.9);
  l_wlegend->SetLineColorAlpha(0., 0.);
  l_wlegend->SetFillStyle(0.);
  l_wlegend->AddEntry(graph_1, "NEW", "P");
  l_wlegend->AddEntry(graph_2, "OANN", "P");
  l_wlegend->AddEntry(graph_3, "GIF++", "P");
  l_wlegend->Draw("SAME");
  */
}
