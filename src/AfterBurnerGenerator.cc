
/*
   ________________________________________________________________________

   AfterBurnerVtxGenerator

   Giving flow modulation for the generated particles

   ________________________________________________________________________
   */

// Quan Wang


#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

#include "CLHEP/Random/RandGaussQ.h"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Units/GlobalSystemOfUnits.h"
#include "CLHEP/Units/GlobalPhysicalConstants.h"
////#include "CLHEP/Vector/ThreeVector.h"
#include "HepMC/GenEvent.h"
#include "HepMC/GenParticle.h"
#include "HepMC/HeavyIon.h"
#include "HepMC/SimpleVector.h"
#include "TMatrixD.h"
#include "TF1.h"
#include "TMath.h"

#include <iostream>

using namespace edm;
using namespace std;
using namespace CLHEP;

class RandGaussQ;

double pEllP(double * x, double * par){

  int nbin = 50;
  //-- [0] = e0
  //-- [1] = alpha
  //-- [2] = kn
  //-- [3] = Scale
  //-- xx  = vn

  double e0    = par[0];
  double alpha = par[1];
  double kn    = par[2];
  double scale = par[3];
  double eccn  = x[0] / kn;

  double pi = TMath::Pi();
  double p1 = (scale * 2. * alpha * eccn / pi / kn) * pow( 1 - e0*e0, alpha + 0.5);

  double integ = 0.;
  double dphi = pi/(double)nbin;
  for(int i = 1; i <= nbin; i++){
    double phi = (2*(double)i-1.)*pi/(2.*(double)nbin);
    integ += dphi * pow(1-eccn*eccn, alpha-1) * pow( 1-e0*eccn*cos(phi), -2.*alpha-1 );
  }

  double ellp = p1 * integ;

  return ellp;

}


class AfterBurnerGenerator : public edm::EDProducer{
	public:
		AfterBurnerGenerator(const edm::ParameterSet & p);
		virtual ~AfterBurnerGenerator();

		/// return a new event vertex
		virtual void produce( edm::Event&, const edm::EventSetup& );


	private:
		/** Copy constructor */
		AfterBurnerGenerator(const AfterBurnerGenerator &p);
		/** Copy assignment operator */
		AfterBurnerGenerator&  operator = (const AfterBurnerGenerator & rhs );


		edm::InputTag            sourceLabel;
		edm::InputTag            modv1;
		edm::InputTag            modv2;
		edm::InputTag            modv3;
		edm::InputTag            modv4;
		edm::InputTag            modv5;
		edm::InputTag            modv6;

		int		modmethod;
		// 0, no mod
		// 1, complete rnd, without non-flow
		// 2, Newton method, with non-flow

		bool		fixEP;
		double		fluct_v1;
		double		fluct_v2;
		double		fluct_v3;
		double		fluct_v4;
		double		fluct_v5;
		double		fluct_v6;

		TF1 *		fv1;
		TF1 *		fv2;
		TF1 *		fv3;
		TF1 *		fv4;
		TF1 *		fv5;
		TF1 *		fv6;

		CLHEP::RandGaussQ*  fRandom ;
//		CLHEP::RandFlat*    fFlat;

		double GetV1(double);
		double GetV2(double);
		double GetV3(double);
		double GetV4(double);
		double GetV5(double);
		double GetV6(double);
};


