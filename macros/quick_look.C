#include "database.C"

void history(std::string data_dir = "./Data", std::string database = "./Data/database.txt", std::string board = "12", float overvoltage = 4., bool time_axis = true)
{
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadBottomMargin(0.15);
  database::basedir = data_dir;
  database::read_database(database);
  auto step_list = database::get_board_step_history(board, false, true);
  auto current_board = database::get_history_iv_at_overvoltage(board, step_list, overvoltage,time_axis);
  current_board->SaveAs(("./figures/board." + board + "." + (time_axis ? "time" : "step") + ".png").c_str());
}
