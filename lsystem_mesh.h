
#ifndef LSYSTEM_MESH_INCLUDED
#define LSYSTEM_MESH_INCLUDED

namespace octet{
  namespace scene{
    enum status_ls{_NONE = -1, _TO_UPDATE = 0, _GENERATED = 1, _TO_GENERATE = 2};
    enum type_leaf{ _FLAT = -1, _POINTY = 0, _MESH = 1, _RANDOM = 2, _HUGE = 3 };
    enum { PRECISION_CYLINDER = 5, RED_INIT = 1 };
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
        bool is_leaf;
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
      
      random rand;
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
      float angle_random_factor;
      float radius_random_factor;
      float distance_random_factor;
      type_leaf leaf_mode;
      int cur_iteration;
      float max_depth;
      bool visualize3D;
      bool reduction_toggle;
      bool distance_random;
      bool angle_random;
      bool radius_random;
      bool ignore_leafs;
      ///stack for the turtle position when generating blocks
      dynarray<ref<Block>> stack3d;

      float lerp(float v1, float v2, float t, float max_t){
        return v1 + (v2 - v1)*(t / max_t);
      }

      float lerp(float v1, float v2, int t, int max_t){
        float t_ = t;
        float max_t_ = max_t;
        return lerp(v1, v2, t_, max_t_);
      }

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
        leaf_mode = _FLAT;
        visualize3D = false;
        reduction_toggle = true;
        distance_random = false;
        angle_random = false;
        radius_random = false;
        distance_random_factor = 0.2f;
        angle_random_factor = 0.2f;
        radius_random_factor = 0.2f;
        ignore_leafs = true;
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

      ///This function set the type of the leaf
      void set_leaf_mode(int i){
        leaf_mode = (type_leaf) i;
        ls_generated[cur_iteration] = _NONE;
        generate_iteration(cur_iteration);
      }

      ///This function reset all the L_system angle, distance, reduction (inputed from keyboard)
      void reset(){
        angle_X = default_angle;
        distance = default_distance;
        r_init = 0.1;
        r_reduction = RED_INIT;
        distance_random = false;
        radius_random = false;
        angle_random = false;
        distance_random_factor = 0.2f;
        angle_random_factor = 0.2f;
        radius_random_factor = 0.2f;
        ls_generated[cur_iteration] = _NONE;
        generate_iteration(cur_iteration);
      }

      void update_generation(){
        ls_generated[cur_iteration] = _NONE;
        generate_iteration(cur_iteration);
      }

      void switch_leafs(){
        ignore_leafs = !ignore_leafs;
        update_generation();
      }

      ///Enable/disable radius random mode
      void switch_radius_random(){
        radius_random = !radius_random;
        update_generation();
      }

      ///Enable/disable distance random mode
      void switch_distance_random(){
        distance_random = !distance_random;
        update_generation();
      }

      ///Enable/disable angle random mode
      void switch_angle_random(){
        angle_random = !angle_random;
        update_generation();
      }

      ///This function switchs the type of visualization 
      void switch3d(){
        visualize3D = !visualize3D;
        ls_generated[cur_iteration] = _NONE;
        generate_iteration(cur_iteration);
      }

      ///This function specifies the way to apply the reduction like a cone or individually
      void switchReduction(){
        reduction_toggle = !reduction_toggle;
        update_generation();
      }

      ///This function is to apply a modifier to the radius
      void modify_radius(float value){
        r_init += value;
        if (r_init < 0.05) r_init = 0.05;
        update_generation();
      }

      ///This function is to apply a modifier to the radius
      void modify_radius_random_factor(float value){
        radius_random_factor += value;
        if (radius_random_factor < 0) radius_random_factor = 0;
        update_generation();
      }

      ///This function is to apply a modifier to the radius
      void modify_distance_random_factor(float value){
        distance_random_factor += value;
        if (distance_random_factor < 0) distance_random_factor = 0;
        update_generation();
      }

      ///This function is to apply a modifier to the radius
      void modify_angle_random_factor(float value){
        angle_random_factor += value;
        if (angle_random_factor < 0) angle_random_factor = 0;
        update_generation();
      }

      void increase_reduction(){
        r_reduction -= 0.001;
        update_generation();
      }

