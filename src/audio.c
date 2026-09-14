#include "../vendored/stb/stb_ds.h"
#include "../include/audio_types.h"

MIX_Audio* audio_load_from_filepath(MIX_Mixer *mixer, char* filepath){
    MIX_Audio *audio = MIX_LoadAudio(mixer, filepath, true);
    if (!audio){
        SDL_Log("Couldn't load audio from %s: %s", filepath, SDL_GetError());
        return NULL;
    }
    else{
        return audio;
    }
}

MIX_Track* audio_create_audio_track(MIX_Mixer *mixer, audio_hash* audio_map, AUDIO_ID aid){
    MIX_Track *track = MIX_CreateTrack(mixer);
    if (!track){
        SDL_Log("Couldn't instantiate track: %s", SDL_GetError());
        return NULL;
    }
    else {
        MIX_SetTrackAudio(track, hmget(audio_map, aid));
        return track;
    }
}

audio_hash* audio_map_create(MIX_Mixer *mixer){
    audio_hash* audio_map = NULL;
    hmput(audio_map, AUDIO_ID_CARD_PLACE_1, audio_load_from_filepath(mixer, "../resources/audio/card-place-1.ogg"));
    hmput(audio_map, AUDIO_ID_CARD_SLIDE_1, audio_load_from_filepath(mixer, "../resources/audio/card-slide-1.ogg"));
    hmput(audio_map, AUDIO_ID_CHIP_LAY_1, audio_load_from_filepath(mixer, "../resources/audio/chip-lay-1.ogg"));
    hmput(audio_map, AUDIO_ID_OWIN31, audio_load_from_filepath(mixer, "../resources/audio/owin31.wav"));
    hmput(audio_map, AUDIO_ID_WOMP_WOMP, audio_load_from_filepath(mixer, "../resources/audio/womp-womp.mp3"));
    return audio_map;
}

void audio_map_destroy(audio_hash* audio_map){
    for (int i = 0; i < hmlen(audio_map); i++){
        MIX_DestroyAudio(audio_map[i].value);
        audio_map[i].value = NULL;
    }
    hmfree(audio_map);
}

track_hash* audio_track_map_create(MIX_Mixer *mixer, audio_hash* audio_map){
    track_hash* track_map = NULL;
    hmput(track_map, TRACK_ID_CARD_DEAL, audio_create_audio_track(mixer, audio_map, AUDIO_ID_CARD_PLACE_1));
    hmput(track_map, TRACK_ID_CARD_FLIP, audio_create_audio_track(mixer, audio_map, AUDIO_ID_CARD_SLIDE_1));
    hmput(track_map, TRACK_ID_CHEQUE_DEAL, audio_create_audio_track(mixer, audio_map, AUDIO_ID_CHIP_LAY_1));
    hmput(track_map, TRACK_ID_BLACKJACK_WIN, audio_create_audio_track(mixer, audio_map, AUDIO_ID_OWIN31));
    hmput(track_map, TRACK_ID_BLACKJACK_LOSS, audio_create_audio_track(mixer, audio_map, AUDIO_ID_WOMP_WOMP));
    return track_map;
}

void audio_track_map_destroy(track_hash* track_map){
    for (int i = 0; i < hmlen(track_map); i++){
        MIX_DestroyTrack(track_map[i].value);
        track_map[i].value = NULL;
    }
    hmfree(track_map);
}