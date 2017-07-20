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
		"root://cms-xrd-global.cern.ch//store/hidata/PARun2016C/PAEmptyBX/AOD/PromptReco-v1/000/286/442/00000/283BB041-2EBC-E611-B104-FA163ED00180.root",
		"root://cms-xrd-global.cern.ch//store/hidata/PARun2016C/PAEmptyBX/AOD/PromptReco-v1/000/286/442/00000/9AFE0052-2EBC-E611-9E29-02163E0137EA.root",
		"root://cms-xrd-global.cern.ch//store/hidata/PARun2016C/PAEmptyBX/AOD/PromptReco-v1/000/286/442/00000/A4B01040-2EBC-E611-AFF7-FA163E3B6BC5.root",
		"root://cms-xrd-global.cern.ch//store/hidata/PARun2016C/PAEmptyBX/AOD/PromptReco-v1/000/286/442/00000/CAF63A6D-2EBC-E611-8DC2-02163E011E69.root",
		"root://cms-xrd-global.cern.ch//store/hidata/PARun2016C/PAEmptyBX/AOD/PromptReco-v1/000/286/442/00000/D449E93E-2EBC-E611-BEC0-FA163EE96F44.root",
		"root://cms-xrd-global.cern.ch//store/hidata/PARun2016C/PAEmptyBX/AOD/PromptReco-v1/000/286/442/00000/FCAE0950-2EBC-E611-8D62-02163E01381B.root",
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

process.hltBptxMinusNotPlus = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.hltBptxMinusNotPlus.HLTPaths = [
		"HLT_PAL1BptxMinusNotBptxPlus_v*",
		]
process.hltBptxMinusNotPlus.andOr = cms.bool(True)
process.hltBptxMinusNotPlus.throw = cms.bool(False)


process.hltBptxPlusNotMinus = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.hltBptxPlusNotMinus.HLTPaths = [
		"HLT_PAL1BptxPlusNotBptxMinus_v*",
		]
process.hltBptxPlusNotMinus.andOr = cms.bool(True)
process.hltBptxPlusNotMinus.throw = cms.bool(False)



#process.load('QWAna.QWNtrkOfflineProducer.QWZDC_cfi')
process.QWZDC = cms.EDProducer("QWZDCProducer",
		Src = cms.untracked.InputTag("castorDigis"),
		)

process.QWCent = cms.EDProducer('QWCentralityProducer',
		src = cms.untracked.InputTag("pACentrality"),
		)

process.monSumN_plus = cms.EDAnalyzer('QWHistDAnalyzer',
		src = cms.untracked.InputTag('QWZDC', 'SumN'),
		Nbins = cms.untracked.int32(6000),
		start = cms.untracked.double(0.),
		end = cms.untracked.double(600.),
		)

process.monHFSumN_plus = cms.EDAnalyzer('QWHistDAnalyzer',
		src = cms.untracked.InputTag('QWCent', 'EtHFtowerSumMinus'),
		Nbins = cms.untracked.int32(200),
		start = cms.untracked.double(0.),
		end = cms.untracked.double(400.),
		)

process.monZDCvsHF_plus = cms.EDAnalyzer('QWCorrAnalyzer',
		srcX = cms.untracked.InputTag('QWCent', 'EtHFtowerSumMinus'),
		NbinsX = cms.untracked.int32(200),
		hstartX = cms.untracked.double(0.),
		hendX = cms.untracked.double(400.),
		srcY = cms.untracked.InputTag('QWZDC', 'SumN'),
		NbinsY = cms.untracked.int32(6000),
		hstartY = cms.untracked.double(0.),
		hendY = cms.untracked.double(600.),
		)

process.monSumN_minus = process.monSumN_plus.clone()
process.monHFSumN_minus = process.monHFSumN_plus.clone()
process.monZDCvsHF_minus = process.monZDCvsHF_plus.clone()

process.path_plus = cms.Path(process.hltBptxMinusNotPlus * process.QWCent * process.QWZDC * process.monSumN_plus * process.monHFSumN_plus * process.monZDCvsHF_plus)
process.path_minus = cms.Path(process.hltBptxPlusNotMinus * process.QWCent * process.QWZDC * process.monSumN_minus * process.monHFSumN_minus * process.monZDCvsHF_minus)

process.RECO = cms.OutputModule("PoolOutputModule",
                outputCommands = cms.untracked.vstring('keep *_QWZDC_*_*', 'keep *_QWCent_*_*'),
                SelectEvents = cms.untracked.PSet(
                        SelectEvents = cms.vstring('path_plus', 'path_minus')
                        ),
                fileName = cms.untracked.string('reco.root')
                )


process.out = cms.EndPath(process.RECO)

process.schedule = cms.Schedule(
	process.path_plus,
	process.path_minus,
#	process.out
)
