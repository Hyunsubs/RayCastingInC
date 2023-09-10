#include "textures.h"

//upng로 불러올 파일 경로
static const char* textureFileNames[NUM_TEXTURES] = {
	"../images/redbrick.png",
	"../images/purplestone.png",
	"../images/mossystone.png",
	"../images/graystone.png",
	"../images/colorstone.png",
	"../images/bluestone.png",
	"../images/wood.png",
	"../images/eagle.png",
	"../images/pikuma.png",
};

//wallTextures 정보 넣기
void loadWallTexutures()
{
	// Todo: load all the png textures from the filepaths
	for (int i = 0; i < NUM_TEXTURES; i++)
	{
		upng_t* upng;
		upng = upng_new_from_file(textureFileNames[i]);
		if (upng != NULL)
		{
			upng_decode(upng);
			if (upng_get_error(upng) == UPNG_EOK)
			{
				wallTextures[i].upngTexture = upng;
				wallTextures[i].width = upng_get_width(upng);
				wallTextures[i].height = upng_get_height(upng);
				wallTextures[i].texture_buffer = (uint32_t*)upng_get_buffer(upng);
			}
		}
		
	}
}

void freeWallTextures()
{
	for (int i = 0; i < NUM_TEXTURES; i++)
	{
		upng_free(wallTextures[i].upngTexture);
	}
}
