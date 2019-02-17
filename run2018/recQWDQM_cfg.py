import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
from DQMServices.Core.DQMEDAnalyzer import DQMEDAnalyzer
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
            file = cms.FileInPath("QWAna/QWNtrkOfflineProducer/run2018/HcalElectronicsMap_2018_v3.0_data_FCD.txt")
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
    wantSummary = cms.untracked.bool(False),
    SkipEvent = cms.untracked.vstring('ProductNotFound')
    )


#-----------------
# Files to process
#-----------------
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000000)
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
process.load('QWZDC2018Producer_cfi')
process.zdcdigi.SOI = cms.untracked.int32(4)


# DQM step
process.load('Configuration.StandardSequences.Services_cff')
#process.load("DQM.HcalTasks.ZDCTask")
process.QWzdcQIE10Task = DQMEDAnalyzer('QWZDCQIE10Task',
		srcADC = cms.untracked.InputTag('zdcdigi', 'ADC'),
		srcfC = cms.untracked.InputTag('zdcdigi', 'regularfC'),
		srcDetId = cms.untracked.InputTag('zdcdigi', 'DetId'),
		srcHigh = cms.untracked.InputTag('zdcdigi', 'chargeHigh'),
		srcLow = cms.untracked.InputTag('zdcdigi', 'chargeLow'),
		srcSum = cms.untracked.InputTag('zdcdigi', 'chargeSum'),
		)

process.dqm = cms.Sequence(process.QWzdcQIE10Task)

process.DQMoutput = cms.OutputModule("DQMRootOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('DQMIO'),
        filterName = cms.untracked.string('')
    ),
    fileName = cms.untracked.string('file:step3_inDQM.root'),
    outputCommands = cms.untracked.vstring("keep *"),
    splitLevel = cms.untracked.int32(0)
)

process.dqmoffline_step = cms.EndPath(process.dqm)
process.DQMoutput_step = cms.EndPath(process.DQMoutput)

# file
#process.TFileService = cms.Service("TFileService",
#		fileName = cms.string('zdcbx'+runNumber+'.root')
#		)
# path
process.digiPath = cms.Path(
    process.hcalDigis 
    * process.zdcdigi
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


process.schedule = cms.Schedule(process.digiPath, process.dqmoffline_step,process.DQMoutput_step)
#process.schedule = cms.Schedule(process.digiPath)
