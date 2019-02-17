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
1. Location: `/afs/cern.ch/work/q/qwang/public/ZDC2018/example`
2. Example B904 data: `B904_Integration_1000030239.root`
3. Example DIGIs output from `ZDC_digi.py`: `HFanalysis_1000030239.root`
4. Ideally, there should be 48 channels with 10 time slices each channel.
5. *Update*, all B904 runs can be found at `/eos/cms/store/group/dpg_hcal/comm_hcal/B904/`
6. Run summary for [PMT testing](https://www.evernote.com/l/AAG1pcCjgENOX6JdCExUbyTMNX4_Mf6Ydcs)

# P5 situation
## Electronics (P5 before run)
1. The ZDC QIE10 cards are installed in the USC, [Elog](http://cmsonline.cern.ch/cms-elog/1054529).
2. The QIE10s will be read out through uHTR card located at crate 38 slot 7.
3. The emap is `HcalElectronicsMap_2018_v3.0_data.txt`.

## Software Setup (Data)
1. Unpacker is working in `10_X_Y`.
2. to run the config: `cmsRun recHitRAW_cfg.py [run number]`
3. It reads express physics data from eos. So, pick your favorite run.
4. The number of samples (TS) has been changing over the time for debugging purpose (3, 4, 6 or 10). Since TS2, it is set to 6TS, beginning from run 323031.
5. Since TS2, FCD is readout as ZDC HAD channels. ZDC HAD is [1-5]. FCD is [0, 6-15]. Don't be scared if you see more than HAD channels from ZDC DIGIs.
6. During TS3, FCD will be out, so no extra ZDC HAD channels will be read out during HI data taking.


## P5 runs
1. 325769 local Pedestal run with ZDC HV 1500V, RPD HV 650V.
2. 325774 local Pedestal run with ZDC HV 0V, RPD HV 0V.
3. 325792 local Pedestal run with ZDC HV 1500V, RPD HV 750V.
4. 325868 global run with ZDC HV 1500V, RPD HV 750V.
5. 325897 local Pedestal run with ZDC HV 1500V, RPD HV 750V. After tunnel work Saturday.
6. 325906 local Pedestal run with ZDC HV 1500V, RPD HV 750V. After tunnel work Saturday. Taking off LV control limo connector for P side.
7. 325915 local Pedestal run with ZDC HV 0V, RPD HV 0V. After tunnel work Saturday. P side HV is disconnected.
8. 325921 local Pedestal run with ZDC HV 0V, RPD HV 0V. After tunnel work Saturday. P side HV is connected.
9. 325988 glbal run with ZDC HV 1500V, RPD HV 600V, initial time in by Dick. Before this run, the RPD HV is 750V, not timed in.
10. 326000 glbal run with ZDC HV 1500V, RPD HV 600V, pp VdM.
11. 326151 glbal run with ZDC HV 1500V, RPD HV 600V, ion circulate, collisions.
12. Fill 7413 has ~1h collisions, at BX=1,1786 (guess from ZB). Run 326175,  with ZDC HV 1500V, RPD HV 600V.
13. Fill 7415 has collisions, at BX=??  (guess from ZB). Run 326217,18..., (lot)  with ZDC HV 1500V, RPD HV 600V.
14. Since run 326237, lower ZDC HV to 1400V, RPD HV 600V. Fill 7416, BX=1,5,9,13...
15. Stable beam Fill 7427, HV setting1, http://cmsonline.cern.ch/cms-elog/1072239
16. Phase change will take effect starting from run 326435. http://cmsonline.cern.ch/cms-elog/1072707
17. ZDC HV changed from run 326449. http://cmsonline.cern.ch/cms-elog/1072784
18. RPD HV changed from run 326472. http://cmsonline.cern.ch/cms-elog/1072843
19. During run 326571, ZDC M side was lost, reading 255 all the time. Seems LV is lost. http://cmsonline.cern.ch/cms-elog/1073182
20. ZDC M side power supply is replaced. The ngFEC connection is restored. http://cmsonline.cern.ch/cms-elog/1073500, http://cmsonline.cern.ch/cms-elog/1073487
21. Fill 7442 VdM scan, run 326676
22. Fiber splitter, http://cmsonline.cern.ch/cms-elog/1073581, http://cmsonline.cern.ch/cms-elog/1073581, http://cmsonline.cern.ch/cms-elog/1073454, Emap is then changed to `ZDCemap_13-nov-2018.txt`. From run 326700 onward. The SOI then shifted to TS5.
23. RPD HV changed from 750 to 800, between Fill 7443 and 7444. Effective run 326722 onward. http://cmsonline.cern.ch/cms-elog/1073685
24. SOI shifted back to TS4 after run 326722. http://cmsonline.cern.ch/cms-elog/1073730


