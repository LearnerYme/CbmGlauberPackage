void AddNpartNcollHist(int nJobs, const char* path, const char* rootName="FastGlauber.root", const char* treeName="nt_Au_Au", const char* outName="npart_ncoll_hist.root") {
    TFile* tfout = new TFile(outName, "recreate");
    tfout->cd();
    TH2F* h2d = new TH2F(
        "hNcoll_Npart", "hNcoll_Npart;N_{part};N_{coll};",
        400, -0.5, 399.5,
        1800, -0.5, 1799.5
    );
    TChain chain(treeName);
    for (int i=1; i<=nJobs; i++) {
        cout << "[LOG] Now add root file: " << Form("%s/%05d/%s", path, i, rootName) << endl;
        chain.Add(Form("%s/%05d/%s", path, i, rootName));
    }
    chain.Draw("Ncoll:Npart>>hNcoll_Npart", "Npart>=2&&Ncoll>=1");
    h2d->Write();
    tfout->Close();
}