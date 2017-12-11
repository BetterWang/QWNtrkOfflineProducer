#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimDataFormats/HiGenData/interface/GenHIEvent.h"

#include "TFile.h"
#include "TH2.h"
#include "TMath.h"
#include "TRandom.h"

using namespace std;
class QWAccHole : public edm::EDProducer {
public:
	explicit QWAccHole(const edm::ParameterSet&);
	~QWAccHole();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	///
	edm::InputTag	srcPhi_;
	edm::InputTag	srcEta_;
	edm::InputTag	srcPt_;
	edm::InputTag	srcWeight_;
	double		PhiMin_;
	double		PhiMax_;
	double		EtaMin_;
	double		EtaMax_;
	double		eff_;
};

QWAccHole::QWAccHole(const edm::ParameterSet& pset) :
	srcPhi_(pset.getUntrackedParameter<edm::InputTag>("srcPhi")),
	srcEta_(pset.getUntrackedParameter<edm::InputTag>("srcEta")),
	srcPt_(pset.getUntrackedParameter<edm::InputTag>("srcPt")),
	srcWeight_(pset.getUntrackedParameter<edm::InputTag>("srcWeight")),
	PhiMin_(pset.getUntrackedParameter<double>("PhiMin")),
	PhiMax_(pset.getUntrackedParameter<double>("PhiMax")),
	EtaMin_(pset.getUntrackedParameter<double>("EtaMin")),
	EtaMax_(pset.getUntrackedParameter<double>("EtaMax")),
	eff_(pset.getUntrackedParameter< double > ("hole") )
{
	consumes< std::vector<double> >(srcPhi_);
	consumes< std::vector<double> >(srcEta_);
	consumes< std::vector<double> >(srcPt_);
	consumes< std::vector<double> >(srcWeight_);

	produces<std::vector<double> >("phi");
	produces<std::vector<double> >("eta");
	produces<std::vector<double> >("pt");
	produces<std::vector<double> >("weight");
}

QWAccHole::~QWAccHole()
{
	return;
}

void QWAccHole::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	Handle< std::vector<double> > pphi;
	Handle< std::vector<double> > peta;
	Handle< std::vector<double> > ppt;
	Handle< std::vector<double> > pweight;
	iEvent.getByLabel(srcPhi_, pphi);
	iEvent.getByLabel(srcEta_, peta);
	iEvent.getByLabel(srcPt_ , ppt);
	iEvent.getByLabel(srcWeight_, pweight);

	std::vector<double> phi = *pphi;
	std::vector<double> eta = *peta;
	std::vector<double> pt = *ppt;
	std::vector<double> weight = *pweight;

	std::vector<double> pphi_new;
	std::vector<double> peta_new;
	std::vector<double> ppt_new;
	std::vector<double> pweight_new;

	for ( unsigned int i = 0; i < phi.size(); i++ ) {
		if ( phi[i] > PhiMin_ and phi[i] < PhiMax_ and eta[i] > EtaMin_ and eta[i] < EtaMax_ ) {
			if ( gRandom->Uniform() < eff_ ) {
				weight[i] /= eff_;
			} else {
				continue;
			}
		}

		pphi_new.push_back(phi[i]);
		peta_new.push_back(eta[i]);
		ppt_new.push_back(pt[i]);
		pweight_new.push_back(weight[i]);
	}

	iEvent.put(std::make_unique<std::vector<double>>(pphi_new), std::string("phi"));
	iEvent.put(std::make_unique<std::vector<double>>(peta_new), std::string("eta"));
	iEvent.put(std::make_unique<std::vector<double>>(ppt_new), std::string("pt"));
	iEvent.put(std::make_unique<std::vector<double>>(pweight_new), std::string("weight"));
}


DEFINE_FWK_MODULE(QWAccHole);
