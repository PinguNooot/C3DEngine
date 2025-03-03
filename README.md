# OpenGLEngine
![C3DRendering_Flying_Opti](https://github.com/user-attachments/assets/ce4e4e94-609c-4bd8-ba98-da357d59b0d4)
![C3DRendering](https://github.com/user-attachments/assets/c57af6b8-1197-451f-92ee-33e0d9c0466a)
## What the goal of this project

- Learning the Graphics Pipeline without using Graphics API (Only using the CPU)
- Try to know the limit of the rendering part back in the PS1 and PS2 era.

## How to Use

### Librairies

- SDL2

🙌 You don't need to download external librairies, all the librairies are in the repo and linked !

⚠️ Only work on Window OS

- Clone the repository
- Launch the Visual Studio Solution
- Build and Play in Debug

## Features

- Display 3D Object (Model or simple primitive).
- Load a texture and display to the model
- Lighting : Simple directionnal Light
- Simple Camera Movement (like a plane).
- Switch Render Parts
- Switch Meshes
- View differents part of the rendering project

## Controls

## Controls

| **Category** | **Key** | **Action** |
| --- | --- | --- |
| **Movement** | UpArrow | Move Forward |
|  | DownArrow | Move Backward |
|  | LeftArrow | Move Left |
|  | RightArrow | Move Right |
|  | A | Move Up |
|  | E | Move Down |
|  | Shift Left | Accelerate |
|  | Ctrl Left | Deccelerate |
| **Rotation** | Z | Rotate Camera Pitch Down |
|  | S | Rotate Camera Pitch Up |
|  | Q | Rotate Camera Pitch Left |
|  | D | Rotate Camera Pitch Right |
| **Camera** | Middle Mouse  | Move Camera on X & Z axis  |
|  | Mouse Wheel | Zoom In/Out  |
|  | M | Orient camera to cursor  |
| if M | Mouse Movement | Move Camera View |
| --------------------- | **Modifying Rendering** | ----------------------------------------- |
| **Switch Render** | 1 | Render Wire Vertex |
|  | 2 | Render Wire |
|  | 3 | Render Fill Triangle |
|  | 4 | Render Fill Triangle Wire |
|  | 5 | Render Fill Textured |
|  | 6 | Render Fill Textured Wire |
|  | C | Render Cull Backfaces |
|  | X | Render Cull None |
| **Switch Mesh** | TAB | Switch to next Render Mesh |
|  | 7 | Render Mesh 1 |
|  | 8 | Render Mesh 2 |
|  | 9 | Render Mesh 3 |
|  | 0 | Render All the meshes |
| Solo Mesh Scene CMD | R  | Rotate Render Mesh |
|  | Left Click | Switch to previous Render Mesh |
|  | Right Click | Switch to next Render Mesh |
| All Meshes Scene CMD | Space | Play/Pause Animation |
|  | Left Click | Switch to a previous placement camera |
|  | Right Click | Switch to a new placement camera |

### Remapping the Input

- Go to the key_mapping.c and add or change the GLFW Key

## Modify the Engine

### Add Meshes

⚠️ CEngine only support .obj file. For the FBX File, you need to install the FBX SDK from Autodesk and implement the logic yourself.

- Add the Model in the Assets Folder
- Open the resource.h and add the path to your(s) model(s)
- Open the main.cpp, go to the initialize_meshes() and add the Model witht the load_mesh function (obj_filename, png_filename, scale, translation, rotation)

✔️ For now the model and his resource are loaded but now you need to display him !

Now you have two Options : 

- You can press 0 and see all the meshes in the scene with your model appear !
- You can comment the third model and load your and press 9
✅ If you follow all this step, now you have the Model display in the Engine.

## More Pictures
### All Meshes Scene

![C3DRendering_Movement_Opti](https://github.com/user-attachments/assets/e5f10fca-88ea-4153-a5aa-ba351239519c)

### Controls in Engine
![C3DRendering_3](https://github.com/user-attachments/assets/60414390-ede1-4eaa-a3e6-54e7791feab3)

### Rendering State

![Vertices State](https://github.com/user-attachments/assets/50cdfd75-bca7-457a-8f27-5670c73fe554)
![Polygons State](https://github.com/user-attachments/assets/743ae70b-3ec2-4c8d-85d7-54894e04da6b)
![Shading State](https://github.com/user-attachments/assets/524c747f-cab5-4ac6-906e-d097a7fc8690)
![Textures State](https://github.com/user-attachments/assets/7596567c-5d97-4de2-9ae4-bc9058888cf1)


## Model uses :

F40 : https://sketchfab.com/3d-models/psx-style-ford-gt40-324b0853af194efe90f8f09784143066
DoomGun : https://sketchfab.com/3d-models/doom-eternal-plasma-rifle-but-low-poly-2e8f3b3adbcf4b59b0de5674b2ff3a69
Spyro : https://www.models-resource.com/playstation/spyrothedragon/model/51736/
Crash Bandicoot : https://www.models-resource.com/playstation/crashbandicoot/model/9125/
