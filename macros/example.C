/**
    this example plots a IV and a DCR scan for one channel
    the data and the database file are assumed to be located in
    "/home/preghenella/EIC/sipm4eic-2023-characterisation/data"
**/

#include "../source/database.C"

void example(std::string data_dir)
{
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadBottomMargin(0.15);

  database::basedir = data_dir;
  database::read_database(database::basedir + "/database.txt");

  auto c = new TCanvas("c", "c", 1200, 600);
  c->Divide(3, 1);
  c->cd(1)->DrawFrame(45, 1.e-11, 65, 1.e-3, ";bias voltage (V);current (A)");
  c->cd(1)->SetLogy();
  database::get_iv_scan("1", "A1", "NEW", 20, kAzure - 3)->Draw("samelp");
  database::get_iv_scan("1", "A1", "TIFPA-IRR1", 25, kRed + 1)->Draw("samelp");
  c->cd(2)->DrawFrame(45, 1., 65, 1.e7, ";bias voltage (V);dark count rate (Hz)");
  c->cd(2)->SetLogy();
  database::get_dcr_vbias_scan("1", "A1", "NEW", 20, kAzure - 3)->Draw("samelp");
  database::get_dcr_vbias_scan("1", "A1", "TIFPA-IRR1", 25, kRed + 1)->Draw("samelp");
  c->cd(3)->DrawFrame(0., 1., 65, 1.e6, ";threshold voltage (a.u.);dark count rate (Hz)");
  c->cd(3)->SetLogy();
  database::get_dcr_threshold_scan("1", "A1", "NEW", 20, kAzure - 3)->Draw("samelp");
  database::get_dcr_threshold_scan("1", "A1", "TIFPA-IRR1", 25, kRed + 1)->Draw("samelp");
}
