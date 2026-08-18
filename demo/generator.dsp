//ceci est un commentaire

import("stdfaust.lib");

vol = hslider("volume [unit:dB]", -20, -96, 0, 0.1) : ba.db2linear : si.smoo ;
freq = vslider("frequency",200,50,1000,0.01) : si.smoo;
gate = button("gate");

process = os.sawtooth(freq) * vol * gate <: _, _;
