(
Ndef('waves', {
    var sig, n=25, ffreq, rq, lfo, amp;

    ffreq = Array.fill(n, { exprand(100.0, 1500.0) });
    rq = Array.fill(n, { rrand(0.8, 1.4) });
    lfo = Array.fill(n, { LFNoise2.ar(0.2).range(0.5, 2.0) });
    amp = Array.fill(n, { LFNoise2.ar(0.33) });
    sig = BPF.ar(
        PinkNoise.ar( amp * Slew.ar(lfo, 0.25) ).atan.tanh,
        ffreq * Slew.ar(lfo, 1, 0.25),
        rq * Slew.ar(lfo, 0.25)
        );
    Splay.ar(sig, 0.9)
});
Ndef('waves').set('fadeTime', 5);
Ndef('waves').play(
	vol: 0.61180276443486

);
);
