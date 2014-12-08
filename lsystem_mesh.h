
#ifndef LSYSTEM_MESH_INCLUDED
#define LSYSTEM_MESH_INCLUDED

namespace octet{
  namespace scene{
    enum status_ls{_NONE = -1, _TO_UPDATE = 0, _GENERATED = 1, _TO_GENERATE = 2};
    enum { PRECISION_CYLINDER = 30, RED_INIT = 1 };
    class lsystem_mesh : public mesh{

      struct my_vertex{
        vec3p pos;
        uint32_t color;
      };

      // this function converts three floats into a RGBA 8 bit color
      static uint32_t make_color(float r, float g, float b) {
        return 0xff000000 + ((int)(r*255.0f) << 0) + ((int)(g*255.0f) << 8) + ((int)(b*255.0f) << 16);
      }

      ///Each one of the elements to be drawed with the lsystem
      struct Block : public resource{
        mat4t pos;
        mat4t transform;
        float radio;
        float radio2;
        float depth;
      };

      void printf_mat4t(mat4t & mat){
        float *temp = mat.get();
        for (unsigned i = 0; i != 4; ++i){
          for (unsigned j = 0; j != 4; ++j){
            printf("%f ", temp[i*4+j]);
          }
          printf("\n");
        }
      }
      
      ///info for each one of the lsystem generated
      dynarray<dynarray<ref<Block>>> blocks;
      dynarray<my_vertex> leaves;
      dynarray<int> num_symbols;
      dynarray<int> num_leaves;
      dynarray<char*> words;
      dynarray<int> size_words;
      dynarray<status_ls> ls_generated;
      float angle_X;
      float angle_Y;
      float distance;
      float r_init;
      float precision;
      float r_reduction;
      float default_angle;
      float default_distance;
      int cur_iteration;
      float max_depth;
      bool visualize3D;
      ///stack for the turtle position when generating blocks
      dynarray<ref<Block>> stack3d;

    public:
      lsystem_mesh(){
        blocks.reserve(10);
        blocks.resize(10);
        num_leaves.reserve(20);
        num_symbols.reserve(20);
        for (int i = 0; i < 20; ++i){
          ls_generated.push_back(_NONE);
          num_symbols.push_back(0);
          num_leaves.push_back(0);
        }
        visualize3D = false;
        cur_iteration = 0;
        angle_Y = 80;
        default_angle = 0;
        angle_X = 0;
        distance = 5;
        default_distance = 5;
        r_init = 0.1;
        r_reduction = RED_INIT;
        precision = PRECISION_CYLINDER;
      }

      void reset(){
        angle_X = default_angle;
        distance = default_distance;
        r_init = 0.1;
        r_reduction = RED_INIT;
        ls_generated[cur_iteration] = _NONE;
        generate_iteration(cur_iteration);
      }

      void increase_radius_strong(){
        r_init += 0.5;
        ls_generated[cur_iteration] = _NONE;
        generate_iteration(cur_iteration);
      }

      void decrease_radius_strong(){
        r_init -= 0.5;
        ls_generated[cur_iteration] = _NONE;
        generate_iteration(cur_iteration);
      }

      void increase_radius(){
        r_init += 0.1;
        ls_generated[cur_iteration] = _NONE;
        generate_iteration(cur_iteration);
      }

      void decrease_radius(){
        r_init -= 0.1;
        ls_generated[cur_iteration] = _NONE;
        generate_iteration(cur_iteration);
      }

      void increase_reduction(){
        r_reduction -= 0.001;
        ls_generated[cur_iteration] = _NONE;
        generate_iteration(cur_iteration);
      }

      void decrease_reduction(){
        r_reduction += 0.001;
        ls_generated[cur_iteration] = _NONE;
        generate_iteration(cur_iteration);
      }

      void increase_angle(){
        angle_X += 0.5;
        ls_generated[cur_iteration] = _NONE;
        generate_iteration(cur_iteration);
      }

      void decrease_angle(){
        angle_X -= 0.5;
        ls_generated[cur_iteration] = _NONE;
        generate_iteration(cur_iteration);
      }

      void increase_angleY(){
        angle_Y += 0.5;
        ls_generated[cur_iteration] = _NONE;
        generate_iteration(cur_iteration);
      }

      void decrease_angleY(){
        angle_Y -= 0.5;
        ls_generated[cur_iteration] = _NONE;
        generate_iteration(cur_iteration);
      }

      void increase_distance(){
        distance += 0.2;
        ls_generated[cur_iteration] = _NONE;
        generate_iteration(cur_iteration);
      }

      void decrease_distance(){
        distance -= 0.2;
        ls_generated[cur_iteration] = _NONE;
        generate_iteration(cur_iteration);
      }

      void init_values(float angle_, float distance_){
        angle_X = angle_;
        angle_Y = angle_X;
        distance = distance_;
        default_angle = angle_;
        default_distance = distance_;
      }

