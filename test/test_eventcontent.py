import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
#process.load('HeavyIonsAnalysis.Configuration.collisionEventSelection_cff')
process.load('Configuration.EventContent.EventContentHeavyIons_cff')

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1))
process.MessageLogger.cerr.FwkReport.reportEvery = 100

from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '75X_dataRun2_v13', '')

process.options = cms.untracked.PSet(
        Rethrow = cms.untracked.vstring('ProductNotFound')
        )

process.source = cms.Source("PoolSource",
		fileNames = cms.untracked.vstring("file:/afs/cern.ch/user/q/qwang/work/cleanroomRun2/Ana/data/ppReco.root"),
        )

import HLTrigger.HLTfilters.hltHighLevel_cfi

process.hltMB = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.hltMB.HLTPaths = [
        "HLT_HIL1MinimumBiasHF2AND*",
        "HLT_HIL1MinimumBiasHF1AND*",
        ]
process.hltMB.andOr = cms.bool(True)
process.hltMB.throw = cms.bool(False)


process.load('QWAna.QWNtrkOfflineProducer.QWMult_cfi')
process.load('QWAna.QWNtrkOfflineProducer.QWNoff_cfi')
process.load('QWAna.QWNtrkOfflineProducer.QWEvent_cfi')
process.centralityBin = cms.EDProducer('QWPPRecoCentBinProducer')

process.output = cms.OutputModule("PoolOutputModule",
	outputCommands = cms.untracked.vstring('keep *'),
	fileName = cms.untracked.string('data.root'),
	SelectEvents = cms.untracked.PSet(
	    SelectEvents = cms.vstring('mb')
	    )
	)

#process.mb = cms.Path(process.hltMB * process.QWMult * process.QWNoff * process.centralityBin )
process.mb = cms.Path(process.QWMult * process.QWNoff * process.centralityBin * process.QWEvent )

process.out = cms.EndPath(process.output)
