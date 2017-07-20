import FWCore.ParameterSet.Config as cms

process = cms.Process("ZDC")

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')


process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

#from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, '75X_dataRun2_v13', '')
#
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '80X_dataRun2_Prompt_v16', '')

process.options = cms.untracked.PSet(
		SkipEvent = cms.untracked.vstring('ProductNotFound')
)

process.source = cms.Source("PoolSource",
	fileNames = cms.untracked.vstring(
		"root://cms-xrd-global.cern.ch//store/hidata/PARun2016C/PAMinimumBiasBkg/AOD/PromptReco-v1/000/286/442/00000/00C19DAE-34BC-E611-A8A4-02163E011C5F.root",
		"root://cms-xrd-global.cern.ch//store/hidata/PARun2016C/PAMinimumBiasBkg/AOD/PromptReco-v1/000/286/442/00000/0286B18B-36BC-E611-A6F4-02163E01348D.root",
		"root://cms-xrd-global.cern.ch//store/hidata/PARun2016C/PAMinimumBiasBkg/AOD/PromptReco-v1/000/286/442/00000/02D1243A-3DBC-E611-8A89-02163E0146A5.root",
		"root://cms-xrd-global.cern.ch//store/hidata/PARun2016C/PAMinimumBiasBkg/AOD/PromptReco-v1/000/286/442/00000/04AB5AA9-34BC-E611-B636-02163E011D24.root",
		)
)

import HLTrigger.HLTfilters.hltHighLevel_cfi

#process.hltMB = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
#process.hltMB.HLTPaths = [
#	"HLT_HIL1MinimumBiasHF2AND_*",
#	"HLT_HIL1MinimumBiasHF1AND_*",
#]
#process.hltMB.andOr = cms.bool(True)
#process.hltMB.throw = cms.bool(False)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('zdc.root')
)

process.hltZB_single = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.hltZB_single.HLTPaths = [
		"HLT_PAZeroBias_SinglePixelTrack_v*",
		]
process.hltZB_single.andOr = cms.bool(True)
process.hltZB_single.throw = cms.bool(False)

process.hltZB_double = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.hltZB_double.HLTPaths = [
		"HLT_PAZeroBias_DoublePixelTrack_v*",
		]
process.hltZB_double.andOr = cms.bool(True)
process.hltZB_double.throw = cms.bool(False)



#process.load('QWAna.QWNtrkOfflineProducer.QWZDC_cfi')
process.QWZDC = cms.EDProducer("QWZDCProducer",
		Src = cms.untracked.InputTag("castorDigis"),
		)

process.QWCent = cms.EDProducer('QWCentralityProducer',
		src = cms.untracked.InputTag("pACentrality"),
		)

process.monSumN_single = cms.EDAnalyzer('QWHistDAnalyzer',
		src = cms.untracked.InputTag('QWZDC', 'SumN'),
		Nbins = cms.untracked.int32(6000),
		start = cms.untracked.double(0.),
		end = cms.untracked.double(600.),
		)

process.monHFSumN_single = cms.EDAnalyzer('QWHistDAnalyzer',
		src = cms.untracked.InputTag('QWCent', 'EtHFtowerSumMinus'),
		Nbins = cms.untracked.int32(200),
		start = cms.untracked.double(0.),
		end = cms.untracked.double(400.),
		)

process.monZDCvsHF_single = cms.EDAnalyzer('QWCorrAnalyzer',
		srcX = cms.untracked.InputTag('QWCent', 'EtHFtowerSumMinus'),
		NbinsX = cms.untracked.int32(200),
		hstartX = cms.untracked.double(0.),
		hendX = cms.untracked.double(400.),
		srcY = cms.untracked.InputTag('QWZDC', 'SumN'),
		NbinsY = cms.untracked.int32(6000),
		hstartY = cms.untracked.double(0.),
		hendY = cms.untracked.double(600.),
		)

process.monSumN_double = process.monSumN_single.clone()
process.monHFSumN_double = process.monHFSumN_single.clone()
process.monZDCvsHF_double = process.monZDCvsHF_single.clone()

process.path_single = cms.Path(process.hltZB_single * process.QWCent * process.QWZDC * process.monSumN_single * process.monHFSumN_single * process.monZDCvsHF_single)
process.path_double = cms.Path(process.hltZB_double * process.QWCent * process.QWZDC * process.monSumN_double * process.monHFSumN_double * process.monZDCvsHF_double)

process.RECO = cms.OutputModule("PoolOutputModule",
                outputCommands = cms.untracked.vstring('keep *_QWZDC_*_*', 'keep *_QWCent_*_*'),
                SelectEvents = cms.untracked.PSet(
                        SelectEvents = cms.vstring('path_single', 'path_double')
                        ),
                fileName = cms.untracked.string('reco.root')
                )


process.out = cms.EndPath(process.RECO)

process.schedule = cms.Schedule(
	process.path_single,
	process.path_double,
#	process.out
)
