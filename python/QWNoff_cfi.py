import FWCore.ParameterSet.Config as cms

QWNoff = cms.EDProducer("QWNtrkOfflineProducer",
		vertexSrc = cms.InputTag("offlinePrimaryVertices"),
		trackSrc  = cms.InputTag("generalTracks")
		)

