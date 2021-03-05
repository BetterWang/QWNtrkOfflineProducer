import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
import os
import sys
import FWCore.ParameterSet.VarParsing as VarParsing


#-----------------------------------
# Standard CMSSW Imports/Definitions
#-----------------------------------
process = cms.Process('MyTree',eras.Run2_2018_pp_on_AA)

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2018_realistic_hi', '')
process.GlobalTag.snapshotTime = cms.string("9999-12-31 23:59:59.000")
process.GlobalTag.toGet.extend([
   cms.PSet(record = cms.string("HeavyIonRcd"),
      tag = cms.string("CentralityTable_HFtowers200_HydjetDrum5F_v1032x02_mc"),
      connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS"),
      label = cms.untracked.string("HFtowers")
   ),
])

process.load("RecoHI.HiCentralityAlgos.CentralityBin_cfi")
process.centralityBin.Centrality = cms.InputTag("hiCentrality")
process.centralityBin.centralityVariable = cms.string("HFtowers")
process.dbCentBin = cms.EDProducer('QWInt2Double',
		src = cms.untracked.InputTag('centralityBin', 'HFtowers')
	)

#-----------------------------------
# Input source
#-----------------------------------
process.source = cms.Source('PoolSource',
        fileNames = cms.untracked.vstring('/store/himc/HINPbPbAutumn18DR/MinBias_Hydjet_Drum5F_2018_5p02TeV/AODSIM/NoPU_103X_upgrade2018_realistic_HI_v11-v1/50000/00EB0097-81C5-7B4B-B512-5C53E4CA41D2.root')
        #fileNames = cms.untracked.vstring('file:/afs/cern.ch/user/q/qwang/work/cleanroomRun2/Ana/data/HydjetDrum5F_RECODEBUG.root')
    )

#-----------
# Log output
#-----------
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = ''
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
    SkipEvent = cms.untracked.vstring('ProductNotFound')
    )


#-----------------
# Files to process
#-----------------
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )


#-----------------------------------------
# CMSSW/Hcal Related Module import
#-----------------------------------------
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
#process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")
process.load('Configuration.StandardSequences.MagneticField_38T_cff')


# event info
process.evtInfo = cms.EDProducer('QWEventInfoProducer')

process.QWcent = cms.EDProducer('QWCentrality2018Producer',
		Src = cms.untracked.InputTag('hiCentrality')
		)

# event tracking
process.QWEvent = cms.EDProducer("QWEventProducer"
        , vertexSrc = cms.untracked.InputTag('offlinePrimaryVertices')
        , trackSrc = cms.untracked.InputTag('generalTracks')
        , fweight = cms.untracked.InputTag('NA')
        , centralitySrc = cms.untracked.InputTag("centralityBin", "HFtowers")
        , dzdzerror = cms.untracked.double(3.0)
        , d0d0error = cms.untracked.double(3.0)
        , pterrorpt = cms.untracked.double(0.1)
        , ptMin = cms.untracked.double(0.0)
        , ptMax= cms.untracked.double(20.0)
        , Etamin = cms.untracked.double(-2.4)
        , Etamax = cms.untracked.double(2.4)
        , Year = cms.untracked.int32(2018)
        , chi2  = cms.untracked.double(0.18)
        )

process.evtTree = cms.EDAnalyzer('QWTreeMaker',
        Vtags = cms.untracked.VPSet(
            cms.PSet(
                tag = cms.untracked.InputTag('QWEvent', 'pt'),
                name = cms.untracked.string('pt')
                ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWEvent', 'eta'),
                name = cms.untracked.string('eta')
                ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWEvent', 'phi'),
                name = cms.untracked.string('phi')
                ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWEvent', 'charge'),
                name = cms.untracked.string('charge')
                ),
        ),
        Dtags = cms.untracked.VPSet(
            cms.PSet(
                tag = cms.untracked.InputTag('evtInfo', 'RunId'),
                name = cms.untracked.string('Run')
                ),
            cms.PSet(
                tag = cms.untracked.InputTag('evtInfo', 'Lumi'),
                name = cms.untracked.string('Lumi')
                ),
            cms.PSet(
                tag = cms.untracked.InputTag('evtInfo', 'BX'),
                name = cms.untracked.string('BX')
                ),
            cms.PSet(
                tag = cms.untracked.InputTag('evtInfo', 'Orbit'),
                name = cms.untracked.string('Orbit')
                ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etHFhitSum'),
                name = cms.untracked.string('etHFhitSum')
                ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etHFhitSumPlus'),
                name = cms.untracked.string('etHFhitSumPlus')
                ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etHFhitSumMinus'),
                name = cms.untracked.string('etHFhitSumMinus')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etHFtowerSum'),
                name = cms.untracked.string('etHFtowerSum')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etHFtowerSumPlus'),
                name = cms.untracked.string('etHFtowerSumPlus')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etHFtowerSumMinus'),
                name = cms.untracked.string('etHFtowerSumMinus')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etHFtruncated'),
                name = cms.untracked.string('etHFtruncated')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etHFtruncatedPlus'),
                name = cms.untracked.string('etHFtruncatedPlus')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etHFtruncatedMinus'),
                name = cms.untracked.string('etHFtruncatedMinus')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etEESum'),
                name = cms.untracked.string('etEESum')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etEESumPlus'),
                name = cms.untracked.string('etEESumPlus')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etEESumMinus'),
                name = cms.untracked.string('etEESumMinus')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etEEtruncated'),
                name = cms.untracked.string('etEEtruncated')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etEEtruncatedPlus'),
                name = cms.untracked.string('etEEtruncatedPlus')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etEEtruncatedMinus'),
                name = cms.untracked.string('etEEtruncatedMinus')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etEBSum'),
                name = cms.untracked.string('etEBSum')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etEcalSum'),
                name = cms.untracked.string('etEcalSum')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etEBtruncated'),
                name = cms.untracked.string('etEBtruncated')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etEcalTrancated'),
                name = cms.untracked.string('etEcalTrancated')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'zdcSum'),
                name = cms.untracked.string('zdcSum')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'zdcSumPlus'),
                name = cms.untracked.string('zdcSumPlus')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'zdcSumMinus'),
                name = cms.untracked.string('zdcSumMinus')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'pixelMultiplicity'),
                name = cms.untracked.string('pixelMultiplicity')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'trackMultiplicity'),
                name = cms.untracked.string('trackMultiplicity')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'ntracksPtCut'),
                name = cms.untracked.string('ntracksPtCut')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'ntracksEtaCut'),
                name = cms.untracked.string('ntracksEtaCut')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'ntracksEtaPtCut'),
                name = cms.untracked.string('ntracksEtaPtCut')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'nPixelTracks'),
                name = cms.untracked.string('nPixelTracks')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('QWcent', 'etMidRapiditySum'),
                name = cms.untracked.string('etMidRapiditySum')
            ),
            cms.PSet(
                tag = cms.untracked.InputTag('dbCentBin'),
                name = cms.untracked.string('CentBin')
            )
        )
    )

process.digiPath = cms.Path(
    process.QWcent*
    process.evtInfo*
    process.centralityBin*
    process.dbCentBin*
    process.QWEvent*
    process.evtTree
)

process.TFileService = cms.Service("TFileService",
		fileName = cms.string('zdcTree.root')
		)

#process.outpath = cms.EndPath(process.output)

