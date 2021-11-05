#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "TFile.h"
#include "TH2.h"
#include "TMath.h"

using namespace std;
class QWMiniAODEventProducer : public edm::EDProducer {
    public:
        explicit QWMiniAODEventProducer(const edm::ParameterSet&);
        ~QWMiniAODEventProducer();

    private:
        virtual void produce(edm::Event&, const edm::EventSetup&) override;
        ///
        edm::EDGetTokenT<reco::VertexCollection>            vertexToken_;
        edm::EDGetTokenT<edm::View<pat::PackedCandidate>>   trackToken_;
        edm::EDGetTokenT<edm::ValueMap<float>>              chi2Map_;

        double	pTmin_;
        double	pTmax_;
        double	Etamin_;
        double	Etamax_;
};

QWMiniAODEventProducer::QWMiniAODEventProducer(const edm::ParameterSet& pset) :
    vertexToken_( consumes<reco::VertexCollection>(pset.getUntrackedParameter<edm::InputTag>("vertexSrc")) ),
    trackToken_( consumes<edm::View<pat::PackedCandidate>>(pset.getUntrackedParameter<edm::InputTag>("trackSrc")) ),
    chi2Map_( consumes<edm::ValueMap<float>>( pset.getUntrackedParameter< edm::InputTag >( "trackschi2" ) ) )
{
    pTmin_ = pset.getUntrackedParameter<double>("ptMin", 0.3);
    pTmax_ = pset.getUntrackedParameter<double>("ptMax", 3.0);
    Etamin_ = pset.getUntrackedParameter<double>("Etamin", -2.4);
    Etamax_ = pset.getUntrackedParameter<double>("Etamax", 2.4);

    produces<std::vector<double> >("phi");
    produces<std::vector<double> >("eta");
    produces<std::vector<double> >("pt");
    produces<std::vector<double> >("px");
    produces<std::vector<double> >("py");
    produces<std::vector<double> >("pz");
    produces<std::vector<double> >("weight");
    produces<std::vector<double> >("charge");
    produces<std::vector<double> >("ref");
    produces<std::vector<double> >("vz");
    produces<std::vector<double> >("Nchi2");
    produces<std::vector<double> >("Nchi2oNLayers");
}

QWMiniAODEventProducer::~QWMiniAODEventProducer()
{
    return;
}

void QWMiniAODEventProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
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
    auto pvz = std::make_unique<std::vector<double>>();
    auto pNchi2 = std::make_unique<std::vector<double>>();
    auto pNchi2oNLayers = std::make_unique<std::vector<double>>();

    auto vertexCollection = iEvent.getHandle( vertexToken_ );
    auto recoVertices = *vertexCollection;

    unsigned int primaryvtx = 0;
    for ( primaryvtx = 0; primaryvtx < recoVertices.size(); primaryvtx++ ) {
        if ( (!recoVertices[primaryvtx].isFake())
                and fabs(recoVertices[primaryvtx].z()) <= 25.
                and recoVertices[primaryvtx].position().Rho() <= 2.0
                ) break;
    }
    if ( primaryvtx == recoVertices.size() ) {
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
        iEvent.put(std::move(pvz), std::string("vz"));
        return;
    }
    pvz->push_back(recoVertices[primaryvtx].z());
    double bestvzError = recoVertices[primaryvtx].zError();
    math::XYZPoint bestvtx = recoVertices[primaryvtx].position();
    math::Error<3>::type vtx_cov = recoVertices[primaryvtx].covariance();

    auto tracks = iEvent.getHandle( trackToken_ );
    auto chi2Map = iEvent.getHandle( chi2Map_ );

    int trk_idx = -1;
    for( auto const& trk : *tracks) {
        trk_idx++;

        if ( !trk.hasTrackDetails() ) continue;
        auto itTrack = trk.pseudoTrack();

        if ( itTrack.eta() > Etamax_ or itTrack.eta() < Etamin_ or itTrack.pt() > pTmax_ or itTrack.pt() < pTmin_ ) continue;

        double dzvtx = itTrack.dz( bestvtx );
        double dxyvtx = itTrack.dxy( bestvtx );
        double dzerror = std::hypot( itTrack.dzError(), bestvzError );
        double dxyerror = itTrack.dxyError( bestvtx, vtx_cov );

        auto hit_pattern = itTrack.hitPattern();

        double chi2ndof = ( double  ) ( *chi2Map  )[ tracks->ptrAt( trk_idx )  ];

        if ( std::abs( itTrack.ptError()  ) / itTrack.pt() >= 0.1  ) continue;
        if ( std::abs( dzvtx / dzerror  ) >= 3  ) continue;
        if ( std::abs( dxyvtx / dxyerror  ) >= 3  ) continue;
        if ( ( chi2ndof / hit_pattern.trackerLayersWithMeasurement()  ) >= 0.18  ) continue;
        if ( itTrack.numberOfValidHits() < 11  ) continue;

        pphi->push_back(itTrack.phi());
        peta->push_back(itTrack.eta());
        ppT->push_back(itTrack.pt());
        ppX->push_back(itTrack.px());
        ppY->push_back(itTrack.py());
        ppZ->push_back(itTrack.pz());
        pweight->push_back(1.);
        pcharge->push_back(itTrack.charge());
        pNchi2->push_back(chi2ndof);
        pNchi2oNLayers->push_back( chi2ndof / hit_pattern.trackerLayersWithMeasurement() );
        pref->push_back(trk_idx++);
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
    iEvent.put(std::move(pvz), std::string("vz"));
}

DEFINE_FWK_MODULE(QWMiniAODEventProducer);
