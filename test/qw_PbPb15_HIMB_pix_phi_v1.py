import FWCore.ParameterSet.Config as cms

process = cms.Process("PCA")

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')


process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '75X_dataRun2_v13', '')

process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)

process.source = cms.Source("PoolSource",
#        fileNames = cms.untracked.vstring("file:/afs/cern.ch/user/q/qwang/work/cleanroomRun2/Ana/CMSSW_7_5_8_patch2/src/QWAna/QWCumuV3/test/HIMinBias_28.root")
	fileNames = cms.untracked.vstring("file:/afs/cern.ch/user/q/qwang/work/cleanroomRun2/Ana/data/pixeltracking_1.root")
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

process.histCent_Cut20 = process.histCent.clone();
process.histCent_Cut23 = process.histCent.clone();
process.histCent_Cut26 = process.histCent.clone();
process.histCent_Cut29 = process.histCent.clone();
process.histCent_Cut32 = process.histCent.clone();
process.histCent_Cut35 = process.histCent.clone();
process.histCent_Cut38 = process.histCent.clone();
process.histCent_Cut41 = process.histCent.clone();

process.histMult0_5_Cut20 = process.histMult.clone()
process.histMult0_5_Cut23 = process.histMult.clone()
process.histMult0_5_Cut26 = process.histMult.clone()
process.histMult0_5_Cut29 = process.histMult.clone()
process.histMult0_5_Cut32 = process.histMult.clone()
process.histMult0_5_Cut35 = process.histMult.clone()
process.histMult0_5_Cut38 = process.histMult.clone()
process.histMult0_5_Cut41 = process.histMult.clone()

process.histMult5_10_Cut20 = process.histMult.clone()
process.histMult5_10_Cut23 = process.histMult.clone()
process.histMult5_10_Cut26 = process.histMult.clone()
process.histMult5_10_Cut29 = process.histMult.clone()
process.histMult5_10_Cut32 = process.histMult.clone()
process.histMult5_10_Cut35 = process.histMult.clone()
process.histMult5_10_Cut38 = process.histMult.clone()
process.histMult5_10_Cut41 = process.histMult.clone()

process.histMult10_20_Cut20 = process.histMult.clone()
process.histMult10_20_Cut23 = process.histMult.clone()
process.histMult10_20_Cut26 = process.histMult.clone()
process.histMult10_20_Cut29 = process.histMult.clone()
process.histMult10_20_Cut32 = process.histMult.clone()
process.histMult10_20_Cut35 = process.histMult.clone()
process.histMult10_20_Cut38 = process.histMult.clone()
process.histMult10_20_Cut41 = process.histMult.clone()

process.histMult20_30_Cut20 = process.histMult.clone()
process.histMult20_30_Cut23 = process.histMult.clone()
process.histMult20_30_Cut26 = process.histMult.clone()
process.histMult20_30_Cut29 = process.histMult.clone()
process.histMult20_30_Cut32 = process.histMult.clone()
process.histMult20_30_Cut35 = process.histMult.clone()
process.histMult20_30_Cut38 = process.histMult.clone()
process.histMult20_30_Cut41 = process.histMult.clone()

process.histMult30_40_Cut20 = process.histMult.clone()
process.histMult30_40_Cut23 = process.histMult.clone()
process.histMult30_40_Cut26 = process.histMult.clone()
process.histMult30_40_Cut29 = process.histMult.clone()
process.histMult30_40_Cut32 = process.histMult.clone()
process.histMult30_40_Cut35 = process.histMult.clone()
process.histMult30_40_Cut38 = process.histMult.clone()
process.histMult30_40_Cut41 = process.histMult.clone()

process.histMult40_50_Cut20 = process.histMult.clone()
process.histMult40_50_Cut23 = process.histMult.clone()
process.histMult40_50_Cut26 = process.histMult.clone()
process.histMult40_50_Cut29 = process.histMult.clone()
process.histMult40_50_Cut32 = process.histMult.clone()
process.histMult40_50_Cut35 = process.histMult.clone()
process.histMult40_50_Cut38 = process.histMult.clone()
process.histMult40_50_Cut41 = process.histMult.clone()

process.histMult50_60_Cut20 = process.histMult.clone()
process.histMult50_60_Cut23 = process.histMult.clone()
process.histMult50_60_Cut26 = process.histMult.clone()
process.histMult50_60_Cut29 = process.histMult.clone()
process.histMult50_60_Cut32 = process.histMult.clone()
process.histMult50_60_Cut35 = process.histMult.clone()
process.histMult50_60_Cut38 = process.histMult.clone()
process.histMult50_60_Cut41 = process.histMult.clone()

process.histMult60_70_Cut20 = process.histMult.clone()
process.histMult60_70_Cut23 = process.histMult.clone()
process.histMult60_70_Cut26 = process.histMult.clone()
process.histMult60_70_Cut29 = process.histMult.clone()
process.histMult60_70_Cut32 = process.histMult.clone()
process.histMult60_70_Cut35 = process.histMult.clone()
process.histMult60_70_Cut38 = process.histMult.clone()
process.histMult60_70_Cut41 = process.histMult.clone()

process.histMult70_80_Cut20 = process.histMult.clone()
process.histMult70_80_Cut23 = process.histMult.clone()
process.histMult70_80_Cut26 = process.histMult.clone()
process.histMult70_80_Cut29 = process.histMult.clone()
process.histMult70_80_Cut32 = process.histMult.clone()
process.histMult70_80_Cut35 = process.histMult.clone()
process.histMult70_80_Cut38 = process.histMult.clone()
process.histMult70_80_Cut41 = process.histMult.clone()

process.histMult80_90_Cut20 = process.histMult.clone()
process.histMult80_90_Cut23 = process.histMult.clone()
process.histMult80_90_Cut26 = process.histMult.clone()
process.histMult80_90_Cut29 = process.histMult.clone()
process.histMult80_90_Cut32 = process.histMult.clone()
process.histMult80_90_Cut35 = process.histMult.clone()
process.histMult80_90_Cut38 = process.histMult.clone()
process.histMult80_90_Cut41 = process.histMult.clone()

process.histMult90_100_Cut20 = process.histMult.clone()
process.histMult90_100_Cut23 = process.histMult.clone()
process.histMult90_100_Cut26 = process.histMult.clone()
process.histMult90_100_Cut29 = process.histMult.clone()
process.histMult90_100_Cut32 = process.histMult.clone()
process.histMult90_100_Cut35 = process.histMult.clone()
process.histMult90_100_Cut38 = process.histMult.clone()
process.histMult90_100_Cut41 = process.histMult.clone()

process.pathCut20_0_5    =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut20*process.centralityBin*process.Centrality0_5   *process.QWEvent*process.Mult *    process.histMult0_5_Cut20 * process.histCent_Cut20)
process.pathCut20_5_10   =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut20*process.centralityBin*process.Centrality5_10  *process.QWEvent*process.Mult *   process.histMult5_10_Cut20 * process.histCent_Cut20)
process.pathCut20_10_20  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut20*process.centralityBin*process.Centrality10_20 *process.QWEvent*process.Mult *  process.histMult10_20_Cut20 * process.histCent_Cut20)
process.pathCut20_20_30  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut20*process.centralityBin*process.Centrality20_30 *process.QWEvent*process.Mult *  process.histMult20_30_Cut20 * process.histCent_Cut20)
process.pathCut20_30_40  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut20*process.centralityBin*process.Centrality30_40 *process.QWEvent*process.Mult *  process.histMult30_40_Cut20 * process.histCent_Cut20)
process.pathCut20_40_50  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut20*process.centralityBin*process.Centrality40_50 *process.QWEvent*process.Mult *  process.histMult40_50_Cut20 * process.histCent_Cut20)
process.pathCut20_50_60  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut20*process.centralityBin*process.Centrality50_60 *process.QWEvent*process.Mult *  process.histMult50_60_Cut20 * process.histCent_Cut20)
process.pathCut20_60_70  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut20*process.centralityBin*process.Centrality60_70 *process.QWEvent*process.Mult *  process.histMult60_70_Cut20 * process.histCent_Cut20)
process.pathCut20_70_80  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut20*process.centralityBin*process.Centrality70_80 *process.QWEvent*process.Mult *  process.histMult70_80_Cut20 * process.histCent_Cut20)
process.pathCut20_80_90  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut20*process.centralityBin*process.Centrality80_90 *process.QWEvent*process.Mult *  process.histMult80_90_Cut20 * process.histCent_Cut20)
process.pathCut20_90_100 =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut20*process.centralityBin*process.Centrality90_100*process.QWEvent*process.Mult * process.histMult90_100_Cut20 * process.histCent_Cut20)

