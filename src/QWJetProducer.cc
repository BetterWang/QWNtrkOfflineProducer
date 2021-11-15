#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "TMath.h"

using namespace std;
class QWJetProducer : public edm::EDProducer {
    public:
        explicit QWJetProducer(const edm::ParameterSet&);
        ~QWJetProducer();

    private:
        virtual void produce(edm::Event&, const edm::EventSetup&) override;
        ///

        edm::InputTag	jetSrc_;
        edm::EDGetTokenT<reco::JetView> jetTag_;
};

QWJetProducer::QWJetProducer(const edm::ParameterSet& pset) :
    jetSrc_(pset.getUntrackedParameter<edm::InputTag>("jetSrc"))
{
    jetTag_ = consumes<reco::JetView>(jetSrc_);

    produces<std::vector<double> >("phi");
    produces<std::vector<double> >("eta");
    produces<std::vector<double> >("pt");
}

QWJetProducer::~QWJetProducer()
{
    return;
}

void QWJetProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    using namespace edm;
    using namespace reco;

    std::unique_ptr<std::vector<double> > pphi( new std::vector<double> );
    std::unique_ptr<std::vector<double> > peta( new std::vector<double> );
    std::unique_ptr<std::vector<double> > ppT( new std::vector<double> );


    edm::Handle<reco::JetView> jets;
    iEvent.getByToken(jetTag_, jets);

    for (unsigned int j = 0; j < jets->size(); ++j) {
        const reco::Jet& jet = (*jets)[j];
        double jtpt = jet.pt();
        double jtphi = jet.phi();
        double jteta = jet.eta();

        pphi->push_back(jtphi);
        peta->push_back(jteta);
        ppT->push_back(jtpt);
    }

    iEvent.put(move(pphi), std::string("phi"));
    iEvent.put(move(peta), std::string("eta"));
    iEvent.put(move(ppT), std::string("pt"));
}


DEFINE_FWK_MODULE(QWJetProducer);
