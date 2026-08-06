//comment test

import("stdfaust.lib");

vol = hslider("volume [unit:dB] [midi:ctrl 15]", -20, -96, 0, 0.1) : ba.db2linear : si.smoo;
freq = hslider("frequency[midi:ctrl 14]",200,50,1000,0.01) : si.smoo;

process = os.osc(freq) * vol;
