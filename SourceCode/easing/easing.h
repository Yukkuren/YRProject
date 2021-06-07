#pragma once
#include <algorithm>

namespace Easing
{
	const float PI = 3.1415926f;
	const float Rad90 = PI / 2.0f;

	//---------------------------------------
	//	Quad
	//---------------------------------------
	inline float QuadIn(float time, float totaltime)
	{
		float t = (std::min)((std::max)(time / totaltime, 0.0f), 1.0f);
		return t * t;
	}

	inline float QuadOut(float time, float totaltime)
	{
		float t = (std::min)((std::max)(time / totaltime, 0.0f), 1.0f);
		return -t * ( t - 2.0f );
	}

	inline float QuadInOut(float time, float totaltime)
	{
		float t = (std::min)((std::max)(time / totaltime, 0.0f), 1.0f);
		if ( t < 0.5f )
		{
			return 2.0f * t * t;
		}
		else
		{
			return 2.0f * -t * ( t - 2.0f ) - 1.0f;	//-time*(time-2)‚ð“WŠJ‚µ‚Ä2”{‚·‚éB“š‚¦‚ÌÅ‘å‚ð1‚É‚·‚é‚½‚ß‚É1‚ðˆø‚­
		}
	}


	//---------------------------------------
	//	Cubic
	//---------------------------------------
	inline float CubicIn(float time, float totaltime)
	{
		float t = ( std::min )( ( std::max )( time / totaltime, 0.0f ), 1.0f );
		return t * t * t;
	}

	inline float CubicOut(float time, float totaltime)
	{
		float t = ( std::min )( ( std::max )( time / totaltime, 0.0f ), 1.0f );
		t -= 1.0f;
		return t * t * t + 1.0f;
	}

	inline float CubicInOut(float time, float totaltime)
	{
		float t = ( std::min )( ( std::max )( time / totaltime, 0.0f ), 1.0f );
		t *= 2.0f;
		if ( t < 1.0f )
		{
			return 0.5f * t*t*t;
		}
		else
		{
			t -= 2.0f;
			return ( t*t*t + 2.0f ) * 0.5f;
		}
	}

	//---------------------------------------
	//	Quart
	//---------------------------------------
	inline float QuartIn(float time, float totaltime)
	{
		float t = ( std::min )( ( std::max )( time / totaltime, 0.0f ), 1.0f );
		return t * t * t * t;
	}

	inline float QuartOut(float time, float totaltime)
	{
		float t = ( std::min )( ( std::max )( time / totaltime, 0.0f ), 1.0f );
		t -= 1.0f;
		return -( t * t * t * t - 1.0f );
	}

	inline float QuartInOut(float time, float totaltime)
	{
		float t = ( std::min )( ( std::max )( time / totaltime, 0.0f ), 1.0f );
		t *= 2.0f;
		if ( t < 1.0f )
		{
			return 0.5f *t*t*t*t;
		}
		else
		{
			t -= 2.0f;
			return -0.5f * ( t*t*t*t- 2.0f );
		}
	}


	//---------------------------------------
	//	Quint
	//---------------------------------------
	inline float QuintIn(float time, float totaltime)
	{
		float t = ( std::min )( ( std::max )( time / totaltime, 0.0f ), 1.0f );
		return t * t * t * t * t;
	}

	inline float QuintOut(float time, float totaltime)
	{
		float t = ( std::min )( ( std::max )( time / totaltime, 0.0f ), 1.0f );
		t -= 1.0f;
		return ( t * t * t * t * t + 1.0f );
	}

	inline float QuintInOut(float time, float totaltime)
	{
		float t = std::min<float>(std::max<float>(time / totaltime, 0.0), 1.0);
		t *= 2.0f;
		if ( t < 1.0f )
		{
			return 0.5f * t * t * t * t * t;
		}
		else
		{
			t -= 2.0f;
			return 0.5f * ( t * t * t* t * t + 2.0f );
		}
	}


	//---------------------------------------
	//	Sine
	//---------------------------------------
	inline float SineIn(float time, float totaltime)
	{
		float t = ( std::min )( ( std::max )( time / totaltime, 0.0f ), 1.0f );
		return -cos(t * Rad90) + 1.0f;
	}

	inline float SineOut(float time, float totaltime)
	{
		float t = ( std::min )( ( std::max )( time / totaltime, 0.0f ), 1.0f );
		return sinf(t * Rad90);
	}

	inline float SineInOut(float time, float totaltime)
	{
		float t = ( std::min )( ( std::max )( time / totaltime, 0.0f ), 1.0f );
		return -0.5f * ( cosf(t * PI) - 1.0f );
	}

	//---------------------------------------
	//	Exp
	//---------------------------------------
	inline float ExpIn(float time, float totaltime)
	{
		float t = ( std::min )( ( std::max )( time / totaltime, 0.0f ), 1.0f );
		return pow(2.0f, 10.0f * ( t - 1.0f ));
	}

	inline float ExpOut(float time, float totaltime)
	{
		float t = ( std::min )( ( std::max )( time / totaltime, 0.0f ), 1.0f );
		return -pow(2.0f, -10.0f * t) + 1.0f;
	}

