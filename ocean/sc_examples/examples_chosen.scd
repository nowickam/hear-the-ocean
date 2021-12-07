(
{
	var out, lfoDepth, lfoRate, freq, filtered;
	lfoDepth = MouseY.kr(200, 8000, 1);
	lfoRate = MouseX.kr(4, 60, 1);
	freq = LFSaw.kr(lfoRate, 0, lfoDepth, lfoDepth * 1.2);
	filtered = RLPF.ar({WhiteNoise.ar(0.03)}.dup, freq, 0.1);
	out = CombN.ar(filtered, 0.3, 0.3, 2, 1, filtered);
	out
}.play;
)

(
{
		x = Pan2.ar(
		LPZ2.ar(LFPulse.ar(80 * (1 + 32.linrand), 2pi.rand, 0.1,

			Mix.ar(SinOsc.ar(0.125 * [2**rrand(0,6),2**rrand(0,6)], [2pi.rand,2pi.rand], 0.04)).max(0)
			* Mix.ar(SinOsc.ar(4 * [rrand(1,6),rrand(1,6)], [2pi.rand,2pi.rand])).abs

		)), 1.0.rand2);
		2.do({ x = AllpassN.ar(x, 0.05, [0.05.rand,0.05.rand], 4.0.rand, 2); });
		x

	}.play;
)

(
{
Pan2.ar(
	SinOsc.ar(rrand(24,108).midicps, 2pi.rand,

		Mix.ar(SinOsc.ar(exprand(0.3,8) * [rrand(0.7,1.3),1], [2pi.rand,2pi.rand], 0.1)).max(0)
		* Mix.ar(SinOsc.ar(exprand(6,24) * [rrand(0.7,1.3),1], [2pi.rand,2pi.rand], 0.1)).abs
	), 1.0.rand2);

}.play;
)

(
{
	var x, r, f;
	x = Mix.fill(4, {
		LFSaw.ar((LFPulse.kr(0.06,0,0.5,2,34 + {0.1.rand2}.dup)).midicps, 0, 0.01)
	});
	r = MouseY.kr(0.1,0.7);
	f = LinExp.kr(SinOsc.kr(0.07),-1,1,300,5000);
	x = RLPF.ar(x, f, r).softclip;
	x = RLPF.ar(x, f, r).softclip;
}.play;
)

(
{
	var root, scale, s;
	// whistle
	var trig, p, exc, x, freq;
		root = 5;
		scale = #[0,2,4,5,7,9,11];
			freq = (scale.choose + #[84,96].choose + root).midicps;
			exc = BrownNoise.ar([0.05,0.05]) * max(0, SinOsc.kr(exprand(0.125,0.5), 2pi.rand));
			s = (Klank.ar(`[
					Array[freq],
					nil,
					Array[0.3]
				], exc) * 0.1).softclip;
}.play
)

(
{
	var root, scale, s;
			// wind chords
	var trig, p, n, exc, x, base, range;
		//root = rrand(3,6);
		root = 5;
		scale = #[0,2,4,5,7,9,11];
		n = 5;
		exc = BrownNoise.ar([0.007,0.007]) * max(0, LFNoise1.kr(exprand(0.125,0.5), 1.25, -0.25));
		s = (Klank.ar(`[Array.fill(n, {(scale.choose + #[36,48,60,72,84].choose + root).midicps}),nil,Array.fill(n, {rrand(0.1,2.0)})], exc) * 0.1).softclip;
}.play;
)

(
// sweepy noise - mouse controls LFO
{
	var lfoDepth, lfoRate, freq, filtered;
	lfoDepth = MouseY.kr(200, 8000, 'exponential');
	lfoRate = MouseX.kr(4, 60, 'exponential');
	freq = LFSaw.kr(lfoRate, 0, lfoDepth, lfoDepth * 1.2);
	filtered = RLPF.ar(WhiteNoise.ar([0.03,0.03]), freq, 0.1);
	CombN.ar(filtered, 0.3, 0.3, 2, 1, filtered);
}.play
)


(
var a, c, d, s, z, y;
// reverberated sine percussion
d = 6; // number of percolators
c = 5; // number of comb delays
a = 4; // number of allpass delays

play({
		// sine percolation sound :
	s = Mix.ar(Array.fill(d, { Resonz.ar(Dust.ar(2/d, 50), 200 + 3000.0.rand, 0.003)}) );

		// reverb predelay time :
	z = DelayN.ar(s, 0.048);

		// 7 length modulated comb delays in parallel :
	y = Mix.ar(CombL.ar(z, 0.1, LFNoise1.kr(Array.fill(c,{0.1.rand}), 0.04, 0.05), 15));

		// chain of 4 allpass delays on each of two channels (8 total) :
	a.do({ y = AllpassN.ar(y, 0.050, [0.050.rand,0.050.rand], 1) });

		// add original sound to reverb and play it :
	s+(0.2*y)
}))

(
// harmonic swimming
play({
	var f, p, z, offset;
	f = 50;		// fundamental frequency
	p = 20;		// number of partials per channel
	z = 0.0;		// start of oscil daisy chain
	offset = Line.kr(0, -0.02, 60); // causes sound to separate and fade
	p.do({ arg i;
		z = FSinOsc.ar(
				f * (i+1), 		// freq of partial
				0,
				max(0, 			// clip negative amplitudes to zero
					LFNoise1.kr(
						6 + [4.0.rand2, 4.0.rand2],	// amplitude rate
						0.02, 					// amplitude scale
						offset					// amplitude offset
					)
				),
				z
		)
	});
	z
}))

(
// harmonic tumbling
play({
	var f, p, z, trig;
	f = 80;	// fundamental frequency
	p = 10;	// number of partials per channel
	z = 0.0;	// start of oscil daisy chain
	trig = XLine.kr([10,10], 0.1, 60);	// trigger probability decreases over time
	p.do({ arg i;
		z = FSinOsc.ar(
				f * (i+1), 		// freq of partial
				0,
				Decay2.kr(
					Dust.kr(
						trig,		// trigger rate
						0.02		// trigger amplitude
					),
					0.005, 	// grain attack time
					0.5.rand	// grain decay time
				),
				z
		)
	});
	z
}))

(
// analog bubbles
{
	f = LFSaw.kr(0.4, 0, 24, LFSaw.kr([8,7.23], 0, 3, 20)).midicps; // glissando function
	CombN.ar(SinOsc.ar(f, 0, 0.01), 0.2, 0.2, 10) // echoing sine wave
}.play)

// A.Broz: Waves & Surf (2020-10-29)

/*
           _.====.._
         ,:._       ~-_
             `\        ~-_
               |          `.
             ,/             ~-_
    -..__..-''                 ~~--..__...----...
*/
(
~waves = {
	var noise = { WhiteNoise.ar(0.04 + LFNoise1.kr(0.3, 0.03)) };
	var motion = { LFNoise1.kr(0.2).exprange(100, 2000) };
	var hps = { HPF.ar(noise.value, 50) };
	var wave = { LPF.ar(hps.value, motion.value).tanh };
	var sig = wave!10;
	sig = Splay.ar(sig, 0.6).tanh;
	sig = sig * Line.kr(0, 1, 10); // fade in
	LeakDC.ar(sig);
}.play;
)

~waves release: 10;
