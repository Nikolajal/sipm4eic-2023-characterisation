#include "../source/database_laser.C"
#include "../utils/general_utility.h"
#include "../utils/tree_database.h"

void my_test_macro_on_LaserReading()
{
  //  Time consumption tracking
  TBenchmark benchmark;

  //  Load database
  cout << "[INFO] Starting to load the database and updating the local data repository" << endl;
  benchmark.Start("db_load");
  database::laser::read_database("/Users/nrubini/Analysis/ePIC/_production_repositories/sipm4eic-2023-characterisation/Data/laser_database.txt");
  database::laser::read_all_sub_runs();
  database::laser::update_local_data_repository();
  benchmark.Stop("db_load");
  cout << "[INFO] Finished loading the database and updating the local data repository: " << benchmark.GetRealTime("db_load") << " s" << endl;

  //  Plot sensor
  TCanvas *c1 = new TCanvas();
  gStyle->SetOptStat(0);
  TH1F *h_frame = new TH1F("", ";bkg level (Hz);pseudo-eff", 10000, 1000, 10000000);
  h_frame->SetMinimum(0);
  h_frame->SetMaximum(0.025);
  h_frame->Draw();
  auto sensor_graphs_new = database::laser::get_sensor_pPDE_vs_bkg("20241002-152837", database::sensors_full[0], "target", "center");
  auto sensor_graphs_new_ref = database::laser::get_pPDE_vs_vbias("20241002-152837", "A1", "reference", "center");
  // database::laser::get_pPDE_vs_vbias("20240326-123731", "A1", "target", "center")->Draw("SAME LEP");
  // database::laser::get_pPDE_vs_vbias("20240326-123731", "A2", "target", "center")->Draw("SAME LEP");
  // database::laser::get_pPDE_vs_vbias("20240326-123731", "A3", "target", "center")->Draw("SAME LEP");
  // database::laser::get_pPDE_vs_vbias("20240326-123731", "A4", "target", "center")->Draw("SAME LEP");
  sensor_graphs_new["ave_err"]->SetMarkerStyle(20);
  sensor_graphs_new["ave_err"]->SetMarkerColor(kAzure - 2);
  sensor_graphs_new["ave_err"]->Draw("SAME EP");
  auto sensor_graphs_gif_irr = database::laser::get_sensor_pPDE_vs_bkg("20241104-124751", database::sensors_full[0], "target", "center");
  auto sensor_graphs_gif_irr_ref = database::laser::get_pPDE_vs_vbias("20241104-124751", "A1", "reference", "center");
  sensor_graphs_gif_irr["ave_err"]->SetMarkerStyle(20);
  sensor_graphs_gif_irr["ave_err"]->SetMarkerColor(kYellow - 2);
  sensor_graphs_gif_irr["ave_err"]->Draw("SAME EP");

  auto div_ref = graphutils::ratio(sensor_graphs_gif_irr_ref, sensor_graphs_new_ref);
  div_ref->Fit("pol0");
  auto scale_pol0 = div_ref->GetFunction("pol0");
  auto bis = graphutils::scale(sensor_graphs_gif_irr["ave_err"], {scale_pol0->GetParameter(0), scale_pol0->GetParError(0)});
  bis->SetMarkerStyle(24);

  TF1 *ftest = new TF1("ftest", "[0]*sqrt([1]+[2]*x)",0,100000000);
  bis->Fit(ftest, "IMRES");

  bis->Draw("SAME PE");

  // database::laser::get_pPDE_vs_vbias("20241105-115341", "A1", "target", "center")->Draw("SAME LEP");
  // database::laser::get_pPDE_vs_vbias("20241105-115341", "A2", "target", "center")->Draw("SAME LEP");
  // database::laser::get_pPDE_vs_vbias("20241105-115341", "A3", "target", "center")->Draw("SAME LEP");
  // database::laser::get_pPDE_vs_vbias("20241105-115341", "A4", "target", "center")->Draw("SAME LEP");
  // database::laser::show_stability_check(database::laser::get_all_laser_runs("good"));
}