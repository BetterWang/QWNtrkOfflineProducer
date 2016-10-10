import FWCore.ParameterSet.Config as cms

# N_{trk}^{offline}, pT > 0.4, |eta| < 2.4
QWNoff = cms.EDProducer("QWNtrkOfflineProducer",
		vertexSrc = cms.untracked.InputTag("offlinePrimaryVertices"),
		trackSrc  = cms.untracked.InputTag("generalTracks")
		)

