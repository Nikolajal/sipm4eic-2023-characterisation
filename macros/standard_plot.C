#include "../source/database.C"

using namespace database;

template <int sensor_number = 0>
void plot_sensor(std::string status = "NEW", int marker_style = 20, int marker_color = kBlack)
{
  auto test_mean = get_average_dcr_at_overvoltage<false>(standard_status_to_boards[status], sensors_full[sensor_number], standard_status_to_status[status]);
  auto test_RMS_ = get_average_dcr_at_overvoltage<true>(standard_status_to_boards[status], sensors_full[sensor_number], standard_status_to_status[status]);

  test_mean->Draw("SAME PE1");
  test_RMS_->Draw("SAME E3");

  set_standard_style(test_mean, marker_style, marker_color); 
  set_standard_style(test_RMS_, marker_style, marker_color);
}

template <int sensor_number = 0>
void plot_states_for_sensor(std::string plot_name, std::vector<std::string> status_list = {"NEW", "TIFPA-IRR1"})
{
  auto current_plot = standard_canvas<false, false>(plot_name.c_str(), "");
  gPad->SetLogy();
  TH1F *frame = new TH1F("frame", "", 1, 0, 8);
  frame->SetMaximum(1.e7);
  frame->SetMinimum(1.e0);
  frame->Draw("");
  plot_sensor<sensor_number>();
  plot_sensor<sensor_number>("TIFPA-IRR1", 21);
  plot_sensor<sensor_number>("TIFPA-OANN", 21);
  current_plot->SaveAs(Form("plots/%s/%s.eps", plot_name.c_str(), sensors_full[0].c_str()));
  current_plot->SaveAs(Form("plots/%s/%s.pdf", plot_name.c_str(), sensors_full[0].c_str()));
}

void standard_plot(std::string data_dir = "./Data/", std::string target_sensor = "", std::string outfile = "")
{
  //  Load database
  database::basedir = data_dir;
  database::read_database(database::basedir + "/database.txt");

  //  Make a folder for the plots
  gROOT->ProcessLine(Form(".! mkdir -p plots/"));
  gROOT->ProcessLine(Form(".! mkdir -p plots/standard_plot/"));

  //  Generate requested TGraph
  auto standard_plot = standard_canvas<false, false>("standard_plot", "");
  gPad->SetLogy();
  TH1F *frame = new TH1F("frame", "", 1, 0, 8);
  frame->SetMaximum(1.e7);
  frame->SetMinimum(1.e0);
  frame->Draw("");
  plot_sensor<0>();
  plot_sensor<0>("TIFPA-IRR1", 21);
  plot_sensor<0>("TIFPA-OANN", 21);
  standard_plot->SaveAs(Form("plots/standard_plot/%s.eps", sensors_full[0].c_str()));
  standard_plot->SaveAs(Form("plots/standard_plot/%s.pdf", sensors_full[0].c_str()));
  frame->Draw("");
  plot_sensor<1>();
  plot_sensor<1>("TIFPA-IRR1", 21);
  plot_sensor<1>("TIFPA-OANN", 21);
  standard_plot->SaveAs(Form("plots/standard_plot/%s.eps", sensors_full[1].c_str()));
  standard_plot->SaveAs(Form("plots/standard_plot/%s.pdf", sensors_full[1].c_str()));
  frame->Draw("");
  plot_sensor<2>();
  plot_sensor<2>("TIFPA-IRR1", 21);
  plot_sensor<2>("TIFPA-OANN", 21);
  standard_plot->SaveAs(Form("plots/standard_plot/%s.eps", sensors_full[2].c_str()));
  standard_plot->SaveAs(Form("plots/standard_plot/%s.pdf", sensors_full[2].c_str()));
}

/*

TLatex *latex = new TLatex();
latex->SetTextAngle(90);
latex->SetTextSize(0.05);

TLegend *legend_test = new TLegend(0.4, 0.16, 0.88, 0.30);
legend_test->SetFillColorAlpha(0., 0.);
legend_test->SetLineColorAlpha(0., 0.);
legend_test->SetTextSize(0.03);
legend_test->AddEntry(full_dcr_graphs["NEW"][sensor][0], "new", "P");
legend_test->AddEntry(full_dcr_graphs["TIFPA-IRR1"][sensor][0], "after 10^{9} n_{eq} irradiation", "P");
legend_test->AddEntry(oven_dcr_graphs[sensor][0], "after irradiation and annealing", "P");
//
legend_test->Draw("same");
latex->SetTextAngle(0);
latex->SetTextSize(0.05);
latex->SetTextFont(42);
latex->DrawLatexNDC(0.12, 0.85, database::sensors_full[database::sensors_number[sensor]].c_str());
latex->SetTextSize(0.025);
latex->DrawLatexNDC(0.46, 0.13, "Reported uncertainty is the RMS");
test->SaveAs(Form("Sensor%s.pdf", sensor.c_str()));

*/