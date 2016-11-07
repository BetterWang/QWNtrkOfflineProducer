import FWCore.ParameterSet.Config as cms

process = cms.Process("PCA")

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')


process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.MessageLogger.cerr.FwkReport.reportEvery = 100

from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '75X_dataRun2_v13', '')

process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)

process.source = cms.Source("PoolSource",
	fileNames = cms.untracked.vstring("file:/afs/cern.ch/user/q/qwang/work/cleanroomRun2/Ana/data/hydjet_pixel.root")
)

import HLTrigger.HLTfilters.hltHighLevel_cfi

process.hltMB = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.hltMB.HLTPaths = [
	"HLT_HIL1MinimumBiasHF2AND_*",
	"HLT_HIL1MinimumBiasHF1AND_*",
]
process.hltMB.andOr = cms.bool(True)
process.hltMB.throw = cms.bool(False)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('tracking.root')
)

process.load('HeavyIonsAnalysis.Configuration.collisionEventSelection_cff')
process.clusterCompatibilityFilter.clusterPars = cms.vdouble(0.0,0.006)

process.eventSelection = cms.Sequence(
        process.hfCoincFilter3
        + process.primaryVertexFilter
        + process.clusterCompatibilityFilter
)


process.load('PbPb_HIMB2_pixel_noeff')
process.load('RecoHI.HiCentralityAlgos.CentralityFilter_cfi')

process.Centrality0_5 = process.centralityFilter.clone(
		selectedBins = cms.vint32(
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9
			)
		)

process.Centrality5_10 = process.centralityFilter.clone(
		selectedBins = cms.vint32(
		10, 11, 12, 13, 14, 15, 16, 17, 18, 19
			)
		)

process.Centrality10_20 = process.centralityFilter.clone(
		selectedBins = cms.vint32(
		20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39
			)
		)

process.Centrality20_30 = process.centralityFilter.clone(
		selectedBins = cms.vint32(
		40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59
			)
		)
process.Centrality30_40 = process.centralityFilter.clone(
		selectedBins = cms.vint32(
		60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79
			)
		)
process.Centrality40_50 = process.centralityFilter.clone(
		selectedBins = cms.vint32(
		80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99
			)
		)
process.Centrality50_60 = process.centralityFilter.clone(
		selectedBins = cms.vint32(
		100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119
			)
		)
process.Centrality60_70 = process.centralityFilter.clone(
		selectedBins = cms.vint32(
		120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139
			)
		)
process.Centrality70_80 = process.centralityFilter.clone(
		selectedBins = cms.vint32(
		140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159
			)
		)
process.Centrality80_90 = process.centralityFilter.clone(
		selectedBins = cms.vint32(
		160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179
			)
		)
process.Centrality90_100 = process.centralityFilter.clone(
		selectedBins = cms.vint32(
		180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199
			)
		)
process.histNoff0_5   = process.histNoff.clone()
process.histNoff5_10  = process.histNoff.clone()
process.histNoff10_20 = process.histNoff.clone()
process.histNoff20_30 = process.histNoff.clone()
process.histNoff30_40 = process.histNoff.clone()
process.histNoff40_50 = process.histNoff.clone()
process.histNoff50_60 = process.histNoff.clone()
process.histNoff60_70 = process.histNoff.clone()
process.histNoff70_80 = process.histNoff.clone()
process.histNoff80_90 = process.histNoff.clone()
process.histNoff90_100= process.histNoff.clone()

process.vectPhi0_5    = process.vectPhi.clone()
process.vectPhi5_10   = process.vectPhi.clone()
process.vectPhi10_20  = process.vectPhi.clone()
process.vectPhi20_30  = process.vectPhi.clone()
process.vectPhi30_40  = process.vectPhi.clone()
process.vectPhi40_50  = process.vectPhi.clone()
process.vectPhi50_60  = process.vectPhi.clone()
process.vectPhi60_70  = process.vectPhi.clone()
process.vectPhi70_80  = process.vectPhi.clone()
process.vectPhi80_90  = process.vectPhi.clone()
process.vectPhi90_100 = process.vectPhi.clone()

