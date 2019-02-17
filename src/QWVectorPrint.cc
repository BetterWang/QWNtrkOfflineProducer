#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1D.h"
#include <iostream>

class QWVectorPrint : public edm::EDAnalyzer {
public:
	explicit QWVectorPrint(const edm::ParameterSet&);
	~QWVectorPrint() {};
private:
	virtual void beginJob() {};
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	virtual void endJob() {};

	edm::InputTag   src_;
};

QWVectorPrint::QWVectorPrint(const edm::ParameterSet& pset) :
	src_(pset.getUntrackedParameter<edm::InputTag>("src"))
{
	consumes<std::vector<double> >(src_);
}

void
QWVectorPrint::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	Handle<std::vector<double> > t;

	iEvent.getByLabel(src_, t);

	int sz = t->size();

	std::cout << " --> Print " << src_ << " size = " << sz << std::endl;
	for ( int i = 0; i < sz; i++ ) {
		std::cout << "    -> " << i << ":\t" << (*t)[i] << std::endl;
	}
	return;
}

DEFINE_FWK_MODULE(QWVectorPrint);
