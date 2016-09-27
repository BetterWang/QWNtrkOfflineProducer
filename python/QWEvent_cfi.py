import FWCore.ParameterSet.Config as cms

QWEvent = cms.EDProducer("QWEventProducer",
		vertexSrc = cms.untracked.InputTag("offlinePrimaryVertices"),
		trackSrc  = cms.untracked.InputTag("generalTracks"),
		centralitySrc = cms.untracked.InputTag("centralityBin"),
		fweight = cms.untracked.InputTag("NA"),
		)