AfterBurnerGenerator::AfterBurnerGenerator(const edm::ParameterSet & p ):
	sourceLabel(p.getParameter<edm::InputTag>("src")),
	fixEP(p.getUntrackedParameter<bool>("fixEP",true))
{

	modv1 = p.getParameter<edm::InputTag>("modv1");
	modv2 = p.getParameter<edm::InputTag>("modv2");
	modv3 = p.getParameter<edm::InputTag>("modv3");
	modv4 = p.getParameter<edm::InputTag>("modv4");
	modv5 = p.getParameter<edm::InputTag>("modv5");
	modv6 = p.getParameter<edm::InputTag>("modv6");

	modmethod = p.getParameter<int>("modmethod");

	fv1 = new TF1("fv1", modv1.label().c_str());
	if ( modv2.label() == string("pBG_4pct_v2") ) {
		//-- Bessel-Gaussian, <v2> ~ 0.04
		double BGmu_4pct_v2  = 3.46887e-02;
		double BGsig_4pct_v2 = 2.47271e-02;
		fv2 = new TF1("fv2","[0]*(x/([2]*[2]))*TMath::Exp(-(x*x+[1]*[1])/(2*[2]*[2]))*TMath::BesselI0(([1]*x)/([2]*[2]))", 0.00, 0.6);
		fv2->SetParNames("Scale", "mu", "sigma");
		fv2->SetParameters(1, BGmu_4pct_v2, BGsig_4pct_v2);
	} else if ( modv2.label() == string("fEllP_8pct_v2") ) {
		//-- Elliptic power law, <v2> ~ 0.08
		double ecc0_8pct_v2  = 2.16286e-01;
		double alpha_8pct_v2 = 5.02657e+01;
		double kn_8pct_v2    = 3.79222e-01;
		fv2 = new TF1("fEllP_8pct_v2", pEllP, 0.0, kn_8pct_v2, 4); //-- There's an asymptote at vn = kn, don't let the TF1 range exceed that value
		fv2->SetParNames("ecc0", "alpha", "kn", "Scale");
		fv2->SetParameters(ecc0_8pct_v2, alpha_8pct_v2, kn_8pct_v2, 1.0);
	} else
		fv2 = new TF1("fv2", modv2.label().c_str());
	if ( modv3.label() == string("pBG_2pct_v3") ) {
		//-- Bessel-Gaussian, <v3> ~ 0.02
		double BGmu_2pct_v3  = 2.14247e-02;
		double BGsig_2pct_v3 = 1.92158e-02;
		TF1 * pBG_2pct_v3 = new TF1("pBG_2pct_v3","[0]*(x/([2]*[2]))*TMath::Exp(-(x*x+[1]*[1])/(2*[2]*[2]))*TMath::BesselI0(([1]*x)/([2]*[2]))", 0.00, 0.6);
		pBG_2pct_v3->SetParNames("Scale", "mu", "sigma");
		pBG_2pct_v3->SetParameters(1, BGmu_2pct_v3, BGsig_2pct_v3);
	} else
		fv3 = new TF1("fv3", modv3.label().c_str());
	fv4 = new TF1("fv4", modv4.label().c_str());
	fv5 = new TF1("fv5", modv5.label().c_str());
	fv6 = new TF1("fv6", modv6.label().c_str());

	fluct_v1 = p.getParameter<double>("fluct_v1");
	fluct_v2 = p.getParameter<double>("fluct_v2");
	fluct_v3 = p.getParameter<double>("fluct_v3");
	fluct_v4 = p.getParameter<double>("fluct_v4");
	fluct_v5 = p.getParameter<double>("fluct_v5");
	fluct_v6 = p.getParameter<double>("fluct_v6");

	produces<double>("v1"); 
	produces<double>("v2"); 
	produces<double>("v3"); 
	produces<double>("v4"); 
	produces<double>("v5"); 
	produces<double>("v6"); 

	produces<double>("flucv1"); 
	produces<double>("flucv2"); 
	produces<double>("flucv3"); 
	produces<double>("flucv4"); 
	produces<double>("flucv5"); 
	produces<double>("flucv6"); 

}

AfterBurnerGenerator::~AfterBurnerGenerator() 
{
}



