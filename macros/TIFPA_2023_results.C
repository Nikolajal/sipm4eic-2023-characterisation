#include "../source/database.C"

void TIFPA_2023_results(std::string outfile_name = "out.root")
{
  //  Load database
  database::basedir = "/Users/nrubini/Analysis/ePIC/memory/sipm4eic-2023-characterisation/Data/";
  database::read_database(database::basedir + "database.txt");

  //  General set-up
  TLatex *l_latex = new TLatex();

  //  --- --- --- OV current
  //  --- --- --- --- OV current @4V pre annealing
  //  Canvas set-up
  TCanvas *ov_current_pre_annealing = new TCanvas("", "", 800, 800);
  gStyle->SetOptStat(0);
  gPad->SetGridy();
  gPad->SetMargin(0.15, 0.05, 0.15, 0.05);
  //  Frame
  TH1F *h_frame_pre_ann = new TH1F("h_frame_pre_ann", ";;Ratio of damage current @ 4 ov.", 3, 0, 3);
  h_frame_pre_ann->SetMinimum(0.65);
  h_frame_pre_ann->SetMaximum(1.35);
  h_frame_pre_ann->GetXaxis()->SetBinLabel(1, "#it{p}-irradiated");
  h_frame_pre_ann->GetXaxis()->SetBinLabel(2, "150h 150C prev. ann.");
  h_frame_pre_ann->GetXaxis()->SetBinLabel(3, "470h 150C prev. ann.");
  h_frame_pre_ann->GetXaxis()->SetLabelSize(0.04);
  h_frame_pre_ann->Draw();

  //  Generate TGraphErrors
  auto graphs_tifpa_pre_ann_sens1 = database::get_general_TGraphs_iv("HPK S13360-3050VS", {{"TIFPA-IRR1", {"1", "2", "13", "14", "15", "16"}}, {"TIFPA-IRR1", {"3"}}, {"TIFPA-IRR1", {"4"}}}, 4., 20, kBlue -2);
  auto graphs_tifpa_pre_ann_baseline_sens1 = database::get_general_TGraphs_iv("HPK S13360-3050VS", {{"NEW", {"1", "2", "13", "14", "15", "16"}}, {"NEW", {"3"}}, {"NEW", {"4"}}}, 4., database::status_to_marker["TIFPA-IRR1"], kBlue -2);
  std::vector<TGraphErrors *> graphs_tifpa_pre_ann_norm_sens1 = {graphutils::diff(graphs_tifpa_pre_ann_sens1[0], graphs_tifpa_pre_ann_baseline_sens1[0]), graphutils::diff(graphs_tifpa_pre_ann_sens1[1], graphs_tifpa_pre_ann_baseline_sens1[1])};
  auto norm_val = graphs_tifpa_pre_ann_norm_sens1[0]->GetPointY(0);
  graphutils::y_scale(graphs_tifpa_pre_ann_norm_sens1[0], 1. / norm_val);
  graphutils::y_scale(graphs_tifpa_pre_ann_norm_sens1[1], 1. / norm_val);
  graphs_tifpa_pre_ann_norm_sens1[0]->SetMarkerSize(2);
  graphs_tifpa_pre_ann_norm_sens1[1]->SetMarkerSize(2);
  graphs_tifpa_pre_ann_norm_sens1[0]->Draw("SAME EP");

  auto graphs_tifpa_pre_ann_sens2 = database::get_general_TGraphs_iv("HPK S13360-3075VS", {{"TIFPA-IRR1", {"1", "2", "13", "14", "15", "16"}}, {"TIFPA-IRR1", {"3"}}, {"TIFPA-IRR1", {"4"}}}, 4., 21, kAzure - 2);
  auto graphs_tifpa_pre_ann_baseline_sens2 = database::get_general_TGraphs_iv("HPK S13360-3075VS", {{"NEW", {"1", "2", "13", "14", "15", "16"}}, {"NEW", {"3"}}, {"NEW", {"4"}}}, 4., database::status_to_marker["TIFPA-IRR1"], kBlue -2);
  std::vector<TGraphErrors *> graphs_tifpa_pre_ann_norm_sens2 = {graphutils::diff(graphs_tifpa_pre_ann_sens2[0], graphs_tifpa_pre_ann_baseline_sens2[0]), graphutils::diff(graphs_tifpa_pre_ann_sens2[1], graphs_tifpa_pre_ann_baseline_sens2[1])};
  norm_val = graphs_tifpa_pre_ann_norm_sens2[0]->GetPointY(0);
  graphutils::y_scale(graphs_tifpa_pre_ann_norm_sens2[0], 1. / norm_val);
  graphutils::y_scale(graphs_tifpa_pre_ann_norm_sens2[1], 1. / norm_val);
  graphs_tifpa_pre_ann_norm_sens2[0]->SetMarkerSize(2);
  graphs_tifpa_pre_ann_norm_sens2[1]->SetMarkerSize(2);
  graphs_tifpa_pre_ann_norm_sens2[0]->Draw("SAME EP");
  graphs_tifpa_pre_ann_norm_sens2[1]->Draw("SAME E[]P");

  auto graphs_tifpa_pre_ann_sens3 = database::get_general_TGraphs_iv("HPK S14161-3050HS", {{"TIFPA-IRR1", {"1", "2", "13", "14", "15", "16"}}, {"TIFPA-IRR1", {"3"}}, {"TIFPA-IRR1", {"4"}}}, 4., 33, kAzure - 4);
  auto graphs_tifpa_pre_ann_baseline_sens3 = database::get_general_TGraphs_iv("HPK S14161-3050HS", {{"NEW", {"1", "2", "13", "14", "15", "16"}}, {"NEW", {"3"}}, {"NEW", {"4"}}}, 4., database::status_to_marker["TIFPA-IRR1"], kBlue -2);
  std::vector<TGraphErrors *> graphs_tifpa_pre_ann_norm_sens3 = {graphutils::diff(graphs_tifpa_pre_ann_sens3[0], graphs_tifpa_pre_ann_baseline_sens3[0]), graphutils::diff(graphs_tifpa_pre_ann_sens3[1], graphs_tifpa_pre_ann_baseline_sens3[1])};
  norm_val = graphs_tifpa_pre_ann_norm_sens3[0]->GetPointY(0);
  graphutils::y_scale(graphs_tifpa_pre_ann_norm_sens3[0], 1. / norm_val);
  graphutils::y_scale(graphs_tifpa_pre_ann_norm_sens3[1], 1. / norm_val);
  graphs_tifpa_pre_ann_norm_sens3[0]->SetMarkerSize(2);
  graphs_tifpa_pre_ann_norm_sens3[1]->SetMarkerSize(2);
  graphs_tifpa_pre_ann_norm_sens3[0]->Draw("SAME EP");
  graphs_tifpa_pre_ann_norm_sens3[1]->Draw("SAME E[]P");

  //  TLegend
  TLegend *l_ov_current_pre_annealing = new TLegend(0.175, 0.825, 0.500, 0.925);
  l_ov_current_pre_annealing->SetLineColorAlpha(0., 0.);
  l_ov_current_pre_annealing->SetFillStyle(0.);
  l_ov_current_pre_annealing->AddEntry(graphs_tifpa_pre_ann_norm_sens1[0], "HPK S13360-3050VS", "P");
  l_ov_current_pre_annealing->AddEntry(graphs_tifpa_pre_ann_norm_sens2[0], "HPK S13360-3075VS", "P");
  l_ov_current_pre_annealing->AddEntry(graphs_tifpa_pre_ann_norm_sens3[0], "HPK S14161-3050HS", "P");
  l_ov_current_pre_annealing->Draw("SAME");

  //  Save canvas
  l_latex->SetTextFont(42);
  l_latex->SetTextSize(0.025);
  l_latex->DrawLatexNDC(0.620, 0.915, "normalised to #it{p}-irradiated eq.");
  l_latex->DrawLatexNDC(0.620, 0.885, "all irradiations at 10^{9} 1-MeV n_{eq}");
  ov_current_pre_annealing->SaveAs("./figures/TIFPA_2023_ov_current_pre_annealing.png");

  //  --- --- --- --- OV current @4V pre annealing
  //  Canvas set-up
  TCanvas *ov_current_online_annealing = new TCanvas("", "", 800, 800);
  gStyle->SetOptStat(0);
  gPad->SetLogy();
  gPad->SetGridy();
  gPad->SetMargin(0.15, 0.05, 0.15, 0.05);
  //  Frame
  TH1F *h_frame_online_ann = new TH1F("h_frame", ";;Ratio of damage current @ 4 ov.", 4, 0, 4);
  h_frame_online_ann->SetMinimum(0.005);
  h_frame_online_ann->SetMaximum(5.);
  h_frame_online_ann->GetXaxis()->SetBinLabel(1, "#it{p}-irradiated");
  h_frame_online_ann->GetXaxis()->SetBinLabel(2, "#splitline{150h 150C}{oven ann.}");
  h_frame_online_ann->GetXaxis()->SetBinLabel(3, "#splitline{30m x9 175C}{on. fwd. ann.}");
  h_frame_online_ann->GetXaxis()->SetBinLabel(4, "#splitline{30m x9 175C}{on. rev. ann.}");
  h_frame_online_ann->GetXaxis()->SetLabelSize(0.04);
  h_frame_online_ann->Draw();

  //  Generate TGraphErrors
  auto graphs_tifpa_online_ann_sens1 = database::get_general_TGraphs_iv("HPK S13360-3050VS", {{"TIFPA-IRR1", {"1", "2", "13", "14", "15", "16"}}, {"OANN-150-150h", {"2"}}, {"TIFPA-IRR1", {"11"}}, {"TIFPA-IRR1", {"12"}}}, 4., 20, kBlue -2);
  auto graphs_tifpa_online_ann_baseline_sens1 = database::get_general_TGraphs_iv("HPK S13360-3050VS", {{"NEW", {"1", "2", "13", "14", "15", "16"}}, {"NEW", {"2"}}, {"NEW", {"11"}}, {"NEW", {"12"}}}, 4., database::status_to_marker["TIFPA-IRR1"], kBlue -2);
  std::vector<TGraphErrors *> graphs_tifpa_online_ann_norm_sens1 = {graphutils::diff(graphs_tifpa_online_ann_sens1[0], graphs_tifpa_online_ann_baseline_sens1[0]), graphutils::diff(graphs_tifpa_online_ann_sens1[1], graphs_tifpa_online_ann_baseline_sens1[1])};
  norm_val = graphs_tifpa_online_ann_norm_sens1[0]->GetPointY(0);
  graphutils::y_scale(graphs_tifpa_online_ann_norm_sens1[0], 1. / norm_val);
  graphutils::y_scale(graphs_tifpa_online_ann_norm_sens1[1], 1. / norm_val);
  graphs_tifpa_online_ann_norm_sens1[0]->SetMarkerSize(2);
  graphs_tifpa_online_ann_norm_sens1[1]->SetMarkerSize(2);
  graphs_tifpa_online_ann_norm_sens1[0]->Draw("SAME EP");

  auto graphs_tifpa_online_ann_sens2 = database::get_general_TGraphs_iv("HPK S13360-3075VS", {{"TIFPA-IRR1", {"1", "2", "13", "14", "15", "16"}}, {"OANN-150-150h", {"2"}}, {"TIFPA-IRR1", {"11"}}, {"TIFPA-IRR1", {"12"}}}, 4., 21, kAzure - 2);
  auto graphs_tifpa_online_ann_baseline_sens2 = database::get_general_TGraphs_iv("HPK S13360-3075VS", {{"NEW", {"1", "2", "13", "14", "15", "16"}}, {"NEW", {"2"}}, {"NEW", {"11"}}, {"NEW", {"12"}}}, 4., database::status_to_marker["TIFPA-IRR1"], kBlue -2);
  std::vector<TGraphErrors *> graphs_tifpa_online_ann_norm_sens2 = {graphutils::diff(graphs_tifpa_online_ann_sens2[0], graphs_tifpa_online_ann_baseline_sens2[0]), graphutils::diff(graphs_tifpa_online_ann_sens2[1], graphs_tifpa_online_ann_baseline_sens2[1])};
  norm_val = graphs_tifpa_online_ann_norm_sens2[0]->GetPointY(0);
  graphutils::y_scale(graphs_tifpa_online_ann_norm_sens2[0], 1. / norm_val);
  graphutils::y_scale(graphs_tifpa_online_ann_norm_sens2[1], 1. / norm_val);
  graphs_tifpa_online_ann_norm_sens2[0]->SetMarkerSize(2);
  graphs_tifpa_online_ann_norm_sens2[1]->SetMarkerSize(2);
  graphs_tifpa_online_ann_norm_sens2[0]->Draw("SAME EP");
  graphs_tifpa_online_ann_norm_sens2[1]->Draw("SAME E[]P");

  auto graphs_tifpa_online_ann_sens3 = database::get_general_TGraphs_iv("HPK S14161-3050HS", {{"TIFPA-IRR1", {"1", "2", "13", "14", "15", "16"}}, {"OANN-150-150h", {"2"}}, {"TIFPA-IRR1", {"11"}}, {"TIFPA-IRR1", {"12"}}}, 4., 33, kAzure - 4);
  auto graphs_tifpa_online_ann_baseline_sens3 = database::get_general_TGraphs_iv("HPK S14161-3050HS", {{"NEW", {"1", "2", "13", "14", "15", "16"}}, {"NEW", {"2"}}, {"NEW", {"11"}}, {"NEW", {"12"}}}, 4., database::status_to_marker["TIFPA-IRR1"], kBlue -2);
  std::vector<TGraphErrors *> graphs_tifpa_online_ann_norm_sens3 = {graphutils::diff(graphs_tifpa_online_ann_sens3[0], graphs_tifpa_online_ann_baseline_sens3[0]), graphutils::diff(graphs_tifpa_online_ann_sens3[1], graphs_tifpa_online_ann_baseline_sens3[1])};
  norm_val = graphs_tifpa_online_ann_norm_sens3[0]->GetPointY(0);
  graphutils::y_scale(graphs_tifpa_online_ann_norm_sens3[0], 1. / norm_val);
  graphutils::y_scale(graphs_tifpa_online_ann_norm_sens3[1], 1. / norm_val);
  graphs_tifpa_online_ann_norm_sens3[0]->SetMarkerSize(2);
  graphs_tifpa_online_ann_norm_sens3[1]->SetMarkerSize(2);
  graphs_tifpa_online_ann_norm_sens3[0]->Draw("SAME EP");
  graphs_tifpa_online_ann_norm_sens3[1]->Draw("SAME E[]P");

  //  TLegend
  TLegend *l_ov_current_online_annealing = new TLegend(0.175, 0.825, 0.500, 0.925);
  l_ov_current_online_annealing->SetLineColorAlpha(0., 0.);
  l_ov_current_online_annealing->SetFillStyle(0.);
  l_ov_current_online_annealing->AddEntry(graphs_tifpa_online_ann_norm_sens1[0], "HPK S13360-3050VS", "P");
  l_ov_current_online_annealing->AddEntry(graphs_tifpa_online_ann_norm_sens2[0], "HPK S13360-3075VS", "P");
  l_ov_current_online_annealing->AddEntry(graphs_tifpa_online_ann_norm_sens3[0], "HPK S14161-3050HS", "P");
  l_ov_current_online_annealing->Draw("SAME");

  //  Save canvas
  l_latex->SetTextFont(42);
  l_latex->SetTextSize(0.025);
  l_latex->DrawLatexNDC(0.620, 0.915, "normalised to #it{p}-irradiated eq.");
  l_latex->DrawLatexNDC(0.620, 0.885, "all irradiations at 10^{9} 1-MeV n_{eq}");
  ov_current_online_annealing->SaveAs("./figures/TIFPA_2023_ov_current_online_annealing.png");

  //  --- --- --- --- OV damage current @4V offline direct annealing
  //  Canvas set-up
  TCanvas *ov_current_online_annealing = new TCanvas("", "", 800, 800);
  gStyle->SetOptStat(0);
  gPad->SetLogy();
  gPad->SetGridy();
  gPad->SetMargin(0.15, 0.05, 0.15, 0.05);
  //  Frame
  TH1F *h_frame_online_ann = new TH1F("h_frame", ";;Ratio of damage current @ 4 ov.", 4, 0, 4);
  h_frame_online_ann->SetMinimum(0.005);
  h_frame_online_ann->SetMaximum(5.);
  h_frame_online_ann->GetXaxis()->SetBinLabel(1, "#it{p}-irradiated");
  h_frame_online_ann->GetXaxis()->SetBinLabel(2, "#splitline{150h 150C}{oven ann.}");
  h_frame_online_ann->GetXaxis()->SetBinLabel(3, "#splitline{30m x9 175C}{on. fwd. ann.}");
  h_frame_online_ann->GetXaxis()->SetBinLabel(4, "#splitline{30m x9 175C}{on. rev. ann.}");
  h_frame_online_ann->GetXaxis()->SetLabelSize(0.04);
  h_frame_online_ann->Draw();

  //  

}
