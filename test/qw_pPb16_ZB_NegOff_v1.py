import FWCore.ParameterSet.Config as cms

process = cms.Process("CumuV3")

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')


process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.MessageLogger.cerr.FwkReport.reportEvery = 100

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '80X_dataRun2_Prompt_v16', '')

process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
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
process.hltMB = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.hltMB.HLTPaths = [
	"HLT_PAL1MinimumBiasHF_OR_SinglePixelTrack_part*_v*",
]
process.hltMB.andOr = cms.bool(True)
process.hltMB.throw = cms.bool(False)

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

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('cumu.root')
)


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
#process.load("HeavyIonsAnalysis.EventAnalysis.pileUpFilter_cff")

process.hfPosOnNegOff = cms.Sequence(
        process.towersAboveThreshold *
        process.hfPosTowers *
        process.hfNegTowers *
        process.hfPosFilter *
        ~process.hfNegFilter
        )

process.QWZDC = cms.EDProducer("QWZDCProducer",
	Src = cms.untracked.InputTag("castorDigis"),
	)

process.QWZDCFilter0N = cms.EDFilter('QWDoubleFilter',
        src = cms.untracked.InputTag('QWZDC', 'SumN'),
        dmin = cms.untracked.double(-10.),
        dmax = cms.untracked.double(1.0)
        )

process.QWZDCFilter1N = cms.EDFilter('QWDoubleFilter',
        src = cms.untracked.InputTag('QWZDC', 'SumN'),
        dmin = cms.untracked.double(1.0),
        dmax = cms.untracked.double(4.0)
        )

process.selection0N = cms.Sequence(
        process.QWZDC *
        process.QWZDCFilter0N
        )

process.selection1N = cms.Sequence(
        process.QWZDC *
        process.QWZDCFilter1N
        )

#process.eventSelection = cms.Sequence(process.hfCoincFilter * process.PAprimaryVertexFilter * process.NoScraping * process.olvFilter_pPb8TeV_dz1p0)
process.eventSelection = cms.Sequence(process.hfPosOnNegOff * process.PAprimaryVertexFilter * process.NoScraping )


process.load('pPb_HM_eff')
process.QWEvent.fweight = cms.untracked.InputTag('NA')

process.histNoff0N_single = process.histNoff.clone()
process.vectPhi0N_single = process.vectPhi.clone()
process.vectEta0N_single = process.vectEta.clone()
process.vectPt0N_single = process.vectPt.clone()

process.histNoff0N_double = process.histNoff.clone()
process.vectPhi0N_double = process.vectPhi.clone()
process.vectEta0N_double = process.vectEta.clone()
process.vectPt0N_double = process.vectPt.clone()

process.histNoff1N_single = process.histNoff.clone()
process.vectPhi1N_single = process.vectPhi.clone()
process.vectEta1N_single = process.vectEta.clone()
process.vectPt1N_single = process.vectPt.clone()

process.histNoff1N_double = process.histNoff.clone()
process.vectPhi1N_double = process.vectPhi.clone()
process.vectEta1N_double = process.vectEta.clone()
process.vectPt1N_double = process.vectPt.clone()

process.monSumN0N_single = cms.EDAnalyzer('QWHistDAnalyzer',
		src = cms.untracked.InputTag('QWZDC', 'SumN'),
		Nbins = cms.untracked.int32(7000),
		start = cms.untracked.double(-100.),
		end = cms.untracked.double(600.),
		)

process.monSumN0N_double = process.monSumN0N_single.clone();

process.monSumN1N_single = process.monSumN0N_single.clone();
process.monSumN1N_double = process.monSumN0N_single.clone();


process.mon0N_single = cms.Sequence( process.histNoff0N_single + process.vectPhi0N_single + process.vectEta0N_single + process.vectPt0N_single + process.monSumN0N_single)
process.mon0N_double = cms.Sequence( process.histNoff0N_double + process.vectPhi0N_double + process.vectEta0N_double + process.vectPt0N_double + process.monSumN0N_double)

process.mon1N_single = cms.Sequence( process.histNoff1N_single + process.vectPhi1N_single + process.vectEta1N_single + process.vectPt1N_single + process.monSumN1N_single)
process.mon1N_double = cms.Sequence( process.histNoff1N_double + process.vectPhi1N_double + process.vectEta1N_double + process.vectPt1N_double + process.monSumN1N_double)

process.ana0N_single = cms.Path(process.hltZB_single * process.eventSelection * process.selection0N * process.Noff * process.QWEvent * process.mon0N_single)
process.ana0N_double = cms.Path(process.hltZB_double * process.eventSelection * process.selection0N * process.Noff * process.QWEvent * process.mon0N_double)

process.ana1N_single = cms.Path(process.hltZB_single * process.eventSelection * process.selection1N * process.Noff * process.QWEvent * process.mon1N_single)
process.ana1N_double = cms.Path(process.hltZB_double * process.eventSelection * process.selection1N * process.Noff * process.QWEvent * process.mon1N_double)

#process.ana1N = cms.Path(process.hltMB * process.eventSelection * process.selection1N * process.Noff * process.QWEvent * process.mon1N)

process.schedule = cms.Schedule(
	process.ana0N_single,
	process.ana0N_double,
	process.ana1N_single,
	process.ana1N_double,
)
