#include "../source/database.C"

void my_test_macro_on_ActEnergy()
{
  //  Load database
  database::basedir = "./Data";
  database::read_database(database::basedir + "/database.txt");

  // List of Target boards
  std::unordered_map<std::string, std::vector<std::string>> selected_boards = {
      {"NEW", {"31", "32"}},
      {"TIFPA-IRR1", {"15", "16"}},
      {"LNL-IRR1", {"24"}},
      {"LNL-ANN", {"25"}}};
  std::unordered_map<std::string, int> state_to_marker = {
      {"NEW", 20},
      {"TIFPA-IRR1", 20},
      {"LNL-IRR1", 20},
      {"LNL-ANN", 20}};
  std::unordered_map<std::string, int> status_to_color = {
      {"NEW", kAzure - 3},
      {"TIFPA-IRR1", kGreen + 2},
      {"LNL-IRR1", kSpring - 7},
      {"LNL-IRR2", kGreen - 5},
      {"GIF-IRR1", kOrange},
      {"OANN-150-150h", kRed - 3},
      {"LNL-ANN", kRed - 3}};
  std::unordered_map<std::string, std::string> status_to_legend = {
      {"NEW", "New"},
      {"TIFPA-IRR1", "#it{p}-irradiated"},
      {"LNL-IRR1", "#it{n}-irradiated"},
      {"LNL-ANN", "#it{n}-irr. and ann."}};

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
        if (fabs(std::stof(database::get_database_info(current_boards[0], current_status, "temp")) - 273 - 30) < 0.1)
        {
          cout << current_sensor << " - " << current_boards[0] << endl;
          cout << "temp: " << std::stof(database::get_database_info(current_boards[0], current_status, "temp")) - 273 << endl;
          cout << "current: " << TMath::Exp(current_val) << endl;
        }
      }
    }
  }

  //  Canvas with results
  TF1 *pol_1_1 = new TF1("pol_1_1", "[0]+x*[1]");
  TF1 *pol_1_2 = new TF1("pol_1_2", "[0]+x*[1]");
  TF1 *pol_1_full = new TF1("pol_1_full", "[0]+x*[1]+[2]+x*[3]");
  TCanvas *cDrawResults = new TCanvas("cDrawResults", "cDrawResults", 1000, 1000);
  gPad->SetMargin(0.15, 0.05, 0.15, 0.05);
  gPad->DrawFrame(35, -23, 50, -12);
  gPad->DrawFrame(37, -24, 49, -10, ";1/kT (eV^{-1});log of current at 4 ov.");
  TLatex *latex = new TLatex();
  latex->SetTextSize(0.025);
  latex->DrawLatexNDC(0.42, 0.20 + 4 * 0.03, "Thermal gen.");
  latex->DrawLatexNDC(0.58, 0.20 + 4 * 0.03, "Trap ass.");
  auto iTer = -1;
  for (auto [current_type, current_boards] : selected_boards)
  {
    iTer++;
    latex->SetTextColor(status_to_color[current_type]);
    latex->DrawLatexNDC(0.18, 0.20 + iTer * 0.03, Form("%s E_{act}", status_to_legend[current_type].c_str()));
  }
  iTer = -1;
  for (auto [current_type, current_boards] : selected_boards)
  {
    iTer++;
    auto current_graph_name = Form("%s_%s", current_type.c_str(), database::all_sensors[0].c_str());
    auto current_graph_name_2 = Form("%s_%s_2", current_type.c_str(), database::all_sensors[0].c_str());
    outputObjects._TGraphErrors[current_graph_name_2] = new TGraphErrors();
    outputObjects._TGraphErrors[current_graph_name]->SetMarkerStyle(state_to_marker[current_type]);
    outputObjects._TGraphErrors[current_graph_name]->SetMarkerColor(status_to_color[current_type]);
    outputObjects._TGraphErrors[current_graph_name]->SetMarkerSize(2);
    outputObjects._TGraphErrors[current_graph_name_2]->SetMarkerStyle(state_to_marker[current_type]);
    outputObjects._TGraphErrors[current_graph_name_2]->SetMarkerColor(status_to_color[current_type]);
    outputObjects._TGraphErrors[current_graph_name_2]->SetMarkerSize(2);
    outputObjects._TGraphErrors[current_graph_name]->Draw("SAME PE");
    pol_1_1->SetLineColor(status_to_color[current_type]);
    pol_1_1->SetLineStyle(kDashed);
    pol_1_1->SetParameter(0, 0);
    pol_1_1->SetRange(1, -0.5);
    pol_1_1->SetRange(30, 43);
    pol_1_2->SetLineColor(status_to_color[current_type]);
    pol_1_2->SetLineStyle(kDashed);
    pol_1_2->SetParameter(0, 0);
    pol_1_2->SetParameter(1, -0.5);
    pol_1_full->SetLineColor(status_to_color[current_type]);
    pol_1_full->SetRange(30, 49);

    outputObjects._TGraphErrors[current_graph_name]->Fit(pol_1_1, "IMRESQ0O", "", 30, 42);
    outputObjects._TGraphErrors[current_graph_name]->Fit(pol_1_2, "IMRESQ0O", "", 42, 60);

    pol_1_1->SetRange(30, 42.2);
    pol_1_1->DrawCopy("SAME");
    pol_1_2->SetRange(41.8, 60);
    pol_1_2->DrawCopy("SAME");

    latex->SetTextColor(status_to_color[current_type]);
    latex->DrawLatexNDC(0.42, 0.20 + iTer * 0.03, Form("%.2f#pm%.2f eV", -pol_1_1->GetParameter(1), pol_1_1->GetParError(1)));
    latex->DrawLatexNDC(0.58, 0.20 + iTer * 0.03, Form("%.2f#pm%.2f eV", -pol_1_2->GetParameter(1), pol_1_2->GetParError(1)));
  }

  cDrawResults->SaveAs("figures/cDrawResults.png");

  //  Write to file
  TFile *output_file = new TFile("out.root", "RECREATE");
  write_all(outputObjects);
  output_file->Close();
}