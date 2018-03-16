from CRABAPI.RawCommand import crabCommand
from CRABClient.UserUtilities import config, getUsernameFromSiteDB
from CRABClient.ClientExceptions import ClientException
from httplib import HTTPException

config = config()

config.General.requestName = 'PAMB1_UPC_QWTree_v1'
config.General.workArea = 'CrabArea'
config.General.transferOutputs = True
config.General.transferLogs = True
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'qw_pPb16_UPC_QWTree.py'
config.JobType.inputFiles = ['Hijing_8TeV_dataBS.root']
config.Data.inputDataset = '/PAMinimumBias1/PARun2016C-PromptReco-v1/AOD'
#config.Data.inputDBS = 'phys03'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 100
#config.Data.outLFNDirBase = '/store/user/qwang/ZDC/'
config.Data.outLFNDirBase = '/store/group/phys_heavyions/qwang/ZDC/'
config.Data.lumiMask = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/HI/Cert_285952-286496_HI8TeV_PromptReco_Pbp_Collisions16_JSON_NoL1T.txt'
#config.Data.publication = False
config.Data.useParent = False
config.Data.runRange = '286178-286496'
#config.Data.publication = True
#config.Data.outputDatasetTag = 'PAMB_Skim_v4'
config.Site.storageSite = 'T2_CH_CERN'
#config.Site.storageSite = 'T2_US_MIT'
config.Site.ignoreGlobalBlacklist = True
config.Data.allowNonValidInputDataset = True
try:
        crabCommand('submit', config = config)
except HTTPException as hte:
        print "Failed submitting task: %s" % (hte.headers)
except ClientException as cle:
        print "Failed submitting task: %s" % (cle)


