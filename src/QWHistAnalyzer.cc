
class QWHistAnalyzer : public edm::EDAnalyzer {
public:
	explicit QWPCA(const edm::ParameterSet&);
	~QWHistAnalyzer() {};
private:
	virtual void beginJob() {};
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	virtual void endJob() {};

	edm::InputTag   src_;
	TH1D * h;
};

QWHistAnalyzer::QWHistAnalyzer(const edm::ParameterSet& pset) :
	src_(pset.getUntrackedParameter<edm::InputTag>("src"))
{
	consumes<int>(src_);
	int Nbins = pset.getUntrackedParameter<int>("Nbins");
	double start = pset.getUntrackedParameter<double>("start");
	double end = pset.getUntrackedParameter<double>("end");

	edm::Service<TFileService> fs;
	h = fs->make<TH1D>("h", "h", Nbins, start, end);
	h->Sumw2();
}

void
QWHistAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	using namespace reco;
	Handle<int> t;
	iEvent.getByLabel(src_, t);
	int c = *(t.product());

	h->Fill(c);
	return;
}

DEFINE_FWK_MODULE(QWHistAnalyzer);