process.pathCut23_0_5    =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut23*process.centralityBin*process.Centrality0_5   *process.QWEvent*process.Mult *    process.histMult0_5_Cut23 * process.histCent_Cut23)
process.pathCut23_5_10   =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut23*process.centralityBin*process.Centrality5_10  *process.QWEvent*process.Mult *   process.histMult5_10_Cut23 * process.histCent_Cut23)
process.pathCut23_10_20  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut23*process.centralityBin*process.Centrality10_20 *process.QWEvent*process.Mult *  process.histMult10_20_Cut23 * process.histCent_Cut23)
process.pathCut23_20_30  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut23*process.centralityBin*process.Centrality20_30 *process.QWEvent*process.Mult *  process.histMult20_30_Cut23 * process.histCent_Cut23)
process.pathCut23_30_40  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut23*process.centralityBin*process.Centrality30_40 *process.QWEvent*process.Mult *  process.histMult30_40_Cut23 * process.histCent_Cut23)
process.pathCut23_40_50  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut23*process.centralityBin*process.Centrality40_50 *process.QWEvent*process.Mult *  process.histMult40_50_Cut23 * process.histCent_Cut23)
process.pathCut23_50_60  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut23*process.centralityBin*process.Centrality50_60 *process.QWEvent*process.Mult *  process.histMult50_60_Cut23 * process.histCent_Cut23)
process.pathCut23_60_70  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut23*process.centralityBin*process.Centrality60_70 *process.QWEvent*process.Mult *  process.histMult60_70_Cut23 * process.histCent_Cut23)
process.pathCut23_70_80  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut23*process.centralityBin*process.Centrality70_80 *process.QWEvent*process.Mult *  process.histMult70_80_Cut23 * process.histCent_Cut23)
process.pathCut23_80_90  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut23*process.centralityBin*process.Centrality80_90 *process.QWEvent*process.Mult *  process.histMult80_90_Cut23 * process.histCent_Cut23)
process.pathCut23_90_100 =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut23*process.centralityBin*process.Centrality90_100*process.QWEvent*process.Mult * process.histMult90_100_Cut23 * process.histCent_Cut23)