      void update_angle(float angle_){
        angle_X = angle_;
        angle_Y = angle_X;
        default_angle = angle_;
        //update positions
        ls_generated[cur_iteration] = _TO_UPDATE;
      }

      void update_distance(float distance_){
        distance = distance_;
        default_distance = distance_;
        //update positions
        ls_generated[cur_iteration] = _TO_UPDATE;
      }

      void update_values(float angle_, float distance_){
        init_values(angle_, distance_);
        //update positions
      }

      void set_iteration(int cur_iteration_){/*
        cur_iteration = cur_iteration_;
        if (ls_generated[cur_iteration] != _NONE)
          ls_generated[cur_iteration] = _TO_UPDATE;*/
      }

      void generate_iteration(int iteration){
        //check if it has been already generated
        //printf("Inputing new word\n");
        cur_iteration = iteration;
        if (ls_generated[iteration] == _GENERATED){ //already generated
          //printf("Nothing to generate here. But it may be needed to update!\n");
        }
        else if (ls_generated[iteration] == _NONE){ //generate new set of blocks
          //printf("Starting generation of word");
          ls_generated[iteration] = _TO_UPDATE;
          num_symbols[iteration] = 0;
          //Check size of l_system
          for (int i = 0; i < size_words[iteration]; ++i){
            if (words[iteration][i] != '[' && words[iteration][i] != ']' && words[iteration][i] != '+' && words[iteration][i] != '-')
              ++num_symbols[iteration];
            else if (words[iteration][i] == ']')
              ++num_leaves[iteration];
          }

          //Set up the generation
          if (blocks.size() < (unsigned)iteration){
            blocks.resize(iteration + 1);
          }
          blocks[iteration].reset();

          blocks[iteration].reserve(num_symbols[iteration]);
          //Generate initial stack for turtle3D
          Block * back_stack = new Block();
          back_stack->pos.loadIdentity();
          back_stack->radio = r_init;
          back_stack->transform.loadIdentity();
          back_stack->transform.translate(0, distance, 0);
          back_stack->depth = 0;
          stack3d.push_back(back_stack);
          Block *new_block;
          Block *last_block;
          my_vertex *new_leaf;
          //Start generation
          for (int i = 0; i < size_words[iteration]; ++i){
            mat4t aux_matrix;
            vec4 aux_vector;
            char symbol = words[iteration][i];
            //printf("\n%c:\n", symbol);
            switch (symbol){
            case 'F':
            case 'A':
              //Reserve new block
              new_block = new Block();
              //Fill new block
              last_block = new_block;
              new_block->pos = back_stack->pos;
              new_block->transform = back_stack->pos;
              new_block->transform.multMatrix(back_stack->transform);
              new_block->radio = back_stack->radio;
              new_block->radio2 = back_stack->radio * r_reduction;
              new_block->depth = back_stack->depth;
              //Add new block
              blocks[iteration].push_back(new_block);
              //Update turtle3d
              back_stack->pos = new_block->transform;
              aux_vector = back_stack->transform.row(3);
              back_stack->transform.loadIdentity();
              back_stack->radio = new_block->radio2;
              back_stack->depth += 1;
              back_stack->transform.translate(aux_vector.xyz().normalize() * distance);
              //Debuging things
              /*  printf("New block pos:\n");
              printf_mat4t(new_block->pos);
              printf("New block transform:\n");
              printf_mat4t(new_block->transform);
              printf("Stack pos:\n");
              printf_mat4t(back_stack->pos);
              printf("Stack transform:\n");
              printf_mat4t(back_stack->transform);*/
              break;
            case '[':
              //Reserve new block
              back_stack = new Block();
              //Copy the block
              back_stack->pos = stack3d.back()->pos;
              back_stack->transform = stack3d.back()->transform;
              back_stack->radio = stack3d.back()->radio;
              back_stack->depth = stack3d.back()->depth;
              //Add to stack of 3dTurtle
              stack3d.push_back(back_stack);
              back_stack = stack3d.back();
              break;
            case ']': //If it's popping add a leaf
              new_leaf = new my_vertex();
              new_leaf->pos = stack3d.back()->pos.row(3).xyz();
              last_block->radio2 = 0;
              stack3d.pop_back();
              back_stack = stack3d.back();
              break;
            case '+':
              aux_matrix.loadIdentity();
              aux_matrix.rotateX(angle_X);
              back_stack->transform.multMatrix(aux_matrix);
              break;
            case '-':
              aux_matrix.loadIdentity();
              aux_matrix.rotateX(-angle_X);
              back_stack->transform.multMatrix(aux_matrix);
              break;
            case '<':
              aux_matrix.loadIdentity();
              aux_matrix.rotateY(angle_Y);
              back_stack->transform.multMatrix(aux_matrix);
              break;
            case '>':
              aux_matrix.loadIdentity();
              aux_matrix.rotateY(-angle_Y);
              back_stack->transform.multMatrix(aux_matrix);
              break;
            }
          }//End for generation of blocks
          //printf("Generated %i new blocks for tree\n", blocks[iteration].size());
        }//End else generate new set of blocks

        generate();
      }

