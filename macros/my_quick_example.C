/**
    this example plots a IV and a DCR scan for one channel
    the data and the database file are assumed to be located in
    "/home/preghenella/EIC/sipm4eic-2023-characterisation/data"
**/

#include "../source/database.C"

void my_quick_example(std::string data_dir = "./Data/")
{
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadBottomMargin(0.15);

  database::basedir = data_dir;
  database::read_database(database::basedir + "/database.txt");

  std::vector<float> new_coordinates;
  for (auto iTer = 0; iTer < 6; iTer++)
    new_coordinates.push_back(-3 + 0.5 * iTer);
  for (auto current_meas : database::standard_overvoltage_points)
    new_coordinates.push_back(current_meas);

  auto c = new TCanvas("c", "c", 1200, 600);
  c->Divide(3, 3);
  for (auto iTer = 0; iTer < 3; iTer++)
  {
    for (auto jTer = 0; jTer < 3; jTer++)
    {
      c->cd(3 * iTer + 1 + jTer);
      c->cd(3 * iTer + 1 + jTer)->SetLogy();
      if (jTer == 0)
      {
        c->cd(3 * iTer + 1 + jTer)->DrawFrame(0, 5.e5, 8, 5.e7, ";overvoltage (V);gain");
        database::get_average_gain_at_overvoltage(database::standard_status_to_boards["NEW"], database::all_sensors[iTer], "NEW", database::standard_overvoltage_points, 20, kBlue)->Draw("SAMELPE");
        database::get_average_gain_at_overvoltage(database::standard_status_to_boards["TIFPA-IRR1"], database::all_sensors[iTer], "TIFPA-IRR1", database::standard_overvoltage_points, 21, kGreen + 2)->Draw("SAMELPE");
        database::get_average_gain_at_overvoltage(database::standard_status_to_boards["LNL-IRR1"], database::all_sensors[iTer], "LNL-IRR1", database::standard_overvoltage_points, 22, kRed)->Draw("SAMELPE");
      }
      if (jTer == 1)
      {
        c->cd(3 * iTer + 1 + jTer)->DrawFrame(0, 1.e1, 8, 1.e7, ";overvoltage (V);DCR (Hz)");
        database::get_average_drc_at_overvoltage(database::standard_status_to_boards["NEW"], database::all_sensors[iTer], "NEW", database::standard_overvoltage_points, 20, kBlue)->Draw("SAMELPE");
        database::get_average_drc_at_overvoltage(database::standard_status_to_boards["TIFPA-IRR1"], database::all_sensors[iTer], "TIFPA-IRR1", database::standard_overvoltage_points, 21, kGreen + 2)->Draw("SAMELPE");
        database::get_average_drc_at_overvoltage(database::standard_status_to_boards["LNL-IRR1"], database::all_sensors[iTer], "LNL-IRR1", database::standard_overvoltage_points, 22, kRed)->Draw("SAMELPE");
      }
      if (jTer == 2)
      {
        c->cd(3 * iTer + 1 + jTer)->DrawFrame(-3, 5.e-11, 8, 1.e-6, ";overvoltage (V);current (A)");
        database::get_average_iv_at_overvoltage(database::standard_status_to_boards["NEW"], database::all_sensors[iTer], "NEW", new_coordinates, 20, kBlue)->Draw("SAMELPE");
        database::get_average_iv_at_overvoltage(database::standard_status_to_boards["TIFPA-IRR1"], database::all_sensors[iTer], "TIFPA-IRR1", new_coordinates, 21, kGreen + 2)->Draw("SAMELPE");
        database::get_average_iv_at_overvoltage(database::standard_status_to_boards["LNL-IRR1"], database::all_sensors[iTer], "LNL-IRR1", new_coordinates, 22, kRed)->Draw("SAMELPE");
      }
    }
  }

  std::vector<std::string> states = {"NEW", "TIFPA-IRR1", "LNL-IRR1"};
  std::map<std::string, int> markers = {{"NEW", 20}, {"TIFPA-IRR1", 21}, {"LNL-IRR1", 22}};
  std::map<std::string, int> colors = {{"NEW", kBlue}, {"TIFPA-IRR1", kGreen + 2}, {"LNL-IRR1", kRed}};
  TCanvas *c2 = new TCanvas("c2", "c2", 1200, 600);
  c2->Divide(3, 3);
  for (auto iTer = 0; iTer < 3; iTer++)
  {
    for (auto jTer = 0; jTer < 3; jTer++)
    {
      c2->cd(3 * iTer + 1 + jTer);
      c2->cd(3 * iTer + 1 + jTer)->SetLogy();

      if (jTer == 0)
      {
        c2->cd(3 * iTer + 1 + jTer)->DrawFrame(30, 5.e-1, 60, 5.e7, ";overvoltage (V);gain");
        auto current_sensor = database::all_sensors[iTer];
        for (auto current_state : states)
          for (auto current_board : database::standard_status_to_boards[current_state])
              for (auto current_channel : database::sensor_to_channels[current_sensor])
                database::get_gain(current_board, current_channel, current_state, markers[current_state], colors[current_state])->Draw("SAMELPE");
      }
      if (jTer == 1)
      {
        c2->cd(3 * iTer + 1 + jTer)->DrawFrame(30, 1.e-11, 60, 1.e7, ";overvoltage (V);DCR (Hz)");
        auto current_sensor = database::all_sensors[iTer];
        for (auto current_state : states)
          for (auto current_board : database::standard_status_to_boards[current_state])
              for (auto current_channel : database::sensor_to_channels[current_sensor])
                database::get_dcr_vbias_scan(current_board, current_channel, current_state, markers[current_state], colors[current_state])->Draw("SAMELPE");
      }
      if (jTer == 2)
      {
        c2->cd(3 * iTer + 1 + jTer)->DrawFrame(30, 5.e-11, 60, 1.e-6, ";overvoltage (V);current (A)");
        auto current_sensor = database::all_sensors[iTer];
        for (auto current_state : states)
          for (auto current_board : database::standard_status_to_boards[current_state])
              for (auto current_channel : database::sensor_to_channels[current_sensor])
                database::get_iv_scan(current_board, current_channel, current_state, markers[current_state], colors[current_state])->Draw("SAMELPE");
      }
    }
  }
}