process.pathCut26_0_5    =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut26*process.centralityBin*process.Centrality0_5   *process.QWEvent*process.Mult *    process.histMult0_5_Cut26 * process.histCent_Cut26)
process.pathCut26_5_10   =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut26*process.centralityBin*process.Centrality5_10  *process.QWEvent*process.Mult *   process.histMult5_10_Cut26 * process.histCent_Cut26)
process.pathCut26_10_20  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut26*process.centralityBin*process.Centrality10_20 *process.QWEvent*process.Mult *  process.histMult10_20_Cut26 * process.histCent_Cut26)
process.pathCut26_20_30  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut26*process.centralityBin*process.Centrality20_30 *process.QWEvent*process.Mult *  process.histMult20_30_Cut26 * process.histCent_Cut26)
process.pathCut26_30_40  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut26*process.centralityBin*process.Centrality30_40 *process.QWEvent*process.Mult *  process.histMult30_40_Cut26 * process.histCent_Cut26)
process.pathCut26_40_50  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut26*process.centralityBin*process.Centrality40_50 *process.QWEvent*process.Mult *  process.histMult40_50_Cut26 * process.histCent_Cut26)
process.pathCut26_50_60  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut26*process.centralityBin*process.Centrality50_60 *process.QWEvent*process.Mult *  process.histMult50_60_Cut26 * process.histCent_Cut26)
process.pathCut26_60_70  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut26*process.centralityBin*process.Centrality60_70 *process.QWEvent*process.Mult *  process.histMult60_70_Cut26 * process.histCent_Cut26)
process.pathCut26_70_80  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut26*process.centralityBin*process.Centrality70_80 *process.QWEvent*process.Mult *  process.histMult70_80_Cut26 * process.histCent_Cut26)
process.pathCut26_80_90  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut26*process.centralityBin*process.Centrality80_90 *process.QWEvent*process.Mult *  process.histMult80_90_Cut26 * process.histCent_Cut26)
process.pathCut26_90_100 =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut26*process.centralityBin*process.Centrality90_100*process.QWEvent*process.Mult * process.histMult90_100_Cut26 * process.histCent_Cut26)