void AfterBurnerGenerator::produce( Event& evt, const EventSetup& )
{
	edm::Service<edm::RandomNumberGenerator> rng;

	if ( ! rng.isAvailable()) {

		throw cms::Exception("Configuration")
			<< "The BaseEvtVtxGenerator requires the RandomNumberGeneratorService\n"
			"which is not present in the configuration file.  You must add the service\n"
			"in the configuration file or remove the modules that require it.";
	}

	CLHEP::HepRandomEngine& engine = rng->getEngine(evt.streamID());

	if ( fRandom ) delete fRandom;
	fRandom = new CLHEP::RandGaussQ(engine);
	CLHEP::RandFlat    fFlat(engine);


	Handle<HepMCProduct> HepMCEvt ;
	evt.getByLabel( sourceLabel, HepMCEvt ) ;

	HepMC::GenEvent * genevt = (HepMC::GenEvent *)HepMCEvt->GetEvent();
	HepMC::HeavyIon * hi = genevt->heavy_ion();
	double ep = 0;
	if ( hi ) {
		ep = hi->event_plane_angle();
	} else {
		ep = 0;
	}

	double meanv1 = 0;
	double meanv2 = 0;
	double meanv3 = 0;
	double meanv4 = 0;
	double meanv5 = 0;
	double meanv6 = 0;

	double sigmav1 = 0;
	double sigmav2 = 0;
	double sigmav3 = 0;
	double sigmav4 = 0;
	double sigmav5 = 0;
	double sigmav6 = 0;

	double v1 = -1;
	double v2 = -1;
	double v3 = -1;
	double v4 = -1;
	double v5 = -1;
	double v6 = -1;

	int n = 0;
	for ( HepMC::GenEvent::particle_iterator part = genevt->particles_begin();
			part != genevt->particles_end(); ++part ) {
		double E = (*part)->momentum().e();
		double px = (*part)->momentum().x();
		double py = (*part)->momentum().y();
		double pz = (*part)->momentum().z();
		double pt = sqrt(px*px+py*py);

		do { v1 = GetV1(pt); } while ( v1 < 0);
		do { v2 = GetV2(pt); } while ( v2 < 0);
		do { v3 = GetV3(pt); } while ( v3 < 0);
		do { v4 = GetV4(pt); } while ( v4 < 0);
		do { v5 = GetV5(pt); } while ( v5 < 0);
		do { v6 = GetV6(pt); } while ( v6 < 0);


		meanv1 += v1;
		meanv2 += v2;
		meanv3 += v3;
		meanv4 += v4;
		meanv5 += v5;
		meanv6 += v6;

		sigmav1 += v1*v1;
		sigmav2 += v2*v2;
		sigmav3 += v3*v3;
		sigmav4 += v4*v4;
		sigmav5 += v5*v5;
		sigmav6 += v6*v6;
		n++;

		if ( modmethod == 0 ) continue;
		if ( modmethod == 1 )  {
			double phi;
			double pmax = 1+2*fabs(v1)+2*fabs(v2)+2*fabs(v3)+2*fabs(v4)+2*fabs(v5)+2*fabs(v6);
			double ptest = 0;
			do {
				phi = fFlat.fire(-CLHEP::pi, CLHEP::pi);
				ptest = 1+2*v1*cos(phi) + 2*v2*cos(2*phi) + 2*v3*cos(3*phi) + 2*v4*cos(4*phi) + 2*v5*cos(5*phi) + 2*v6*cos(6*phi);
			} while ( ptest < fFlat.fire(pmax) );
			if (!fixEP) phi += ep;
			px = pt*cos(phi);
			py = pt*sin(phi);
			(*part)->set_momentum(HepMC::FourVector(px, py, pz, E));
		}
		if ( modmethod == 2 ) {
			HepMC::FourVector p = (*part)->momentum();
			double phi1 = -999;
			double dphi = 999;
			double phi_RP = 0;
			if ( !fixEP ) phi_RP = ep;
			double phi0 = p.phi() - v2 * sin( 2*(p.phi() - phi_RP) );
			phi1 = phi0 - v2*sin( 2*(phi0 - phi_RP) );
			do {
				double f = phi0 - p.phi() + 2*v1*sin( phi0 - phi_RP ) + v2 * sin( 2*(phi0-phi_RP) ) + 2./3.*v3*sin( 3*(phi0-phi_RP) ) + 0.5*v4*sin( 4*(phi0-phi_RP) ) + 0.4*v5*sin( 5*(phi0-phi_RP) ) + 1./3.*v6*sin( 6*(phi0-phi_RP) );
				double fp = 1 + 2*v1*cos(phi0-phi_RP) + 2*v2*cos( 2*(phi0-phi_RP) ) + 2*v3*cos( 3*(phi0-phi_RP) ) + 2*v4*cos( 4*(phi0-phi_RP) ) + 2*v5*cos( 5*(phi0-phi_RP)) + 2*v5*cos( 6*(phi0-phi_RP) ) + 2*v6*cos( 6*(phi0-phi_RP) );
				phi1 = phi0 - f / fp;
				dphi = phi1 - phi0;
				phi0 = phi1;
			} while ( fabs(dphi)>0.01 );
			px = pt * cos( phi1 );
			py = pt * sin( phi1 );
			(*part)->set_momentum(HepMC::FourVector(px, py, pz, E));
		}
	}

	auto_ptr<double> ptr_v1(new double(meanv1/n)) ;
	auto_ptr<double> ptr_v2(new double(meanv2/n)) ;
	auto_ptr<double> ptr_v3(new double(meanv3/n)) ;
	auto_ptr<double> ptr_v4(new double(meanv4/n)) ;
	auto_ptr<double> ptr_v5(new double(meanv5/n)) ;
	auto_ptr<double> ptr_v6(new double(meanv6/n)) ;
	evt.put(ptr_v1, "v1");
	evt.put(ptr_v2, "v2");
	evt.put(ptr_v3, "v3");
	evt.put(ptr_v4, "v4");
	evt.put(ptr_v5, "v5");
	evt.put(ptr_v6, "v6");
	auto_ptr<double> ptr_fv1(new double(sigmav1/n)) ;
	auto_ptr<double> ptr_fv2(new double(sigmav2/n)) ;
	auto_ptr<double> ptr_fv3(new double(sigmav3/n)) ;
	auto_ptr<double> ptr_fv4(new double(sigmav4/n)) ;
	auto_ptr<double> ptr_fv5(new double(sigmav5/n)) ;
	auto_ptr<double> ptr_fv6(new double(sigmav6/n)) ;
	evt.put(ptr_fv1, "flucv1");
	evt.put(ptr_fv2, "flucv2");
	evt.put(ptr_fv3, "flucv3");
	evt.put(ptr_fv4, "flucv4");
	evt.put(ptr_fv5, "flucv5");
	evt.put(ptr_fv6, "flucv6");
	return ;
}

double
AfterBurnerGenerator::GetV1(double pt)
{
	return fRandom->fire(fv1->Eval(pt), fluct_v1);
}


double
AfterBurnerGenerator::GetV2(double pt)
{
//	return fRandom->fire(fv2->Eval(pt), fluct_v2);
	return fv2->GetRandom();
}

double
AfterBurnerGenerator::GetV3(double pt)
{
//	return fRandom->fire(fv3->Eval(pt), fluct_v3);
	return fv3->GetRandom();
}

double
AfterBurnerGenerator::GetV4(double pt)
{
	return fRandom->fire(fv4->Eval(pt), fluct_v4);
}

double
AfterBurnerGenerator::GetV5(double pt)
{
	return fRandom->fire(fv5->Eval(pt), fluct_v5);
}

double
AfterBurnerGenerator::GetV6(double pt)
{
	return fRandom->fire(fv6->Eval(pt), fluct_v6);
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(AfterBurnerGenerator);
