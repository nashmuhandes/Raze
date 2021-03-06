#ifndef __GL_SAMPLERS_H
#define __GL_SAMPLERS_H

#include <stdint.h>

enum ESampler
{
	NoSampler = -1,
	SamplerRepeat,
	SamplerClampX,
	SamplerClampY,
	SamplerClampXY,
	SamplerNoFilterRepeat,
	SamplerNoFilterClampX,
	SamplerNoFilterClampY,
	SamplerNoFilterClampXY,
	Sampler2DFiltered,	// Currently unused  shpuld be used for 2D content
	NumSamplers
};


class FSamplerManager
{
	// We need 6 different samplers: 4 for the different clamping modes,
	// one for 2D-textures and one for voxel textures
	unsigned int mSamplers[NumSamplers];

	void UnbindAll();
	void CreateSamplers();

public:

	FSamplerManager();
	~FSamplerManager();

	uint8_t Bind(int texunit, int num, int lastval);
	void SetTextureFilterMode(int mode, int aniso);


};

#endif

