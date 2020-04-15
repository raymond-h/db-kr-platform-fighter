#include <iostream>
#include "ggponet.h"

using namespace std;

bool ggpo_begin_game(const char* game) {
	cout << "begin_game callback: " << game << endl;
	return true;
}

bool ggpo_save_game_state(unsigned char **buffer, int *len, int *checksum, int frame) {
	return true;
}

bool ggpo_load_game_state(unsigned char *buffer, int len) {
	return true;
}

bool ggpo_log_game_state(char *filename, unsigned char *buffer, int len) {
	return true;
}

bool ggpo_advance_frame(int flags) {
	return true;
}

void ggpo_free_buffer(void* buffer) {
	cout << "free_buffer callback: " << buffer << endl;
}

bool ggpo_on_event(GGPOEvent *info) {
	return true;
}

int main(int argc, char const *argv[]) {
	cout << "Hello world!" << endl;

	GGPOSession* session;
	GGPOSessionCallbacks cbs = {
		ggpo_begin_game,
		ggpo_save_game_state,
		ggpo_load_game_state,
		ggpo_log_game_state,
		ggpo_free_buffer,
		ggpo_advance_frame,
		ggpo_on_event
	};
	GGPOErrorCode res = ggpo_start_session(&session, &cbs, "db kr platform fighter", 2, sizeof(int), 7001);
	cout << "ggpo_start_session: " << res << endl;

	res = ggpo_close_session(session);
	cout << "ggpo_close_session: " << res << endl;

	return 0;
}