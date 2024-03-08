#include "../source/database.C"

void TIFPA_STD(std::string data_dir = "./Data/", std::string outfile = "out.root")
{
  //  Load database
  database::basedir = data_dir;
  database::read_database(database::basedir + "/database.txt");
  //  Output requested
  //  State - Sensor
  std::map<std::string, std::map<std::string, TGraphErrors *>> _TGraphErrors;
  std::map<std::string, TH1F *> _TH1F;
  //  Tag - Boards, State
  std::map<std::string, std::pair<std::vector<std::string>, std::string>> target_boards_and_state = {
      {"NEW", {database::standard_status_to_boards["NEW"], "NEW"}},
      {"TIFPA-IRR1", {database::standard_status_to_boards["TIFPA-IRR1"], "TIFPA-IRR1"}},
      {"TIFPA-OANN-STD", {{"2"}, "OANN-150-150h"}},
      {"TIFPA-SANN-ON-REV", {{"12"}, "TIFPA-IRR1"}},
      {"TIFPA-SANN-ON-FWD", {{"11"}, "TIFPA-IRR1"}},
      {"TIFPA-PANN-1", {{"3"}, "TIFPA-IRR1"}},
      {"TIFPA-PANN-2", {{"4"}, "TIFPA-IRR1"}}};
  //  Tag - Marker, color
  std::map<std::string, std::pair<int, int>> target_tag_style = {
      {"NEW", {20, kAzure - 3}},
      {"TIFPA-IRR1", {20, kGreen - 2}},
      {"TIFPA-OANN-STD", {20, kRed}},
      {"TIFPA-SANN-ON-REV", {24, kOrange + 2}},
      {"TIFPA-SANN-ON-FWD", {25, kOrange + 6}},
      {"TIFPA-PANN-1", {24, kOrange + 6}},
      {"TIFPA-PANN-2", {25, kOrange + 2}}};
  //  Tag - Legend
  std::map<std::string, std::string> tag_to_legend = {
      {"NEW", "new"},
      {"TIFPA-IRR1", "standard #it{p}-irr, 10^{9} n_{eq}"},
      {"TIFPA-OANN-STD", "standard #it{p}-irr, after oven annealing"},
      {"TIFPA-SANN-ON-REV", "standard #it{p}-irr, after online rev. ann."},
      {"TIFPA-SANN-ON-FWD", "standard #it{p}-irr, after online fwd. ann."},
      {"TIFPA-PANN-1", "standard #it{p}-irr, after prevent. oven ann."},
      {"TIFPA-PANN-2", "standard #it{p}-irr, after heavier prevent. oven ann."}};
  //
  //  Recover all standard states
  for (auto [current_tag, data] : target_boards_and_state)
  {
    _TH1F[current_tag + "_avg"] = new TH1F((current_tag + "_avg").c_str(), "", 3, 0, 3);
    _TH1F[current_tag + "_RMS"] = new TH1F((current_tag + "_RMS").c_str(), "", 3, 0, 3);
    _TH1F[current_tag + "_INC_avg"] = new TH1F((current_tag + "_INC_avg").c_str(), "", 3, 0, 3);
    _TH1F[current_tag + "_INC_RMS"] = new TH1F((current_tag + "_INC_RMS").c_str(), "", 3, 0, 3);
    _TH1F[current_tag + "_PDE_merit_avg"] = new TH1F((current_tag + "_PDE_merit_avg").c_str(), "", 3, 0, 3);
    _TH1F[current_tag + "_PDE_merit_RMS"] = new TH1F((current_tag + "_PDE_merit_RMS").c_str(), "", 3, 0, 3);
    _TH1F[current_tag + "_PDE_merit_toNEW_avg"] = new TH1F((current_tag + "_PDE_merit_avg").c_str(), "", 3, 0, 3);
    _TH1F[current_tag + "_PDE_merit_toNEW_RMS"] = new TH1F((current_tag + "_PDE_merit_RMS").c_str(), "", 3, 0, 3);
    database::set_as_main_line(_TH1F[current_tag + "_avg"], target_tag_style[current_tag].first, target_tag_style[current_tag].second);
    database::set_as_main_line(_TH1F[current_tag + "_RMS"], target_tag_style[current_tag].first, target_tag_style[current_tag].second);
    database::set_as_main_line(_TH1F[current_tag + "_INC_avg"], target_tag_style[current_tag].first, target_tag_style[current_tag].second);
    database::set_as_main_line(_TH1F[current_tag + "_INC_RMS"], target_tag_style[current_tag].first, target_tag_style[current_tag].second);
    database::set_as_main_line(_TH1F[current_tag + "_PDE_merit_avg"], target_tag_style[current_tag].first, target_tag_style[current_tag].second);
    database::set_as_main_line(_TH1F[current_tag + "_PDE_merit_RMS"], target_tag_style[current_tag].first, target_tag_style[current_tag].second);
    database::set_as_main_line(_TH1F[current_tag + "_PDE_merit_toNEW_avg"], target_tag_style[current_tag].first, target_tag_style[current_tag].second);
    database::set_as_main_line(_TH1F[current_tag + "_PDE_merit_toNEW_RMS"], target_tag_style[current_tag].first, target_tag_style[current_tag].second);
    auto iSns = -1;
    for (auto current_sensor : database::all_sensors)
    {
      iSns++;
      auto boards_list = data.first;
      auto current_state = data.second;
      // DCR vs Overvoltage graphs
      _TGraphErrors[current_tag + "_avg"][current_sensor] = database::get_dcr_overvoltage_scan<false>(boards_list, current_sensor, current_state);
      _TGraphErrors[current_tag + "_RMS"][current_sensor] = database::get_dcr_overvoltage_scan<true>(boards_list, current_sensor, current_state);
      _TGraphErrors[current_tag + "_INC_avg"][current_sensor] = database::get_dcr_increase_overvoltage_scan<false>(boards_list, current_sensor, current_state);
      _TGraphErrors[current_tag + "_INC_RMS"][current_sensor] = database::get_dcr_increase_overvoltage_scan<true>(boards_list, current_sensor, current_state);
      database::set_as_main_line(_TGraphErrors[current_tag + "_avg"][current_sensor], target_tag_style[current_tag].first, target_tag_style[current_tag].second);
      database::set_as_main_line(_TGraphErrors[current_tag + "_RMS"][current_sensor], target_tag_style[current_tag].first, target_tag_style[current_tag].second);
      database::set_as_main_line(_TGraphErrors[current_tag + "_INC_avg"][current_sensor], target_tag_style[current_tag].first, target_tag_style[current_tag].second);
      database::set_as_main_line(_TGraphErrors[current_tag + "_INC_RMS"][current_sensor], target_tag_style[current_tag].first, target_tag_style[current_tag].second);
      auto current_avg = database::measure_average(database::get_all_dcr_at_overvoltage(boards_list, current_sensor, current_state, database::DTS_OVop[current_sensor]), false);
      _TH1F[current_tag + "_avg"]->SetBinContent(iSns + 1, current_avg.first);
      _TH1F[current_tag + "_avg"]->SetBinError(iSns + 1, current_avg.second);
      current_avg = database::measure_average(database::get_all_dcr_at_overvoltage(boards_list, current_sensor, current_state, database::DTS_OVop[current_sensor]), true);
      _TH1F[current_tag + "_RMS"]->SetBinContent(iSns + 1, current_avg.first);
      _TH1F[current_tag + "_RMS"]->SetBinError(iSns + 1, current_avg.second);
      current_avg = database::measure_average(database::get_all_dcr_excess_at_overvoltage(boards_list, current_sensor, current_state, database::DTS_OVop[current_sensor]), false);
      _TH1F[current_tag + "_INC_avg"]->SetBinContent(iSns + 1, current_avg.first);
      _TH1F[current_tag + "_INC_avg"]->SetBinError(iSns + 1, current_avg.second);
      current_avg = database::measure_average(database::get_all_dcr_excess_at_overvoltage(boards_list, current_sensor, current_state, database::DTS_OVop[current_sensor]), true);
      _TH1F[current_tag + "_INC_RMS"]->SetBinContent(iSns + 1, current_avg.first);
      _TH1F[current_tag + "_INC_RMS"]->SetBinError(iSns + 1, current_avg.second);
      auto current_relative_PDE_merit = database::get_relative_PDE_merit(database::get_PDE_merit(boards_list, current_sensor, current_state, 1.e-6), database::get_PDE_merit(boards_list, database::all_sensors[0], current_state, 1.e-6));
      _TH1F[current_tag + "_PDE_merit_avg"]->SetBinContent(iSns + 1, get<0>(current_relative_PDE_merit));
      _TH1F[current_tag + "_PDE_merit_avg"]->SetBinError(iSns + 1, get<1>(current_relative_PDE_merit));
      _TH1F[current_tag + "_PDE_merit_RMS"]->SetBinContent(iSns + 1, get<0>(current_relative_PDE_merit));
      _TH1F[current_tag + "_PDE_merit_RMS"]->SetBinError(iSns + 1, get<2>(current_relative_PDE_merit));
      current_relative_PDE_merit = database::get_relative_PDE_merit(database::get_PDE_merit(boards_list, current_sensor, current_state, 1.e-6), database::get_PDE_merit(boards_list, current_sensor, "TIFPA-IRR1", 1.e-6));
      _TH1F[current_tag + "_PDE_merit_toNEW_avg"]->SetBinContent(iSns + 1, get<0>(current_relative_PDE_merit));
      _TH1F[current_tag + "_PDE_merit_toNEW_avg"]->SetBinError(iSns + 1, get<1>(current_relative_PDE_merit));
      _TH1F[current_tag + "_PDE_merit_toNEW_RMS"]->SetBinContent(iSns + 1, get<0>(current_relative_PDE_merit));
      _TH1F[current_tag + "_PDE_merit_toNEW_RMS"]->SetBinError(iSns + 1, get<2>(current_relative_PDE_merit));
    }
  }

  //  Save all to file
  TFile *fOutfile = new TFile(outfile.c_str(), "RECREATE");
  // TODO
  fOutfile->Close();

  //  Beautify
  TCanvas *cDraw_Single_Sensor = new TCanvas("cDraw_Single_Sensor", "cDraw_Single_Sensor", 1000, 1000);
  gPad->SetTopMargin(0.05);
  gPad->SetRightMargin(0.05);
  gPad->SetBottomMargin(0.10);
  gPad->SetLeftMargin(0.10);

  TLatex *cLatexSensor = new TLatex();
  cLatexSensor->SetTextFont(43);
  cLatexSensor->SetTextSize(40);

  for (auto current_sensor : database::all_sensors)
  {
    //  Absolute DCR
    auto current_frame = gPad->DrawFrame(1.0, 5.e1, 7.5, 5.e7, ";overvoltage (V);dark count rate (Hz)");
    gPad->SetLogy();
    TLegend *cLegend_DCR = new TLegend(0.11, 0.8, 0.6, 0.94);
    cLegend_DCR->SetFillStyle(0);
    cLegend_DCR->SetFillColorAlpha(0., 0.);
    cLegend_DCR->SetLineColorAlpha(0., 0.);
    for (auto current_tag : {"NEW", "TIFPA-IRR1", "TIFPA-OANN-STD"})
    {
      cLegend_DCR->AddEntry(_TGraphErrors[current_tag + std::string("_avg")][current_sensor], tag_to_legend[current_tag].c_str(), "P");
      _TGraphErrors[current_tag + std::string("_avg")][current_sensor]->Draw("SAME LPE");
      _TGraphErrors[current_tag + std::string("_RMS")][current_sensor]->SetLineColor(kRed);
      _TGraphErrors[current_tag + std::string("_RMS")][current_sensor]->Draw("SAME PE4");
    }
    cLatexSensor->DrawLatexNDC(0.56, 0.14, current_sensor.c_str());
    cLegend_DCR->Draw("same");
    cDraw_Single_Sensor->SaveAs(("DCR_" + current_sensor + ".pdf").c_str());

    //  Increase in DCR, REV e FWD
    current_frame = gPad->DrawFrame(1.0, 1.e3, 7.5, 5.e7, ";overvoltage (V);dark count rate increase (Hz)");
    gPad->SetLogy();
    TLegend *cLegend_DCR_INC = new TLegend(0.11, 0.8, 0.6, 0.94);
    cLegend_DCR_INC->SetFillStyle(0);
    cLegend_DCR_INC->SetFillColorAlpha(0., 0.);
    cLegend_DCR_INC->SetLineColorAlpha(0., 0.);
    for (auto current_tag : {"TIFPA-IRR1", "TIFPA-OANN-STD", "TIFPA-SANN-ON-REV", "TIFPA-SANN-ON-FWD"})
    {
      cLegend_DCR_INC->AddEntry(_TGraphErrors[current_tag + std::string("_avg")][current_sensor], tag_to_legend[current_tag].c_str(), "PE");
      _TGraphErrors[current_tag + std::string("_INC_avg")][current_sensor]->Draw("SAME LPE");
      _TGraphErrors[current_tag + std::string("_INC_RMS")][current_sensor]->Draw("SAME PE[]");
    }
    cLatexSensor->DrawLatexNDC(0.56, 0.14, current_sensor.c_str());
    cLegend_DCR_INC->Draw("same");
    cDraw_Single_Sensor->SaveAs(("DCR_INC_ONLINE_" + current_sensor + ".pdf").c_str());

    //  Increase in DCR, PANN
    current_frame = gPad->DrawFrame(1.0, 1.e4, 7.5, 1.e7, ";overvoltage (V);dark count rate increase (Hz)");
    gPad->SetLogy();
    TLegend *cLegend_DCR_INC_PANN = new TLegend(0.11, 0.8, 0.6, 0.94);
    cLegend_DCR_INC_PANN->SetFillStyle(0);
    cLegend_DCR_INC_PANN->SetFillColorAlpha(0., 0.);
    cLegend_DCR_INC_PANN->SetLineColorAlpha(0., 0.);
    for (auto current_tag : {"TIFPA-IRR1", "TIFPA-PANN-1", "TIFPA-PANN-2"})
    {
      cLegend_DCR_INC_PANN->AddEntry(_TGraphErrors[current_tag + std::string("_avg")][current_sensor], tag_to_legend[current_tag].c_str(), "PE");
      _TGraphErrors[current_tag + std::string("_INC_avg")][current_sensor]->Draw("SAME LPE");
      _TGraphErrors[current_tag + std::string("_INC_RMS")][current_sensor]->Draw("SAME PE[]");
    }
    cLatexSensor->DrawLatexNDC(0.56, 0.14, current_sensor.c_str());
    cLegend_DCR_INC_PANN->Draw("same");
    cDraw_Single_Sensor->SaveAs(("DCR_INC_PANN_" + current_sensor + ".pdf").c_str());
  }

  //  DCR, sensors comparison
  //  Absolute comparison
  TCanvas *cDraw_DCR_sensor_compare = new TCanvas("cDraw_DCR_sensor_compare", "cDraw_DCR_sensor_compare", 1000, 1000);
  gStyle->SetOptStat(0);
  gPad->SetTopMargin(0.05);
  gPad->SetRightMargin(0.05);
  gPad->SetBottomMargin(0.10);
  gPad->SetLeftMargin(0.10);

  TLegend *cLegend_DCR = new TLegend(0.11, 0.8, 0.6, 0.94);
  cLegend_DCR->SetFillStyle(0);
  cLegend_DCR->SetFillColorAlpha(0., 0.);
  cLegend_DCR->SetLineColorAlpha(0., 0.);

  for (auto current_tag : {"NEW", "TIFPA-IRR1", "TIFPA-OANN-STD"})
  {
    cLegend_DCR->AddEntry(_TH1F[current_tag + std::string("_avg")], tag_to_legend[current_tag].c_str(), "PE");
    _TH1F[current_tag + std::string("_avg")]->SetMaximum(5.e7);
    _TH1F[current_tag + std::string("_avg")]->SetMinimum(5.e1);
    _TH1F[current_tag + std::string("_avg")]->GetXaxis()->SetLabelSize(0.045);
    _TH1F[current_tag + std::string("_avg")]->GetYaxis()->SetTitle("dark count rate (Hz)");
    _TH1F[current_tag + std::string("_avg")]->Draw("SAME PE");
    _TH1F[current_tag + std::string("_RMS")]->Draw("SAME PE2");
  }
  gPad->SetLogy();

  cLatexSensor->DrawLatexNDC(0.15, 0.12, "V_{over} = 3V");
  cLatexSensor->DrawLatexNDC(0.42, 0.12, "V_{over} = 3V");
  cLatexSensor->DrawLatexNDC(0.69, 0.12, "V_{over} = 2.7V");
  cLatexSensor->DrawLatexNDC(0.80, 0.91, "T = -30C");

  cLegend_DCR->Draw("same");
  cDraw_DCR_sensor_compare->SaveAs("cDraw_DCR_sensor_compare.pdf");

  //  Increase comparison
  TCanvas *cDraw_DCR_INC_sensor_compare = new TCanvas("cDraw_DCR_INC_sensor_compare", "cDraw_DCR_INC_sensor_compare", 1000, 1000);
  gStyle->SetOptStat(0);
  gPad->SetTopMargin(0.05);
  gPad->SetRightMargin(0.05);
  gPad->SetBottomMargin(0.10);
  gPad->SetLeftMargin(0.10);

  TLegend *cLegend_DCR_INC = new TLegend(0.11, 0.8, 0.6, 0.94);
  cLegend_DCR_INC->SetFillStyle(0);
  cLegend_DCR_INC->SetFillColorAlpha(0., 0.);
  cLegend_DCR_INC->SetLineColorAlpha(0., 0.);

  for (auto current_tag : {"TIFPA-IRR1", "TIFPA-OANN-STD"})
  {
    cLegend_DCR_INC->AddEntry(_TH1F[current_tag + std::string("_avg")], tag_to_legend[current_tag].c_str(), "PE");
    _TH1F[current_tag + std::string("_INC_avg")]->SetMaximum(1.e7);
    _TH1F[current_tag + std::string("_INC_avg")]->SetMinimum(5.e3);
    _TH1F[current_tag + std::string("_INC_avg")]->GetXaxis()->SetLabelSize(0.045);
    _TH1F[current_tag + std::string("_INC_avg")]->GetYaxis()->SetTitle("dark count rate increase (Hz)");
    _TH1F[current_tag + std::string("_INC_avg")]->Draw("SAME PE");
    _TH1F[current_tag + std::string("_INC_RMS")]->Draw("SAME PE2");
  }
  gPad->SetLogy();

  cLatexSensor->DrawLatexNDC(0.15, 0.12, "V_{over} = 3V");
  cLatexSensor->DrawLatexNDC(0.42, 0.12, "V_{over} = 3V");
  cLatexSensor->DrawLatexNDC(0.69, 0.12, "V_{over} = 2.7V");
  cLatexSensor->DrawLatexNDC(0.80, 0.91, "T = -30C");

  cLegend_DCR_INC->Draw("same");
  cDraw_DCR_INC_sensor_compare->SaveAs("cDraw_DCR_INC_sensor_compare.pdf");

  //  Increase comparison for PANN
  TCanvas *cDraw_DCR_INC_PANN_sensor_compare = new TCanvas("cDraw_DCR_INC_PANN_sensor_compare", "cDraw_DCR_INC_PANN_sensor_compare", 1000, 1000);
  gStyle->SetOptStat(0);
  gPad->SetTopMargin(0.05);
  gPad->SetRightMargin(0.05);
  gPad->SetBottomMargin(0.10);
  gPad->SetLeftMargin(0.10);

  TLegend *cLegend_DCR_INC_PANN = new TLegend(0.11, 0.8, 0.6, 0.94);
  cLegend_DCR_INC_PANN->SetFillStyle(0);
  cLegend_DCR_INC_PANN->SetFillColorAlpha(0., 0.);
  cLegend_DCR_INC_PANN->SetLineColorAlpha(0., 0.);

  for (auto current_tag : {"TIFPA-IRR1", "TIFPA-PANN-1", "TIFPA-PANN-2"})
  {
    cLegend_DCR_INC_PANN->AddEntry(_TH1F[current_tag + std::string("_avg")], tag_to_legend[current_tag].c_str(), "PE");
    _TH1F[current_tag + std::string("_INC_avg")]->SetMaximum(1.e7);
    _TH1F[current_tag + std::string("_INC_avg")]->SetMinimum(5.e3);
    _TH1F[current_tag + std::string("_INC_avg")]->GetXaxis()->SetLabelSize(0.045);
    _TH1F[current_tag + std::string("_INC_avg")]->GetYaxis()->SetTitle("dark count rate increase (Hz)");
    _TH1F[current_tag + std::string("_INC_avg")]->Draw("SAME PE");
    _TH1F[current_tag + std::string("_INC_RMS")]->Draw("SAME PE2");
  }
  gPad->SetLogy();

  cLatexSensor->DrawLatexNDC(0.15, 0.12, "V_{over} = 3V");
  cLatexSensor->DrawLatexNDC(0.42, 0.12, "V_{over} = 3V");
  cLatexSensor->DrawLatexNDC(0.69, 0.12, "V_{over} = 2.7V");
  cLatexSensor->DrawLatexNDC(0.80, 0.91, "T = -30C");

  cLegend_DCR_INC_PANN->Draw("same");
  cDraw_DCR_INC_PANN_sensor_compare->SaveAs("cDraw_DCR_INC_PANN_sensor_compare.pdf");

  //  Increase comparison w/ online annealing
  TCanvas *cDraw_DCR_INC_ONLINE_sensor_compare = new TCanvas("cDraw_DCR_INC_ONLINE_sensor_compare", "cDraw_DCR_INC_ONLINE_sensor_compare", 1000, 1000);
  gStyle->SetOptStat(0);
  gPad->SetTopMargin(0.05);
  gPad->SetRightMargin(0.05);
  gPad->SetBottomMargin(0.10);
  gPad->SetLeftMargin(0.10);

  TLegend *cLegend_DCR_INC_ONLINE = new TLegend(0.11, 0.8, 0.6, 0.94);
  cLegend_DCR_INC_ONLINE->SetFillStyle(0);
  cLegend_DCR_INC_ONLINE->SetFillColorAlpha(0., 0.);
  cLegend_DCR_INC_ONLINE->SetLineColorAlpha(0., 0.);

  for (auto current_tag : {"TIFPA-OANN-STD", "TIFPA-IRR1", "TIFPA-SANN-ON-REV", "TIFPA-SANN-ON-FWD"})
  {
    cLegend_DCR_INC_ONLINE->AddEntry(_TH1F[current_tag + std::string("_avg")], tag_to_legend[current_tag].c_str(), "PE");
    _TH1F[current_tag + std::string("_INC_avg")]->SetMaximum(1.e7);
    _TH1F[current_tag + std::string("_INC_avg")]->SetMinimum(5.e3);
    _TH1F[current_tag + std::string("_INC_avg")]->GetXaxis()->SetLabelSize(0.045);
    _TH1F[current_tag + std::string("_INC_avg")]->GetYaxis()->SetTitle("dark count rate increase (Hz)");
    _TH1F[current_tag + std::string("_INC_avg")]->Draw("SAME PE");
    _TH1F[current_tag + std::string("_INC_RMS")]->Draw("SAME PE2");
  }
  gPad->SetLogy();

  cLatexSensor->DrawLatexNDC(0.15, 0.12, "V_{over} = 3V");
  cLatexSensor->DrawLatexNDC(0.42, 0.12, "V_{over} = 3V");
  cLatexSensor->DrawLatexNDC(0.69, 0.12, "V_{over} = 2.7V");
  cLatexSensor->DrawLatexNDC(0.80, 0.91, "T = -30C");

  cLegend_DCR_INC_ONLINE->Draw("same");
  cDraw_DCR_INC_ONLINE_sensor_compare->SaveAs("cDraw_DCR_INC_ONLINE_sensor_compare.pdf");

  //  PDE figure of merit
  //  Absolute comparison
  TCanvas *cDraw_DCR_PDE_sensor_compare = new TCanvas("cDraw_DCR_PDE_sensor_compare", "cDraw_DCR_PDE_sensor_compare", 1000, 1000);
  gStyle->SetOptStat(0);
  gPad->SetTopMargin(0.05);
  gPad->SetRightMargin(0.05);
  gPad->SetBottomMargin(0.10);
  gPad->SetLeftMargin(0.12);

  TLegend *cLegend_DCR_PDE = new TLegend(0.11, 0.8, 0.6, 0.94);
  cLegend_DCR_PDE->SetFillStyle(0);
  cLegend_DCR_PDE->SetFillColorAlpha(0., 0.);
  cLegend_DCR_PDE->SetLineColorAlpha(0., 0.);

  for (auto current_tag : {"NEW", "TIFPA-IRR1", "TIFPA-OANN-STD"})
  {
    cLegend_DCR_PDE->AddEntry(_TH1F[current_tag + std::string("_avg")], tag_to_legend[current_tag].c_str(), "PE2");
    _TH1F[current_tag + std::string("_PDE_merit_avg")]->SetMaximum(2.5);
    _TH1F[current_tag + std::string("_PDE_merit_avg")]->SetMinimum(-1.8);
    _TH1F[current_tag + std::string("_PDE_merit_avg")]->GetXaxis()->SetLabelSize(0.045);
    _TH1F[current_tag + std::string("_PDE_merit_avg")]->GetYaxis()->SetTitleOffset(1.6);
    _TH1F[current_tag + std::string("_PDE_merit_avg")]->GetYaxis()->SetTitle("figure of merit");
    _TH1F[current_tag + std::string("_PDE_merit_avg")]->Draw("SAME PE");
    _TH1F[current_tag + std::string("_PDE_merit_RMS")]->Draw("SAME PE2");
  }

  cLatexSensor->DrawLatexNDC(0.15, 0.12, "V_{over} = 3V");
  cLatexSensor->DrawLatexNDC(0.42, 0.12, "V_{over} = 3V");
  cLatexSensor->DrawLatexNDC(0.69, 0.12, "V_{over} = 2.7V");
  cLatexSensor->DrawLatexNDC(0.15, 0.16, "PDE = 40%");
  cLatexSensor->DrawLatexNDC(0.42, 0.16, "PDE = 50%");
  cLatexSensor->DrawLatexNDC(0.69, 0.16, "PDE = 50%");
  cLatexSensor->DrawLatexNDC(0.80, 0.91, "T = -30C");

  cLegend_DCR_PDE->Draw("same");
  cDraw_DCR_PDE_sensor_compare->SaveAs("cDraw_DCR_PDE_sensor_compare.pdf");
}
