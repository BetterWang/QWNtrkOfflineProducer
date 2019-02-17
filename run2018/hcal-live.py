import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
from DQMServices.Core.DQMEDAnalyzer import DQMEDAnalyzer
import os
import FWCore.ParameterSet.VarParsing as VarParsing
import sys

options = VarParsing.VarParsing('analysis')

# options.inputFiles are inherited from 'analysis'
options.register('runNumber',
		111,
		VarParsing.VarParsing.multiplicity.singleton,
		VarParsing.VarParsing.varType.int,
		"Run number.")

options.register('runInputDir',
		'/fff/production_bu',
		VarParsing.VarParsing.multiplicity.singleton,
		VarParsing.VarParsing.varType.string,
		"Directory where the DQM files will appear. P5: /fff/BU0/output/lookarea or /fff/production_bu")

options.register('hlt',
		'',
		VarParsing.VarParsing.multiplicity.singleton,
		VarParsing.VarParsing.varType.string,
                "HLT path")

options.register('source',
		'Streamer', # default value
		VarParsing.VarParsing.multiplicity.singleton,
		VarParsing.VarParsing.varType.string,
		"Source type. Can be either PoolSource, or Streamer")

options.register('rawTag',
		'rawDataCollector', # default value
		VarParsing.VarParsing.multiplicity.singleton,
		VarParsing.VarParsing.varType.string,
		"RAW inputtag. Can be either rawDataCollector, or rawDataRepacker")

options.register('era',
		'Run2_2018', # default value
		VarParsing.VarParsing.multiplicity.singleton,
		VarParsing.VarParsing.varType.string,
		"Era, does not matter now")

options.register('emap',
		'', # default value
		VarParsing.VarParsing.multiplicity.singleton,
		VarParsing.VarParsing.varType.string,
		"Emap used for ZDC. Can be 'mockup', 'real', or 'FCD'")

options.parseArguments()

#---------------
# My definitions
#---------------

rawTag    = cms.InputTag(options.rawTag)
GT        = "103X_dataRun2_HLT_v1"
runNumber = str(options.runNumber)


#-----------------------------------
# Standard CMSSW Imports/Definitions
#-----------------------------------
process = cms.Process('MyTree',eras.Run2_2018_pp_on_AA)

process.load('DQM.Integration.config.environment_cfi')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.GlobalTag.globaltag = GT

#-----------------------------------
# Input source
#-----------------------------------
if options.source=='PoolSource':
    filedir = options.runInputDir+runNumber[:3]+'/'+runNumber[3:]+'/00000'
    infile    = cms.untracked.vstring()
    for f in reversed(os.listdir(filedir)):
    	if f[-5:] == '.root' :
    		infile.append('file:'+filedir+'/'+f)
    process.source = cms.Source('PoolSource',
        fileNames = infile
        )
else:
    process.source = cms.Source("DQMStreamerReader",
        SelectEvents = cms.untracked.vstring('*'),
        delayMillis = cms.untracked.uint32(500),
        deleteDatFiles = cms.untracked.bool(False),
        endOfRunKills = cms.untracked.bool(False),
        minEventsPerLumi = cms.untracked.int32(-1),
        nextLumiTimeoutMillis = cms.untracked.int32(0),
        runInputDir = cms.untracked.string(options.runInputDir),
        runNumber = cms.untracked.uint32(options.runNumber),
        scanOnce = cms.untracked.bool(True),
        skipFirstLumis = cms.untracked.bool(False),
        streamLabel = cms.untracked.string('streamDQM')
    )

#----------
_emap = {
    'mockup': "QWAna/QWNtrkOfflineProducer/run2018/HcalElectronicsMap_2018_v3.0_data_ZDCRPD_mockup.txt",
    'real'  : "QWAna/QWNtrkOfflineProducer/run2018/HcalElectronicsMap_2018_v3.0_data.txt",
    'ext'   : "QWAna/QWNtrkOfflineProducer/run2018/HcalElectronicsMap_2018_v3.0_data_ext.txt",
    'FCD'   : "QWAna/QWNtrkOfflineProducer/run2018/HcalElectronicsMap_2018_v3.0_data_FCD.txt",
}


if options.emap != '':
    process.es_ascii = cms.ESSource(
        'HcalTextCalibrations',
        input = cms.VPSet(
            cms.PSet(
                object = cms.string('ElectronicsMap'),
                file = cms.FileInPath(_emap[options.emap])
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
    input = cms.untracked.int32(-1)
    )

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('293765:264-293765:9999')

#-----------------------------------------
# HLT selection -- Random
#-----------------------------------------

import HLTrigger.HLTfilters.hltHighLevel_cfi

process.hltSelect = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.hltSelect.HLTPaths = [
		options.hlt+"*",
		]
process.hltSelect.andOr = cms.bool(True)
process.hltSelect.throw = cms.bool(False)

#-----------------------------------------
# CMSSW/Hcal Related Module import
#-----------------------------------------
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")


#set digi and analyzer
process.hcalDigis.InputLabel = rawTag

if options.rawTag == '':
    process.digis = cms.Sequence()
else:
    process.digis = cms.Sequence(process.hcalDigis)

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

process.QWBXTask = DQMEDAnalyzer('QWZDC2018BXTask',
		srcfC = cms.untracked.InputTag('zdcdigi', 'regularfC'),
		srcDid = cms.untracked.InputTag('zdcdigi', 'DetId'),
		BX = cms.untracked.InputTag('QWInfo', 'BX'),
		SOI = cms.untracked.int32(1),
		BXShift = cms.untracked.int32(0),
		)

process.zdcADCFilter = cms.EDFilter('QWZDC2018ADCFilter',
		srcDetId = cms.untracked.InputTag('zdcdigi', 'DetId'),
		srcCapId = cms.untracked.InputTag('zdcdigi', 'CapId'),
		srcADC = cms.untracked.InputTag('zdcdigi', 'ADC'),
		)

#process.dqm = cms.Sequence(process.QWBXTask )
#process.dqm = cms.Sequence(process.QWzdcQIE10Task)

process.DQMoutput = cms.OutputModule("DQMRootOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('DQMIO'),
        filterName = cms.untracked.string('')
    ),
    fileName = cms.untracked.string('file:step3_inDQM_'+runNumber+'.root'),
    outputCommands = cms.untracked.vstring("keep *"),
    splitLevel = cms.untracked.int32(0)
)

#process.dqmoffline_step = cms.EndPath(process.dqm)
process.DQMoutput_step = cms.EndPath(process.DQMoutput)

# file
#process.TFileService = cms.Service("TFileService",
#		fileName = cms.string('zdcbx'+runNumber+'.root')
#		)

# path
process.digiPath = cms.Path(
    process.hltSelect *
    process.digis
    * process.zdcdigi
    * process.zdcADCFilter
    * process.QWInfo
    * process.QWBXTask
    * process.QWzdcQIE10Task
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


#process.schedule = cms.Schedule(process.digiPath, process.dqmoffline_step,process.DQMoutput_step)
process.schedule = cms.Schedule(process.digiPath, process.DQMoutput_step)
#process.schedule = cms.Schedule(process.digiPath)
