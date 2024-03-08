/**
    this example plots a IV and a DCR scan for one channel
    the data and the database file are assumed to be located in
    "/home/preghenella/EIC/sipm4eic-2023-characterisation/data"
**/

#include "../source/database.C"

void temporary_check(std::string data_dir = "./Data")
{
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadBottomMargin(0.15);

  database::basedir = data_dir;
  database::read_database(database::basedir + "/database.txt");

  std::string current_sensor = database::all_sensors[2];
  std::string current_board_1 = "25";
  std::string current_board_2 = "26";

  auto c = new TCanvas("c", "c", 1200, 600);
  c->Divide(2, 1);
  c->cd(1)->DrawFrame(33, 5.e-11, 53, 5.e-5, ";bias voltage (V);dark count rate (Hz)");
  c->cd(1)->SetLogy();
  for (auto current_channel : database::sensor_to_channels[current_sensor])
    database::get_iv_scan(current_board_1, current_channel, "LNL-IRR1", 25, kGreen + 1)->Draw("samelp");
  for (auto current_channel : database::sensor_to_channels[current_sensor])
    database::get_iv_scan(current_board_1, current_channel, "OANN-150-150h", 25, kRed + 1)->Draw("samelp");
  c->cd(2)->DrawFrame(33, 5.e-11, 53, 5.e-5, ";bias voltage (V);dark count rate (Hz)");
  c->cd(2)->SetLogy();
  for (auto current_channel : database::sensor_to_channels[current_sensor])
    database::get_iv_scan(current_board_2, current_channel, "LNL-IRR1", 25, kGreen + 1)->Draw("samelp");
  for (auto current_channel : database::sensor_to_channels[current_sensor])
    database::get_iv_scan(current_board_2, current_channel, "OANN-150-150h", 25, kRed + 1)->Draw("samelp");
}
