from ROOT import TCanvas, TPad, TFile, TPaveLabel, TPaveText, TTree, TH1F, TLegend
from ROOT import gROOT, gStyle

gStyle.SetOptStat(0)
 
c1 = TCanvas( 'c1', 'Histogram Drawing Options', 200, 10, 700, 900 )

f1 = TFile('newSamples/DiLepRegion/2018/DiLepRegion/TTH_DiLepRegion.root')
f2 = TFile('newSamples/DiLepRegion/2018/DiLepRegion/ttH_DiLepRegion.root')

f1tree     = f1.Get('syncTree')
f2tree     = f2.Get('syncTree')

h1 =TH1F()
h2 =TH1F()

f2tree.Draw('jet1_eta-jet2_eta>>h1(20,-5,5)','(EVENT_rWeights[59]/EVENT_originalXWGTUP)*(angle_bbpp_highest2b>0&&(n_presel_jet>=3 && is_tH_like_and_not_ttH_like !=1 && HiggsDecay!=1))')
h1 = gROOT.FindObject('h1')
h1.Scale(1/h1.Integral())
f2tree.Draw('jet1_eta-jet2_eta>>h2(20,-5,5)','(EVENT_rWeights[11]/EVENT_originalXWGTUP)*(angle_bbpp_highest2b>0&&(n_presel_jet>=3 && is_tH_like_and_not_ttH_like !=1 && HiggsDecay!=1))','same')
h2 = gROOT.FindObject('h2')
h2.Scale(1/h2.Integral())


h1.SetMarkerColor(2)
h1.SetLineColor(2)
h1.SetMarkerSize(2)
h1.SetMarkerStyle(21)


h2.SetMarkerColor(4)
h2.SetLineColor(4)
h2.SetMarkerSize(2)
h2.SetMarkerStyle(22)

h1.GetYaxis().SetRangeUser(0., 0.2);
h1.GetXaxis().SetTitle("#Delta #eta leading two jets");
h1.SetTitle("#Delta #eta leading two jets (2018)");
h1.Draw('E1')
h2.Draw('E1SAME')

leg = TLegend(.4,.2,.6,.3)
leg.SetBorderSize(0)
leg.SetFillColor(0)
leg.SetFillStyle(0)
leg.SetTextFont(42)
leg.SetTextSize(0.035)
leg.AddEntry(h1,"Pure CP odd","p")
leg.AddEntry(h2,"Pure CP even","p")
leg.Draw()

c1.SaveAs('plot_deta_2jets_notbs_2018.png')
