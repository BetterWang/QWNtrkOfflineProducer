# 2018 ZDC B904 Setup for Software Development
## Update on (06/07/2018)
1. Emap updated ZDC904_emap_v2.txt
2. Example code updated on [github](https://github.com/BetterWang/QWNtrkOfflineProducer/blob/ZDC2018/src/QWZDC2018Producer.cc)
3. Config located at [here](https://github.com/BetterWang/QWNtrkOfflineProducer/blob/ZDC2018/run2018/ZDC_digi.py)

## Update on (05/29/2018)
## Detector
1. ZDC, on each side of the CMS
  * EM section, 5 channels on each side, in X direction;
  * HAD section, 4 channels on each side, in Z direction, behind EM section;
2. RPD, on each side of the CMS
  * 4x4 in XY plane, in front of ZDC EM section
3. Total channels, (4+5+16) = 25 channels on each side
4. Since each QIE10 card has only 24 channels, 1 HAD channel (HAD4) might be sacrificed

## Data Flow
1. Neutron hits detector (ZDC/RPD)
2. Energy deposit
3. Light produced
4. PMT collects the light and produces a pulse
5. Pulse being integrated in QIE10 and digitized (8bit), DIGI
6. Pack DIGIs from all 24x2 channels in uHTR card
7. Save to disk as RAW

## Electronics (B904)
1. In each side of the tunnel, one crate is powered by a 50V LV supply, (namely ZDCP/ZDCM in B904)
2. Inside each crate, one ngCCM in slot8, one QIE10 in slot4
3. In crate 50, slot2, one ngFEC is installed to control the ngCCM(s) in ZDCP/ZDCM
4. In crate 50, slot11,12, two uHTR cards are installed to receive data from QIE10s from ZDCP/ZDCM
5. Each QIE10 card is modified with 16 LEMO connectors, connecting to channel 1-16.

## Software Setup
1. Generally, the QIE10 unpacker software is already in the CMSSW release along with the HF
2. `CMSSW_10_X_X`
3. The latest emap for B904 is `ZDC904\_emap\_ext12.txt`
4. to run the config: `cmsRun recHit904_extPed_cfg.py 1000031441`
5. This should run out of the box. It subtracts the pedestal which is loaded from `ZDC2018Pedestal_cfg.py`
6. The pedestal is calculated from run 1000031442, with `cmsRun recHit904_Ped_cfg.py 1000031442`

## Example data (B904)
1. Location: /afs/cern.ch/work/q/qwang/public/ZDC2018/example
2. Example B904 data: B904_Integration_1000030239.root
3. Example DIGIs output from ZDC\_digi.py: HFanalysis_1000030239.root
4. Ideally, there should be 48 channels with 10 time slices each channel.
5. *Update*, all B904 runs can be found at `/eos/cms/store/group/dpg_hcal/comm_hcal/B904/`


