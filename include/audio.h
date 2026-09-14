#pragma once

#include "audio_types.h"

audio_hash* audio_map_create(MIX_Mixer *mixer);
void audio_map_destroy(audio_hash* audio_map);
track_hash* audio_track_map_create(MIX_Mixer *mixer, audio_hash* audio_map);
void audio_track_map_destroy(track_hash* track_map);