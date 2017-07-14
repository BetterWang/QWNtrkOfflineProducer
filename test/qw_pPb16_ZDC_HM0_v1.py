import FWCore.ParameterSet.Config as cms

process = cms.Process("ZDC")

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')


process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10000))
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
#        fileNames = cms.untracked.vstring("file:/afs/cern.ch/user/q/qwang/work/cleanroomRun2/Ana/CMSSW_7_5_8_patch2/src/QWAna/QWCumuV3/test/HIMinBias_28.root")
#	fileNames = cms.untracked.vstring("file:pickevents.root")
	fileNames = cms.untracked.vstring("file:/afs/cern.ch/user/q/qwang/work/cleanroomRun2/Ana/data/pPb_8_FEVT.root")
#	fileNames = cms.untracked.vstring("file:/eos/cms/store/hidata/PARun2016C/PAHighMultiplicity0/AOD/PromptReco-v1/000/286/201/00000/0695AC66-FFB9-E611-A015-02163E01478E.root")
#	fileNames = cms.untracked.vstring("root://cms-xrd-global.cern.ch//store/hidata/PARun2016C/PAMinimumBias2/AOD/PromptReco-v1/000/286/178/00000/00C9804B-BEB9-E611-9F3E-FA163E2E1963.root'")
#	fileNames = cms.untracked.vstring("root://cms-xrd-global.cern.ch//store/hidata/PARun2016C/PAMinimumBias1/AOD/PromptReco-v1/000/286/178/00000/0ACB2C21-BDB9-E611-9F95-02163E01231F.root",
#		"root://cms-xrd-global.cern.ch//store/hidata/PARun2016C/PAMinimumBias1/AOD/PromptReco-v1/000/286/178/00000/08AEE0E9-BEB9-E611-9813-02163E01354E.root",
#		"root://cms-xrd-global.cern.ch//store/hidata/PARun2016C/PAMinimumBias1/AOD/PromptReco-v1/000/286/178/00000/08E45934-BBB9-E611-8B47-02163E0146FA.root",
#		"root://cms-xrd-global.cern.ch//store/hidata/PARun2016C/PAMinimumBias1/AOD/PromptReco-v1/000/286/178/00000/0A3A6BEF-BEB9-E611-A6DF-02163E014626.root"
#		)
#	fileNames = cms.untracked.vstring("root://node12.datagrid.cea.fr//store/hidata/PARun2016C/PAMinimumBias1/AOD/PromptReco-v1/000/286/200/00000/10DD9439-1ABA-E611-ABB1-FA163EA2A6FB.root")
#	fileNames = cms.untracked.vstring("file:HcalQIEData.root")
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

import HLTrigger.HLTfilters.hltHighLevel_cfi
process.hltHM120 = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.hltHM120.HLTPaths = [
	"HLT_PAFullTracks_Multiplicity120_v*",
#	"HLT_PAFullTracks_Multiplicity150_v*",
#	"HLT_PAFullTracks_Multiplicity185_*",
#	"HLT_PAFullTracks_Multiplicity220_v*",
#	"HLT_PAFullTracks_Multiplicity250_v*",
#	"HLT_PAFullTracks_Multiplicity280_v*",
]
process.hltHM120.andOr = cms.bool(True)
process.hltHM120.throw = cms.bool(False)

process.hltHM150 = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.hltHM150.HLTPaths = [
	"HLT_PAFullTracks_Multiplicity120_v*",
	"HLT_PAFullTracks_Multiplicity150_v*",
#	"HLT_PAFullTracks_Multiplicity185_*",
#	"HLT_PAFullTracks_Multiplicity220_v*",
#	"HLT_PAFullTracks_Multiplicity250_v*",
#	"HLT_PAFullTracks_Multiplicity280_v*",
]
process.hltHM150.andOr = cms.bool(True)
process.hltHM150.throw = cms.bool(False)

process.hltHM185 = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.hltHM185.HLTPaths = [
#	"HLT_PAFullTracks_Multiplicity120_v*",
#	"HLT_PAFullTracks_Multiplicity150_v*",
	"HLT_PAFullTracks_Multiplicity185_*",
#	"HLT_PAFullTracks_Multiplicity220_v*",
#	"HLT_PAFullTracks_Multiplicity250_v*",
#	"HLT_PAFullTracks_Multiplicity280_v*",
]
process.hltHM185.andOr = cms.bool(True)
process.hltHM185.throw = cms.bool(False)

process.hltHM250 = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.hltHM250.HLTPaths = [
#	"HLT_PAFullTracks_Multiplicity120_v*",
#	"HLT_PAFullTracks_Multiplicity150_v*",
#	"HLT_PAFullTracks_Multiplicity185_*",
#	"HLT_PAFullTracks_Multiplicity220_v*",
	"HLT_PAFullTracks_Multiplicity250_v*",
#	"HLT_PAFullTracks_Multiplicity280_v*",
]
process.hltHM250.andOr = cms.bool(True)
process.hltHM250.throw = cms.bool(False)


process.load('pPb_HM_eff')
#process.load('QWAna.QWNtrkOfflineProducer.QWZDC_cfi')
process.QWZDC = cms.EDProducer("QWZDCProducer",
		Src = cms.untracked.InputTag("castorDigis"),
		)

