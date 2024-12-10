#include "../source/database.C"

void playground(std::string outfile_name = "out.root")
{
  //  Load database
  database::basedir = "/Users/nrubini/Analysis/ePIC/memory/sipm4eic-2023-characterisation/Data/";
  database::read_database(database::basedir + "database.txt");

  //  Canvas set-up
  TCanvas *result_canvas = new TCanvas("", "", 800, 800);
  gStyle->SetOptStat(0);

  //  Draw axes
  std::vector<std::array<float, 2>> x_axis_time = {{0.15, 0.}, {0.5, 0}, {2.5, 0}, {24, 0}, {73, 0}, {250, 0}};
  std::vector<double> max_measured = {utility::round_digits(log(24), 3), utility::round_digits(log(24), 3), utility::round_digits(log(250), 3), utility::round_digits(log(73), 3)};
  auto max_val = 0.;
  for (auto i_val : max_measured)
    max_val += i_val;

  TPad *pad_100C = new TPad("pad_100C", "", 0.100, 0.095, 0.100 + 0.85 * (max_measured[0] / max_val), 0.950);
  pad_100C->SetMargin(0.0, 0.0, 0.0, 0.0); // No margins
  pad_100C->SetFillColor(0);
  pad_100C->SetFillStyle(0);
  pad_100C->SetFrameFillStyle(0);
  pad_100C->SetGridy();
  TH1F *hFrame_100C = make_log_th1(1000, 0.15, TMath::Exp(max_measured[0]));
  hFrame_100C->SetMaximum(1.1);
  hFrame_100C->SetMinimum(1.e-3);
  hFrame_100C->GetXaxis()->SetLabelSize(0.1);
  hFrame_100C->GetYaxis()->SetLabelSize(0.1);

  TPad *pad_125C = new TPad("pad_125C", "", 0.100 + 0.85 * (max_measured[0] / max_val), 0.095, 0.100 + 0.85 * ((max_measured[0] + max_measured[1]) / max_val), 0.950);
  pad_125C->SetMargin(0.0, 0.0, 0.0, 0.0); // No margins
  pad_125C->SetFillColor(0);
  pad_125C->SetFillStyle(0);
  pad_125C->SetFrameFillStyle(0);
  pad_125C->SetGridy();
  TH1F *hFrame_125C = make_log_th1(1000, 0.15, TMath::Exp(max_measured[1]));
  hFrame_125C->SetMaximum(1.1);
  hFrame_125C->SetMinimum(1.e-3);
  hFrame_125C->GetXaxis()->SetLabelSize(0.1);

  TPad *pad_150C = new TPad("pad_150C", "", 0.100 + 0.85 * ((max_measured[0] + max_measured[1]) / max_val), 0.095, 0.100 + 0.85 * ((max_measured[0] + max_measured[1] + max_measured[2]) / max_val), 0.950);
  pad_150C->SetMargin(0.0, 0.0, 0.0, 0.0); // No margins
  pad_150C->SetFillColor(0);
  pad_150C->SetFillStyle(0);
  pad_150C->SetFrameFillStyle(0);
  pad_150C->SetGridy();
  TH1F *hFrame_150C = make_log_th1(1000, 0.15, TMath::Exp(max_measured[2]));
  hFrame_150C->SetMaximum(1.1);
  hFrame_150C->SetMinimum(1.e-3);
  hFrame_150C->GetXaxis()->SetLabelSize(0.1);

  TPad *pad_175C = new TPad("pad_175C", "", 0.100 + 0.85 * ((max_measured[0] + max_measured[1] + max_measured[2]) / max_val), 0.095, 0.100 + 0.85 * ((max_measured[0] + max_measured[1] + max_measured[2] + max_measured[3]) / max_val), 0.950);
  pad_175C->SetMargin(0.0, 0.0, 0.0, 0.0); // No margins
  pad_175C->SetFillColor(0);
  pad_175C->SetFillStyle(0);
  pad_175C->SetFrameFillStyle(0);
  pad_175C->SetGridy();
  TH1F *hFrame_175C = make_log_th1(1000, 0.15, TMath::Exp(max_measured[3]));
  hFrame_175C->SetMaximum(1.1);
  hFrame_175C->SetMinimum(1.e-3);
  hFrame_175C->GetXaxis()->SetLabelSize(0.1);

  result_canvas->cd();
  pad_100C->Draw();
  pad_100C->cd();
  hFrame_100C->Draw();
  gPad->SetLogx();
  gPad->SetLogy();
  auto current_graphs = database::get_steps_history_FOD_std_TIFPA("13", "*", {"TIFPA-IRR1", "SANN-100-30m", "SANN-100-2.5h", "SANN-100-24h"}, 3., kFullCircle, kGreen - 2, kCircle, kGreen - 2);
  for (auto current_graph : current_graphs)
  {
    graphutils::reassign_x_values(current_graph, x_axis_time);
    current_graph->Draw("SAME EP");
  }

  result_canvas->cd();
  pad_125C->Draw();
  pad_125C->cd();
  hFrame_125C->Draw();
  gPad->SetLogx();
  gPad->SetLogy();
  current_graphs = database::get_steps_history_FOD_std_TIFPA("13", "*", {"SANN-100-24h", "SANN-125-30m", "SANN-125-2.5h", "SANN-125-24h"}, 3., kFullCircle, kYellow + 2, kCircle, kYellow + 2);
  for (auto current_graph : current_graphs)
  {
    graphutils::reassign_x_values(current_graph, x_axis_time);
    current_graph->Draw("SAME EP");
  }
  result_canvas->cd();
  pad_150C->Draw();
  pad_150C->cd();
  hFrame_150C->Draw();
  gPad->SetLogx();
  gPad->SetLogy();
  current_graphs = database::get_steps_history_FOD_std_TIFPA("13", "*", {"SANN-125-24h", "SANN-150-30m", "SANN-150-2.5h", "SANN-150-24h", "SANN-150-73h", "SANN-150-250h"}, 3., kFullCircle, kOrange + 2, kCircle, kOrange + 2);
  for (auto current_graph : current_graphs)
  {
    graphutils::reassign_x_values(current_graph, x_axis_time);
    current_graph->Draw("SAME EP");
  }

  result_canvas->cd();
  pad_175C->Draw();
  pad_175C->cd();
  hFrame_175C->Draw();
  gPad->SetLogx();
  gPad->SetLogy();
  current_graphs = database::get_steps_history_FOD_std_TIFPA("13", "*", {"SANN-150-250h", "SANN-175-30m", "SANN-175-2.5h", "SANN-175-24h", "SANN-175-73h"}, 3., kFullCircle, kRed - 2, kCircle, kRed - 2);
  for (auto current_graph : current_graphs)
  {
    graphutils::reassign_x_values(current_graph, x_axis_time);
    current_graph->Draw("SAME EP");
  }

  result_canvas->cd();
  // Update the canvas to make sure everything is drawn
  result_canvas->Update();

  // Create the secondary axis on the right
  TGaxis *axis_right = new TGaxis(0.1, 0.099, 0.1, 0.95, 0.001, 1.1, 510, "G");
  axis_right->SetTitle("Y-axis (Right)");
  axis_right->SetLabelSize(0.03);
  axis_right->SetTitleSize(0.04);
  axis_right->Draw();

  auto min_axis = 0.100;
  auto max_axis = 0.100 + 0.85 * (max_measured[0] / max_val);
  for (auto iTer = 0; iTer < max_measured.size(); iTer++)
  {
    TGaxis *axis_right = new TGaxis(min_axis, 0.1, max_axis, 0.1, 0.015, TMath::Exp(max_measured[iTer]), 510, "G");
    min_axis += 0.85 * max_measured[iTer] / max_val;
    max_axis += 0.85 * max_measured[iTer + 1] / max_val;
    if (iTer == 0)
      axis_right->SetTitle("Y-axis (Right)");
    // if (iTer != 3)
    // continue;
    axis_right->SetLabelSize(0.03);
    axis_right->SetTitleSize(0.04);
    axis_right->Draw();
    // if (iTer == 1)
    // break;
  }

  result_canvas->SaveAs("test.png");
}
