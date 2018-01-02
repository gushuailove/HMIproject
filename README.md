# HMIproject
stm32f103 as main controller , user a industry touch screen realize HMI.

The HMI project contain a main control board, a industrial touch screen(TPC1061ti) and some IO device, 

	|Device			|Code
	|-----------------------|----------------
	|1. main control board	|1. MCB
	|-----------------------|----------------
	|2. touch screen	|2. TS
	|-----------------------|----------------
	|3. Atomizer switch 1/2	|3. S1_1/S2_1
	|-----------------------|----------------
	|4. Ozone switch 1/2	|4. S1_2/S2_2
	|-----------------------|----------------
	|5. water sensor 1/2	|5. Ss1/Ss2
	|-----------------------|----------------
	|6. foot key 1/2	|6. K1_1/K2_1



use case list as follows:

1. the system contain two channel, each channel can be work independently;

2. Each channel can be PowerOn or PowerOff by TS or K1_1/K2_1, K1_1 contorl channel_1, K2_1 contorl channel_2;

3. Each channel have two work mode: Atomizer mode and Ozone mode, just open Ozone switch when work in Ozone switch, and open Atomizer switch and Ozone switch when work in Atomizer mode;

4. Channel can be stop/continue when working by TS and K1_1/K2_1;

5. When S1_1 is On, MCB will detect Ss1, TS will be warning if Ss1 indicate no water, and channel_1 will be stop;

6. The length of work can be set between 10 and 30 minutes.
