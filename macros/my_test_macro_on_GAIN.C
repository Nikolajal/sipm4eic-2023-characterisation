/**
    this example plots a IV and a DCR scan for one channel
    the data and the database file are assumed to be located in
    "/home/preghenella/EIC/sipm4eic-2023-characterisation/data"
**/

#include "../source/database.C"

void my_test_macro_on_GAIN(std::string data_dir = "./Data")
{
  database::basedir = data_dir;
  database::read_database(database::basedir + "/database.txt");

}
