#include <iostream>
#include <ggponet.h>
#include <entt/entt.hpp>
#include <SDL.h>

using namespace std;

bool ggpo_begin_game(const char *game)
{
	cout << "begin_game callback: " << game << endl;
	return true;
}

bool ggpo_save_game_state(unsigned char **buffer, int *len, int *checksum, int frame)
{
	return true;
}

bool ggpo_load_game_state(unsigned char *buffer, int len)
{
	return true;
}

bool ggpo_log_game_state(char *filename, unsigned char *buffer, int len)
{
	return true;
}

bool ggpo_advance_frame(int flags)
{
	return true;
}

void ggpo_free_buffer(void *buffer)
{
	cout << "free_buffer callback: " << buffer << endl;
}

bool ggpo_on_event(GGPOEvent *info)
{
	return true;
}

int main(int argc, char *argv[])
{
	entt::registry registry;

	cout << "Hello world! " << &registry << endl;

	GGPOSession *session;
	GGPOSessionCallbacks cbs = {
		ggpo_begin_game,
		ggpo_save_game_state,
		ggpo_load_game_state,
		ggpo_log_game_state,
		ggpo_free_buffer,
		ggpo_advance_frame,
		ggpo_on_event};
	auto res = ggpo_start_session(&session, &cbs, "db kr platform fighter", 2, sizeof(int), 7001);
	cout << "ggpo_start_session: " << res << endl;

	res = ggpo_close_session(session);
	cout << "ggpo_close_session: " << res << endl;

	SDL_Window *window; // Declare a pointer

	SDL_Init(SDL_INIT_VIDEO); // Initialize SDL2

	// Create an application window with the following settings:
	window = SDL_CreateWindow(
		"An SDL2 window",		 // window title
		SDL_WINDOWPOS_UNDEFINED, // initial x position
		SDL_WINDOWPOS_UNDEFINED, // initial y position
		640,					 // width, in pixels
		480,					 // height, in pixels
		SDL_WINDOW_OPENGL		 // flags - see below
	);

	// Check that the window was successfully created
	if (window == NULL)
	{
		// In the case that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	// The window is open: could enter program loop here (see SDL_PollEvent())

	SDL_Delay(3000); // Pause execution for 3000 milliseconds, for example

	// Close and destroy the window
	SDL_DestroyWindow(window);

	// Clean up
	SDL_Quit();

	return 0;
}