      void decrease_reduction(){
        r_reduction += 0.001;
        update_generation();
      }

      void increase_angle(){
        angle_X += 0.5;
        update_generation();
      }

      void decrease_angle(){
        angle_X -= 0.5;
        if (angle_X < 0) angle_X = 0;
        update_generation();
      }

      void increase_angleY(){
        angle_Y += 0.5;
        update_generation();
      }

      void decrease_angleY(){
        angle_Y -= 0.5;
        if (angle_Y < 0) angle_Y = 0;
        update_generation();
      }

      void increase_distance(){
        distance += 0.2;
        update_generation();
      }

      void decrease_distance(){
        distance -= 0.2;
        distance = distance < 0.1 ? 0.1 : distance;
        update_generation();
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

        //Set up the generation
        if (blocks.size() <= (unsigned)iteration){
          blocks.resize(iteration + 1);
          ls_generated.resize(iteration + 1);
          num_symbols.resize(iteration + 1);
          num_leaves.resize(iteration + 1);
        }
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
          blocks[iteration].reset();

          blocks[iteration].reserve(num_symbols[iteration]);
          //Generate initial stack for turtle3D
          Block * back_stack = new Block();
          back_stack->pos.loadIdentity();
          back_stack->radio = r_init;
          back_stack->transform.loadIdentity();
          back_stack->transform.translate(0, distance, 0);
          back_stack->depth = 0;
          max_depth = 0;
          stack3d.push_back(back_stack);
          Block *new_block;
          Block *last_block;
          my_vertex *new_leaf;
          //Start generation
          for (int i = 0; i < size_words[iteration]; ++i){
            vec3 translation = vec3(0, distance* (distance_random ? rand.get(1-distance_random_factor, 1+distance_random_factor) : 1), 0);
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
              new_block->is_leaf = false;
              new_block->pos = back_stack->pos;
              new_block->transform = back_stack->pos;
              new_block->transform.translate(translation);
              new_block->radio = back_stack->radio * (radius_random ? rand.get(1-radius_random_factor, 1+radius_random_factor) : 1);
              if (reduction_toggle)
                new_block->radio2 = back_stack->radio * r_reduction;
              else
                new_block->radio2 = back_stack->radio;
              new_block->depth = back_stack->depth;
              //Add new block
              blocks[iteration].push_back(new_block);
              //Update turtle3d
              back_stack->pos = new_block->transform;
              back_stack->radio = new_block->radio2;
              back_stack->depth += 1;
              max_depth = (back_stack->depth < max_depth)? max_depth : back_stack->depth;
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
              //leaves.push_back(new_leaf);
              last_block->is_leaf = true;
              if (leaf_mode == _POINTY)
                last_block->radio2 = 0;
              else if (leaf_mode == _HUGE)
                last_block->radio2 = last_block->radio * 3;
              stack3d.pop_back();
              back_stack = stack3d.back();
              break;
            case '+':
              back_stack->pos.rotateX(angle_X * (angle_random ? rand.get(1-angle_random_factor, 1+angle_random_factor) : 1));
              break;
            case '-':
              back_stack->pos.rotateX(-angle_X * (angle_random ? rand.get(1-angle_random_factor, 1+angle_random_factor) : 1));
              break;
            case '<':
              back_stack->pos.rotateY(angle_Y * (angle_random ? rand.get(1-angle_random_factor, 1+angle_random_factor) : 1));
              break;
            case '>':
              back_stack->pos.rotateY(-angle_Y * (angle_random ? rand.get(1-angle_random_factor, 1+angle_random_factor) : 1));
              break;
            }
          }//End for generation of blocks
          last_block->is_leaf = true;
          if (leaf_mode == _POINTY)
            last_block->radio2 = 0;
          else if (leaf_mode == _HUGE)
            last_block->radio2 = last_block->radio * 3;
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
          size_t num_vertexes = PRECISION_CYLINDER * 2 * blocks[cur_iteration].size();
          size_t num_indices = PRECISION_CYLINDER * 6 * blocks[cur_iteration].size();
          allocate(sizeof(my_vertex) * num_vertexes, sizeof(uint32_t) * num_indices);
          if (visualize3D)
            set_params(sizeof(my_vertex), num_indices, num_vertexes, GL_POINTS, GL_UNSIGNED_INT);
          else
            set_params(sizeof(my_vertex), num_indices, num_vertexes, GL_TRIANGLES, GL_UNSIGNED_INT);
          