	inline float ExpInOut(float time, float totaltime)
	{
		float t = ( std::min )( ( std::max )( time / totaltime, 0.0f ), 1.0f );
		t *= 2.0f;
		if ( t < 1.0f )
		{
			return 0.5f * pow(2.0f, 10.0f * ( t - 1.0f ));
		}
		else
		{
			t--;
			return 0.5f * ( -pow(2.0f, -10.0f * t) + 2.0f );
		}
	}


	//---------------------------------------
	//	Circ
	//---------------------------------------
	inline float CircIn(float time, float totaltime)
	{
		float t = ( std::min )( ( std::max )( time / totaltime, 0.0f ), 1.0f );
		return -( sqrtf(1.0f - t * t) - 1.0f );
	}

	inline float CircOut(float time, float totaltime)
	{
		float t = ( std::min )( ( std::max )( time / totaltime, 0.0f ), 1.0f );
		t -= 1;
		return sqrtf(1.0f - t * t);
	}

	inline float CircInOut(float time, float totaltime)
	{
		float t = ( std::min )( ( std::max )( time / totaltime, 0.0f ), 1.0f );
		t *= 2.0f;
		if ( t < 1.0f )
		{
			return -0.5f * ( sqrtf(1.0f - t * t) - 1.0f );
		}
		else
		{
			t -= 2;
			return 0.5f * ( sqrtf(1.0f - t * t) + 1.0f );
		}
	}


	//---------------------------------------
	//	Back
	//---------------------------------------
	inline float BackIn(float time, float totaltime)
	{
		float t = ( std::min )( ( std::max )( time / totaltime, 0.0f ), 1.0f );
		return t * t * ( 2.70158f * t - 1.70158f );
	}

	inline float BackOut(float time, float totaltime)
	{
		float t = ( std::min )( ( std::max )( time / totaltime, 0.0f ), 1.0f );
		return 1.0f + ( --t ) * t * ( 2.70158f * t + 1.70158f );
	}

	inline float BackInOut(float time, float totaltime)
	{
		float t = ( std::min )( ( std::max )( time / totaltime, 0.0f ), 1.0f );
		if ( t < 0.5f )
		{
			return t * t * ( 7.0f * t - 2.5f ) * 2.0f;
		}
		else
		{
			return 1.0f + ( --t ) * t * 2.0f * ( 7.0f * t + 2.5f );
		}
	}


	//---------------------------------------
	//	Elastic
	//---------------------------------------
	inline float ElasticIn(float time, float totaltime)
	{
		float t = ( std::min )( ( std::max )( time / totaltime, 0.0f ), 1.0f );
		float t2 = t * t;
		return t2 * t2 * sinf(t * PI * 4.5f);
	}

	inline float ElasticOut(float time, float totaltime)
	{
		float t = ( std::min )( ( std::max )( time / totaltime, 0.0f ), 1.0f );
		float t2 = ( t - 1.0f ) * ( t - 1.0f );
		return 1 - t2 * t2 * cosf(t * PI * 4.5f);
	}

	inline float ElasticInOut(float time, float totaltime)
	{
		float t2;
		float t = ( std::min )( ( std::max )( time / totaltime, 0.0f ), 1.0f );
		if ( t < 0.45f )
		{
			t2 = t * t;
			return 8.0f * t2 * t2 * sinf(t * PI * 9.0f);
		}
		else if ( t < 0.55f )
		{
			return 0.5f + 0.75f * sinf(t * PI * 4.0f);
		}
		else
		{
			t2 = ( t - 1.0f ) * ( t - 1.0f );
			return 1.0f - 8.0f * t2 * t2 * sinf(t * PI * 9.0f);
		}
	}


	//---------------------------------------
	//	Bounce
	//---------------------------------------
	inline float BounceIn(float time, float totaltime)
	{
		float t = ( std::min )( ( std::max )( time / totaltime, 0.0f ), 1.0f );
		return powf(2.0f, 6.0f * ( t - 1.0f )) * fabsf(sinf(t * PI * 3.5f));
	}

	inline float BounceOut(float time, float totaltime)
	{
		float t = ( std::min )( ( std::max )( time / totaltime, 0.0f ), 1.0f );
		return 1.0f - powf(2.0f, -6.0f * t) * fabsf(cosf(t * PI * 3.5f));
	}

	inline float BounceInOut(float time, float totaltime)
	{
		float t = ( std::min )( ( std::max )( time / totaltime, 0.0f ), 1.0f );

		if ( t < 0.5f )
		{
			return 8.0f * powf(2.0f, 8.0f * ( t - 1.0f )) * fabsf(sinf(t * 3.1415926f * 7.0f));
		}
		else
		{
			return 1.0f - 8.0f * powf(2.0f, -8.0f * t) * fabsf(sinf(t * 3.1415926f * 7.0f));
		}
	}


	//---------------------------------------
	//	Linear
	//---------------------------------------
	inline float Linear(float time, float totaltime)
	{
		float t = ( std::min )( ( std::max )( time / totaltime, 0.0f ), 1.0f );
		return t;
	}
}