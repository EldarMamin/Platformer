#include "raylib.h"

#include "globals.h"
#include "level.h"
#include "player.h"
#include "graphics.h"
#include "assets.h"
#include "utilities.h"

void update_game() {
    game_frame++;
    switch (game_state) {
        case MENU_STATE:
            if (IsKeyPressed(KEY_ENTER)) {
                ResumeSound(background_sound);
                game_state = GAME_STATE;
            }
        break;
        case GAME_STATE:
            if (IsKeyPressed(KEY_ESCAPE)) {
                game_state = PAUSE_STATE;
            }

            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
                move_player_horizontally(MOVEMENT_SPEED);
            }
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
            move_player_horizontally(-MOVEMENT_SPEED);
        }

        // Calculating collisions to decide whether the player is allowed to jump: don't want them to suction cup to the ceiling or jump midair
        is_player_on_ground = is_colliding({player_pos.x, player_pos.y + 0.1f}, WALL);
        if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) || IsKeyDown(KEY_SPACE)) && is_player_on_ground) {
            player_y_velocity = -JUMP_STRENGTH;
        }

        update_player();
        break;
        case PAUSE_STATE:
            if (IsKeyPressed(KEY_ENTER)) {
                game_state = GAME_STATE;
            }
        break;

        case VICTORY_STATE:
            if (IsKeyPressed(KEY_ENTER)) {
                game_state = MENU_STATE;
            }
        break;

        case LOOSE_STATE:
            if (IsKeyPressed(KEY_ENTER)) {
                level_index = 0;
                game_state = MENU_STATE;
            }
        break;
    }
}


void draw_game() {
   //*Set volume*//
    SetSoundVolume(background_sound, 0.05);
    ClearBackground(BLACK);
    switch (game_state) {
        case MENU_STATE:
            StopSound(background_sound);
            draw_menu();
            load_level(0);
            player_score = 0;
            player_health = 3;
        break;
        case GAME_STATE:
            draw_level();
            draw_game_overlay();
        break;

        case VICTORY_STATE:
            StopSound(background_sound);
            draw_victory_menu();
        break;

        case PAUSE_STATE:
            PauseSound(background_sound);
            draw_pause_menu();
        break;

        case LOOSE_STATE:
            StopSound(background_sound);
            draw_loose_menu();
        break;
    }

}

int main() {
    InitWindow(1024, 480, "Platformer");
    SetTargetFPS(60);

    load_fonts();
    load_images();
    load_sounds();
    load_level();

    SetExitKey(0);
    while (!WindowShouldClose()) {
        if (!IsSoundPlaying(background_sound)) {
            PlaySound(background_sound);
        }
        BeginDrawing();

        update_game();
        draw_game();

        EndDrawing();
    }

    unload_level();
    unload_sounds();
    unload_images();
    unload_fonts();

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
