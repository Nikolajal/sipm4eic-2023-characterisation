#include "../source/database.C"

void my_test_macro_on_DCR(std::string data_dir = "./Data/", std::string outfile = "out.root")
{
  //  Load database
  database::basedir = data_dir;
  database::read_database(database::basedir + "/database.txt");
  //  Output requested
  std::map<std::string, TH1F *> _TH1F;
  std::map<std::string, TH2F *> _TH2F;
  std::map<std::string, TGraph *> _TGraph;
  std::map<std::string, TGraphErrors *> _TGraphErrors;
  std::map<std::string, std::pair<std::string, std::vector<std::string>>> states_to_loop_on = {
      /* {"NEW", {"NEW", database::standard_status_to_boards["NEW"]}},*/
      {"PIRR1_1e9", {"TIFPA-IRR1", database::standard_status_to_boards["TIFPA-IRR1"]}},
      {"NIRR1_1e9", {"LNL-IRR1", database::standard_status_to_boards["LNL-IRR1"]}},
      {"NIRR1_1e10", {"LNL-IRR1", {"19"}}},
      {"NIRR1_5e9", {"LNL-IRR1", {"20"}}},
      {"NIRR1_1e8", {"LNL-IRR1", {"26"}}},
      {"PANN_1e9", {"OANN-150-150h", {"2"}}},
      {"NANN_1e9", {"OANN-150-150h", {"25"}}},
      {"NANN_1e10", {"OANN-150-150h", {"19"}}},
      {"NANN_5e9", {"OANN-150-150h", {"20"}}},
      {"NANN_1e8", {"OANN-150-150h", {"26"}}}};
  std::map<std::string, float> irradiation_map = {
      {"NEW", 10.},
      {"PIRR1_1e9", 1.e9},
      {"NIRR1_1e9", 1.e9},
      {"NIRR1_1e10", 1.e10},
      {"NIRR1_5e9", 5.e9},
      {"NIRR1_1e8", 1.e8},
      {"PANN_1e9", 1.e9},
      {"NANN_1e9", 1.e9},
      {"NANN_1e10", 1.e10},
      {"NANN_5e9", 5.e9},
      {"NANN_1e8", 1.e8}};
  //  Calculate DCR @ given overvoltage
  //  [NEW, PIRR, NIRR] - Sensor
  std::map<std::string, std::map<std::string, database::mkgraph_data_structure>> all_graphs_data;
  //  [NEW, PIRR, NIRR] - Sensor
  std::map<std::string, std::map<std::string, std::vector<TGraphErrors *>>> all_graphs;
  for (auto [state_tag, state_vector] : states_to_loop_on)
  {
    for (auto current_board : state_vector.second)
    {
      for (auto [current_sensor, sensor_tag] : database::sensor_to_code)
      {
        auto current_state = state_vector.first;
        auto current_irrlvl = irradiation_map[state_tag];
        std::string current_histo = "NEW";
        if (state_tag.find("PIRR1") != std::string::npos)
          current_histo = "PIRR";
        if (state_tag.find("NIRR1") != std::string::npos)
          current_histo = "NIRR";
        if (state_tag.find("PANN") != std::string::npos)
          current_histo = "PANN";
        if (state_tag.find("NANN") != std::string::npos)
          current_histo = "NANN";
        auto current_data_vector = database::get_all_iv_at_overvoltage(current_board, current_sensor, current_state, database::DTS_OVop[current_sensor]);
        for (auto iTer : current_data_vector)
          all_graphs_data[current_histo][sensor_tag][{current_irrlvl, 0.}].push_back(iTer);
      }
    }
  }

  //  Loop on graph data
  std::map<std::string, std::string> kTag_to_color = {
      {"NEW", "NEW"},
      {"PIRR", "TIFPA-IRR1"},
      {"NIRR", "LNL-IRR1"},
      {"PANN", "OANN-150-150h"},
      {"NANN", "OANN-150-150h"}};
  std::map<std::string, std::string> kTag_to_marker = {
      {"NEW", "NEW"},
      {"PIRR", "TIFPA-IRR1"},
      {"NIRR", "LNL-IRR1"},
      {"PANN", "TIFPA-IRR1"},
      {"NANN", "LNL-IRR1"}};
  for (auto &[graph_tag, all_sensors_graph_data] : all_graphs_data)
  {
    for (auto &[sensor_tag, graph_data] : all_sensors_graph_data)
    {
      for (auto &[x_coordinate, y_coordinates] : graph_data)
      {
        auto all_avg = database::measure_average(y_coordinates, true);
        auto all_RMS = database::measure_average(y_coordinates, false);
        y_coordinates.insert(y_coordinates.begin(), all_RMS);
        y_coordinates.insert(y_coordinates.begin(), all_avg);
      }
      all_graphs[graph_tag][sensor_tag] = database::make_graph(graph_data);
      database::set_multi_graph(all_graphs[graph_tag][sensor_tag], database::status_to_color[kTag_to_color[graph_tag]], database::status_to_marker[kTag_to_marker[graph_tag]]);
    }
  }

  //  Save all to file
  TFile *fOutfile = new TFile(outfile.c_str(), "RECREATE");
  for (auto [tag, object] : _TH1F)
    object->Write(tag.c_str());
  for (auto [tag, object] : _TH2F)
    object->Write(tag.c_str());
  for (auto [tag, object] : _TGraph)
    object->Write(tag.c_str());
  for (auto [tag, object] : _TGraphErrors)
    object->Write(tag.c_str());
  fOutfile->Close();

  //  Beautify to plot
  std::map<std::string, std::string> kCode_to_legend = {
      {"NEW", "New"},
      {"PIRR", "#it{p}-irradiated"},
      {"NIRR", "#it{n}-irradiated"},
      {"PANN", "#it{p}-irradiated and oven annealed"},
      {"NANN", "#it{n}-irradiated and oven annealed"},
  };
  TLegend *lDraw_sensor = new TLegend(0.10, 0.7, 0.6, 0.88);
  lDraw_sensor->SetFillColorAlpha(0., 0.);
  lDraw_sensor->SetLineColorAlpha(0., 0.);
  std::map<std::string, TCanvas *> cDraw_sensor;
  for (auto &[graph_tag, all_sensors_graphs] : all_graphs)
  {
    lDraw_sensor->AddEntry(all_sensors_graphs[database::sensor_to_code[database::all_sensors[0]]][0], kCode_to_legend[graph_tag].c_str(), "EP");
    for (auto &[sensor_tag, current_graphs_vector] : all_sensors_graphs)
    {
      if (!cDraw_sensor[sensor_tag])
      {
        TLatex *cWriteSensor = new TLatex();
        cDraw_sensor[sensor_tag] = new TCanvas(sensor_tag.c_str(), sensor_tag.c_str(), 1000, 1000);
        gPad->SetTopMargin(0.05);
        gPad->SetRightMargin(0.05);
        gPad->SetBottomMargin(0.1);
        gPad->SetLeftMargin(0.1);
        auto current_frame = gPad->DrawFrame(5.e7, 5.e-10, 5.e10, 5.e-5, ";Irradiation level (1-MeV n_{eq});dark current increase (A)");
        current_frame->GetXaxis()->SetTitleOffset(1.35);
        current_frame->GetYaxis()->SetTitleOffset(1.4);
        gPad->SetLogx();
        gPad->SetLogy();
        cWriteSensor->SetTextFont(43);
        cWriteSensor->SetTextSize(40);
        cWriteSensor->DrawLatexNDC(0.14, 0.9, database::code_to_sensor[sensor_tag].c_str());
        cWriteSensor->DrawLatexNDC(0.75, 0.91, "T = -30C");
        cWriteSensor->DrawLatexNDC(0.75, 0.87, Form("V_{over} = %.1fV", database::DTS_OVop[database::code_to_sensor[sensor_tag]]));
        lDraw_sensor->Draw("SAME");
      }
      if (graph_tag.find("NEW") != std::string::npos)
        continue;
      cDraw_sensor[sensor_tag]->cd();
      for (auto current_graph : current_graphs_vector)
        current_graph->Draw("SAME PE");
      current_graphs_vector[0]->SetMarkerSize(2.5);
      current_graphs_vector[1]->SetMarkerSize(2.5);
      current_graphs_vector[0]->Draw("SAME PE");
      current_graphs_vector[1]->Draw("SAME PE[]");

      TF1 *pol1 = new TF1("pol1", "pol1");
      pol1->FixParameter(0, 0.);
      if (graph_tag.find("NIRR") != std::string::npos)
      {
        pol1->SetLineColor(kSpring - 7);
        current_graphs_vector[0]->Fit(pol1);
      }

      if (graph_tag.find("NANN") != std::string::npos)
      {
        pol1->SetLineColor(kRed - 3);
        current_graphs_vector[0]->Fit(pol1);
      }
    }
  }
  for (auto [sensor_tag, current_canvas] : cDraw_sensor)
    current_canvas->SaveAs(Form("%s.pdf", sensor_tag.c_str()));
}
