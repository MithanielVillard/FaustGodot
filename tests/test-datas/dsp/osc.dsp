declare options "[midi:on]";

import("stdfaust.lib");

vol = hslider("volume [unit:dB] [midi:ctrl 15]", -20, -96, 0, 0.1) : ba.db2linear : si.smoo ;
freq = vslider("frequency[midi:ctrl 14]",200,50,1000,0.01) : si.smoo;
gate = button("gate");

process = os.sawtooth(freq) * vol * gate <: _, _;