process.vectPt0_5    = process.vectPt.clone()
process.vectPt5_10   = process.vectPt.clone()
process.vectPt10_20  = process.vectPt.clone()
process.vectPt20_30  = process.vectPt.clone()
process.vectPt30_40  = process.vectPt.clone()
process.vectPt40_50  = process.vectPt.clone()
process.vectPt50_60  = process.vectPt.clone()
process.vectPt60_70  = process.vectPt.clone()
process.vectPt70_80  = process.vectPt.clone()
process.vectPt80_90  = process.vectPt.clone()
process.vectPt90_100 = process.vectPt.clone()

process.vectEta0_5    = process.vectEta.clone()
process.vectEta5_10   = process.vectEta.clone()
process.vectEta10_20  = process.vectEta.clone()
process.vectEta20_30  = process.vectEta.clone()
process.vectEta30_40  = process.vectEta.clone()
process.vectEta40_50  = process.vectEta.clone()
process.vectEta50_60  = process.vectEta.clone()
process.vectEta60_70  = process.vectEta.clone()
process.vectEta70_80  = process.vectEta.clone()
process.vectEta80_90  = process.vectEta.clone()
process.vectEta90_100 = process.vectEta.clone()

process.Mon0_5 = cms.Sequence(process.histNoff0_5*process.vectPhi0_5*process.vectPt0_5*process.vectEta0_5)
process.Mon5_10 = cms.Sequence(process.histNoff5_10*process.vectPhi5_10*process.vectPt5_10*process.vectEta5_10)
process.Mon10_20 = cms.Sequence(process.histNoff10_20*process.vectPhi10_20*process.vectPt10_20*process.vectEta10_20)
process.Mon20_30 = cms.Sequence(process.histNoff20_30*process.vectPhi20_30*process.vectPt20_30*process.vectEta20_30)
process.Mon30_40 = cms.Sequence(process.histNoff30_40*process.vectPhi30_40*process.vectPt30_40*process.vectEta30_40)
process.Mon40_50 = cms.Sequence(process.histNoff40_50*process.vectPhi40_50*process.vectPt40_50*process.vectEta40_50)
process.Mon50_60 = cms.Sequence(process.histNoff50_60*process.vectPhi50_60*process.vectPt50_60*process.vectEta50_60)
process.Mon60_70 = cms.Sequence(process.histNoff60_70*process.vectPhi60_70*process.vectPt60_70*process.vectEta60_70)
process.Mon70_80 = cms.Sequence(process.histNoff70_80*process.vectPhi70_80*process.vectPt70_80*process.vectEta70_80)
process.Mon80_90 = cms.Sequence(process.histNoff80_90*process.vectPhi80_90*process.vectPt80_90*process.vectEta80_90)
process.Mon90_100 = cms.Sequence(process.histNoff90_100*process.vectPhi90_100*process.vectPt90_100*process.vectEta90_100)

process.path0_5  = cms.Path(process.Centrality0_5  *process.QWEvent*process.Mon0_5)
process.path5_10 = cms.Path(process.Centrality5_10 *process.QWEvent*process.Mon5_10)
process.path10_20= cms.Path(process.Centrality10_20*process.QWEvent*process.Mon10_20)
process.path20_30= cms.Path(process.Centrality20_30*process.QWEvent*process.Mon20_30)
process.path30_40= cms.Path(process.Centrality30_40*process.QWEvent*process.Mon30_40)
process.path40_50= cms.Path(process.Centrality40_50*process.QWEvent*process.Mon40_50)
process.path50_60= cms.Path(process.Centrality50_60*process.QWEvent*process.Mon50_60)
process.path60_70= cms.Path(process.Centrality60_70*process.QWEvent*process.Mon60_70)
process.path70_80= cms.Path(process.Centrality70_80*process.QWEvent*process.Mon70_80)
process.path80_90= cms.Path(process.Centrality80_90*process.QWEvent*process.Mon80_90)
process.path90_100=cms.Path(process.Centrality90_100*process.QWEvent*process.Mon90_100)

process.schedule = cms.Schedule(
	process.path0_5,
	process.path5_10,
	process.path10_20,
	process.path20_30,
	process.path30_40,
	process.path40_50,
	process.path50_60,
	process.path60_70,
	process.path70_80,
	process.path80_90,
	process.path90_100,
)