process.pathCut29_0_5    =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut29*process.centralityBin*process.Centrality0_5   *process.QWEvent*process.Mult *    process.histMult0_5_Cut29 * process.histCent_Cut29)
process.pathCut29_5_10   =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut29*process.centralityBin*process.Centrality5_10  *process.QWEvent*process.Mult *   process.histMult5_10_Cut29 * process.histCent_Cut29)
process.pathCut29_10_20  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut29*process.centralityBin*process.Centrality10_20 *process.QWEvent*process.Mult *  process.histMult10_20_Cut29 * process.histCent_Cut29)
process.pathCut29_20_30  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut29*process.centralityBin*process.Centrality20_30 *process.QWEvent*process.Mult *  process.histMult20_30_Cut29 * process.histCent_Cut29)
process.pathCut29_30_40  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut29*process.centralityBin*process.Centrality30_40 *process.QWEvent*process.Mult *  process.histMult30_40_Cut29 * process.histCent_Cut29)
process.pathCut29_40_50  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut29*process.centralityBin*process.Centrality40_50 *process.QWEvent*process.Mult *  process.histMult40_50_Cut29 * process.histCent_Cut29)
process.pathCut29_50_60  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut29*process.centralityBin*process.Centrality50_60 *process.QWEvent*process.Mult *  process.histMult50_60_Cut29 * process.histCent_Cut29)
process.pathCut29_60_70  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut29*process.centralityBin*process.Centrality60_70 *process.QWEvent*process.Mult *  process.histMult60_70_Cut29 * process.histCent_Cut29)
process.pathCut29_70_80  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut29*process.centralityBin*process.Centrality70_80 *process.QWEvent*process.Mult *  process.histMult70_80_Cut29 * process.histCent_Cut29)
process.pathCut29_80_90  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut29*process.centralityBin*process.Centrality80_90 *process.QWEvent*process.Mult *  process.histMult80_90_Cut29 * process.histCent_Cut29)
process.pathCut29_90_100 =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut29*process.centralityBin*process.Centrality90_100*process.QWEvent*process.Mult * process.histMult90_100_Cut29 * process.histCent_Cut29)

