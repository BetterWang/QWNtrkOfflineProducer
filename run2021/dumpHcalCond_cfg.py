## example cfg to dump HCAL conditions from the database
## (can be also used to dump sqlite content or to test fake conditions reading in CMSSW)
## Radek Ofierzynski, 9.11.2008
##
## Gena Kukartsev, July 29, 2009
## Gena Kukartsev, September 21, 2009

import FWCore.ParameterSet.Config as cms

process = cms.Process("DUMP")

#process.load("CondCore.DBCommon.CondDBSetup_cfi")
process.load("CondCore.CondDB.CondDB_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = '103X_dataRun2_Prompt_v2'

## specify which conditions you would like to dump to a text file in the "dump" vstring
process.prod = cms.EDAnalyzer("HcalDumpConditions",
    dump = cms.untracked.vstring(
#        'Pedestals'
#        ,'PedestalWidths'
#        ,'Gains'
#        ,'QIEData'
#        ,'QIETypes'
        'ElectronicsMap'
#        ,'ChannelQuality'
#        ,'GainWidths'
#        ,'RespCorrs'
#        ,'TimeCorrs'
#        ,'LUTCorrs'
#        ,'PFCorrs'
#        'L1TriggerObjects'
#        ,'ZSThresholds'
#        ,'ValidationCorrs'
#        ,'LutMetadata'
#        ,'DcsValues'
#        ,'DcsMap'
#        'TimingParams'
#    'RecoParams'
#    ,'LongRecoParams'
#    ,'MCParams'
#    ,'FlagHFDigiTimeParams'
#    ,'SiPMParameters'
#    ,'SiPMCharacteristics'
#    ,'TPParameters'
#    ,'TPChannelParameters'
#    ,'FrontEndMap'
#    ,'CalibrationsSet'
#    ,'CalibrationWidthsSet'
        ),
    outFilePrefix = cms.untracked.string('DumpCond')
)

## specify for which run you would like to get the conditions in the "firstRun"
process.source = cms.Source("EmptySource",
    numberEventsInRun = cms.untracked.uint32(1),
    firstRun = cms.untracked.uint32(327564)
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)
process.p = cms.Path(process.prod)


