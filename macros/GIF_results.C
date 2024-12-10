#include "../source/database.C"

void GIF_results(std::string outfile_name = "out.root")
{
  //  Load database
  database::basedir = "/Users/nrubini/Analysis/ePIC/memory/sipm4eic-2023-characterisation/Data/";
  database::read_database(database::basedir + "database.txt");

  //  General set-up
  TLatex *l_latex = new TLatex();

  //  --- --- --- Leakage current
  //  --- --- --- --- Leakage current @100rad
  //  Canvas set-up
  TCanvas *leakage_100rad = new TCanvas("", "", 800, 800);
  gStyle->SetOptStat(0);
  gPad->SetGridy();
  gPad->SetMargin(0.15, 0.05, 0.15, 0.05);
  //  Frame
  TH1F *h_frame = new TH1F("h_frame", ";;Leakage current @ -1.5V ov. (nA)", 3, 0, 3);
  h_frame->SetMinimum(1.e-2);
  h_frame->SetMaximum(5.e-1);
  h_frame->GetXaxis()->SetBinLabel(1, "New");
  h_frame->GetXaxis()->SetBinLabel(2, "Scratched");
  h_frame->GetXaxis()->SetBinLabel(3, "#gamma-irradiated");
  h_frame->Draw();

  //  Generate TGraphErrors
  auto graphs_gif_irr1 = database::get_general_TGraphs_iv("HPK S13360-3050VS", {{"NEW", {"11"}}, {"OANN-150-150h", {"11"}}, {"GIF-IRR1", {"11"}}}, -1.5, 20, kOrange);
  graphs_gif_irr1[0]->SetMarkerSize(2);
  graphs_gif_irr1[1]->SetMarkerSize(2);
  graphutils::y_scale(graphs_gif_irr1[0],1.e9);
  graphutils::y_scale(graphs_gif_irr1[1],1.e9);
  graphs_gif_irr1[0]->Draw("SAME EPL");
  graphs_gif_irr1[1]->Draw("SAME E[]P");
  auto graphs_gif_irr3 = database::get_general_TGraphs_iv("HPK S13360-3075VS", {{"NEW", {"11"}}, {"OANN-150-150h", {"11"}}, {"GIF-IRR1", {"11"}}}, -1.5, 21, kOrange);
  graphs_gif_irr3[0]->SetMarkerSize(2);
  graphs_gif_irr3[1]->SetMarkerSize(2);
  graphutils::y_scale(graphs_gif_irr3[0],1.e9);
  graphutils::y_scale(graphs_gif_irr3[1],1.e9);
  graphs_gif_irr3[0]->Draw("SAME EPL");
  graphs_gif_irr3[1]->Draw("SAME E[]P");
  auto graphs_gif_irr5 = database::get_general_TGraphs_iv("HPK S14161-3050HS", {{"NEW", {"11"}}, {"OANN-150-150h", {"11"}}, {"GIF-IRR1", {"11"}}}, -1.5, 33, kOrange);
  graphs_gif_irr5[0]->SetMarkerSize(2);
  graphs_gif_irr5[1]->SetMarkerSize(2);
  graphutils::y_scale(graphs_gif_irr5[0],1.e9);
  graphutils::y_scale(graphs_gif_irr5[1],1.e9);
  graphs_gif_irr5[0]->Draw("SAME EPL");
  graphs_gif_irr5[1]->Draw("SAME E[]P");

  //  TLegend
  TLegend *l_legend_100rad = new TLegend(0.175, 0.825, 0.500, 0.925);
  l_legend_100rad->SetLineColorAlpha(0., 0.);
  l_legend_100rad->SetFillStyle(0.);
  l_legend_100rad->AddEntry(graphs_gif_irr1[0], "HPK S13360-3050VS", "P");
  l_legend_100rad->AddEntry(graphs_gif_irr3[0], "HPK S13360-3075VS", "P");
  l_legend_100rad->AddEntry(graphs_gif_irr5[0], "HPK S14161-3050HS", "P");
  l_legend_100rad->Draw("SAME");

  //  Save canvas
  l_latex->SetTextFont(42);
  l_latex->DrawLatexNDC(0.775, 0.9, "100 rad");
  leakage_100rad->SaveAs("./figures/leakage_100rad.png");

  //  --- --- --- --- Leakage current @100rad
  //  Canvas set-up
  TCanvas *leakage_1krad = new TCanvas("", "", 800, 800);
  gStyle->SetOptStat(0);
  gPad->SetGridy();
  gPad->SetMargin(0.15, 0.05, 0.15, 0.05);
  //  Frame
  h_frame->SetMinimum(1.e-2);
  h_frame->SetMaximum(5.e-1);
  h_frame->Draw();

  //  Generate TGraphErrors
  auto graphs_gif_irr2 = database::get_general_TGraphs_iv("HPK S13360-3050VS", {{"NEW", {"12"}}, {"OANN-150-150h", {"12"}}, {"GIF-IRR1", {"12"}}}, -1.5, 20, kRed);
  graphs_gif_irr2[0]->SetMarkerSize(2);
  graphs_gif_irr2[1]->SetMarkerSize(2);
  graphutils::y_scale(graphs_gif_irr2[0],1.e9);
  graphutils::y_scale(graphs_gif_irr2[1],1.e9);
  graphs_gif_irr2[0]->Draw("SAME EPL");
  graphs_gif_irr2[1]->Draw("SAME E[]P");
  auto graphs_gif_irr4 = database::get_general_TGraphs_iv("HPK S13360-3075VS", {{"NEW", {"12"}}, {"OANN-150-150h", {"12"}}, {"GIF-IRR1", {"12"}}}, -1.5, 21, kRed);
  graphs_gif_irr4[0]->SetMarkerSize(2);
  graphs_gif_irr4[1]->SetMarkerSize(2);
  graphutils::y_scale(graphs_gif_irr4[0],1.e9);
  graphutils::y_scale(graphs_gif_irr4[1],1.e9);
  graphs_gif_irr4[0]->Draw("SAME EPL");
  graphs_gif_irr4[1]->Draw("SAME E[]P");
  auto graphs_gif_irr6 = database::get_general_TGraphs_iv("HPK S14161-3050HS", {{"NEW", {"12"}}, {"OANN-150-150h", {"12"}}, {"GIF-IRR1", {"12"}}}, -1.5, 33, kRed);
  graphs_gif_irr6[0]->SetMarkerSize(2);
  graphs_gif_irr6[1]->SetMarkerSize(2);
  graphutils::y_scale(graphs_gif_irr6[0],1.e9);
  graphutils::y_scale(graphs_gif_irr6[1],1.e9);
  graphs_gif_irr6[0]->Draw("SAME EPL");
  graphs_gif_irr6[1]->Draw("SAME E[]P");

  //  TLegend
  TLegend *l_legend_1krad = new TLegend(0.175, 0.825, 0.500, 0.925);
  l_legend_1krad->SetLineColorAlpha(0., 0.);
  l_legend_1krad->SetFillStyle(0.);
  l_legend_1krad->AddEntry(graphs_gif_irr2[0], "HPK S13360-3050VS", "P");
  l_legend_1krad->AddEntry(graphs_gif_irr4[0], "HPK S13360-3075VS", "P");
  l_legend_1krad->AddEntry(graphs_gif_irr6[0], "HPK S14161-3050HS", "P");
  l_legend_1krad->Draw("SAME");

  //  Save canvas
  l_latex->SetTextFont(42);
  l_latex->DrawLatexNDC(0.775, 0.9, "1 krad");
  leakage_1krad->SaveAs("./figures/leakage_1krad.png");






  /*
    auto graphs_gif_irr2 = database::get_general_TGraphs_iv("HPK S13360-3050VS", {{"NEW", {"12"}}, {"OANN-150-150h", {"12"}}, {"GIF-IRR1", {"12"}}}, -1.5, 24, kRed);
    graphs_gif_irr2[0]->SetMarkerSize(2);
    graphs_gif_irr2[1]->SetMarkerSize(2);
    graphs_gif_irr2[0]->Draw("SAME EP");
    graphs_gif_irr2[1]->Draw("SAME E[]P");
    auto graphs_gif_irr4 = database::get_general_TGraphs_iv("HPK S13360-3075VS", {{"NEW", {"12"}}, {"OANN-150-150h", {"12"}}, {"GIF-IRR1", {"12"}}}, -1.5, 25, kRed);
    graphs_gif_irr4[0]->SetMarkerSize(2);
    graphs_gif_irr4[1]->SetMarkerSize(2);
    graphs_gif_irr4[0]->Draw("SAME EP");
    graphs_gif_irr4[1]->Draw("SAME E[]P");
    auto graphs_gif_irr6 = database::get_general_TGraphs_iv("HPK S14161-3050HS", {{"NEW", {"12"}}, {"OANN-150-150h", {"12"}}, {"GIF-IRR1", {"12"}}}, -1.5, 27, kRed);
    graphs_gif_irr6[0]->SetMarkerSize(2);
    graphs_gif_irr6[1]->SetMarkerSize(2);
    graphs_gif_irr6[0]->Draw("SAME EP");
    graphs_gif_irr6[1]->Draw("SAME E[]P");
    TLatex *l_latex = new TLatex();
    l_latex->SetTextFont(42);
    //l_latex->DrawLatexNDC(0.175, 0.9, "HPK S13360-3050VS");

    delete ov_current_canvas;
    delete h_frame;

    //  Canvas set-up
    //{"NEW", {"12"}}, {"OANN-150-150h", {"12"}}, {"GIF-IRR1", {"12"}}
    // ng board, std::string channel, std::string step, int marker = 1, int color = 1);
    TCanvas *result_canvas = new TCanvas("", "", 800, 800);
    gStyle->SetOptStat(0);
    gPad->SetLogy();
    gPad->SetMargin(0.15, 0.05, 0.15, 0.05);
    h_frame = new TH1F("h_frame", ";V_{bias} (V);Current (A)", 100, 0, 100);
    h_frame->SetMinimum(5.e-12);
    h_frame->SetMaximum(1.e-7);
    h_frame->Draw();
    auto graph_1 = database::get_iv_scan("12", "C1", "NEW", kFullCircle, kBlue);
    auto graph_2 = database::get_iv_scan("12", "C1", "OANN-150-150h", kFullCircle, kRed);
    auto graph_3 = database::get_iv_scan("12", "C1", "GIF-IRR1", kFullCircle, kOrange);
    graph_1->Draw("SAME");
    graph_2->Draw("SAME");
    graph_3->Draw("SAME");
    database::get_iv_scan("12", "C2", "NEW", kFullCircle, kBlue)->Draw("SAME");
    database::get_iv_scan("12", "C2", "OANN-150-150h", kFullCircle, kRed)->Draw("SAME");
    database::get_iv_scan("12", "C2", "GIF-IRR1", kFullCircle, kOrange)->Draw("SAME");
    database::get_iv_scan("12", "C3", "NEW", kFullCircle, kBlue)->Draw("SAME");
    database::get_iv_scan("12", "C3", "OANN-150-150h", kFullCircle, kRed)->Draw("SAME");
    database::get_iv_scan("12", "C3", "GIF-IRR1", kFullCircle, kOrange)->Draw("SAME");
    database::get_iv_scan("12", "C4", "NEW", kFullCircle, kBlue)->Draw("SAME");
    database::get_iv_scan("12", "C4", "OANN-150-150h", kFullCircle, kRed)->Draw("SAME");
    database::get_iv_scan("12", "C4", "GIF-IRR1", kFullCircle, kOrange)->Draw("SAME");
    TLegend *l_wlegend = new TLegend(0.175, 0.75, 0.45, 0.9);
    l_wlegend->SetLineColorAlpha(0., 0.);
    l_wlegend->SetFillStyle(0.);
    l_wlegend->AddEntry(graph_1, "NEW", "P");
    l_wlegend->AddEntry(graph_2, "OANN", "P");
    l_wlegend->AddEntry(graph_3, "GIF++", "P");
    l_wlegend->Draw("SAME");

    */
}
