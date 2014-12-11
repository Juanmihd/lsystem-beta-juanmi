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
    enum {_FAR_FAR_AWAY = 100000};
    // scene for drawing box
    ref<visual_scene> app_scene;
    dynarray<ref<lsystem>> lsystems;
    dynarray<ref<lsystem_mesh>> lsystem_meshes;
    dynarray<ref<scene_node>> lsystem_nodes;
    ref<scene_node> floor_node;
    char * file_name;
    bool floor_on;
    int cur_lsystem;
    //Camera stuff!
    mouse_ball camera;

  public:
    /// this is called when we construct the class before everything is initialised.
    lsystem_app(int argc, char **argv) : app(argc, argv) {
      if (argc == 2)
        file_name = argv[1];
      else
        file_name = "assets/lsystem/tree_i.ls";
    }

    /// this is called once OpenGL is initialized
    void app_init() {
      app_scene =  new visual_scene();
      app_scene->create_default_camera_and_lights();

      //Adding the floor to the scene (not important, actually, removable by a keystroke)
      material *green = new material(vec4(0, 0.3f, 0.1f, 1));
      mesh_box *floor = new mesh_box(vec3(4));
      floor_node = new scene_node();
      floor_node->translate(vec3(0, -0.01f, 0));
      floor_node->scale(vec3(1000, 0.01f, 1000));
      app_scene->add_child(floor_node);
      app_scene->add_mesh_instance(new mesh_instance(floor_node, floor, green));
      floor_on = true;
      cur_lsystem = 1;
      lsystem *temp_system = new lsystem();
      temp_system->load_file("assets/lsystem/tree_j.ls");
      lsystems.push_back(temp_system);
      temp_system = new lsystem();
      temp_system->load_file("assets/lsystem/tree_a.ls");
      lsystems.push_back(temp_system);
      temp_system = new lsystem();
      temp_system->load_file("assets/lsystem/tree_b.ls");
      lsystems.push_back(temp_system);
      temp_system = new lsystem();
      temp_system->load_file("assets/lsystem/tree_c.ls");
      lsystems.push_back(temp_system);
      temp_system = new lsystem();
      temp_system->load_file("assets/lsystem/tree_d.ls");
      lsystems.push_back(temp_system);
      temp_system = new lsystem();
      temp_system->load_file("assets/lsystem/tree_e.ls");
      lsystems.push_back(temp_system);
      temp_system = new lsystem();
      temp_system->load_file("assets/lsystem/tree_f.ls");
      lsystems.push_back(temp_system);
      temp_system = new lsystem();
      temp_system->load_file("assets/lsystem/tree_g.ls");
      lsystems.push_back(temp_system);
      temp_system = new lsystem();
      temp_system->load_file("assets/lsystem/tree_h.ls");
      lsystems.push_back(temp_system);
      temp_system = new lsystem();
      //It will try to load a file given from the command line, if not
      // it will upload the tree_i from the assets folder
      temp_system->load_file(file_name);
      lsystems.push_back(temp_system);
      lsystem_mesh *temp_mesh;
      for (int i = 0; i != 10; ++i){
        temp_mesh = new lsystem_mesh;
        lsystem_meshes.push_back(temp_mesh);
        lsystem_meshes[i]->init_values(lsystems[i]->get_angle(), lsystems[i]->get_distance());
        // use a shader that just outputs the color_ attribute.
        param_shader *shader = new param_shader("shaders/default.vs", "shaders/simple_color.fs");
        material *red = new material(vec4(1, 0, 0, 1), shader);
        scene_node *node = new scene_node();
        node->rotate(-90, vec3(0, 1, 0));
        node->translate(vec3(_FAR_FAR_AWAY, 0, _FAR_FAR_AWAY));
        lsystem_nodes.push_back(node);
        app_scene->add_child(node);
        lsystem_meshes[i]->input_word(lsystems[i]->get_iteration(), lsystems[i]->get_word(), lsystems[i]->get_size_word());
        app_scene->add_mesh_instance(new mesh_instance(node, lsystem_meshes[i], red));
      }
      lsystem_nodes[cur_lsystem]->translate(vec3(-_FAR_FAR_AWAY, 0, -_FAR_FAR_AWAY));
      camera.init(this, 1, 100);
    }

    /// this will receive inputs from the keyboard
    void keyboard(){
      //1-9 0 select l_system
      if (is_key_going_down('1') && !is_key_down(key_ctrl)){
        lsystem_nodes[cur_lsystem]->translate(vec3(_FAR_FAR_AWAY, 0, _FAR_FAR_AWAY));
        cur_lsystem = 1;
        lsystem_nodes[cur_lsystem]->translate(vec3(-_FAR_FAR_AWAY, 0, -_FAR_FAR_AWAY));
        //printf("Switch to tree 1\n");
      }
      else if (is_key_going_down('2') && !is_key_down(key_ctrl)){
        lsystem_nodes[cur_lsystem]->translate(vec3(_FAR_FAR_AWAY, 0, _FAR_FAR_AWAY));
        cur_lsystem = 2;
        lsystem_nodes[cur_lsystem]->translate(vec3(-_FAR_FAR_AWAY, 0, -_FAR_FAR_AWAY));
        //printf("Switch to tree 2\n");
      }
      else if (is_key_going_down('3') && !is_key_down(key_ctrl)){
        lsystem_nodes[cur_lsystem]->translate(vec3(_FAR_FAR_AWAY, 0, _FAR_FAR_AWAY));
        cur_lsystem = 3;
        lsystem_nodes[cur_lsystem]->translate(vec3(-_FAR_FAR_AWAY, 0, -_FAR_FAR_AWAY));
        //printf("Switch to tree 3\n");
      }
      else if (is_key_going_down('4') && !is_key_down(key_ctrl)){
        lsystem_nodes[cur_lsystem]->translate(vec3(_FAR_FAR_AWAY, 0, _FAR_FAR_AWAY));
        cur_lsystem = 4;
        lsystem_nodes[cur_lsystem]->translate(vec3(-_FAR_FAR_AWAY, 0, -_FAR_FAR_AWAY));
        //printf("Switch to tree 4\n");
      }
      else if (is_key_going_down('5') && !is_key_down(key_ctrl)){
        lsystem_nodes[cur_lsystem]->translate(vec3(_FAR_FAR_AWAY, 0, _FAR_FAR_AWAY));
        cur_lsystem = 5;
        lsystem_nodes[cur_lsystem]->translate(vec3(-_FAR_FAR_AWAY, 0, -_FAR_FAR_AWAY));
        //printf("Switch to tree 5\n");
      }
      else if (is_key_going_down('6') && !is_key_down(key_ctrl)){
        lsystem_nodes[cur_lsystem]->translate(vec3(_FAR_FAR_AWAY, 0, _FAR_FAR_AWAY));
        cur_lsystem = 6;
        lsystem_nodes[cur_lsystem]->translate(vec3(-_FAR_FAR_AWAY, 0, -_FAR_FAR_AWAY));
        //printf("Switch to tree 6\n");
      }
      else if (is_key_going_down('7') && !is_key_down(key_ctrl)){
        lsystem_nodes[cur_lsystem]->translate(vec3(_FAR_FAR_AWAY, 0, _FAR_FAR_AWAY));
        cur_lsystem = 7;
        lsystem_nodes[cur_lsystem]->translate(vec3(-_FAR_FAR_AWAY, 0, -_FAR_FAR_AWAY));
        //printf("Switch to tree 7\n");
      }
      else if (is_key_going_down('8') && !is_key_down(key_ctrl)){
        lsystem_nodes[cur_lsystem]->translate(vec3(_FAR_FAR_AWAY, 0, _FAR_FAR_AWAY));
        cur_lsystem = 8;
        lsystem_nodes[cur_lsystem]->translate(vec3(-_FAR_FAR_AWAY, 0, -_FAR_FAR_AWAY));
        //printf("Switch to tree 8\n");
      }
      else if (is_key_going_down('9') && !is_key_down(key_ctrl)){
        lsystem_nodes[cur_lsystem]->translate(vec3(_FAR_FAR_AWAY, 0, _FAR_FAR_AWAY));
        cur_lsystem = 9;
        lsystem_nodes[cur_lsystem]->translate(vec3(-_FAR_FAR_AWAY, 0, -_FAR_FAR_AWAY));
        //printf("Switch to tree 9\n");
      }
      else if (is_key_going_down('0') && !is_key_down(key_ctrl)){
        lsystem_nodes[cur_lsystem]->translate(vec3(_FAR_FAR_AWAY, 0, _FAR_FAR_AWAY));
        cur_lsystem = 0;
        lsystem_nodes[cur_lsystem]->translate(vec3(-_FAR_FAR_AWAY, 0, -_FAR_FAR_AWAY));
        //printf("Switch to initial tree\n");
      }
      else if (is_key_going_down('1') && is_key_down(key_ctrl)){
        lsystem_meshes[cur_lsystem]->switch_leafs();
      }
      else if (is_key_down('2') && is_key_down(key_ctrl)){
        lsystem_meshes[cur_lsystem]->modify_radius_random_factor(-0.05f);
      }
      else if (is_key_down('3') && is_key_down(key_ctrl)){
        lsystem_meshes[cur_lsystem]->modify_radius_random_factor(0.05f);
      }
      else if (is_key_down('4') && is_key_down(key_ctrl)){
        lsystem_meshes[cur_lsystem]->modify_distance_random_factor(-0.05f);
      }
      else if (is_key_down('5') && is_key_down(key_ctrl)){
        lsystem_meshes[cur_lsystem]->modify_distance_random_factor(0.05f);
      }
      else if (is_key_down('6') && is_key_down(key_ctrl)){
        lsystem_meshes[cur_lsystem]->modify_angle_random_factor(-0.05f);
      }
      else if (is_key_down('7') && is_key_down(key_ctrl)){
        lsystem_meshes[cur_lsystem]->modify_angle_random_factor(0.05f);
      }
      //O next step
      else if (is_key_going_down('O')){
        lsystems[cur_lsystem]->next();
        int cur_iteration = lsystems[cur_lsystem]->get_iteration();
        printf("Changing tree %i to iteration %i.\n", cur_lsystem, cur_iteration);
        lsystem_meshes[cur_lsystem]->set_iteration(cur_iteration);
        lsystem_meshes[cur_lsystem]->input_word(cur_iteration, lsystems[cur_lsystem]->get_word(), lsystems[cur_lsystem]->get_size_word());
        lsystem_meshes[cur_lsystem]->update_generation();
      }
      //L previous step
      else if (is_key_going_down('L')){
        lsystems[cur_lsystem]->previous();
        int cur_iteration = lsystems[cur_lsystem]->get_iteration();
        printf("Changing tree %i to iteration %i.\n", cur_lsystem, cur_iteration);
        lsystem_meshes[cur_lsystem]->set_iteration(cur_iteration);
        lsystem_meshes[cur_lsystem]->input_word(cur_iteration, lsystems[cur_lsystem]->get_word(), lsystems[cur_lsystem]->get_size_word());
        lsystem_meshes[cur_lsystem]->update_generation();
      }
      //P print current word
      else if (is_key_going_down('P')){
        lsystems[cur_lsystem]->print();
      }
      //WSAD control camera
      else if (is_key_down('W') && !is_key_down(key_ctrl)){
        app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().translate(0, 2.5, 0);
      }
      else if (is_key_down('S') && !is_key_down(key_ctrl)){
        app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().translate(0, -2.5, 0);
      }
      else if (is_key_down('A') && !is_key_down(key_ctrl)){
        app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().translate(-2.5, 0, 0);
      }
      else if (is_key_down('D') && !is_key_down(key_ctrl)){
        app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().translate(2.5, 0, 0);
      }
      else if (is_key_down('Q') && !is_key_down(key_ctrl)){
        app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().translate(0, 0, -2.5);
      }
      else if (is_key_down('E') && !is_key_down(key_ctrl)){
        app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().translate(0, 0, 2.5);
      }
      //AWSD + ctrl to control tree (rotate and up-down)
      else if (is_key_down('W') && is_key_down(key_ctrl)){
        lsystem_nodes[cur_lsystem]->translate(vec3(0, 1, 0));
      }
      else if (is_key_down('S') && is_key_down(key_ctrl)){
        lsystem_nodes[cur_lsystem]->translate(vec3(0, -1, 0));
      }
      else if (is_key_down('A') && is_key_down(key_ctrl)){
        lsystem_nodes[cur_lsystem]->rotate(10, vec3(0, 1, 0));
      }
      else if (is_key_down('D') && is_key_down(key_ctrl)){
        lsystem_nodes[cur_lsystem]->rotate(-10, vec3(0, 1, 0));
      }
      //Q + ctrl togles on-off the ground
      else if (is_key_going_down('Q') && is_key_down(key_ctrl)){
        if (floor_on)
          floor_node->translate(vec3(_FAR_FAR_AWAY, _FAR_FAR_AWAY, _FAR_FAR_AWAY));
        else
          floor_node->translate(vec3(-_FAR_FAR_AWAY, -_FAR_FAR_AWAY, -_FAR_FAR_AWAY));
        floor_on = !floor_on;
      }
      //E + ctrl togles the type of visualization
      else if (is_key_going_down('E') && is_key_down(key_ctrl)){
        lsystem_meshes[cur_lsystem]->switch3d();
      }
      //ZX controles the radius
      else if (is_key_down('Z') && !is_key_down(key_ctrl)){
        lsystem_meshes[cur_lsystem]->modify_radius(-0.05);
      }
      else if (is_key_down('X') && !is_key_down(key_ctrl)){
        lsystem_meshes[cur_lsystem]->modify_radius(0.05);
      }
      else if (is_key_down('Z') && is_key_down(key_ctrl)){
        lsystem_meshes[cur_lsystem]->modify_radius(-0.1);
      }
      else if (is_key_down('X') && is_key_down(key_ctrl)){
        lsystem_meshes[cur_lsystem]->modify_radius(0.1);
      }
      //CV controls the angles
      else if (is_key_down('C') && !is_key_down(key_ctrl)){
        lsystem_meshes[cur_lsystem]->decrease_angle();
      }
      else if (is_key_down('V') && !is_key_down(key_ctrl)){
        lsystem_meshes[cur_lsystem]->increase_angle();
      }
      else if (is_key_down('C') && is_key_down(key_ctrl)){
        lsystem_meshes[cur_lsystem]->decrease_angleY();
      }
      else if (is_key_down('V') && is_key_down(key_ctrl)){
        lsystem_meshes[cur_lsystem]->increase_angleY();
      }
      //BN controls the reduction of the radius
      else if (is_key_down('B') && !is_key_down(key_ctrl)){
        lsystem_meshes[cur_lsystem]->decrease_reduction();
      }
      else if (is_key_down('N') && !is_key_down(key_ctrl)){
        lsystem_meshes[cur_lsystem]->increase_reduction();
      }
      else if (is_key_going_down('N') && is_key_down(key_ctrl)){
        lsystem_meshes[cur_lsystem]->switchReduction();
      }
      //MJ controls the length of destiny
      else if (is_key_down('M')){
        lsystem_meshes[cur_lsystem]->decrease_distance();
      }
      else if (is_key_down('J')){
        lsystem_meshes[cur_lsystem]->increase_distance();
      }
      else if (is_key_going_down('R') && !is_key_down(key_ctrl)){
        lsystem_meshes[cur_lsystem]->reset();
      }
      else if (is_key_down('F') && !is_key_down(key_ctrl)){
        lsystem_meshes[cur_lsystem]->set_leaf_mode(-1);
      }
      else if (is_key_down('G') && !is_key_down(key_ctrl)){
        lsystem_meshes[cur_lsystem]->set_leaf_mode(0);
      }
      else if (is_key_down('H') && !is_key_down(key_ctrl)){
        lsystem_meshes[cur_lsystem]->set_leaf_mode(3);
      }
      else if (is_key_down('G') && is_key_down(key_ctrl)){
        lsystem_meshes[cur_lsystem]->set_leaf_mode(2);
      }
      else if (is_key_down('H') && is_key_down(key_ctrl)){
        lsystem_meshes[cur_lsystem]->set_leaf_mode(1);
      }
      else if (is_key_going_down('R') && is_key_down(key_ctrl)){
        lsystem_meshes[cur_lsystem]->switch_radius_random();
      }
      else if (is_key_going_down('T') && is_key_down(key_ctrl)){
        lsystem_meshes[cur_lsystem]->switch_distance_random();
      }
      else if (is_key_going_down('Y') && is_key_down(key_ctrl)){
        lsystem_meshes[cur_lsystem]->switch_angle_random();
      }
      else if (is_key_going_down(key_space) && !is_key_down(key_ctrl)){
        lsystem_meshes[cur_lsystem]->update_generation();
      }
      else if (is_key_going_down(key_space) && is_key_down(key_ctrl)){
        int cur_iteration = lsystems[cur_lsystem]->get_iteration();
        lsystems[cur_lsystem]->recalculate();
        lsystems[cur_lsystem]->go_to(cur_iteration);
        lsystem_meshes[cur_lsystem]->set_iteration(cur_iteration);
        lsystem_meshes[cur_lsystem]->input_word(cur_iteration, lsystems[cur_lsystem]->get_word(), lsystems[cur_lsystem]->get_size_word());
        lsystem_meshes[cur_lsystem]->update_generation();
      }
      else if (is_key_down(key_down)){
        lsystem_meshes[cur_lsystem]->modify_wind(1, -0.001f);
      }
      else if (is_key_down(key_up)){
        lsystem_meshes[cur_lsystem]->modify_wind(1, 0.001f);
      }
      else if (is_key_down(key_left) && !is_key_down(key_ctrl)){
        lsystem_meshes[cur_lsystem]->modify_wind(2, -0.001f);
      }
      else if (is_key_down(key_right) && !is_key_down(key_ctrl)){
        lsystem_meshes[cur_lsystem]->modify_wind(2, 0.001f);
      }
      else if (is_key_down(key_left) && is_key_down(key_ctrl)){
        lsystem_meshes[cur_lsystem]->modify_wind(0, -0.001f);
      }
      else if (is_key_down(key_right) && is_key_down(key_ctrl)){
        lsystem_meshes[cur_lsystem]->modify_wind(0, 0.001f);
      }
    }

    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      app_scene->begin_render(vx, vy, vec4(0,0,0,0));

      //Move the camera with the mouse
      camera.update(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent());
      // update matrices. assume 30 fps.
      app_scene->update(1.0f/30);
      
      // draw the scene
      app_scene->render((float)vx / vy);

      //Ask for inputs of the keyboard
      keyboard();
    }
  };
}