      ///It will input and generate the word as an l_system. It will check previously if it has been already generated
      void input_word(int iteration, char *word, int size_word){
        if (size_words.size() < (unsigned)iteration){
          int temp_size = size_words.size();
          size_words.resize(iteration + 2);
          words.resize(iteration + 2);
          for (int i = temp_size; i <= iteration + 2; ++i){
            size_words[i] = 0;
            words[i] = '\0';
          }
        }
        words[iteration] = word;
        size_words[iteration] = size_word;

        generate_iteration(iteration);
      }

      void generate(){
        //If the tree is not up to date
        if (true){
          //printf("REGENERATE iteration %i!\n", cur_iteration);
          ls_generated[cur_iteration] = _GENERATED;

          //Reserve space for the vertexes and indices
          size_t num_vertexes = PRECISION_CYLINDER * 2 * num_symbols[cur_iteration];
          size_t num_indices = PRECISION_CYLINDER * 6 * num_symbols[cur_iteration];
          allocate(sizeof(my_vertex) * num_vertexes, sizeof(uint32_t) * num_indices);
          set_params(sizeof(my_vertex), num_indices, num_vertexes, GL_TRIANGLES, GL_UNSIGNED_INT);
          if (get_num_slots() != 2){
            add_attribute(attribute_pos, 3, GL_FLOAT, 0);
            add_attribute(attribute_color, 4, GL_UNSIGNED_BYTE, 12, GL_TRUE);
          }
          
          gl_resource::wolock vlock(get_vertices());
          my_vertex *vtx = (my_vertex*)vlock.u8();
          gl_resource::wolock ilock(get_indices());
          uint32_t *idx = ilock.u32();
          unsigned nv = 0;
          unsigned ni = 0;
          vertex circle[PRECISION_CYLINDER];
          float angle_slice = 2 * 3.141592653f / PRECISION_CYLINDER;
          
          //Build the initial values of the cylinder without radius
          for (unsigned i = 0; i != PRECISION_CYLINDER; ++i){
            circle[i].pos = vec3(sin(i*angle_slice), 0, cos(i*angle_slice));
            circle[i].uv = vec2(0, 1);
          }

          //Draw circles in given positions
          for (unsigned i = 0; i != blocks[cur_iteration].size(); ++i){
            Block *block_ = blocks[cur_iteration][i];
            float * mat_1 = block_->pos.get();
            float * mat_2 = block_->transform.get();
            //Get the bottom and top position of the block and it's orientation
            vec3 pos_1 = vec3(mat_1[3 * 4], mat_1[3 * 4 + 1], mat_1[3 * 4 + 2]);
            vec3 pos_2 = vec3(mat_2[3 * 4], mat_2[3 * 4 + 1], mat_2[3 * 4 + 2]);
            for (unsigned j = 0; j != PRECISION_CYLINDER; ++j){
              //Obtain the rotated circle with that orientation
              float r = 0.4f + (0.4f * i / blocks[cur_iteration].size());
              float g = 0.4f + (0.5f * i / blocks[cur_iteration].size());
              float b = 0.3f + (0.2f * i / blocks[cur_iteration].size());
              vec3 pos_c = circle[j].pos*block_->radio; //Rotate with orientation
              vec3 pos_c2 = circle[j].pos*block_->radio2; //Rotate with orientation
              //Obtain both sides of the cylinder
              vtx->pos = pos_1 + pos_c;
              vtx->color = make_color(r, g, b);
              float t1, t2, t3;
              t1 = (pos_1.get()[0] + pos_c.get()[0]);
              t2 = (pos_1.get()[1] + pos_c.get()[1]);
              t3 = (pos_1.get()[2] + pos_c.get()[2]);
              vtx++;
              vtx->pos = pos_2 + pos_c2;
              vtx->color = make_color(r, g, b);
              t1 = (pos_2.get()[0] + pos_c2.get()[0]);
              t2 = (pos_2.get()[1] + pos_c2.get()[1]);
              t3 = (pos_2.get()[2] + pos_c2.get()[2]);
              vtx++;
              idx[0] = nv; idx[1] = nv + 1; idx[2] = nv + 3;
              idx[3] = nv; idx[4] = nv + 3; idx[5] = nv + 2;
              idx += 6;
              nv += 2;
            }
            //Fix last 3 of the last 6 indices to complete the cylinder
            idx -= 6;
            idx[2] = nv - 2 * PRECISION_CYLINDER + 1;
            idx[4] = nv - 2 * PRECISION_CYLINDER + 1; 
            idx[5] = nv - 2 * PRECISION_CYLINDER;
            idx += 6;
          }
        }
        
        //printf("Indices %d\n", get_num_indices());
      }


      /// Serialise
      void visit(visitor &v) {
        mesh::visit(v);
      }
    };
  }
}

#endif