#include "../source/database.C"

void annealing_excursus(std::string data_dir = "./Data/", std::string outfile = "out.root")
{
  //  Load database
  database::basedir = data_dir;
  database::read_database(database::basedir + "/database.txt");

  //  Output structure
  std::map<std::string, TH1F *> _TH1F;
  std::map<std::string, TH2F *> _TH2F;
  std::map<std::string, TGraph *> _TGraph;
  std::map<std::string, TGraphErrors *> _TGraphErrors;
  std::vector<std::any> _all_objects;
  _all_objects.push_back(_TH1F);
  _all_objects.push_back(_TH2F);
  _all_objects.push_back(_TGraphErrors);

  //  Output requested
  std::map<std::string, std::vector<std::string>> target_boards_and_states = {
      {"13", {"SANN-100-30m", "SANN-100-2.5h", "SANN-100-24h", "SANN-125-30m", "SANN-125-2.5h", "SANN-125-24h", "SANN-150-30m", "SANN-150-2.5h", "SANN-150-24h", "SANN-150-73h", "SANN-150-250h"}},
      {"14", {"SANN-100-30m", "SANN-100-2.5h", "SANN-100-24h", "SANN-125-30m", "SANN-125-2.5h", "SANN-125-24h", "SANN-150-30m", "SANN-150-2.5h", "SANN-150-24h", "SANN-150-73h", "SANN-150-250h"}}};

  //  Calculate DCR @ given overvoltage
  //  Board - Sensor - Ann-temp
  std::map < std::string, std::map < std::string, std::map < std::string, database::mkgraph_data_structure >>>>> all_graphs_data;
  //  Board - Sensor - Ann-temp
  std::map < std::string, std::map < std::string, std::map < std::string, std::vector < TGraphErrors * >>>>> all_graphs;
  for (auto [current_board, current_state_list] : target_boards_and_states)
  {
    for (auto current_state : current_state_list)
    {
      for (auto [current_sensor, current_channels_list] : database::sensor_to_channels)
      {
        auto current_annealing_time = std::atoi(database::get_database_info(current_board, current_state, "ann-time").c_str());
        auto current_annealing_temp = database::get_database_info(current_board, current_state, "ann-temp");
        auto current_data_vector = database::get_all_values_at_overvoltage<database::get_dcr_vbias_scan>(current_board, current_sensor, current_state, 3.);
        all_graphs_data[current_board][current_sensor][current_annealing_temp][{current_annealing_time, 0.}].push_back(database::measure_average(current_data_vector, true));
        all_graphs_data[current_board][current_sensor][current_annealing_temp][{current_annealing_time, 0.}].push_back(database::measure_average(current_data_vector, false));
        for (auto iTer : current_data_vector)
          all_graphs_data[current_board][current_sensor][current_annealing_temp][{current_annealing_time, 0.}].push_back(iTer);
      }
    }
  }
  for (auto [current_board, all_sensors] : all_graphs_data)
  {
    for (auto [current_sensor, all_temps] : all_sensors)
    {
      for (auto [current_temperature, current_data] : all_temps)
      {
        all_graphs[current_board][current_sensor][current_temperature] = database::make_graph(current_data);
      }
    }
  }

  gROOT->SetBatch();
  for (auto [current_board, all_sensors] : all_graphs)
  {
    for (auto [current_sensor, all_temps] : all_sensors)
    {
      TCanvas *cDrawTempScan = new TCanvas();
      cDrawTempScan->Draw();
      //  Find min and max of X-axis of all graphs
      std::vector<std::pair<float, float>> graphs_min_max;
      for (auto [current_temperature, all_graphs] : all_temps)
        graphs_min_max.push_back(all_graphs[0]->GetPointX(0), all_graphs[0]->GetPointX(all_graphs[0]->GetN() - 1));
      //  Find the points to set the TPads
      auto absolute_max = 0.;
      auto current_max = 0.;
      auto current_min = 0.;
      std::vector<std::pair<float, float>> pads_min_max;
      for (auto current_min_max : graphs_min_max)
        absolute_max += current_min_max.second;
      for (auto [current_temperature, all_graphs] : all_temps)
      {
        current_min = all_graphs[0]->GetPointX(0);
        current_max = all_graphs[0]->GetPointX(all_graphs[0]->GetN() - 1);
        pads_min_max.push_back({0.1 + 0.8 * (current_min) / (absolute_max), 0.1 + 0.8 * (current_max) / (absolute_max)});
      }
      auto iTer = -1;
      for (auto current_pad_min_max : pads_min_max)
      {
        iTer++;
        database::set_multi_graph(all_graphs, kRed, 20);
        TPad *p1 = new TPad("p1", "p1", current_pad_min_max.first, 0.1, current_pad_min_max.second, 0.9);
        gPad->SetLogx();
        p1->Draw();
      }

      for (auto [current_temperature, all_graphs] : all_temps)
      {
        database::set_multi_graph(all_graphs, kRed, 20);
        gPad->SetLogx();
        for (auto current_graph : all_graphs)
        {
        }
      }
    }
  }
  for (auto &[tag, graph_list] : all_graphs)
  {
    database::set_multi_graph(graph_list, kRed, 20);
    TCanvas *c1 = new TCanvas();
    gPad->SetLogx();
    graph_list[0]->Draw("ALPE");
    for (auto graph : graph_list)
    {
      graph->Draw("SAME LPE");
    }
    c1->SaveAs(Form("%s.pdf", tag.c_str()));
  }
  gROOT->SetBatch(false);

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
}
