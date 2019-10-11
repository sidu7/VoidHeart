#pragma once

namespace Hollow {

class HOLLOW_API AudioManager {
  SINGLETON(AudioManager)
 public:
  void Init();
  void CleanUp();
  void Update();


 private:

};
}  // namespace Hollow