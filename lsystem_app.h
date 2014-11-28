////////////////////////////////////////////////////////////////////////////////
//
// (C) Juanmihd
//
////////////////////////////////////////////////////////////////////////////////

#include "lsystem.h"
#include "lsystem_mesh.h"

namespace octet {
  /// Scene containing a box with octet.
  class lsystem_app : public app {
    // scene for drawing box
    ref<visual_scene> app_scene;
    dynarray<ref<lsystem>> lsystems;
    int cur_lsystem;

    ///stack for the turtle position when generating blocks
    dynarray<mesh::vertex> stack3d;
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

      cur_lsystem = 0;
      ref<lsystem> temp_system = new lsystem();
      temp_system->load_file("assets/lsystem/tree_a.ls");
      lsystems.push_back(temp_system);
      stack3d.push_back(mesh::vertex(vec3(0), vec3(0), vec3(0)));
    }

    /// this will receive inputs from the keyboard
    void keyboard(){
      bool O_down = false;
      bool L_down = false;
      bool P_down = false;

      //1-9 0 select l_system
      if (is_key_going_down('1')){
        cur_lsystem = 1;
      }
      else if (is_key_going_down('2')){
        cur_lsystem = 2;
      }
      else if (is_key_going_down('3')){
        cur_lsystem = 3;
      }
      else if (is_key_going_down('4')){
        cur_lsystem = 4;
      }
      else if (is_key_going_down('5')){
        cur_lsystem = 5;
      }
      else if (is_key_going_down('6')){
        cur_lsystem = 6;
      }
      else if (is_key_going_down('7')){
        cur_lsystem = 7;
      }
      else if (is_key_going_down('8')){
        cur_lsystem = 8;
      }
      else if (is_key_going_down('9')){
        cur_lsystem = 9;
      }
      else if (is_key_going_down('0')){
        cur_lsystem = 0;
      }
      //O next step
      else if (is_key_going_down('O')){
        lsystems[cur_lsystem]->next();
      }
      //L previous step
      else if (is_key_going_down('L')){
        lsystems[cur_lsystem]->previous();
      }
      //P print current word
      else if (is_key_going_down('P')){
          lsystems[cur_lsystem]->print();
      }
      //WSAD control camera
      else if (is_key_down('W')){
      }
      else if (is_key_down('S')){
      }
      else if (is_key_down('A')){
      }
      else if (is_key_down('D')){
      }
      //TGFH control tree (rotate and up-down)
      else if (is_key_down('T')){
      }
      else if (is_key_down('G')){
      }
      else if (is_key_down('F')){
      }
      else if (is_key_down('H')){
      }
      //B togles on-off the ground
      else if (is_key_down('B')){
      }
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
