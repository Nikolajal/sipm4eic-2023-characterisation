#include "../source/database.C"

void my_test_macro_on_CT_meas(std::string data_dir = "./Data/", std::string outfile = "out.root")
{
  //  Load database
  database::basedir = data_dir;
  database::read_database(database::basedir + "/database.txt");
  //  Output requested
  std::map<std::string, TH1F *> _TH1F;
  std::map<std::string, TGraphErrors *> _TGraphErrors;
  std::map<std::string, std::pair<std::string, std::vector<std::string>>> states_to_loop_on = {
      {"NEW", {"NEW", database::standard_status_to_boards["NEW"]}},
      {"PIRR1", {"TIFPA-IRR1", database::standard_status_to_boards["TIFPA-IRR1"]}},
      {"NIRR1", {"LNL-IRR1", database::standard_status_to_boards["LNL-IRR1"]}},
      {"PIRR1_138", {"TIFPA-IRR1", {"5"}}},
      {"PIRR1_075", {"TIFPA-IRR1", {"6"}}},
      {"PIRR1_045", {"TIFPA-IRR1", {"8"}}},
      {"PIRR1_025", {"TIFPA-IRR1", {"9"}}},
      {"PIRR1_018", {"TIFPA-IRR1", {"10"}}}};
  for (auto [state_tag, state_vector] : states_to_loop_on)
  {
    for (auto [sensor, sensor_tag] : database::sensor_to_code)
    {
      std::string histo_tag = Form("hCT_%s_%s", sensor_tag.c_str(), state_tag.c_str());
      std::string histo_title = Form("Cross-talk of %s for state %s;cross-talk probability (%);entries", sensor.c_str(), state_vector.first.c_str());
      _TH1F[histo_tag] = new TH1F(histo_tag.c_str(), histo_title.c_str(), 24, 0., 12);
    }
  }
  //  Fill all histograms
  gROOT->SetBatch(true);
  for (auto [state_tag, state_vector] : states_to_loop_on)
  {
    for (auto current_board : state_vector.second)
    {
      for (auto [current_sensor, sensor_tag] : database::sensor_to_code)
      {
        for (auto current_channel : database::sensor_to_channels[current_sensor])
        {
          auto current_state = state_vector.first;
          std::string histo_tag = Form("hCT_%s_%s", sensor_tag.c_str(), state_tag.c_str());
          auto current_curve = database::get_dcr_threshold_scan(current_board, current_channel, current_state);
          if (!current_curve)
            continue;
          _TH1F[histo_tag]->Fill(100 * database::measure_cross_talk(current_curve /*, Form("./plots/hCT_%s_%s_%s", current_board.c_str(), current_channel.c_str(), current_state.c_str())*/)[0 ]);
        }
      }
    }
  }
  gROOT->SetBatch(false);
  //  Building Final TH1Fs
  for (auto [state_tag, state_vector] : states_to_loop_on)
  {
    std::string histo_title = Form("Cross-talk for state %s;;cross-talk probability (\%)", state_vector.first.c_str());
    _TH1F[Form("%s_avg", state_tag.c_str())] = new TH1F(state_tag.c_str(), histo_title.c_str(), 3, 0., 3);
    _TH1F[Form("%s_RMS", state_tag.c_str())] = new TH1F(state_tag.c_str(), histo_title.c_str(), 3, 0., 3);
    auto iSns = 0;
    for (auto [sensor, sensor_tag] : database::sensor_to_code)
    {
      iSns++;
      std::string histo_tag = Form("hCT_%s_%s", sensor_tag.c_str(), state_tag.c_str());
      _TH1F[Form("%s_avg", state_tag.c_str())]->SetBinContent(iSns, _TH1F[histo_tag]->GetMean());
      _TH1F[Form("%s_avg", state_tag.c_str())]->SetBinError(iSns, _TH1F[histo_tag]->GetMeanError());
      _TH1F[Form("%s_RMS", state_tag.c_str())]->SetBinContent(iSns, _TH1F[histo_tag]->GetMean());
      _TH1F[Form("%s_RMS", state_tag.c_str())]->SetBinError(iSns, _TH1F[histo_tag]->GetRMS());
    }
  }
  //  Output file
  TFile *fOutfile = new TFile(outfile.c_str(), "RECREATE");
  for (auto [tag, object] : _TH1F)
    object->Write(tag.c_str());
  for (auto [tag, object] : _TGraphErrors)
    object->Write(tag.c_str());
  fOutfile->Close();

  //  --- Beautify

  //  proton scan results
  for (auto [state_tag, state_vector] : states_to_loop_on)
  {
    if (state_tag.find("PIRR1_") == std::string::npos)
      continue;
    std::string new_state_tag(state_tag);
    new_state_tag.erase(0, 6);
    auto current_energy = atoi(new_state_tag.c_str());
    for (auto iSns = 1; iSns <= 3; iSns++)
    {
      auto current_sensor = database::all_sensors[iSns - 1];
      if (!_TGraphErrors[Form("gSensors_avg_%i", iSns)])
      {
        _TGraphErrors[Form("gSensors_avg_%i", iSns)] = new TGraphErrors();
        _TGraphErrors[Form("gSensors_avg_%i", iSns)]->SetMarkerColor(database::sensor_to_color[current_sensor]);
        _TGraphErrors[Form("gSensors_avg_%i", iSns)]->SetMarkerSize(1.3);
        _TGraphErrors[Form("gSensors_avg_%i", iSns)]->SetMarkerStyle(database::sensor_to_marker[current_sensor]);
        _TGraphErrors[Form("gSensors_avg_%i", iSns)]->SetLineColor(database::sensor_to_color[current_sensor]);
        _TGraphErrors[Form("gSensors_avg_%i", iSns)]->SetFillColorAlpha(database::sensor_to_color[current_sensor], 0.3);
        _TGraphErrors[Form("gSensors_avg_%i", iSns)]->SetFillStyle(0);
      }
      if (!_TGraphErrors[Form("gSensors_RMS_%i", iSns)])
      {
        _TGraphErrors[Form("gSensors_RMS_%i", iSns)] = new TGraphErrors();
        _TGraphErrors[Form("gSensors_RMS_%i", iSns)]->SetMarkerColor(database::sensor_to_color[current_sensor]);
        _TGraphErrors[Form("gSensors_RMS_%i", iSns)]->SetMarkerSize(1.3);
        _TGraphErrors[Form("gSensors_RMS_%i", iSns)]->SetMarkerStyle(database::sensor_to_marker[current_sensor]);
        _TGraphErrors[Form("gSensors_RMS_%i", iSns)]->SetLineColor(database::sensor_to_color[current_sensor]);
        _TGraphErrors[Form("gSensors_RMS_%i", iSns)]->SetFillColorAlpha(database::sensor_to_color[current_sensor], 0.3);
        _TGraphErrors[Form("gSensors_RMS_%i", iSns)]->SetFillStyle(0);
      }
      auto current_point = _TGraphErrors[Form("gSensors_avg_%i", iSns)]->GetN();
      _TGraphErrors[Form("gSensors_avg_%i", iSns)]->SetPoint(current_point, current_energy, _TH1F[Form("%s_avg", state_tag.c_str())]->GetBinContent(iSns));
      _TGraphErrors[Form("gSensors_avg_%i", iSns)]->SetPointError(current_point, 0, _TH1F[Form("%s_avg", state_tag.c_str())]->GetBinError(iSns));
      _TGraphErrors[Form("gSensors_RMS_%i", iSns)]->SetPoint(current_point, current_energy, _TH1F[Form("%s_RMS", state_tag.c_str())]->GetBinContent(iSns));
      _TGraphErrors[Form("gSensors_RMS_%i", iSns)]->SetPointError(current_point, 0, _TH1F[Form("%s_RMS", state_tag.c_str())]->GetBinError(iSns));
    }
  }

  //  TLegend
  TLegend *legend_proton_scan = new TLegend(0.5, 0.12, 0.88, 0.35);
  legend_proton_scan->SetFillColorAlpha(0., 0.);
  legend_proton_scan->SetLineColorAlpha(0., 0.);
  legend_proton_scan->AddEntry(_TGraphErrors["gSensors_avg_1"], database::all_sensors[0].c_str(), "EP");
  legend_proton_scan->AddEntry(_TGraphErrors["gSensors_avg_2"], database::all_sensors[1].c_str(), "EP");
  legend_proton_scan->AddEntry(_TGraphErrors["gSensors_avg_3"], database::all_sensors[2].c_str(), "EP");

  //  Plot
  TCanvas *c1 = new TCanvas();
  gPad->DrawFrame(15, 0, 145, 14, ";proton energy (MeV);cross-talk probability (%)");
  for (auto iSns = 1; iSns <= 3; iSns++)
  {
    _TGraphErrors[Form("gSensors_avg_%i", iSns)]->Draw("SAME PE2");
    _TGraphErrors[Form("gSensors_RMS_%i", iSns)]->Draw("SAME PE[]");
  }
  legend_proton_scan->Draw("SAME");
  c1->SaveAs("CT_proton_en_scan.pdf");

  //  TIFPA and LNL results

  //  --- TLegend
  TLegend *legend_pirr_nirr = new TLegend(0.5, 0.12, 0.88, 0.35);
  legend_pirr_nirr->SetFillColorAlpha(0., 0.);
  legend_pirr_nirr->SetLineColorAlpha(0., 0.);

  //  --- Plot
  TCanvas *canvas_pirr_nirr = new TCanvas();
  gStyle->SetOptStat(0);

  _TH1F["NEW_avg"]->SetTitle("");
  _TH1F["NEW_avg"]->GetXaxis()->SetBinLabel(1, database::all_sensors[0].c_str());
  _TH1F["NEW_avg"]->GetXaxis()->SetBinLabel(2, database::all_sensors[1].c_str());
  _TH1F["NEW_avg"]->GetXaxis()->SetBinLabel(3, database::all_sensors[2].c_str());
  _TH1F["NEW_avg"]->SetMaximum(10);
  _TH1F["NEW_avg"]->SetMinimum(2);
  _TH1F["NEW_avg"]->Draw("PE");

  auto iSns = 0;
  for (auto [state_tag, state_vector] : states_to_loop_on)
  {
    if (state_tag.find("PIRR1_") != std::string::npos)
      continue;
    iSns++;
    auto current_sensor = database::all_sensors[iSns - 1];
    _TH1F[Form("%s_avg", state_tag.c_str())]->SetMarkerColor(database::status_to_color[state_vector.first]);
    _TH1F[Form("%s_avg", state_tag.c_str())]->SetMarkerSize(1.3);
    _TH1F[Form("%s_avg", state_tag.c_str())]->SetMarkerStyle(database::sensor_to_marker[current_sensor]);
    _TH1F[Form("%s_avg", state_tag.c_str())]->SetLineColor(database::status_to_color[state_vector.first]);
    _TH1F[Form("%s_avg", state_tag.c_str())]->SetFillColorAlpha(database::status_to_color[state_vector.first], 0.3);
    _TH1F[Form("%s_avg", state_tag.c_str())]->SetFillStyle(0);

    _TH1F[Form("%s_RMS", state_tag.c_str())]->SetMarkerColor(database::status_to_color[state_vector.first]);
    _TH1F[Form("%s_RMS", state_tag.c_str())]->SetMarkerSize(1.3);
    _TH1F[Form("%s_RMS", state_tag.c_str())]->SetMarkerStyle(database::sensor_to_marker[current_sensor]);
    _TH1F[Form("%s_RMS", state_tag.c_str())]->SetLineColor(database::status_to_color[state_vector.first]);
    _TH1F[Form("%s_RMS", state_tag.c_str())]->SetFillColorAlpha(database::status_to_color[state_vector.first], 0.3);
    _TH1F[Form("%s_RMS", state_tag.c_str())]->SetFillStyle(0);

    legend_pirr_nirr->AddEntry(_TH1F[Form("%s_avg", state_tag.c_str())], state_vector.first.c_str(), "EP");

    _TH1F[Form("%s_avg", state_tag.c_str())]->Draw("SAME PE");
    _TH1F[Form("%s_RMS", state_tag.c_str())]->Draw("SAME PE2");
  }
  legend_pirr_nirr->Draw("SAME");
  canvas_pirr_nirr->SaveAs("CT_result_pirr_nirr.pdf");
}
