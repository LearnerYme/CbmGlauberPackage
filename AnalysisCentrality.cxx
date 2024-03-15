void AnalysisCentrality() {

    const char* fileName = "BestFit.root";
    auto tf = new TFile(fileName);
    TH1D *hSample, *hReal, *hChi2;
    TH2D* h2d;

    hChi2 = (TH1D*)tf->Get("hChi2");
    hReal = (TH1D*)tf->Get("hReal");
    hSample = (TH1D*)tf->Get("hSample");
    h2d = (TH2D*)tf->Get("h2NpartVsMult");

    double npp, x, k, eff;
    double nsignn;
    double xAxisMax = 280;

    // set your parameters here
    npp = 0.7172;
    x = 0.12;
    k = 3.395;
    eff = 1.00;
    nsignn = 27.7;
    int minMultCut = 10;

    // read chi2
    double chi2 = hChi2->GetBinContent(1);
    double ndf = hChi2->GetBinContent(2);
    double chi2overndf = ndf == 0 ? -999 : chi2 / ndf;

    auto c = new TCanvas();
    hReal->SetTitle(Form(
        "n_{pp}=%.3f k=%.3f x=%.3f #epsilon=%.3f", npp, k, x, eff
    ));
    hSample->SetTitle(Form(
        "n_{pp}=%.3f k=%.3f x=%.3f #epsilon=%.3f", npp, k, x, eff
    ));

    double scaleFactor = hReal->Integral(minMultCut, hReal->GetNbinsX()) / hSample->Integral(minMultCut, hReal->GetNbinsX());
    hSample->Scale(scaleFactor);
    hReal->SetLineColor(kBlack);
    hSample->SetLineColor(kRed);
    auto ratio = new TRatioPlot(hSample, hReal);

    c->cd();
    gPad->SetLogy();
    gPad->SetFillStyle(4000);
    gStyle->SetOptStat(0);
    ratio->Draw();
    ratio->GetLowerRefYaxis()->SetTitle("Glauber/Data");
    ratio->GetUpperRefYaxis()->SetTitle("Count (scaled)");
    ratio->GetLowerRefYaxis()->SetNdivisions(203);
    ratio->GetLowerRefGraph()->SetMaximum(2);
    ratio->GetLowerRefGraph()->SetMinimum(0);
    ratio->GetUpperRefXaxis()->SetRangeUser(0, xAxisMax);
    ratio->GetLowerRefXaxis()->SetRangeUser(0, xAxisMax);

    TLatex* lat = new TLatex();
    lat->SetNDC();
    double section = 31.5;
    lat->DrawLatex(0.2, 0.88, Form("#sigma_{NN} = %.2f mb", nsignn));
    lat->DrawLatex(0.6, 0.88, "CBM Simulation");
    lat->DrawLatex(0.6, 0.8, "Au+Au @ 4.9 GeV");
    lat->DrawLatex(0.2, 0.8, Form("#chi^{2}/NDF = %.3f", chi2overndf));
    lat->SetTextSize(0.03);
    // lat->DrawLatex(0.45, 0.62, Form("# of events (data): %.0f", hReal->GetEntries()));
    // lat->DrawLatex(0.45, 0.58, Form("# of events (MC): %.0f", hSample->GetEntries()));

    // do centrality definition
    double tot = hSample->Integral();
    int cnt = 0;
    int nbins = hSample->GetNbinsX();
    int cur_bin = 0;
    const int nBins = 6;
    double edge[nBins] = {0};
    double rate_edge[nBins] = {
        0.00, 0.10, 0.20, 0.30, 0.40, 0.50
    };
    // v1.1: change the way we do centrality selection with measured multiplicity
    // for 0 to 20%, just use data
    // for higher cases, concate MC's distribution (later 80%) and data (former 20%)
    bool UseGlauber = false;
    int mult_20percent = -1;
    for (int i=nbins; i>=0; i--){
        if (UseGlauber) { 
            cnt += hSample->GetBinContent(i);
        } else {
            cnt += hReal->GetBinContent(i);
        }
        if (cnt*1.0 / tot > rate_edge[cur_bin]){
            cout << "[LOG] For Multiplicity: " << cur_bin << ", " << rate_edge[cur_bin] << ", " << i << ", " << cnt << " of " << tot << ".\n";
            edge[cur_bin] = i; 
            if (cur_bin > 0 && cur_bin+1 < nBins-1 && rate_edge[cur_bin] < 0.21 && rate_edge[cur_bin+1] > 0.21) { // here, we change the value of [tot]
                tot = hReal->Integral(i+1, nbins) + hSample->Integral(1, i);
                UseGlauber = true;
                mult_20percent = i;
            }
            cur_bin ++;
        }
        if (cur_bin >= nBins){
            break;
        }
    }

    ratio->GetUpperPad()->cd();
    TLine* l = new TLine();
    l->SetLineColor(4);
    l->SetLineStyle(3);
    for (int i=1; i<10; i++){
        l->DrawLine(edge[i], hSample->GetYaxis()->GetXmin(), edge[i], hSample->GetBinContent(edge[i]));
    }

    // fit ratio plot
    // for event-wise reweight
    ratio->GetLowerPad()->cd();
    TF1* fRatioFitFunc = new TF1(
        "fRatioFitFunc",
        "[0]+[1]/([2]*x + [3]) + [4]*([2]*x + [3]) + [6]/TMath::Power([2]*x+[3] ,2) + [5]*TMath::Power([2]*x+[3] ,2)",
        5.0, mult_20percent*1.0
    );
    fRatioFitFunc->SetParameters(1, -0.1, 0.4, -0.3, -1e-3, 1e-5, 4); // init. pars
    fRatioFitFunc->SetLineColor(2);
    auto tgRatio = (TGraph*)ratio->GetLowerRefGraph();
    double ratioPars[7];
    tgRatio->Fit(fRatioFitFunc, "RN0Q");
    fRatioFitFunc->GetParameters(ratioPars);
    ratio->GetLowerPad()->cd();
    fRatioFitFunc->DrawClone("lsame");

    // do centrality definition with Npart
    ratio->GetUpperPad()->cd();
    TH1D* hNpart = (TH1D*)h2d->ProjectionX();
    TH1D* hMult = (TH1D*)h2d->ProjectionY();
    double edgeNpart[nBins] = {0};
    cnt = 0;
    tot = hNpart->Integral();
    cur_bin = 0;
    int nbinsNpart = hNpart->GetNbinsX();
    for (int i=nbinsNpart; i>=0; i--){
        cnt += hNpart->GetBinContent(i);
        if (cnt*1.0 / tot > rate_edge[cur_bin]){
            cout << "[LOG] For Npart: " << cur_bin << ", " << rate_edge[cur_bin] << ", " << i << ", " << cnt << " of " << tot << ".\n";
            edgeNpart[cur_bin] = i; 
            cur_bin ++;
        }
        if (cur_bin >= nBins){
            break;
        }
    }

    // using Npart centrality, define
    scaleFactor = hReal->Integral(minMultCut, hReal->GetNbinsX()) / hMult->Integral(minMultCut, hMult->GetNbinsX());
    TH1D* hMultCent[nBins-1];
    for (int i=1; i<nBins; i++) {
        hMultCent[i-1] = h2d->ProjectionY(
            Form("cent%d", i), 
            edgeNpart[i],
            edgeNpart[i-1]
        );
        hMultCent[i-1]->SetLineColor(4);
        hMultCent[i-1]->Scale(scaleFactor);
        hMultCent[i-1]->Draw("hist same");
    }

    TLegend * leg = new TLegend(0.13, 0.4, 0.4, 0.5);
    leg->AddEntry(hReal, "Data", "lep");
    leg->AddEntry(hSample, "MC Glauber", "lep");
    leg->AddEntry(hMultCent[0], "Centrality fractions", "l");
    leg->Draw("same");

    // calculate <Npart>
    double meanNpart[nBins-1];
    TH1D* hNpartCent[nBins-1];
    for (int i=0; i<nBins-1; i++) {
        int nEvent = 0;
        int sumNpart = 0;
        hNpartCent[i] = h2d->ProjectionX(
            Form("npartcent%d", i),
            edge[i+1], edge[i]
        );
        for (int j=0; j<400; j++) {
            sumNpart += hNpartCent[i]->GetBinContent(j+1) * hNpartCent[i]->GetBinCenter(j+1);
            nEvent += hNpartCent[i]->GetBinContent(j+1);
        }
        meanNpart[i] = sumNpart*1.0 / nEvent;
        cout << "[LOG] <Npart> of centrality bin " << i << " (multiplicity " << edge[i+1] << " to " << edge[i] << ") is " << meanNpart[i] << endl;
    }

    ofstream fout("CentralityDefinition.txt");
    const char * cents[nBins-1] = {"0-10%", "10-20%", "20-30%", "30-40%", "40-50%"};
    fout << "|\tCentrality\t|\t<Npart>\t|\tMult>\t|\tMult<\t|\n";
    for (int i=0; i<nBins-1; i++) {
        fout << "|\t" << cents[i] << "\t\t|\t" << meanNpart[i] << "\t|\t" << edge[i+1] << "\t\t|\t" << edge[i] << "\t\t|\n";
    }
    fout << "Reweight function: [0]+[1]/([2]*x + [3]) + [4]*([2]*x + [3]) + [6]/TMath::Power([2]*x+[3] ,2) + [5]*TMath::Power([2]*x+[3] ,2)\n";
    fout << "Reweight parameters: \n{\t";
    for (int i=0; i<7; i++) {
        fout << ratioPars[i];
        if (i != 6) { fout << ",\t"; }
        else { fout << "}\n"; }
    }
    fout.close();

    cout << "[LOG] This is the end of centrality analysis program, please check CentralityDefinition.txt and CentralityDefinition.root!" << endl;
    

    c->Print("CentralityDefinition.pdf");



}