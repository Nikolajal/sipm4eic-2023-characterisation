#include "../source/database.C"

void Plots_for_Roberto(std::string data_dir = "./Data/", std::string outfile = "out.root", std::string channel = "A1")
{
  //  Load database
  database::basedir = data_dir;
  database::read_database(database::basedir + "/database.txt");

  //  Make the requested graphs
  std::map<std::string, TGraphErrors *> _TGraphErrors;
  std::vector<std::string> list_of_temp_state = {"TEMP303", "TEMP293", "TEMP283", "TEMP273", "TEMP263", "TEMP253"};
  _TGraphErrors["A_NEW"] = new TGraphErrors();
  _TGraphErrors["C_NEW"] = new TGraphErrors();
  _TGraphErrors["A_IRR"] = new TGraphErrors();
  _TGraphErrors["C_IRR"] = new TGraphErrors();
  for (auto current_state : list_of_temp_state)
  {
    auto current_temp = std::stof(database::get_database_info("31", current_state, "temp"));
    std::vector<std::string> new_boards = {"31", "32"};
    auto overvoltage = 3.;
    auto current_point = database::measure_average(database::get_all_sensor_Yvalues_at_X<database::get_iv_scan>(new_boards, database::all_sensors[0], current_state, database::sensor_to_vbd[database::all_sensors[0]] + overvoltage + 0.054 * (current_temp - 243)));
    auto iPoint = _TGraphErrors["A_NEW"]->GetN();
    _TGraphErrors["A_NEW"]->SetPoint(iPoint, current_temp, current_point.first);
    _TGraphErrors["A_NEW"]->SetPointError(iPoint, 0, current_point.second);
    current_point = database::measure_average(database::get_all_sensor_Yvalues_at_X<database::get_iv_scan>(new_boards, database::all_sensors[2], current_state, database::sensor_to_vbd[database::all_sensors[2]] + overvoltage + 0.034 * (current_temp - 243)));
    iPoint = _TGraphErrors["C_NEW"]->GetN();
    _TGraphErrors["C_NEW"]->SetPoint(iPoint, current_temp, current_point.first);
    _TGraphErrors["C_NEW"]->SetPointError(iPoint, 0, current_point.second);

    new_boards = {"15", "16"};
    current_point = database::measure_average(database::get_all_sensor_Yvalues_at_X<database::get_iv_scan>(new_boards, database::all_sensors[0], current_state, database::sensor_to_vbd[database::all_sensors[0]] + overvoltage + 0.054 * (current_temp - 243)));
    iPoint = _TGraphErrors["A_IRR"]->GetN();
    _TGraphErrors["A_IRR"]->SetPoint(iPoint, current_temp, current_point.first);
    _TGraphErrors["A_IRR"]->SetPointError(iPoint, 0, current_point.second);
    current_point = database::measure_average(database::get_all_sensor_Yvalues_at_X<database::get_iv_scan>(new_boards, database::all_sensors[2], current_state, database::sensor_to_vbd[database::all_sensors[2]] + overvoltage + 0.034 * (current_temp - 243)));
    iPoint = _TGraphErrors["C_IRR"]->GetN();
    _TGraphErrors["C_IRR"]->SetPoint(iPoint, current_temp, current_point.first);
    _TGraphErrors["C_IRR"]->SetPointError(iPoint, 0, current_point.second);
  }
  TCanvas *c1 = new TCanvas("c1", "c1", 1000, 1000);
  gPad->SetLogy();
  gPad->SetGridx();
  gPad->SetRightMargin(0.05);
  gPad->SetTopMargin(0.05);
  gPad->SetLeftMargin(0.12);
  gPad->SetBottomMargin(0.12);
  c1->cd()->DrawFrame(245, 5.e-10, 315, 5.e-5, ";temperature (K);dark current (A)");
  _TGraphErrors["A_NEW"]->Draw("SAME LPE");
  _TGraphErrors["C_NEW"]->Draw("SAME LPE");
  _TGraphErrors["A_IRR"]->Draw("SAME LPE");
  _TGraphErrors["C_IRR"]->Draw("SAME LPE");
  //  Set markers
  _TGraphErrors["A_NEW"]->SetMarkerStyle(20);
  _TGraphErrors["C_NEW"]->SetMarkerStyle(21);
  _TGraphErrors["A_IRR"]->SetMarkerStyle(24);
  _TGraphErrors["C_IRR"]->SetMarkerStyle(25);
  _TGraphErrors["A_NEW"]->SetMarkerSize(2);
  _TGraphErrors["C_NEW"]->SetMarkerSize(2);
  _TGraphErrors["A_IRR"]->SetMarkerSize(2);
  _TGraphErrors["C_IRR"]->SetMarkerSize(2);
  //  Set markers
  _TGraphErrors["A_NEW"]->SetMarkerColor(kBlue + 1);
  _TGraphErrors["C_NEW"]->SetMarkerColor(kRed + 1);
  _TGraphErrors["A_IRR"]->SetMarkerColor(kBlue + 1);
  _TGraphErrors["C_IRR"]->SetMarkerColor(kRed + 1);

  c1->SaveAs("c1.pdf");

  TFile *cout = new TFile("outRoberto.root", "RECREATE");
  c1->Write();
  _TGraphErrors["A_NEW"]->Write("A_NEW");
  _TGraphErrors["C_NEW"]->Write("C_NEW");
  _TGraphErrors["A_IRR"]->Write("A_IRR");
  _TGraphErrors["C_IRR"]->Write("C_IRR");
  cout->Close();
}