          clear_attributes();
          add_attribute(attribute_pos, 3, GL_FLOAT, 0);
          add_attribute(attribute_color, 4, GL_UNSIGNED_BYTE, 12, GL_TRUE);

          gl_resource::wolock vlock(get_vertices());
          my_vertex *vtx = (my_vertex*)vlock.u8();
          gl_resource::wolock ilock(get_indices());
          uint32_t *idx = ilock.u32();
          unsigned nv = 0;
          unsigned ni = 0;
          vec4 circle[PRECISION_CYLINDER];
          float angle_slice = 2 * 3.141592653f / PRECISION_CYLINDER;
          
          //Build the initial values of the cylinder without radius
          for (unsigned i = 0; i != PRECISION_CYLINDER; ++i){
            circle[i]= vec4(cos(i*angle_slice), 0, sin(i*angle_slice), 1);
          }

          //Draw circles in given positions
          for (unsigned i = 0; i != blocks[cur_iteration].size(); ++i){
            Block *block_ = blocks[cur_iteration][i];
            if (ignore_leafs)
              block_->is_leaf = false;
            float * mat_1 = block_->pos.get();
            float * mat_2 = block_->transform.get();
            //Get the bottom and top position of the block and it's orientation
            vec4 pos_1 = vec4(mat_1[3 * 4], mat_1[3 * 4 + 1], mat_1[3 * 4 + 2],1);
            vec4 pos_2 = vec4(mat_2[3 * 4], mat_2[3 * 4 + 1], mat_2[3 * 4 + 2], 1);
            block_->pos[3] = vec4(0, 0, 0, 1);
            block_->transform[3] = vec4(0, 0, 0, 1);
            block_->pos = block_->pos.transpose4x4();
            block_->transform = block_->transform.transpose4x4();
            for (unsigned j = 0; j != PRECISION_CYLINDER; ++j){
              //Obtain the rotated circle with that orientation
              float r = 0.4f + (0.4f * i / blocks[cur_iteration].size());
              float g = 0.0f + (0.5f * i / blocks[cur_iteration].size());
              float b = 0.0f + (0.2f * i / blocks[cur_iteration].size());
              vec4 pos_c = circle[j]*block_->radio; //Rotate with orientation
              vec4 pos_c2 = circle[j]*block_->radio2 * (!reduction_toggle ? r_reduction*r_reduction : 1); //Rotate with orientation
              pos_c = block_->pos.rmul(pos_c);
              pos_c2 = block_->transform.rmul(pos_c2);
              //Obtain both sides of the cylinder
              vtx->pos = pos_1.xyz() + pos_c.xyz();
              if (block_->is_leaf)
                vtx->color = make_color(0.1f, 1.0f, 0.1f);
              else if (!reduction_toggle)
                vtx->color = make_color(r + 0.1f, g + 0.1f, b + 0.1f);
              else
                vtx->color = make_color(r, g, b);
              vtx++;
              vtx->pos = pos_2.xyz() + pos_c2.xyz();
              if (block_->is_leaf)
                vtx->color = make_color(0.1f, 8.1f, 0.1f);
              else
                vtx->color = make_color(r, g, b);
              vtx++;
              idx[0] = nv; idx[1] = nv + 1; idx[2] = nv + 3;
              idx[3] = nv; idx[4] = nv + 3; idx[5] = nv + 2;
              idx += 6;
              nv += 2;
              ni += 6;
            }
            //Fix last 3 of the last 6 indices to complete the cylinder
            idx -= 6;
            idx[2] = nv - 2 * PRECISION_CYLINDER + 1;
            idx[4] = nv - 2 * PRECISION_CYLINDER + 1; 
            idx[5] = nv - 2 * PRECISION_CYLINDER;
            idx += 6;
          }
         
        }
      }


      /// Serialise
      void visit(visitor &v) {
        mesh::visit(v);
      }
    };
  }
}

#endif