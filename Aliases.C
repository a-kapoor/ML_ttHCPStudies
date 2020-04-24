float DPhi( float phi1, float phi2 )
{
  float deltaphi = phi2-phi1;
  if(      deltaphi>  TMath::Pi() ) deltaphi -= 2.0*TMath::Pi();
  else if( deltaphi<=-TMath::Pi() ) deltaphi += 2.0*TMath::Pi();
  deltaphi=TMath::Abs(deltaphi);
  return deltaphi;
}

float Wt( float a, float b)
{
  float c=(a)/b;
  return c;
}

float ThreeObjMass(float pt1, float eta1, float phi1, float E1, float pt2, float eta2, float phi2, float E2, float pt3, float eta3, float phi3, float E3)
{
  TLorentzVector Obj1{0,0,0,0};
  TLorentzVector Obj2{0,0,0,0};
  TLorentzVector Obj3{0,0,0,0};
  Obj1.SetPtEtaPhiE(pt1,eta1,phi1,E1);
  Obj2.SetPtEtaPhiE(pt2,eta2,phi2,E2);
  Obj3.SetPtEtaPhiE(pt3,eta3,phi3,E3);
  return (Obj1+Obj2+Obj3).M();
}


float ThreeObjPt(float pt1, float eta1, float phi1, float E1, float pt2, float eta2, float phi2, float E2, float pt3, float eta3, float phi3, float E3)
{
  TLorentzVector Obj1{0,0,0,0};
  TLorentzVector Obj2{0,0,0,0};
  TLorentzVector Obj3{0,0,0,0};
  Obj1.SetPtEtaPhiE(pt1,eta1,phi1,E1);
  Obj2.SetPtEtaPhiE(pt2,eta2,phi2,E2);
  Obj3.SetPtEtaPhiE(pt3,eta3,phi3,E3);
  return (Obj1+Obj2+Obj3).Pt();
}
