#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <DataFormats/HeavyIonEvent/interface/ClusterCompatibility.h>

using namespace std;
class QWClusterCompatibilityProducer : public edm::EDProducer {
public:
	explicit QWClusterCompatibilityProducer(const edm::ParameterSet&);
	~QWClusterCompatibilityProducer();

private:
	virtual void produce(edm::Event&, const edm::EventSetup&) override;
	///

	edm::InputTag	src_;
    double minZ_, maxZ_;

};

QWClusterCompatibilityProducer::QWClusterCompatibilityProducer(const edm::ParameterSet& pset) :
	src_(pset.getUntrackedParameter<edm::InputTag>("src")),
    minZ_(pset.getUntrackedParameter<double>("minZ")),
    maxZ_(pset.getUntrackedParameter<double>("maxZ"))
{
	consumes<reco::ClusterCompatibility>(src_);

	produces<double>("nPxlHits");
	produces<double>("clusVtxQual");
}

QWClusterCompatibilityProducer::~QWClusterCompatibilityProducer()
{
	return;
}

void QWClusterCompatibilityProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    edm::Handle<reco::ClusterCompatibility> cc;
	iEvent.getByLabel(src_ , cc) ;

    double nPxlHits = cc->nValidPixelHits();
    double clusVtxQual = 0.;
    {
        // will compare cluster compatibility at a determined best
        // z position to + and - 10 cm from the best position
        float best_z = 0.;
        int best_n= 0.,low_n = 0.,high_n = 0.;


        // look for best vertex z position within zMin to zMax range
        // best position is determined by maximum nHit with
        // chi used for breaking a tie
        int nhits_max = 0;
        double chi_max = 1e+9;
        for( int i=0; i<cc->size(); i++ )
        {
            if( cc->z0(i) > maxZ_ || cc->z0(i) < minZ_ ) continue;
            if(cc->nHit(i) == 0) continue;
            if(cc->nHit(i) > nhits_max) {
                chi_max = 1e+9;
                nhits_max = cc->nHit(i);
            }
            if(cc->nHit(i) >= nhits_max && cc->chi(i) < chi_max) {
                chi_max = cc->chi(i);
                best_z = cc->z0(i); best_n = cc->nHit(i);
            }
        }

        // find compatible clusters at + or - 10 cm of the best,
        // or get as close as possible in terms of z position.
        double low_target = best_z - 10.0;
        double high_target = best_z + 10.0;
        double low_match = 1000., high_match = 1000.;
        for( int i=0; i<cc->size(); i++ )
        {
            if( fabs(cc->z0(i)-low_target) < low_match )
            {
                low_n = cc->nHit(i);
                low_match = fabs(cc->z0(i)-low_target);
            }
            if( fabs(cc->z0(i)-high_target) < high_match )
            {
                high_n = cc->nHit(i);
                high_match = fabs(cc->z0(i)-high_target);
            }
        }

        // determine vertex compatibility quality score
        if ((low_n+high_n)> 0)
            clusVtxQual = (2.0*best_n)/(low_n+high_n);  // A/B
        else if (best_n > 0)
            clusVtxQual = 1000.0;                      // A/0 (set to arbitrarily large number)
        else
            clusVtxQual = 0;
    }

    iEvent.put(make_unique<double>(nPxlHits), "nPxlHits");
    iEvent.put(make_unique<double>(clusVtxQual), "clusVtxQual");
}


DEFINE_FWK_MODULE(QWClusterCompatibilityProducer);