process.pathCut32_0_5    =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut32*process.centralityBin*process.Centrality0_5   *process.QWEvent*process.Mult *    process.histMult0_5_Cut32 * process.histCent_Cut32)
process.pathCut32_5_10   =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut32*process.centralityBin*process.Centrality5_10  *process.QWEvent*process.Mult *   process.histMult5_10_Cut32 * process.histCent_Cut32)
process.pathCut32_10_20  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut32*process.centralityBin*process.Centrality10_20 *process.QWEvent*process.Mult *  process.histMult10_20_Cut32 * process.histCent_Cut32)
process.pathCut32_20_30  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut32*process.centralityBin*process.Centrality20_30 *process.QWEvent*process.Mult *  process.histMult20_30_Cut32 * process.histCent_Cut32)
process.pathCut32_30_40  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut32*process.centralityBin*process.Centrality30_40 *process.QWEvent*process.Mult *  process.histMult30_40_Cut32 * process.histCent_Cut32)
process.pathCut32_40_50  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut32*process.centralityBin*process.Centrality40_50 *process.QWEvent*process.Mult *  process.histMult40_50_Cut32 * process.histCent_Cut32)
process.pathCut32_50_60  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut32*process.centralityBin*process.Centrality50_60 *process.QWEvent*process.Mult *  process.histMult50_60_Cut32 * process.histCent_Cut32)
process.pathCut32_60_70  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut32*process.centralityBin*process.Centrality60_70 *process.QWEvent*process.Mult *  process.histMult60_70_Cut32 * process.histCent_Cut32)
process.pathCut32_70_80  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut32*process.centralityBin*process.Centrality70_80 *process.QWEvent*process.Mult *  process.histMult70_80_Cut32 * process.histCent_Cut32)
process.pathCut32_80_90  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut32*process.centralityBin*process.Centrality80_90 *process.QWEvent*process.Mult *  process.histMult80_90_Cut32 * process.histCent_Cut32)
process.pathCut32_90_100 =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut32*process.centralityBin*process.Centrality90_100*process.QWEvent*process.Mult * process.histMult90_100_Cut32 * process.histCent_Cut32)

process.pathCut35_0_5    =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut35*process.centralityBin*process.Centrality0_5   *process.QWEvent*process.Mult *    process.histMult0_5_Cut35 * process.histCent_Cut35)
process.pathCut35_5_10   =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut35*process.centralityBin*process.Centrality5_10  *process.QWEvent*process.Mult *   process.histMult5_10_Cut35 * process.histCent_Cut35)
process.pathCut35_10_20  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut35*process.centralityBin*process.Centrality10_20 *process.QWEvent*process.Mult *  process.histMult10_20_Cut35 * process.histCent_Cut35)
process.pathCut35_20_30  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut35*process.centralityBin*process.Centrality20_30 *process.QWEvent*process.Mult *  process.histMult20_30_Cut35 * process.histCent_Cut35)
process.pathCut35_30_40  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut35*process.centralityBin*process.Centrality30_40 *process.QWEvent*process.Mult *  process.histMult30_40_Cut35 * process.histCent_Cut35)
process.pathCut35_40_50  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut35*process.centralityBin*process.Centrality40_50 *process.QWEvent*process.Mult *  process.histMult40_50_Cut35 * process.histCent_Cut35)
process.pathCut35_50_60  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut35*process.centralityBin*process.Centrality50_60 *process.QWEvent*process.Mult *  process.histMult50_60_Cut35 * process.histCent_Cut35)
process.pathCut35_60_70  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut35*process.centralityBin*process.Centrality60_70 *process.QWEvent*process.Mult *  process.histMult60_70_Cut35 * process.histCent_Cut35)
process.pathCut35_70_80  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut35*process.centralityBin*process.Centrality70_80 *process.QWEvent*process.Mult *  process.histMult70_80_Cut35 * process.histCent_Cut35)
process.pathCut35_80_90  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut35*process.centralityBin*process.Centrality80_90 *process.QWEvent*process.Mult *  process.histMult80_90_Cut35 * process.histCent_Cut35)
process.pathCut35_90_100 =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut35*process.centralityBin*process.Centrality90_100*process.QWEvent*process.Mult * process.histMult90_100_Cut35 * process.histCent_Cut35)

