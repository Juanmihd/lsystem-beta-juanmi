
#ifndef LSYSTEM_MESH_INCLUDED
#define LSYSTEM_MESH_INCLUDED

namespace octet{
  namespace scene{
    enum status_ls{_NONE = -1, _TO_UPDATE = 0, _GENERATED = 1};
    class lsystem_mesh : public mesh{
      ///Each one of the elements to be drawed with the lsystem
      struct block : public resource{
        vec3 origin;
        vec3 next;
        float angle_X;
        float angle_Y;
        float distance;
        int precision;
        float r1;
        float r2;
      };

      struct turtle3D : public resource{
        vec3 position;
        mat4t transform;
      };

      ///info for each one of the lsystem generated
      dynarray<dynarray<ref<block>>> blocks;
      float angle_X;
      float angle_Y;
      float distance;
      float r_init;
      dynarray<status_ls> ls_generated;
      int cur_iteration;
      bool visualize3D;
      ///stack for the turtle position when generating blocks
      dynarray<ref<turtle3D>> stack3d;

    public:
      lsystem_mesh(){
        blocks.reserve(10);
        blocks.resize(10);
        for (int i = 0; i < 10; ++i) ls_generated.push_back(_NONE);
        visualize3D = false;
        cur_iteration = 0;
        angle_Y = 0;
        angle_X = 0;
        distance = 1;
        r_init = 10;
      }

      void init_values(float angle_, float distance_){
        angle_X = angle_;
        distance = distance_;
      }

      void update_angle(float angle_){
        angle_X = angle_;
        //update positions
      }

      void update_distance(float distance_){
        distance = distance_;
        //update positions
      }

      void update_values(float angle_, float distance_){
        init_values(angle_, distance_);
        //update positions
      }

      ///It will input and generate the word as an l_system. It will check previously if it has been already generated
      void input_word(int iteration, char *word, int size_word){
        //check if it has been already generated
        cur_iteration = iteration;
        if (ls_generated[iteration] != _NONE){ //already generated
          printf("Nothing to generate here. \n");
        }
        else{ //generate new set of blocks
          ls_generated[iteration] = _GENERATED;
          //Check size of l_system
          int num_symbols = 0;
          for (int i = 0; i < size_word; ++i){
            if (word[i] != '[' && word[i] != ']' && word[i] != '+' && word[i] != '-')
              ++num_symbols;
          }
          
          //Reserve space for the vertexes and indices
          unsigned vsize = 3 * 2 * num_symbols * sizeof(vertex);
          unsigned isize = 3 * 6 * num_symbols * sizeof(uint32_t);
          mesh::allocate(vsize, isize);
          
          //Generate initial block
          turtle3D *origin;
          origin->position = vec3(0);
          origin->transform.loadIdentity();
          origin->transform.translate(0, distance, 0);
          stack3d.push_back(origin);

          //Set up the generation
          block *new_block;
          if (blocks.size() < iteration){
            blocks.resize(iteration + 1);
          }
          blocks[iteration].reserve(num_symbols);
          
          //Start generation
          for (int i = 0; i < size_word; ++i){
            turtle3D * current_top = stack3d.back();
            char symbol = word[i];
            switch (symbol){
            case 'F':
              mat4t next_transformation;
              new_block = new block();
              new_block->origin = current_top->position;
              new_block->next = new_block->origin;
                //Apply trans
              blocks[iteration].push_back(new_block);
              break;
            case '[':
              break;
            case ']':
              break;
            case '+':
              break;
            case '-':
              break;
            }
          }//End for generation of blocks
          printf("Generated %i new blocks for tree\n",blocks[iteration].size());
        }//End else generate new set of blocks

      }

      void update(){
        //If the tree is not up to date
        if (ls_generated[cur_iteration] != _GENERATED){
          gl_resource::wolock vlock(get_vertices());
          vertex *vtx = (vertex*)vlock.u8();
          gl_resource::wolock ilock(get_indices());
          uint32_t *idx = ilock.u32();
          unsigned num_vertices = 0;
          unsigned num_indices = 0;

          for (unsigned i = 0; i != blocks[cur_iteration].size(); ++i){
            block *block_ = blocks[cur_iteration][i];
            if (block_->enable){
              
            }
          }
        }
      }
    };
  }
}

#endif