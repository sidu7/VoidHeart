#include <hollowpch.h>
#include "AudioManager.h"

namespace Hollow {
void AudioManager::Init() {
  FMOD::Studio::System* system = NULL;
  FMOD::Studio::System::create(&system);
}
void AudioManager::CleanUp() {}
void AudioManager::Update() {}
}  // namespace Hollow