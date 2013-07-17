#include <SDL.h>
#include <SDL_image.h>
#include <android/log.h>

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);   // Initialize SDL2

	SDL_DisplayMode mode;
	int WIDTH,HEIGHT;
	if (SDL_GetCurrentDisplayMode(0, &mode)==0)
	{
		/* I read that android ignores these so you can just as well set
		them to 0 */
		WIDTH=mode.w;
		HEIGHT=mode.h;
	}
	__android_log_print(ANDROID_LOG_DEBUG, "LOG_TAG", "Window size: %d x %d!\n", WIDTH, HEIGHT);

	// Create an application window with the following settings:
	SDL_Window *window = SDL_CreateWindow(
	"Aldebaran",                 
	SDL_WINDOWPOS_UNDEFINED,           
	SDL_WINDOWPOS_UNDEFINED,           
	WIDTH,
	HEIGHT,
	SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL 
	);

	// Check that the window was successfully made
	if(window == NULL)
	{   
		// In the event that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// load our image
	SDL_Texture *img = IMG_LoadTexture(renderer, "kagamin.jpg");
	int w,h;
	SDL_QueryTexture(img, NULL, NULL, &w, &h); // get the width and height of the texture
	// put the location where we want the texture to be drawn into a rectangle
	// I'm also scaling the texture 2x simply by setting the width and height
	SDL_Rect texr; texr.x = 50; texr.y = 50; texr.w = w; texr.h = h;

	while (1)
	{

		// event handling
		SDL_Event e;
		if ( SDL_PollEvent(&e) )
		{
			if (e.type == SDL_QUIT)
			{
				break;
			}
			else if (e.type == SDL_KEYUP)
			{
				__android_log_print(ANDROID_LOG_DEBUG, "LOG_TAG", "Quit!\n");
				break;
			}
		}

		// clear the screen
		SDL_RenderClear(renderer);
		// copy the texture to the rendering context
		SDL_RenderCopy(renderer, img, NULL, &texr);

		// flip the backbuffer
		// this means that everything that we prepared behind the screens is actually shown
		SDL_RenderPresent(renderer);

	}

	// Close and destroy the window
	SDL_DestroyWindow(window); 

	// Clean up
	SDL_Quit(); 
	
	__android_log_print(ANDROID_LOG_DEBUG, "LOG_TAG", "Success!\n");
	
	return 0;
}
