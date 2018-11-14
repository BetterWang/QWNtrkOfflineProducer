# run 326383

## Had
### P side
| ratio     | data | should be | calib |
| :---      |:----:| :-----:   | :---: |
| Had2/Had1 | 0.75 | 0.61      | 0.81  |
| Had3/Had1 | 0.45 | 0.30      | 0.67  |
| Had4/Had1 | 0.32 | 0.26      | 0.81  |

### M side
| ratio     | data | should be | calib |
| :---      |:----:| :-----:   | :---: |
| Had2/Had1 | 0.95 | 0.61      | 0.62  |
| Had3/Had1 | 0.45 | 0.30      | 0.67  |
| Had4/Had1 | 0.30 | 0.26      | 0.87  |

## EM
Using 1n peak, take the peak number
### P side
| ratio     | data | 
| :---      |:----:| 
| Had1/EM1  | 1.   | 
| Had1/EM2  | 1.   | 
| Had1/EM3  | 0.7  | 
| Had1/EM4  | 0.8  | 
| Had1/EM5  | 0.6  | 

### M side
| ratio     | data | 
| :---      |:----:| 
| Had1/EM1  | 1.   | 
| Had1/EM2  | 1.   | 
| Had1/EM3  | 1.   | 
| Had1/EM4  | 0.8  | 
| Had1/EM5  | 0.6  | 

## Summary

M-side 1n 18000,
P-side 1n 12000,

# Run 326480, 326482 (Current)

## Had
### P side
| ratio     | data | should be | calib |
| :---      |:----:| :-----:   | :---: |
| Had2/Had1 | 1    | 0.61      | 0.61  |
| Had3/Had1 | 0.65 | 0.30      | 0.46  |
| Had4/Had1 | 0.50 | 0.26      | 0.52  |

### M side
| ratio         | data | should be | calib |
| :---          |:----:| :-----:   | :---: |
| Had2/Had1     | 0.75 | 0.61      | 0.81  |
| Had3/Had1     | 0.65 | 0.30      | 0.46  |
| Had4/Had1     | 0.45 | 0.26      | 0.58  |
| P_Had1/M_Had1 | 0.6  | 1.        | XXXX  |


## EM
Using 1n peak, take the peak number
### P side
| ratio     | data | 
| :---      |:----:| 
| Had1/EM1  | 1.1  | 
| Had1/EM2  | 1.1  | 
| Had1/EM3  | 0.8  | 
| Had1/EM4  | 1.0  | 
| Had1/EM5  | 0.6  | 

### M side
| ratio     | data | 
| :---      |:----:| 
| Had1/EM1  | 1.2  | 
| Had1/EM2  | 1.2  | 
| Had1/EM3  | 0.8  | 
| Had1/EM4  | 1.0  | 
| Had1/EM5  | 1.1  | 

## Summary

Peaked at 6700.

# Run 326480, pedestal subtraction

### P side
| ratio     | data | should be | calib |
| :---      |:----:| :-----:   | :---: |
| Had2/Had1 | 1    | 0.61      | 0.61  |
| Had3/Had1 | 0.65 | 0.30      | 0.46  |
| Had4/Had1 | 0.50 | 0.26      | 0.52  |

### M side
| ratio         | data | should be | calib |
| :---          |:----:| :-----:   | :---: |
| Had2/Had1     | 0.75 | 0.61      | 0.81  |
| Had3/Had1     | 0.65 | 0.30      | 0.46  |
| Had4/Had1     | 0.45 | 0.26      | 0.58  |
| P_Had1/M_Had1 | 0.33 | 1.        | XXXX  |

## EM
Using 1n peak, take the peak number
### P side
| ratio     | data | 
| :---      |:----:| 
| Had1/EM1  | 1.   | 
| Had1/EM2  | 1.   | 
| Had1/EM3  | 0.7  | 
| Had1/EM4  | 0.8  | 
| Had1/EM5  | 0.6  | 

### M side
| ratio     | data | 
| :---      |:----:| 
| Had1/EM1  | 1.   | 
| Had1/EM2  | 1.   | 
| Had1/EM3  | 1.   | 
| Had1/EM4  | 0.8  | 
| Had1/EM5  | 0.6  | 



# Matching Fill 7433

1n, with and without fC>1000, take the mean.

## HV setting

| channel   | HV   |
| ZDCP_EM1  | 1300 |
| ZDCP_EM2  | 1200 |
| ZDCP_EM3  | 1150 |
| ZDCP_EM4  | 1050 |
| ZDCP_EM5  | 1150 |
| ZDCP_HAD1 | 1100 |
| ZDCP_HAD2 | 1350 |
| ZDCP_HAD3 | 1400 |
| ZDCP_HAD4 | 1400 |
| ZDCM_EM1  | 1200 |
| ZDCM_EM2  | 1200 |
| ZDCM_EM3  | 1200 |
| ZDCM_EM4  | 1100 |
| ZDCM_EM5  | 1000 |
| ZDCM_HAD1 | 1200 |
| ZDCM_HAD2 | 1150 |
| ZDCM_HAD3 | 1300 |
| ZDCM_HAD4 | 1300 |
| ZDCP_RPD1 | 750  |
| ZDCP_RPD2 | 750  |
| ZDCP_RPD3 | 750  |
| ZDCP_RPD4 | 750  |
| ZDCM_RPD1 | 750  |
| ZDCM_RPD2 | 750  |
| ZDCM_RPD3 | 700  |
| ZDCM_RPD4 | 700  |


## EM P side

| channel | mean | norm to EM3 | w/o cut mean | norm to EM3 |
| EM1     | 4199 | 1.34        |        349.4 | 0.42        |
| EM2     | 3913 | 1.25        |        410.3 | 0.49        |
| EM3     | 3139 | 1.00        |        831.8 | 1.00        |
| EM4     | 6762 | 2.15        |       3166.0 | 3.81        |
| EM5     | 7442 | 2.37        |       3467.0 | 4.17        |


## EM M side

| channel | mean | norm to EM3 | w/o cut mean | norm to EM3 |
| EM1     | 4465 | 1.28        |        574.7 | 0.49        |
| EM2     | 4172 | 1.20        |        609.9 | 0.52        |
| EM3     | 3487 | 1.00        |       1182.0 | 1.00        |
| EM4     | 8632 | 2.48        |       4129.0 | 3.49        |
| EM5     | 9321 | 2.67        |       4566.0 | 3.86        |


## HAD P side

| channel | mean | norm to HAD1|
| HAD1    | 3458 | 1.00        |
| HAD2    | 3109 | 0.90        |
| HAD3    | 2297 | 0.66        |
| HAD4    | 2083 | 0.60        |

## HAD M side

| channel | mean | norm to HAD1|
| HAD1    | 5588 | 1.00        |
| HAD2    | 3861 | 0.69        |
| HAD3    | 3213 | 0.57        |
| HAD4    | 2535 | 0.45        |





