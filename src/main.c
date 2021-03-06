#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <SDL2/SDL.h>

#define FREQ 48000
#define SAMPLE_DT (1.0f / FREQ)
#define GEN_STEP (1.0f / (6.0f * SAMPLE_DT)) // control "pixelation"

typedef struct {
	Sint16 current;
	Sint16 next;
	float a;
} Gen;

Sint16 sgn(Sint16 x){
	return (x > 0) - (x < 0);
}

void white_noise(Gen *gen, Sint16 *stream, size_t stream_len){
	for(size_t i = 0; i < stream_len; ++i) {
		gen->a += GEN_STEP * SAMPLE_DT;
		stream[i] = (Sint16) floorf((gen->next - gen->current) * gen->a);

		if(gen->a >= 1.0f) {
			Sint16 value = rand() % (1 << 10); // control volume
			Sint16 sign = (rand() % 2) * 2 - 1;
			gen->current = gen->next;
			gen->next = value * sign;
			gen->a = 0.0f;
		}
	}
}

void white_noise_callback(void *userdata, Uint8 *stream, int len){
	assert(len % 2 == 0);
	white_noise(userdata, (Sint16*) stream, len/2);
}

int main(int argc, char **argv){
	(void) argc;
	(void) argv;

	if (SDL_Init(SDL_INIT_AUDIO) < 0){
		fprintf(stderr, "ERROR: could not init SDL because: %s\n", SDL_GetError());
		exit(1);
	}

	Gen gen = {0};

	SDL_AudioSpec desired = {
		.freq = FREQ,
		.format = AUDIO_S16LSB,
		.channels = 1,
		.callback = white_noise_callback,
		.userdata = &gen,
	};

	if (SDL_OpenAudio(&desired, NULL) < 0){
		fprintf(stderr, "ERROR: could not open audio device: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_PauseAudio(0);

	bool quit = false;
	while(!quit){

	}

	SDL_Quit();

	return 0;
}

