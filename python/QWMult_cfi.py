import FWCore.ParameterSet.Config as cms

# Multiplicity, 0.3 < pT < 3.0, |eta| < 2.4
QWMult = cms.EDProducer("QWMultProducer",
		vertexSrc = cms.untracked.InputTag("offlinePrimaryVertices"),
		trackSrc  = cms.untracked.InputTag("generalTracks"),
		centralitySrc = cms.untracked.InputTag("centralityBin"),
		)

