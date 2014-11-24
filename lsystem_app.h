////////////////////////////////////////////////////////////////////////////////
//
// (C) Juanmihd
//
////////////////////////////////////////////////////////////////////////////////

#include "lsystem.h"

namespace octet {
  /// Scene containing a box with octet.
  class lsystem_app : public app {
    // scene for drawing box
    ref<visual_scene> app_scene;
  public:
    /// this is called when we construct the class before everything is initialised.
    lsystem_app(int argc, char **argv) : app(argc, argv) {
    }

    /// this is called once OpenGL is initialized
    void app_init() {
      app_scene =  new visual_scene();
      app_scene->create_default_camera_and_lights();

      //Adding the floor to the scene (not important, actually, remove by a keystroke)
      material *green = new material(vec4(0, 1, 0, 1));
      mesh_box *floor = new mesh_box(vec3(4));
      scene_node *node = new scene_node();
      node->translate(vec3(0, -15, 0));
      node->scale(vec3(10, 0.1, 10));
      app_scene->add_child(node);
      app_scene->add_mesh_instance(new mesh_instance(node, floor, green));


    }

    /// this will receive inputs from the keyboard
    void keyboard(){
      //O next step

      //L previous step

      //WSAD control camera

      //TGFH control tree (rotate and up-down)

      //B togles on-off the ground
    }

    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      app_scene->begin_render(vx, vy);

      //Ask for inputs of the keyboard
      keyboard();

      // update matrices. assume 30 fps.
      app_scene->update(1.0f/30);

      // draw the scene
      app_scene->render((float)vx / vy);
    }
  };
}
