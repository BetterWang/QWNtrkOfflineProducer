import FWCore.ParameterSet.Config as cms

QWMult = cms.EDProducer("QWMultProducer",
		vertexSrc = cms.untracked.InputTag("offlinePrimaryVertices"),
		trackSrc  = cms.untracked.InputTag("generalTracks"),
		centralitySrc = cms.untracked.InputTag("centralityBin"),
		)