process.pathCut38_0_5    =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut38*process.centralityBin*process.Centrality0_5   *process.QWEvent*process.Mult *    process.histMult0_5_Cut38 * process.histCent_Cut38)
process.pathCut38_5_10   =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut38*process.centralityBin*process.Centrality5_10  *process.QWEvent*process.Mult *   process.histMult5_10_Cut38 * process.histCent_Cut38)
process.pathCut38_10_20  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut38*process.centralityBin*process.Centrality10_20 *process.QWEvent*process.Mult *  process.histMult10_20_Cut38 * process.histCent_Cut38)
process.pathCut38_20_30  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut38*process.centralityBin*process.Centrality20_30 *process.QWEvent*process.Mult *  process.histMult20_30_Cut38 * process.histCent_Cut38)
process.pathCut38_30_40  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut38*process.centralityBin*process.Centrality30_40 *process.QWEvent*process.Mult *  process.histMult30_40_Cut38 * process.histCent_Cut38)
process.pathCut38_40_50  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut38*process.centralityBin*process.Centrality40_50 *process.QWEvent*process.Mult *  process.histMult40_50_Cut38 * process.histCent_Cut38)
process.pathCut38_50_60  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut38*process.centralityBin*process.Centrality50_60 *process.QWEvent*process.Mult *  process.histMult50_60_Cut38 * process.histCent_Cut38)
process.pathCut38_60_70  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut38*process.centralityBin*process.Centrality60_70 *process.QWEvent*process.Mult *  process.histMult60_70_Cut38 * process.histCent_Cut38)
process.pathCut38_70_80  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut38*process.centralityBin*process.Centrality70_80 *process.QWEvent*process.Mult *  process.histMult70_80_Cut38 * process.histCent_Cut38)
process.pathCut38_80_90  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut38*process.centralityBin*process.Centrality80_90 *process.QWEvent*process.Mult *  process.histMult80_90_Cut38 * process.histCent_Cut38)
process.pathCut38_90_100 =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut38*process.centralityBin*process.Centrality90_100*process.QWEvent*process.Mult * process.histMult90_100_Cut38 * process.histCent_Cut38)

process.pathCut41_0_5    =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut41*process.centralityBin*process.Centrality0_5   *process.QWEvent*process.Mult *    process.histMult0_5_Cut41 * process.histCent_Cut41)
process.pathCut41_5_10   =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut41*process.centralityBin*process.Centrality5_10  *process.QWEvent*process.Mult *   process.histMult5_10_Cut41 * process.histCent_Cut41)
process.pathCut41_10_20  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut41*process.centralityBin*process.Centrality10_20 *process.QWEvent*process.Mult *  process.histMult10_20_Cut41 * process.histCent_Cut41)
process.pathCut41_20_30  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut41*process.centralityBin*process.Centrality20_30 *process.QWEvent*process.Mult *  process.histMult20_30_Cut41 * process.histCent_Cut41)
process.pathCut41_30_40  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut41*process.centralityBin*process.Centrality30_40 *process.QWEvent*process.Mult *  process.histMult30_40_Cut41 * process.histCent_Cut41)
process.pathCut41_40_50  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut41*process.centralityBin*process.Centrality40_50 *process.QWEvent*process.Mult *  process.histMult40_50_Cut41 * process.histCent_Cut41)
process.pathCut41_50_60  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut41*process.centralityBin*process.Centrality50_60 *process.QWEvent*process.Mult *  process.histMult50_60_Cut41 * process.histCent_Cut41)
process.pathCut41_60_70  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut41*process.centralityBin*process.Centrality60_70 *process.QWEvent*process.Mult *  process.histMult60_70_Cut41 * process.histCent_Cut41)
process.pathCut41_70_80  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut41*process.centralityBin*process.Centrality70_80 *process.QWEvent*process.Mult *  process.histMult70_80_Cut41 * process.histCent_Cut41)
process.pathCut41_80_90  =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut41*process.centralityBin*process.Centrality80_90 *process.QWEvent*process.Mult *  process.histMult80_90_Cut41 * process.histCent_Cut41)
process.pathCut41_90_100 =   cms.Path(process.hltMB*process.eventSelection*process.clusterCompatibilityFilterCut41*process.centralityBin*process.Centrality90_100*process.QWEvent*process.Mult * process.histMult90_100_Cut41 * process.histCent_Cut41)



