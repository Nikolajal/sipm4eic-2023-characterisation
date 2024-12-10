#include "../source/database.C"

// Function to display the helper message
void print_help()
{
  std::cout << "Usage: ./my_macro_executable [options]\n"
            << "Options:\n"
            << "  --data_dir <directory>    Directory containing data files\n"
            << "                            Def: ./Data\n"
            << "  --database <file>         Path to the database file\n"
            << "                            Def: ./Data/database.txt\n"
            << "  --board <board_number>    Board number\n"
            << "                            Def: 12\n"
            << "  --overvoltage <value>     Overvoltage value\n"
            << "                            Def: 4.0\n"
            << "  --time_axis                Use time axis for figures\n"
            << "                            Def: true\n"
            << "  --step_axis                Use step axis for figures\n"
            << "                            Def: false\n"
            << "  -h, --help                 Show this help message\n";
}

using namespace std;

int main(int argc, char **argv)
{
  // Set default parameters
  std::string data_dir = "./Data";
  std::string database = "./Data/database.txt";
  std::string board = "12";
  float overvoltage = 4.0;
  bool time_axis = true;

  // Parse command line arguments
  for (int i = 1; i < argc; ++i)
  {
    std::string arg = argv[i];

    if (arg == "--data_dir" && i + 1 < argc)
    {
      data_dir = argv[++i];
    }
    else if (arg == "--database" && i + 1 < argc)
    {
      database = argv[++i];
    }
    else if (arg == "--board" && i + 1 < argc)
    {
      board = argv[++i];
    }
    else if (arg == "--overvoltage" && i + 1 < argc)
    {
      overvoltage = std::stof(argv[++i]); // Convert string to float
    }
    else if (arg == "--time_axis")
    {
      time_axis = true; // Set time_axis to true if specified
    }
    else if (arg == "--step_axis")
    {
      time_axis = false; // Set time_axis to false if specified
    }
    else if (arg == "-h" || arg == "--help")
    {
      print_help(); // Show help message
      return 0;     // Exit after displaying help
    }
    else
    {
      std::cerr << "Unknown argument: " << arg << std::endl;
      print_help(); // Show help message
      return 1;     // Exit with an error
    }
  }

  // Set ROOT style
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadBottomMargin(0.15);

  // Set database directory and read data
  database::basedir = data_dir;
  cout <<  database::basedir << endl;
  cout <<  database << endl;
  database::read_database(database);
  database::show_database();

  // Retrieve step history and IV data
  auto step_list = database::get_board_step_history(board, false, true);
  auto current_board = database::get_history_iv_at_overvoltage(board, step_list, overvoltage);

  // Save figure
  current_board->SaveAs(("./figures/board." + board + "." + (time_axis ? "time" : "step") + ".png").c_str());

  return 0;
}
