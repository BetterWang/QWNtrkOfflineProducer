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
        fileNames = cms.untracked.vstring("file:/afs/cern.ch/user/q/qwang/work/cleanroomRun2/Ana/data/ppReco.root")
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
    fileName = cms.string('pca.root')
)

process.load('HeavyIonsAnalysis.Configuration.collisionEventSelection_cff')
process.clusterCompatibilityFilter.clusterPars = cms.vdouble(0.0,0.006)

process.primaryVertexFilter.src = cms.InputTag("offlinePrimaryVertices")

process.eventSelection = cms.Sequence(
        process.hfCoincFilter3
        + process.primaryVertexFilter
#        + process.clusterCompatibilityFilter
)


process.QWPCA = cms.EDAnalyzer('QWPCA'
		, trackEta = cms.untracked.InputTag('QWEvent', "eta")
		, trackPhi = cms.untracked.InputTag('QWEvent', "phi")
		, trackWeight = cms.untracked.InputTag('QWEvent', "weight")
		, vertexZ = cms.untracked.InputTag('QWEvent', "vz")
		, centrality = cms.untracked.InputTag('Noff', "")
		, minvz = cms.untracked.double(-1.0)
		, maxvz = cms.untracked.double(15.0)
		, nvtx = cms.untracked.int32(100)
		)

process.load('PbPb_HIMB5_ppReco_noeff')
process.load('RecoHI.HiCentralityAlgos.CentralityFilter_cfi')

process.ppRecoCentFilter = process.centralityFilter.clone(
		selectedBins = cms.vint32(
			60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179),
		BinLabel = cms.InputTag("centralityBins")
		)

process.ppRecoCentFilter30_50 = process.centralityFilter.clone(
		selectedBins = cms.vint32(
			60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99),
		BinLabel = cms.InputTag("centralityBins")
		)

process.ppRecoCentFilter80_90 = process.centralityFilter.clone(
		selectedBins = cms.vint32(
			160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179),
		BinLabel = cms.InputTag("centralityBins")
		)
#process.path= cms.Path(process.eventSelection*process.centralityBin*process.QWEvent*process.QWPCA*process.histNoff*process.vectPhi*process.vectEta*process.vectPt*process.vectPhiW*process.vectEtaW*process.vectPtW)
process.path= cms.Path(process.eventSelection*process.makeEvent*process.ppRecoCentFilter80_90*process.QWPCA*process.vectMonW*process.corr2DW)

process.schedule = cms.Schedule(
	process.path,
)
