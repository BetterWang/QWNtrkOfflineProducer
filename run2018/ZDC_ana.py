import os
import FWCore.ParameterSet.Config as cms
from getpass import getuser as user

process = cms.Process("QWZDC")

process.load("FWCore.MessageService.MessageLogger_cfi")
#process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))

#
#   Command Line Input(Copied from DQM for now)
#
import sys

#
#   Change the filename to process
#
runNumber = sys.argv[2]

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:./HFanalysis_'+runNumber+'.root'
    ),
    labelRawDataLikeMC = cms.untracked.bool(False)
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('zdc_'+runNumber+'.root')
)

process.options = cms.untracked.PSet(
        wantSummary = cms.untracked.bool(False)
        )

process.load('Configuration.Geometry.GeometryIdeal_cff')

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.autoCond import autoCond
#from CondCore.DBCommon.CondDBSetup_cfi import *
from CondCore.CondDB.CondDB_cfi import *

process.GlobalTag.globaltag = autoCond['startup']

process.zdcana = cms.EDAnalyzer('QWZDC2018Analyzer',
		srcADC = cms.untracked.InputTag('zdcdigi', 'ADC')
		)

process.p = cms.Path(process.zdcana)


