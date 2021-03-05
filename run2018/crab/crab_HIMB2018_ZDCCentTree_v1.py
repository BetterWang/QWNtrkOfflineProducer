from CRABAPI.RawCommand import crabCommand
from CRABClient.UserUtilities import config
from CRABClient.ClientExceptions import ClientException
from httplib import HTTPException

config = config()

config.General.requestName = 'HIMB1_ZDCCentTree_run326961_v6'
config.General.workArea = 'CrabArea'
config.General.transferOutputs = True
config.General.transferLogs = False
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'qw_ZDCCent_v1.py'
#config.JobType.maxJobRuntimeMin = 2500
#config.JobType.inputFiles = ['Hydjet_PbPb_eff_v1.root', 'Hydjet_ppReco_v5_loose.root']
config.Data.inputDataset = '/HIMinimumBias1/HIRun2018A-04Apr2019-v1/AOD'
#config.Data.inputDBS = 'phys03'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 50
config.Data.outLFNDirBase = '/store/group/phys_heavyions/qwang/PbPb2018/'
config.Data.lumiMask = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions18/HI/PromptReco/Cert_326381-327564_HI_PromptReco_Collisions18_JSON.txt'
config.Data.runRange = '326961'
config.Data.publication = False
#config.Data.outputDatasetTag = ''
config.Data.useParent = False
config.Site.storageSite = 'T2_CH_CERN'
#config.Data.allowNonValidInputDataset = True
config.JobType.allowUndistributedCMSSW = True
try:
        crabCommand('submit', config = config)
except HTTPException as hte:
        print "Failed submitting task: %s" % (hte.headers)
except ClientException as cle:
        print "Failed submitting task: %s" % (cle)

#config.General.requestName = 'HIMB0_ZDCCentTree_run326961_v6'
#config.Data.inputDataset = '/HIMinimumBias0/HIRun2018A-04Apr2019-v1/AOD'
#try:
#        crabCommand('submit', config = config)
#except HTTPException as hte:
#        print "Failed submitting task: %s" % (hte.headers)
#except ClientException as cle:
#        print "Failed submitting task: %s" % (cle)
#

#config.JobType.psetName = 'qw_ZDCCent_v2.py'
#config.General.requestName = 'HIMB2_ZDCCentTree_run326961_v5'
#config.Data.inputDataset = '/HIMinimumBias2/HIRun2018A-04Apr2019-v1/AOD'
#config.Data.unitsPerJob = 5
#try:
#        crabCommand('submit', config = config)
#except HTTPException as hte:
#        print "Failed submitting task: %s" % (hte.headers)
#except ClientException as cle:
#        print "Failed submitting task: %s" % (cle)
#
