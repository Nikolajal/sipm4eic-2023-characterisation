struct recodata_struct
{
  UShort_t n;
  float x[256];
  float y[256];
  float t[256];
};

void read_example(std::string infilename, std::string outfilename = "out.root")
{
  //  Open file
  TFile *input_file = new TFile(infilename.c_str());

  //  Get TTree
  auto data_tree = (TTree *)(input_file->Get("recodata"));

  //  Create data structure to read tree and set addresses
  recodata_struct data;
  data_tree->SetBranchAddress("n", &data.n);
  data_tree->SetBranchAddress("x", &data.x);
  data_tree->SetBranchAddress("y", &data.y);
  data_tree->SetBranchAddress("t", &data.t);

  //  Create data structure for output
  std::map<std::string, TH2F *> _TH2F;
  _TH2F["hitmap"] = new TH2F("hitmap", "hitmap", 80, -100, 100, 100, -100, 100);

  //  Loop on tree entris
  for (auto iEvnt = 0; iEvnt < data_tree->GetEntries(); iEvnt++)
  {
    //  Loading current event
    data_tree->GetEntry(iEvnt);
    //  Now the variables defined above (data.n, data.x, etc...) have the info from the tree for the given event

    //  Looping on the event hits
    for (auto iHit = 0; iHit < data.n; iHit++)
    {
      _TH2F["hitmap"]->Fill(data.x[iHit], data.y[iHit]);
    }
  }

  //  Plot the hit map
  TCanvas *c1 = new TCanvas();
  _TH2F["hitmap"]->Draw("COLZ");

  //  Write to file
  TFile* outfile = new TFile(outfilename.c_str(),"RECREATE");
  _TH2F["hitmap"]->Write();
}
