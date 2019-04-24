#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "TrackingTools/TransientTrack/interface/TransientTrack.h"

#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "MagneticField/VolumeBasedEngine/interface/VolumeBasedMagneticField.h"


#include "TFile.h"
#include "TH2.h"
#include "TMath.h"

using namespace std;
class QWEvent2018Producer : public edm::EDProducer {
public:
	explicit QWEvent2018Producer(const edm::ParameterSet&);
	~QWEvent2018Producer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	///

	edm::InputTag	trackSrc_;
	edm::InputTag	fweight_;
	edm::InputTag	centralitySrc_;

	double	pTmin_;
	double	pTmax_;
	double	Etamin_;
	double	Etamax_;

	bool	bEff_;

	TH2D*	hEff_cbin[200];
};

QWEvent2018Producer::QWEvent2018Producer(const edm::ParameterSet& pset) :
	trackSrc_(pset.getUntrackedParameter<edm::InputTag>("trackSrc")),
	fweight_(pset.getUntrackedParameter<edm::InputTag>("fweight")),
	centralitySrc_(pset.getUntrackedParameter<edm::InputTag>("centralitySrc"))
{
	consumes<int>(centralitySrc_);
	consumes<reco::TrackCollection>(trackSrc_);

	pTmin_ = pset.getUntrackedParameter<double>("ptMin", 0.3);
	pTmax_ = pset.getUntrackedParameter<double>("ptMax", 3.0);
	Etamin_ = pset.getUntrackedParameter<double>("Etamin", -2.4);
	Etamax_ = pset.getUntrackedParameter<double>("Etamax", 2.4);

	bEff_ = true;

	string streff = fweight_.label();
	if ( streff == string("NA") ) {
		cout << "!!! eff NA" << endl;
		bEff_ = false;
	} else if ( streff == string("EffCorrectionsPixelPbPb2018_v0.root") ) {
        TFile * fEffFak = new TFile(streff.c_str());
        cout << "!!! Using particle weight " << streff << endl;
        cout << "!!! Apply Eff correction" << endl;
        if ( streff == string("EffCorrectionsPixelPbPb2018_v0.root") ) {
            TH2D * h = (TH2D*) fEffFak->Get("Eff_0_5");
            for ( int c = 0; c < 10; c++ ) {
                hEff_cbin[c] = h;
            }
            h = (TH2D*) fEffFak->Get("Eff_5_10");
            for ( int c = 10; c < 20; c++ ) {
                hEff_cbin[c] = h;
            }
            h = (TH2D*) fEffFak->Get("Eff_10_30");
            for ( int c = 20; c < 60; c++ ) {
                hEff_cbin[c] = h;
            }
            h = (TH2D*) fEffFak->Get("Eff_30_50");
            for ( int c = 60; c < 100; c++ ) {
                hEff_cbin[c] = h;
            }
            h = (TH2D*) fEffFak->Get("Eff_50_100");
            for ( int c = 100; c < 200; c++ ) {
                hEff_cbin[c] = h;
            }
        }
    }
	produces<std::vector<double> >("phi");
	produces<std::vector<double> >("eta");
	produces<std::vector<double> >("pt");
	produces<std::vector<double> >("px");
	produces<std::vector<double> >("py");
	produces<std::vector<double> >("pz");
	produces<std::vector<double> >("weight");
	produces<std::vector<double> >("charge");
	produces<std::vector<double> >("ref");
	produces<std::vector<double> >("Nchi2");
	produces<std::vector<double> >("Nchi2oNLayers");
}

QWEvent2018Producer::~QWEvent2018Producer()
{
	return;
}

void QWEvent2018Producer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	using namespace reco;

	auto pphi = std::make_unique<std::vector<double>>();
	auto peta = std::make_unique<std::vector<double>>();
	auto ppT = std::make_unique<std::vector<double>>();
	auto ppX = std::make_unique<std::vector<double>>();
	auto ppY = std::make_unique<std::vector<double>>();
	auto ppZ = std::make_unique<std::vector<double>>();
	auto pweight = std::make_unique<std::vector<double>>();
	auto pref = std::make_unique<std::vector<double>>();
	auto pcharge = std::make_unique<std::vector<double>>();
	auto pNchi2 = std::make_unique<std::vector<double>>();
	auto pNchi2oNLayers = std::make_unique<std::vector<double>>();

	Handle<TrackCollection> tracks;
	iEvent.getByLabel(trackSrc_,tracks);

	edm::Handle<int> ch;
	iEvent.getByLabel(centralitySrc_,ch);
	int Cent = *(ch.product());
	for(TrackCollection::const_iterator itTrack = tracks->begin();
			itTrack != tracks->end();
			++itTrack) {

		if ( itTrack->eta() > Etamax_ or itTrack->eta() < Etamin_ or itTrack->pt() > pTmax_ or itTrack->pt() < pTmin_ ) continue;

		double eff = 0.;

		if ( bEff_ ) {
			eff = hEff_cbin[Cent]->GetBinContent( hEff_cbin[Cent]->FindBin( itTrack->eta(), itTrack->pt() ) );
		} else {
			eff = 1.;
		}

		double weight;
		if ( eff >= 0.01 and not TMath::IsNaN(eff) ) {
			weight = 1./eff;
		} else {
			weight = 0.;
		}

		pphi->push_back(itTrack->phi());
		peta->push_back(itTrack->eta());
		ppT->push_back(itTrack->pt());
		ppX->push_back(itTrack->px());
		ppY->push_back(itTrack->py());
		ppZ->push_back(itTrack->pz());
		pweight->push_back(weight);
		pcharge->push_back(itTrack->charge());
		pNchi2->push_back(itTrack->normalizedChi2());
		pNchi2oNLayers->push_back(itTrack->normalizedChi2() / itTrack->hitPattern().trackerLayersWithMeasurement());
		pref->push_back(std::distance(tracks->begin(), itTrack));
	}

	iEvent.put(std::move(pphi), std::string("phi"));
	iEvent.put(std::move(peta), std::string("eta"));
	iEvent.put(std::move(ppT), std::string("pt"));
	iEvent.put(std::move(ppX), std::string("px"));
	iEvent.put(std::move(ppY), std::string("py"));
	iEvent.put(std::move(ppZ), std::string("pz"));
	iEvent.put(std::move(pweight), std::string("weight"));
	iEvent.put(std::move(pcharge), std::string("charge"));
	iEvent.put(std::move(pNchi2), std::string("Nchi2"));
	iEvent.put(std::move(pNchi2oNLayers), std::string("Nchi2oNLayers"));
	iEvent.put(std::move(pref), std::string("ref"));
}


DEFINE_FWK_MODULE(QWEvent2018Producer);
