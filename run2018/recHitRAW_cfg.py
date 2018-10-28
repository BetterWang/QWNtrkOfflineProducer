import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
import os
import sys
import FWCore.ParameterSet.VarParsing as VarParsing


options = VarParsing.VarParsing('analysis')

options.register('runNumber',
		111,
		VarParsing.VarParsing.multiplicity.singleton,
		VarParsing.VarParsing.varType.int,
		"Run number.")

options.register('runInputDir',
		'/eos/cms/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/',
		VarParsing.VarParsing.multiplicity.singleton,
		VarParsing.VarParsing.varType.string,
                "Directory where the RAW files will appear. lxplus: /eos/cms/store/t0streamer/Data/HIPhysicsMinimumBiasReducedFormat0/ or ...")

options.register('source',
		'Streamer', # default value
		VarParsing.VarParsing.multiplicity.singleton,
		VarParsing.VarParsing.varType.string,
		"Source type. Can be either PoolSource, or Streamer")

options.register('rawTag',
		'rawDataRepacker', # default value
		VarParsing.VarParsing.multiplicity.singleton,
		VarParsing.VarParsing.varType.string,
		"RAW input tag. Can be either rawDataCollector, or rawDataRepacker")


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
runNumber = str(options.runNumber)
GT        = "103X_dataRun2_HLT_v1"


#-----------------------------------
# Standard CMSSW Imports/Definitions
#-----------------------------------
process = cms.Process('MyTree',eras.Run2_2018_pp_on_AA)

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
    filedir = options.runInputDir+'000/'+runNumber[:3]+'/'+runNumber[3:]
    infile    = cms.untracked.vstring()
    for f in reversed(os.listdir(filedir)):
    	if f[-4:] == '.dat' :
    		infile.append('file:'+filedir+'/'+f)
    process.source = cms.Source('NewEventStreamFileReader',
        fileNames = infile
        )

#----------
_emap = {
    'mockup': "QWAna/QWNtrkOfflineProducer/run2018/HcalElectronicsMap_2018_v3.0_data_ZDCRPD_mockup.txt",
    'real'  : "QWAna/QWNtrkOfflineProducer/run2018/HcalElectronicsMap_2018_v3.0_data.txt",
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
    wantSummary = cms.untracked.bool(True),
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
#process.load('ZDC2018Pedestal_cfg')
process.zdcdigi.SOI = cms.untracked.int32(4)

process.zdcana = cms.EDAnalyzer('QWZDC2018Analyzer',
		srcADC = cms.untracked.InputTag('zdcdigi', 'ADC'),
		srcfC = cms.untracked.InputTag('zdcdigi', 'regularfC'),
		srcDetId = cms.untracked.InputTag('zdcdigi', 'DetId'),
		srcHigh = cms.untracked.InputTag('zdcdigi', 'chargeHigh'),
		srcLow = cms.untracked.InputTag('zdcdigi', 'chargeLow'),
		srcSum = cms.untracked.InputTag('zdcdigi', 'chargeSum'),
		)

process.digiPath = cms.Path(
    process.hcalDigis *
    process.zdcdigi *
    process.zdcana
)

process.TFileService = cms.Service("TFileService",
		fileName = cms.string('zdc_'+runNumber+'.root')
		)

process.output = cms.OutputModule(
		'PoolOutputModule',
		outputCommands = cms.untracked.vstring("drop *",
			"keep *_zdcdigi_*_MyTree",
			"keep *_*_ZDC_MyTree",
			"keep *_TriggerResults_*_MyTree",
			),
		SelectEvents = cms.untracked.PSet(
			SelectEvents = cms.vstring('digiPath')
			),
		fileName = cms.untracked.string('digisRAW_'+runNumber+'.root')
		)

#process.outpath = cms.EndPath(process.output)
