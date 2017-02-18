import FWCore.ParameterSet.Config as cms

process = cms.Process("Flow")

process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.EventContent.EventContentHeavyIons_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('QWAna.QWNtrkOfflineProducer.AfterBurnerGenerator_cfi')


process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(200000))
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:startup', '')

process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)

process.source = cms.Source("EmptySource")

process.generator = cms.EDFilter("HijingGeneratorFilter",
		rotateEventPlane = cms.bool(True),
		frame = cms.string('CMS     '),
		targ = cms.string('A       '),
		izp = cms.int32(82),
		bMin = cms.double(0),
		izt = cms.int32(82),
		proj = cms.string('A       '),
		comEnergy = cms.double(5023.0),
		iat = cms.int32(208),
		bMax = cms.double(15),
		iap = cms.int32(208)
		)

configurationMetadata = cms.untracked.PSet(
		version = cms.untracked.string('$Revision: 1.3 $'),
		annotation = cms.untracked.string('HIJING generator'),
		name = cms.untracked.string('$Source: QW $')
		)

process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = cms.untracked.vstring('keep *'),
    fileName = cms.untracked.string('Hijing.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    )
)

process.AfterBurner = cms.Sequence(cms.SequencePlaceholder("AftBurner"))

#process.pgen = cms.Sequence(cms.SequencePlaceholder("randomEngineStateProducer")+process.AfterBurner+process.GeneInfo)
process.pgen = cms.Sequence(cms.SequencePlaceholder("randomEngineStateProducer")+process.GeneInfo)

from IOMC.RandomEngine.RandomServiceHelper import RandomNumberServiceHelper
process.RandomNumberGeneratorService.AftBurner = cms.PSet(
		initialSeed = cms.untracked.uint32(7410904),
		engineName = cms.untracked.string('HepJamesRandom')
		)
randSvc = RandomNumberServiceHelper(process.RandomNumberGeneratorService)
randSvc.populate()

process.generation_step = cms.Path(process.pgen)
process.RAWSIMoutput_step = cms.EndPath(process.RAWSIMoutput)


process.schedule = cms.Schedule(
	process.generation_step,
)


for path in process.paths:
                getattr(process,path)._seq = process.generator * getattr(process,path)._seq

