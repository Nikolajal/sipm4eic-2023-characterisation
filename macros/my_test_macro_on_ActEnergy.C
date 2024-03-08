#include "../source/database.C"

void my_test_macro_on_ActEnergy()
{
  //  Load database
  database::basedir = "./Data";
  database::read_database(database::basedir + "/database.txt");

  // List of Target boards
  std::map<std::string, std::vector<std::string>> selected_boards = {
      {"NEW", {"31", "32"}},
      {"TIFPA-IRR1", {"15", "16"}},
      {"LNL-IRR1", {"24"}},
      {"LNL-ANN", {"25"}}};
  std::map<std::string, int> state_to_marker = {
      {"NEW", 20},
      {"TIFPA-IRR1", 20},
      {"LNL-IRR1", 20},
      {"LNL-ANN", 20}};
  std::map<std::string, int> state_to_color = {
      {"NEW", kAzure - 2},
      {"TIFPA-IRR1", kGreen - 3},
      {"LNL-IRR1", kGreen + 3},
      {"LNL-ANN", kRed}};
  std::map<std::string, bool> state_to_bool = {
      {"NEW", true},
      {"TIFPA-IRR1", true},
      {"LNL-IRR1", true},
      {"LNL-ANN", true}};

  //  Creat output
  TObjects outputObjects;
  for (auto [current_type, current_boards] : selected_boards)
  {
    for (auto current_sensor : database::all_sensors)
    {
      auto current_graph_name = Form("%s_%s", current_type.c_str(), current_sensor.c_str());
      outputObjects._TGraphErrors[current_graph_name] = new TGraphErrors();
    }
  }

  //  Loop on the states of the T Scan
  for (auto [current_type, current_boards] : selected_boards)
  {
    for (auto current_sensor : database::all_sensors)
    {
      auto iBin = -1;
      for (auto current_status : database::TScan_statuses)
      {
        iBin++;
        auto current_graph_name = Form("%s_%s", current_type.c_str(), current_sensor.c_str());
        auto current_graph = database::get_average_iv_at_overvoltage(current_boards, current_sensor, current_status, {3});
        auto current_val = TMath::Log(current_graph->GetPointY(0));
        auto current_err = current_val * (current_graph->GetErrorY(0) / current_graph->GetPointY(0));
        auto current_invT = 1. / (8.61733e-5 * std::stof(database::get_database_info(current_boards[0], current_status, "temp")));
        outputObjects._TGraphErrors[current_graph_name]->SetPoint(iBin, current_invT, current_val);
        outputObjects._TGraphErrors[current_graph_name]->SetPointError(iBin, 0, current_err);
      }
    }
  }

  //  Canvas with results
  TF1 *pol_1_1 = new TF1("pol_1_1", "[0]+x*[1]");
  TF1 *pol_1_2 = new TF1("pol_1_2", "[0]+x*[1]");
  TF1 *pol_1_full = new TF1("pol_1_full", "[0]+x*[1]+[2]+x*[3]");
  TCanvas *cDrawResults = new TCanvas("cDrawResults", "cDrawResults", 1000, 1000);
  gPad->DrawFrame(35, -23, 50, -12);
  gPad->SetTopMargin(0.05);
  gPad->SetRightMargin(0.05);
  gPad->SetBottomMargin(0.15);
  gPad->SetLeftMargin(0.15);
  gPad->DrawFrame(37, -23, 49, 10, ";1/kT (eV^{-1});log(I @ 3-OV)");
  TLatex *latex = new TLatex();
  latex->SetTextSize(0.025);
  auto iTer = -1;
  for (auto [current_type, current_boards] : selected_boards)
  {
    iTer++;
    auto current_graph_name = Form("%s_%s", current_type.c_str(), database::all_sensors[0].c_str());
    auto current_graph_name_2 = Form("%s_%s_2", current_type.c_str(), database::all_sensors[0].c_str());
    outputObjects._TGraphErrors[current_graph_name_2] = new TGraphErrors();
    outputObjects._TGraphErrors[current_graph_name]->SetMarkerStyle(state_to_marker[current_type]);
    outputObjects._TGraphErrors[current_graph_name]->SetMarkerColor(state_to_color[current_type]);
    outputObjects._TGraphErrors[current_graph_name]->SetMarkerSize(2);
    outputObjects._TGraphErrors[current_graph_name_2]->SetMarkerStyle(state_to_marker[current_type]);
    outputObjects._TGraphErrors[current_graph_name_2]->SetMarkerColor(state_to_color[current_type]);
    outputObjects._TGraphErrors[current_graph_name_2]->SetMarkerSize(2);
    outputObjects._TGraphErrors[current_graph_name]->Draw("SAME PE");
    latex->SetTextColor(state_to_color[current_type]);
    pol_1_1->SetLineColor(state_to_color[current_type]);
    pol_1_1->SetParameter(0, 0);
    pol_1_1->SetParameter(1, -0.5);
    pol_1_2->SetLineColor(state_to_color[current_type]);
    pol_1_2->SetParameter(0, 0);
    pol_1_2->SetParameter(1, -0.5);
    pol_1_full->SetLineColor(state_to_color[current_type]);

    outputObjects._TGraphErrors[current_graph_name]->Fit(pol_1_1, "IMRESQ", "", 30, 42);

    for (auto iPnt = 3; iPnt < outputObjects._TGraphErrors[current_graph_name]->GetN(); iPnt++)
    {
      outputObjects._TGraphErrors[current_graph_name_2]->SetPoint(iPnt - 3, outputObjects._TGraphErrors[current_graph_name]->GetPointX(iPnt), outputObjects._TGraphErrors[current_graph_name]->GetPointY(iPnt) - pol_1_1->Eval(outputObjects._TGraphErrors[current_graph_name]->GetPointX(iPnt)));
    }
    outputObjects._TGraphErrors[current_graph_name_2]->Fit(pol_1_2, "IMRESQ", "", 42, 60);
    outputObjects._TGraphErrors[current_graph_name_2]->Draw("SAME PE");

    latex->DrawLatexNDC(0.18, 0.20 + iTer * 0.03, Form("%s E_{act} : %.2f#pm%.2f eV", current_type.c_str(), -pol_1_1->GetParameter(1), pol_1_1->GetParError(1)));
    latex->DrawLatexNDC(0.60, 0.92 - iTer * 0.03, Form("%s E_{act} : %.2f#pm%.2f eV", current_type.c_str(), -pol_1_2->GetParameter(1), pol_1_2->GetParError(1)));
  }

  //  Write to file
  TFile *output_file = new TFile("out.root", "RECREATE");
  write_all(outputObjects);
  output_file->Close();
}