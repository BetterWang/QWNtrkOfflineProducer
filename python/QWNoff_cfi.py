import FWCore.ParameterSet.Config as cms

QWNoff = cms.EDProducer("QWNtrkOfflineProducer",
		vertexSrc = cms.untracked.InputTag("offlinePrimaryVertices"),
		trackSrc  = cms.untracked.InputTag("generalTracks")
		)