#
#
## 2
#config.General.requestName = 'PAMB2_UPC_QWTree_v1'
#config.Data.inputDataset = '/PAMinimumBias2/PARun2016C-PromptReco-v1/AOD'
#try:
#        crabCommand('submit', config = config)
#except HTTPException as hte:
#        print "Failed submitting task: %s" % (hte.headers)
#except ClientException as cle:
#        print "Failed submitting task: %s" % (cle)
#
#
## 3
#config.General.requestName = 'PAMB3_UPC_QWTree_v1'
#config.Data.inputDataset = '/PAMinimumBias3/PARun2016C-PromptReco-v1/AOD'
#try:
#        crabCommand('submit', config = config)
#except HTTPException as hte:
#        print "Failed submitting task: %s" % (hte.headers)
#except ClientException as cle:
#        print "Failed submitting task: %s" % (cle)
#
#
#
## 4
#config.General.requestName = 'PAMB4_UPC_QWTree_v1'
#config.Data.inputDataset = '/PAMinimumBias4/PARun2016C-PromptReco-v1/AOD'
#try:
#        crabCommand('submit', config = config)
#except HTTPException as hte:
#        print "Failed submitting task: %s" % (hte.headers)
#except ClientException as cle:
#        print "Failed submitting task: %s" % (cle)
#
#
## 5
#config.General.requestName = 'PAMB5_UPC_QWTree_v1'
#config.Data.inputDataset = '/PAMinimumBias5/PARun2016C-PromptReco-v1/AOD'
#try:
#        crabCommand('submit', config = config)
#except HTTPException as hte:
#        print "Failed submitting task: %s" % (hte.headers)
#except ClientException as cle:
#        print "Failed submitting task: %s" % (cle)
#
#
## 6
#config.General.requestName = 'PAMB6_UPC_QWTree_v1'
#config.Data.inputDataset = '/PAMinimumBias6/PARun2016C-PromptReco-v1/AOD'
#try:
#        crabCommand('submit', config = config)
#except HTTPException as hte:
#        print "Failed submitting task: %s" % (hte.headers)
#except ClientException as cle:
#        print "Failed submitting task: %s" % (cle)
#
#
## 7
#config.General.requestName = 'PAMB7_UPC_QWTree_v1'
#config.Data.inputDataset = '/PAMinimumBias7/PARun2016C-PromptReco-v1/AOD'
#try:
#        crabCommand('submit', config = config)
#except HTTPException as hte:
#        print "Failed submitting task: %s" % (hte.headers)
#except ClientException as cle:
#        print "Failed submitting task: %s" % (cle)
#
#
## 8
#config.General.requestName = 'PAMB8_UPC_QWTree_v1'
#config.Data.inputDataset = '/PAMinimumBias8/PARun2016C-PromptReco-v1/AOD'
#try:
#        crabCommand('submit', config = config)
#except HTTPException as hte:
#        print "Failed submitting task: %s" % (hte.headers)
#except ClientException as cle:
#        print "Failed submitting task: %s" % (cle)
#
#
## 9
#config.General.requestName = 'PAMB9_UPC_QWTree_v1'
#config.Data.inputDataset = '/PAMinimumBias9/PARun2016C-PromptReco-v1/AOD'
#try:
#        crabCommand('submit', config = config)
#except HTTPException as hte:
#        print "Failed submitting task: %s" % (hte.headers)
#except ClientException as cle:
#        print "Failed submitting task: %s" % (cle)
#
#
## 10
#config.General.requestName = 'PAMB10_UPC_QWTree_v1'
#config.Data.inputDataset = '/PAMinimumBias10/PARun2016C-PromptReco-v1/AOD'
#try:
#        crabCommand('submit', config = config)
#except HTTPException as hte:
#        print "Failed submitting task: %s" % (hte.headers)
#except ClientException as cle:
#        print "Failed submitting task: %s" % (cle)
#
#
## 11
#config.General.requestName = 'PAMB11_UPC_QWTree_v1'
#config.Data.inputDataset = '/PAMinimumBias11/PARun2016C-PromptReco-v1/AOD'
#try:
#        crabCommand('submit', config = config)
#except HTTPException as hte:
#        print "Failed submitting task: %s" % (hte.headers)
#except ClientException as cle:
#        print "Failed submitting task: %s" % (cle)
#
## 12
#config.General.requestName = 'PAMB12_UPC_QWTree_v1'
#config.Data.inputDataset = '/PAMinimumBias12/PARun2016C-PromptReco-v1/AOD'
#try:
#        crabCommand('submit', config = config)
#except HTTPException as hte:
#        print "Failed submitting task: %s" % (hte.headers)
#except ClientException as cle:
#        print "Failed submitting task: %s" % (cle)
#
#
## 13
#config.General.requestName = 'PAMB13_UPC_QWTree_v1'
#config.Data.inputDataset = '/PAMinimumBias13/PARun2016C-PromptReco-v1/AOD'
#try:
#        crabCommand('submit', config = config)
#except HTTPException as hte:
#        print "Failed submitting task: %s" % (hte.headers)
#except ClientException as cle:
#        print "Failed submitting task: %s" % (cle)
#
#
## 14
#config.General.requestName = 'PAMB14_UPC_QWTree_v1'
#config.Data.inputDataset = '/PAMinimumBias14/PARun2016C-PromptReco-v1/AOD'
#try:
#        crabCommand('submit', config = config)
#except HTTPException as hte:
#        print "Failed submitting task: %s" % (hte.headers)
#except ClientException as cle:
#        print "Failed submitting task: %s" % (cle)
#
#
## 15
#config.General.requestName = 'PAMB15_UPC_QWTree_v1'
#config.Data.inputDataset = '/PAMinimumBias15/PARun2016C-PromptReco-v1/AOD'
#try:
#        crabCommand('submit', config = config)
#except HTTPException as hte:
#        print "Failed submitting task: %s" % (hte.headers)
#except ClientException as cle:
#        print "Failed submitting task: %s" % (cle)
#
#
## 16
#config.General.requestName = 'PAMB16_UPC_QWTree_v1'
#config.Data.inputDataset = '/PAMinimumBias16/PARun2016C-PromptReco-v1/AOD'
#try:
#        crabCommand('submit', config = config)
#except HTTPException as hte:
#        print "Failed submitting task: %s" % (hte.headers)
#except ClientException as cle:
#        print "Failed submitting task: %s" % (cle)
#
#
## 17
#config.General.requestName = 'PAMB17_UPC_QWTree_v1'
#config.Data.inputDataset = '/PAMinimumBias17/PARun2016C-PromptReco-v1/AOD'
#try:
#        crabCommand('submit', config = config)
#except HTTPException as hte:
#        print "Failed submitting task: %s" % (hte.headers)
#except ClientException as cle:
#        print "Failed submitting task: %s" % (cle)
#
#
## 18
#config.General.requestName = 'PAMB18_UPC_QWTree_v1'
#config.Data.inputDataset = '/PAMinimumBias18/PARun2016C-PromptReco-v1/AOD'
#try:
#        crabCommand('submit', config = config)
#except HTTPException as hte:
#        print "Failed submitting task: %s" % (hte.headers)
#except ClientException as cle:
#        print "Failed submitting task: %s" % (cle)
#
#
## 19
#config.General.requestName = 'PAMB19_UPC_QWTree_v1'
#config.Data.inputDataset = '/PAMinimumBias19/PARun2016C-PromptReco-v1/AOD'
#try:
#        crabCommand('submit', config = config)
#except HTTPException as hte:
#        print "Failed submitting task: %s" % (hte.headers)
#except ClientException as cle:
#        print "Failed submitting task: %s" % (cle)
#
#
## 20
#config.General.requestName = 'PAMB20_UPC_QWTree_v1'
#config.Data.inputDataset = '/PAMinimumBias20/PARun2016C-PromptReco-v1/AOD'
#try:
#        crabCommand('submit', config = config)
#except HTTPException as hte:
#        print "Failed submitting task: %s" % (hte.headers)
#except ClientException as cle:
#        print "Failed submitting task: %s" % (cle)
#
