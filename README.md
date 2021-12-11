# ImguiCandy
Color utils, Themes and other cool stuff for [Dear ImGui](https://github.com/ocornut/imgui)
## Usage
```cpp
#include "candy.h"
```
## Utils
These functions are to be used inside a loop, hence the use of static/global variables.
```cpp
ImVec4 Rainbow(double &static_ratio, double step = 0.01);

ImVec4 Gradient2(ImVec4 col1, ImVec4 col2, double &static_ratio, double step = 0.01);

ImVec4 Gradient3(ImVec4 col1, ImVec4 col2, ImVec4 col3, double &static_ratio, double step = 0.01);
```
#### Example
```cpp
  //Rainbow
  static double s0 = 0.0;
  ImGui::PushStyleColor(ImGuiCol_Button, ImCandy::Rainbow(s0));
```
![rainbow](https://user-images.githubusercontent.com/64605172/145681647-b9445ec7-5f4d-4c5f-98b8-eea840503392.gif)
```cpp
  //Gradient2
  static double s1 = 0.0;
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImCandy::Gradient2(IV4_VIOLET, IV4_BUBBLEGUM, s1));
```
![gradient](https://user-images.githubusercontent.com/64605172/145683127-8439a3fb-8a44-4642-94a4-97dd46c5349b.gif)
## Themes
 - Blender Dark [Improvised]
```cpp
  ImCandy::Theme_Blender();
```
![blender](https://user-images.githubusercontent.com/64605172/145681657-dc9786ae-5f79-4eca-90a6-197579b94fa5.png)
 - Cyberpunk Neon [Improvised]
```cpp
  ImCandy::Theme_Cyberpunk();
```
![cyberpunk](https://user-images.githubusercontent.com/64605172/145681654-2ebea5fc-ec99-44ba-a808-d20721415f6c.png)
## Contribute
Contribute your own themes by opening an [issue](https://github.com/Raais/ImguiCandy/issues) or [pull request](https://github.com/Raais/ImguiCandy/pulls).
