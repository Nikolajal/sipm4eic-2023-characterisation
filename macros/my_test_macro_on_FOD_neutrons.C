#include "../source/database.C"

void my_test_macro_on_FOD_neutrons(std::string data_dir = "./Data/", std::string outfile = "out.root")
{
  //  Load database
  database::basedir = data_dir;
  database::read_database(database::basedir + "/database.txt");
  
  //  Output requested
  
}
