import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
import os
import sys

runNumber = '321730'
if len(sys.argv) > 2:
	runNumber = sys.argv[2]

#---------------
# My definitions
#---------------

sourceTag = "PoolSource"         # for global runs
rawTag    = cms.InputTag('rawDataCollector')
era       = eras.Run2_2018
GT        = "101X_dataRun2_Prompt_v11"
filedir = '/eos/cms/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/'+runNumber[:3]+'/'+runNumber[3:]+'/00000'
infile    = cms.untracked.vstring()
for f in reversed(os.listdir(filedir)):
	if f[-5:] == '.root' :
		infile.append('file:'+filedir+'/'+f)

#-----------------------------------
# Standard CMSSW Imports/Definitions
#-----------------------------------
process = cms.Process('MyTree',era)

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.GlobalTag.globaltag = GT

#----------
process.es_ascii = cms.ESSource(
    'HcalTextCalibrations',
    input = cms.VPSet(
        cms.PSet(
            object = cms.string('ElectronicsMap'),
            file = cms.FileInPath("HcalElectronicsMap_2018_v3.0_data_ZDCRPD_mockup.txt")
            )
        )
    )
process.es_prefer = cms.ESPrefer('HcalTextCalibrations', 'es_ascii')


#-----------
# Log output
#-----------
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = ''
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
    SkipEvent = cms.untracked.vstring('ProductNotFound')
    )


#-----------------
# Files to process
#-----------------
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100000)
    )

process.source = cms.Source(
    sourceTag,
    fileNames = infile
    )
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('293765:264-293765:9999')

#-----------------------------------------
# HLT selection -- Random
#-----------------------------------------

import HLTrigger.HLTfilters.hltHighLevel_cfi

process.hltRND = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.hltRND.HLTPaths = [
		"HLT_Random_v*",
		]
process.hltRND.andOr = cms.bool(True)
process.hltRND.throw = cms.bool(False)


#-----------------------------------------
# CMSSW/Hcal Related Module import
#-----------------------------------------
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")


#set digi and analyzer
process.hcalDigis.InputLabel = rawTag


# event info
process.QWInfo = cms.EDProducer('QWEventInfoProducer')

# ZDC info
process.zdcdigi = cms.EDProducer('QWZDC2018Producer',
		Src = cms.untracked.InputTag('hcalDigis', 'ZDC')
		)

# BX analyzer
process.zdcbx0 = cms.EDAnalyzer('QWBXAnalyzer',
		BX = cms.untracked.InputTag('QWInfo', 'BX'),
		fC = cms.untracked.InputTag('zdcdigi', 'nominalfC'),
		NS = cms.untracked.int32(3),
		TS = cms.untracked.int32(-1),
		nChannel = cms.untracked.int32(0),
		nSig = cms.untracked.int32(1),
		)

for i in range(1,50):
	exec('process.zdcbx'+str(i)+' = process.zdcbx0.clone(nChannel = cms.untracked.int32('+str(i*3)+'))')




# file
process.TFileService = cms.Service("TFileService",
		fileName = cms.string('zdcbx'+runNumber+'.root')
		)
# path
process.digiPath = cms.Path(
    process.hltRND *
    process.hcalDigis *
    process.QWInfo *
    process.zdcdigi *
    process.zdcbx0 *
    process.zdcbx1 *
    process.zdcbx2 *
    process.zdcbx3 *
    process.zdcbx4 *
    process.zdcbx5 *
    process.zdcbx6 *
    process.zdcbx7 *
    process.zdcbx8 *
    process.zdcbx9 *
    process.zdcbx10 *
    process.zdcbx11 *
    process.zdcbx12 *
    process.zdcbx13 *
    process.zdcbx14 *
    process.zdcbx15 *
    process.zdcbx16 *
    process.zdcbx17 *
    process.zdcbx18 *
    process.zdcbx19 *
    process.zdcbx20 *
    process.zdcbx21 *
    process.zdcbx22 *
    process.zdcbx23 *
    process.zdcbx24 *
    process.zdcbx25 *
    process.zdcbx26 *
    process.zdcbx27 *
    process.zdcbx28 *
    process.zdcbx29 *
    process.zdcbx30 *
    process.zdcbx31 *
    process.zdcbx32 *
    process.zdcbx33 *
    process.zdcbx34 *
    process.zdcbx35 *
    process.zdcbx36 *
    process.zdcbx37 *
    process.zdcbx38 *
    process.zdcbx39 *
    process.zdcbx40 *
    process.zdcbx41 *
    process.zdcbx42 *
    process.zdcbx43 *
    process.zdcbx44 *
    process.zdcbx45 *
    process.zdcbx46 *
    process.zdcbx47 *
    process.zdcbx48 *
    process.zdcbx49
)

process.output = cms.OutputModule(
		'PoolOutputModule',
		outputCommands = cms.untracked.vstring("drop *",
			"keep *_*_ZDC_MyTree",
			"keep *_QWInfo_*_MyTree",
			"keep *_zdcdigi_*_MyTree",
			"keep *_TriggerResults_*_MyTree",
			),
		SelectEvents = cms.untracked.PSet(
			SelectEvents = cms.vstring('digiPath')
			),
		fileName = cms.untracked.string('digisRAW_'+runNumber+'.root')
		)

process.outpath = cms.EndPath(process.output)
