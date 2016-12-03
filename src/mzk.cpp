//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008                                        //
//--------------------------------------------------------------------------//

#include <math.h>
#include "mzk.h"


#define BPM 140
#define SAMPLES_PER_BEAT 60*MZK_RATE/BPM

#define TWOPI 6.2831f

static const float notes [] = {
	440.00f, //A
	493.88f, //H
	523.25f, //C
	587.33f, //D
	659.26f, //E
	698.46f, //F
	783.99f, //G
};

static const int melody_length = 4;
static const char melody1 [] = {
	'A', 'D', 'G', 'D'
};
static const char melody2 [] = {
	'D', 'A', 'G', 'A'
};

static const char melody3 [] = {
	'G', 'A', 'G', 'D'
};

static const char melody4 [] = {
	'G', 'A', 'D', 'G'
};

static const char melody5 [] = {
	'A', 'G', 'A', 'D'
};

static const char melody6 [] = {
	'D', 'A', 'D', 'G'
};

inline float osc_sin(float freq, int i)
{
	return sinf( TWOPI*freq * (float)i/(float)MZK_RATE );
}

// put here your synth
void mzk_init( short *buffer )
{
	float amp = 0.0f, oldamp = 0.0f;
	float oldfl = 0.0f, oldfr = 0.0f;
    for( int i=0; i<MZK_NUMSAMPLES; i++ ) 
    {
        //const float fl = sinf( TWOPI*440.0f * (float)i/(float)MZK_RATE );
        //const float fr = sinf( TWOPI*587.3f * (float)i/(float)MZK_RATE );

		//const float fl = sinf( TWOPI*146.83f * (float)i/(float)MZK_RATE );
		if (i < 441000) {
			const float fr = 1.0f*sinf( TWOPI*55.0f * (float)i/(float)MZK_RATE ) * (1/sqrtf(TWOPI * 30 *(float)i/(float)MZK_RATE));
			const float fl = 1.001f*osc_sin(220.00f, i) * osc_sin(146.83f, i) * osc_sin(164.81f, i);
			
					
			

			//buffer[2*i+0] = (int)(fl +oldfl / 2 *32767.0f);
			//buffer[2*i+1] = (int)(fl + oldfl / 2 *32767.0f);
			//oldfl = fl;
			buffer[2*i+0] = (int)(fl*32767.0f);
			buffer[2*i+1] = (int)(fr*32767.0f);
		} else{
			float bass = sinf( TWOPI*36.71f * (float)i/(float)MZK_RATE )/2 + sinf( TWOPI*49.0f * (float)i/(float)MZK_RATE )/2 * ((amp + oldamp)/2);
			oldamp = amp;
			if (amp < 0.01f)
				amp = 0.99f;
			amp /= 1.0001f;

			if (i < 882000) {
				const float fl = osc_sin(notes[(melody1[(i/10000) % melody_length] - 'A')/2], i)/2 + bass/2;
				const float fr = osc_sin(notes[(melody2[(i/10000) % melody_length] - 'A')/2], i)/2 + bass/2;
				buffer[2*i+0] = (int)(fl+oldfl / 2 *32767.0f);
				buffer[2*i+1] = (int)(fr + oldfr / 2 *32767.0f);
				oldfl = fl;
				oldfr = fr;
			} else if (i < 1323000) {
				const float fl = osc_sin(notes[(melody3[(i/10000) % melody_length] - 'A')/2], i)/2 + bass/2;
				const float fr = osc_sin(notes[(melody4[(i/10000) % melody_length] - 'A')/2], i)/2 + bass/2;
				buffer[2*i+0] = (int)(fl+oldfl / 2 *32767.0f);
				buffer[2*i+1] = (int)(fr + oldfr / 2 *32767.0f);
				oldfl = fl;
				oldfr = fr;
			} else {
				const float fl = osc_sin(notes[(melody5[(i/10000) % melody_length] - 'A')/2], i)/2 + bass/2;
				const float fr = osc_sin(notes[(melody6[(i/10000) % melody_length] - 'A')/2], i)/2 + bass/2;
				buffer[2*i+0] = (int)(fl+oldfl / 2 *32767.0f);
				buffer[2*i+1] = (int)(fr + oldfr / 2 *32767.0f);
				oldfl = fl;
				oldfr = fr;
			}
		}

		//const float fl = osc_sin(notes[melody[(i/(MZK_RATE * BPM * 60)) % melody_length] - 'A'], i);

        //buffer[2*i+0] = (int)(fl*32767.0f);
		//buffer[2*i+0] = (int)(fl*32767.0f);
		//buffer[2*i+1] = (int)(fl*32767.0f);
        //buffer[2*i+1] = (int)(fr*32767.0f);
    }
}