process.QWCentObj = cms.EDProducer('QWCentralityProducer',
		src = cms.untracked.InputTag("pACentrality"),
		)

process.monSumN120 = cms.EDAnalyzer('QWHistDAnalyzer',
		src = cms.untracked.InputTag('QWZDC', 'SumN'),
		Nbins = cms.untracked.int32(6000),
		start = cms.untracked.double(0.),
		end = cms.untracked.double(600.),
		)

process.monHFSumN120 = cms.EDAnalyzer('QWHistDAnalyzer',
		src = cms.untracked.InputTag('QWCentObj', 'EtHFtowerSumMinus'),
		Nbins = cms.untracked.int32(200),
		start = cms.untracked.double(0.),
		end = cms.untracked.double(400.),
		)

process.monZDCvsHF120 = cms.EDAnalyzer('QWCorrAnalyzer',
		srcX = cms.untracked.InputTag('QWCentObj', 'EtHFtowerSumMinus'),
		NbinsX = cms.untracked.int32(200),
		hstartX = cms.untracked.double(0.),
		hendX = cms.untracked.double(400.),
		srcY = cms.untracked.InputTag('QWZDC', 'SumN'),
		NbinsY = cms.untracked.int32(6000),
		hstartY = cms.untracked.double(0.),
		hendY = cms.untracked.double(600.),
		)

process.monSumN150 = process.monSumN120.clone()
process.monSumN185 = process.monSumN120.clone()
process.monSumN250 = process.monSumN120.clone()

process.monHFSumN150 = process.monHFSumN120.clone()
process.monHFSumN185 = process.monHFSumN120.clone()
process.monHFSumN250 = process.monHFSumN120.clone()

process.monZDCvsHF150 = process.monZDCvsHF120.clone()
process.monZDCvsHF185 = process.monZDCvsHF120.clone()
process.monZDCvsHF250 = process.monZDCvsHF120.clone()

process.load("HeavyIonsAnalysis.VertexAnalysis.PAPileUpVertexFilter_cff")

process.PAprimaryVertexFilter = cms.EDFilter("VertexSelector",
    src = cms.InputTag("offlinePrimaryVertices"),
    cut = cms.string("!isFake && abs(z) <= 25 && position.Rho <= 2 && tracksSize >= 2"),
    filter = cms.bool(True), # otherwise it won't filter the events
)

process.NoScraping = cms.EDFilter("FilterOutScraping",
 applyfilter = cms.untracked.bool(True),
 debugOn = cms.untracked.bool(False),
 numtrack = cms.untracked.uint32(10),
 thresh = cms.untracked.double(0.25)
)

process.load("HeavyIonsAnalysis.Configuration.hfCoincFilter_cff")
process.load("HeavyIonsAnalysis.EventAnalysis.pileUpFilter_cff")

process.eventSelection = cms.Sequence(process.hfCoincFilter * process.PAprimaryVertexFilter * process.NoScraping * process.olvFilter_pPb8TeV_dz1p0)

process.load('RecoHI.HiCentralityAlgos.CentralityFilter_cfi')
process.ppNoffFilter120 = process.centralityFilter.clone(
		selectedBins = cms.vint32(
			*range(120, 150)
			),
		BinLabel = cms.InputTag("Noff")
		)

process.ppNoffFilter150 = process.centralityFilter.clone(
		selectedBins = cms.vint32(
			*range(150, 185)
			),
		BinLabel = cms.InputTag("Noff")
		)

process.ppNoffFilter185 = process.centralityFilter.clone(
		selectedBins = cms.vint32(
			*range(185, 250)
			),
		BinLabel = cms.InputTag("Noff")
		)

process.ppNoffFilter250 = process.centralityFilter.clone(
		selectedBins = cms.vint32(
			*range(250, 600)
			),
		BinLabel = cms.InputTag("Noff")
		)


process.path120 = cms.Path(process.hltHM120 * process.eventSelection * process.Noff * process.ppNoffFilter120 * process.QWCentObj * process.QWZDC * process.monSumN120 * process.monHFSumN120 * process.monZDCvsHF120)
process.path150 = cms.Path(process.hltHM150 * process.eventSelection * process.Noff * process.ppNoffFilter150 * process.QWCentObj * process.QWZDC * process.monSumN150 * process.monHFSumN150 * process.monZDCvsHF150)
process.path185 = cms.Path(process.hltHM185 * process.eventSelection * process.Noff * process.ppNoffFilter185 * process.QWCentObj * process.QWZDC * process.monSumN185 * process.monHFSumN185 * process.monZDCvsHF185)
process.path250 = cms.Path(process.hltHM250 * process.eventSelection * process.Noff * process.ppNoffFilter250 * process.QWCentObj * process.QWZDC * process.monSumN250 * process.monHFSumN250 * process.monZDCvsHF250)




process.RECO = cms.OutputModule("PoolOutputModule",
                outputCommands = cms.untracked.vstring('keep *_QWZDC_*_*', 'keep *_QWCent_*_*'),
                SelectEvents = cms.untracked.PSet(
                        SelectEvents = cms.vstring('path120', 'path150')
                        ),
                fileName = cms.untracked.string('reco.root')
                )


process.out = cms.EndPath(process.RECO)

process.schedule = cms.Schedule(
	process.path120,
	process.path150,
#	process.path185,
#	process.path250,
	process.out
)