process.schedule = cms.Schedule(
 process.pathCut20_0_5    ,
 process.pathCut20_5_10   ,
 process.pathCut20_10_20  ,
 process.pathCut20_20_30  ,
 process.pathCut20_30_40  ,
 process.pathCut20_40_50  ,
 process.pathCut20_50_60  ,
 process.pathCut20_60_70  ,
 process.pathCut20_70_80  ,
 process.pathCut20_80_90  ,
 process.pathCut20_90_100 ,

 process.pathCut23_0_5    ,
 process.pathCut23_5_10   ,
 process.pathCut23_10_20  ,
 process.pathCut23_20_30  ,
 process.pathCut23_30_40  ,
 process.pathCut23_40_50  ,
 process.pathCut23_50_60  ,
 process.pathCut23_60_70  ,
 process.pathCut23_70_80  ,
 process.pathCut23_80_90  ,
 process.pathCut23_90_100 ,

 process.pathCut26_0_5    ,
 process.pathCut26_5_10   ,
 process.pathCut26_10_20  ,
 process.pathCut26_20_30  ,
 process.pathCut26_30_40  ,
 process.pathCut26_40_50  ,
 process.pathCut26_50_60  ,
 process.pathCut26_60_70  ,
 process.pathCut26_70_80  ,
 process.pathCut26_80_90  ,
 process.pathCut26_90_100 ,

 process.pathCut29_0_5    ,
 process.pathCut29_5_10   ,
 process.pathCut29_10_20  ,
 process.pathCut29_20_30  ,
 process.pathCut29_30_40  ,
 process.pathCut29_40_50  ,
 process.pathCut29_50_60  ,
 process.pathCut29_60_70  ,
 process.pathCut29_70_80  ,
 process.pathCut29_80_90  ,
 process.pathCut29_90_100 ,

 process.pathCut32_0_5    ,
 process.pathCut32_5_10   ,
 process.pathCut32_10_20  ,
 process.pathCut32_20_30  ,
 process.pathCut32_30_40  ,
 process.pathCut32_40_50  ,
 process.pathCut32_50_60  ,
 process.pathCut32_60_70  ,
 process.pathCut32_70_80  ,
 process.pathCut32_80_90  ,
 process.pathCut32_90_100 ,

 process.pathCut35_0_5    ,
 process.pathCut35_5_10   ,
 process.pathCut35_10_20  ,
 process.pathCut35_20_30  ,
 process.pathCut35_30_40  ,
 process.pathCut35_40_50  ,
 process.pathCut35_50_60  ,
 process.pathCut35_60_70  ,
 process.pathCut35_70_80  ,
 process.pathCut35_80_90  ,
 process.pathCut35_90_100 ,

 process.pathCut38_0_5    ,
 process.pathCut38_5_10   ,
 process.pathCut38_10_20  ,
 process.pathCut38_20_30  ,
 process.pathCut38_30_40  ,
 process.pathCut38_40_50  ,
 process.pathCut38_50_60  ,
 process.pathCut38_60_70  ,
 process.pathCut38_70_80  ,
 process.pathCut38_80_90  ,
 process.pathCut38_90_100 ,

 process.pathCut41_0_5    ,
 process.pathCut41_5_10   ,
 process.pathCut41_10_20  ,
 process.pathCut41_20_30  ,
 process.pathCut41_30_40  ,
 process.pathCut41_40_50  ,
 process.pathCut41_50_60  ,
 process.pathCut41_60_70  ,
 process.pathCut41_70_80  ,
 process.pathCut41_80_90  ,
 process.pathCut41_90_100 ,
)
