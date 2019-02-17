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

options.register('skipEvents',
		0,
		VarParsing.VarParsing.multiplicity.singleton,
		VarParsing.VarParsing.varType.int,
		"Skip first N events.")

options.register('runInputDir',
		'/eos/cms/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/',
		VarParsing.VarParsing.multiplicity.singleton,
		VarParsing.VarParsing.varType.string,
                "Directory where the RAW files will appear. lxplus: /eos/cms/store/t0streamer/Data/HIPhysicsMinimumBiasReducedFormat0/ or ...")

options.register('outputTag',
		'',
		VarParsing.VarParsing.multiplicity.singleton,
		VarParsing.VarParsing.varType.string,
                "Output file name tag.")

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
		'rawDataRepacker', # default value
		VarParsing.VarParsing.multiplicity.singleton,
		VarParsing.VarParsing.varType.string,
		"RAW input tag. Can be either rawDataCollector, or rawDataRepacker, or rawDataReducedFormat, or virginRawDataRepacker")


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
    if options.runInputDir[-5:] == '.root':
        print options.runInputDir
        process.source = cms.Source('PoolSource',
            fileNames = cms.untracked.vstring("file:"+options.runInputDir),
            skipEvents=cms.untracked.uint32(options.skipEvents)
            )
    else:
        filedir = options.runInputDir+runNumber[:3]+'/'+runNumber[3:]+'/00000'
        print filedir
        infile    = cms.untracked.vstring()
        for f in reversed(os.listdir(filedir)):
            if f[-5:] == '.root' :
                infile.append('file:'+filedir+'/'+f)
            process.source = cms.Source('PoolSource',
                fileNames = infile,
                skipEvents=cms.untracked.uint32(options.skipEvents)
            )
elif options.source=='B904':
    infile = "file:"+options.runInputDir+"/run"+runNumber+"/B904_Integration_"+runNumber+'.root'
    print infile
    process.source = cms.Source('HcalTBSource',
        fileNames = cms.untracked.vstring(infile),
        skipEvents=cms.untracked.uint32(options.skipEvents)
        )
elif options.source=='HcalTBSource':
    infile    = 'file:'+options.runInputDir+'/run'+runNumber+'/USC_'+runNumber+'.root'
    process.source = cms.Source(
        'HcalTBSource',
        fileNames = cms.untracked.vstring(infile),
        skipEvents=cms.untracked.uint32(options.skipEvents)
        )
else:
    filedir = options.runInputDir+'000/'+runNumber[:3]+'/'+runNumber[3:]
    infile    = cms.untracked.vstring()
    for f in reversed(os.listdir(filedir)):
    	if f[-4:] == '.dat' :
    		infile.append('file:'+filedir+'/'+f)
    process.source = cms.Source('NewEventStreamFileReader',
        fileNames = infile,
	skipEvents=cms.untracked.uint32(options.skipEvents)
        )

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
    input = cms.untracked.int32(options.maxEvents)
    )

#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('293765:264-293765:9999')

#-----------------------------------------
# HLT selection -- Random
#-----------------------------------------

import HLTrigger.HLTfilters.hltHighLevel_cfi

if options.hlt!='':
    process.hltSelect = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
    process.hltSelect.HLTPaths = [
        options.hlt+"*",
    ]
    process.hltSelect.andOr = cms.bool(True)
    process.hltSelect.throw = cms.bool(False)
else:
    process.hltSelect = cms.Sequence()

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


# ZDC info
process.load('QWAna.QWZDC2018RecHit.QWZDC2018Producer_cfi')
process.load('QWAna.QWZDC2018RecHit.QWZDC2018RecHit_cfi')


process.zdcADCFilter = cms.EDFilter('QWZDC2018ADCFilter',
		srcDetId = cms.untracked.InputTag('zdcdigi', 'DetId'),
		srcCapId = cms.untracked.InputTag('zdcdigi', 'CapId'),
		srcADC = cms.untracked.InputTag('zdcdigi', 'ADC'),
		)

process.load('QWAna.QWZDC2018RecHit.QWZDC2018RecHit_cfi')
process.QWzdcreco.bFilter = cms.untracked.bool(True)

process.QWRecHitAna = cms.EDProducer('QWZDCRecHitProducer',
        Src = cms.untracked.InputTag('QWzdcreco')
        )

process.Pside = cms.EDFilter('QWDoubleFilter',
        src = cms.untracked.InputTag('QWzdcreco', 'NpeakP'),
        dmin = cms.untracked.double(0.9),
        dmax = cms.untracked.double(1.1)
        )

process.Mside = cms.EDFilter('QWDoubleFilter',
        src = cms.untracked.InputTag('QWzdcreco', 'NpeakM'),
        dmin = cms.untracked.double(0.9),
        dmax = cms.untracked.double(1.1)
        )

process.ZDCEnergyM = cms.EDProducer('QWDouble2VectorProducer',
        src = cms.untracked.InputTag('QWRecHitAna', 'sumMlow')
        )

process.ZDCEnergyP = cms.EDProducer('QWDouble2VectorProducer',
        src = cms.untracked.InputTag('QWRecHitAna', 'sumPlow')
        )

process.ZDCNSpecM = cms.EDAnalyzer('QWVectorAnalyzer',
        src = cms.untracked.InputTag('QWRecHitAna', 'NegLow'),
        cNbins = cms.untracked.int32(80),
        cstart = cms.untracked.double(0),
        cend = cms.untracked.double(800),
        hNbins = cms.untracked.int32(50),
        hstart = cms.untracked.double(0),
        hend = cms.untracked.double(50)
        )

process.ZDCNSpecP = process.ZDCNSpecM.clone( src = cms.untracked.InputTag('QWRecHitAna', 'PosLow') )

process.DRPath = cms.Sequence(
    process.hltSelect *
    process.digis *
    process.zdcdigi *
    process.zdcADCFilter *
    process.QWzdcreco
)


process.NpeakM = cms.Path(
    process.DRPath *
#    process.Mside *
    process.QWRecHitAna *
    process.ZDCEnergyM *
    process.ZDCNSpecM
)

process.NpeakP = cms.Path(
    process.DRPath *
#    process.Pside *
    process.QWRecHitAna *
    process.ZDCEnergyP *
    process.ZDCNSpecP
)

#if options.bTree:
#	process.digiPath += process.BXTree

process.TFileService = cms.Service("TFileService",
		fileName = cms.string('zdcRecHit_'+runNumber+options.outputTag+'.root')
		)

process.output = cms.OutputModule(
		'PoolOutputModule',
		outputCommands = cms.untracked.vstring("drop *",
			"keep *_*_*_MyTree",
			),
		SelectEvents = cms.untracked.PSet(
			SelectEvents = cms.vstring('NpeakP')
			),
		fileName = cms.untracked.string('digisRAW_'+runNumber+'.root')
		)

process.outpath = cms.EndPath(process.output)

#process.schedule = cms.Schedule(process.NpeakP, process.NpeakM, process.outpath)
process.schedule = cms.Schedule(process.NpeakP, process.NpeakM)
