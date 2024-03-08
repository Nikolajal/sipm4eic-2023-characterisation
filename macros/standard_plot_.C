#include "../source/database.C"

using namespace database;

TGraphErrors *make_graph()
{
  
}

void standard_plot_(std::string data_dir = "./Data/", std::string target_sensor = "", std::string outfile = "")
{
  //  Load database
  database::basedir = data_dir;
  database::read_database(database::basedir + "/database.txt");

  //  Make a folder for the plots
  gROOT->ProcessLine(Form(".! mkdir -p plots/"));
  gROOT->ProcessLine(Form(".! mkdir -p plots/standard_plot/"));
}