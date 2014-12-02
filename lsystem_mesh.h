
#ifndef LSYSTEM_MESH_INCLUDED
#define LSYSTEM_MESH_INCLUDED

namespace octet{
  namespace scene{
    enum status_ls{_NONE = -1, _TO_UPDATE = 0, _GENERATED = 1};
    enum {PRECISION_CYLINDER = 10, R_INIT = 10};
    class lsystem_mesh : public mesh{
      ///Each one of the elements to be drawed with the lsystem
      struct Block : public resource{
        mat4t pos;
        mat4t transform;
        float radio;
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
      float angle_X;
      float angle_Y;
      float distance;
      float r_init;
      float precision;
      dynarray<status_ls> ls_generated;
      int cur_iteration;
      bool visualize3D;
      ///stack for the turtle position when generating blocks
      dynarray<ref<Block>> stack3d;

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
        r_init = R_INIT;
        precision = PRECISION_CYLINDER;
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
          printf("Nothing to generate here. But it may be needed to update!\n");
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
          
          //Set up the generation
          if (blocks.size() < (unsigned) iteration){
            blocks.resize(iteration + 1);
          }
          blocks[iteration].reserve(num_symbols);

          //Generate initial stack for turtle3D
          Block * back_stack = new Block();
          back_stack->pos.loadIdentity();
          back_stack->radio = r_init;
          back_stack->transform.loadIdentity();
          back_stack->transform.translate(0, distance, 0);
          stack3d.push_back(back_stack);
          Block *new_block;

          //Start generation
          for (int i = 0; i < size_word; ++i){
            char symbol = word[i];
            printf("\n%c:\n", symbol);
            switch (symbol){
            case 'F':
              //Reserve new block
              new_block = new Block();
              printf("Stack pos:\n");
              printf_mat4t(back_stack->pos);
              back_stack->pos.multMatrix(back_stack->transform);
              new_block->pos = back_stack->pos;
              new_block->transform = back_stack->transform;
              printf("New block pos:\n");
              printf_mat4t(new_block->pos);
              printf("New block transform:\n");
              printf_mat4t(new_block->transform);
              new_block->radio = back_stack->radio;
              blocks[iteration].push_back(new_block);
              break;
            case '[':
              back_stack = new Block();
              back_stack->pos = blocks[iteration].back()->pos;
              back_stack->transform = stack3d.back()->transform;
              back_stack->radio = stack3d.back()->radio;
              back_stack->pos.multMatrix(back_stack->transform);
              printf("Stack pos:\n");
              printf_mat4t(back_stack->pos);
              printf("Stack transform:\n");
              printf_mat4t(back_stack->transform);
              stack3d.push_back(back_stack);
              back_stack = stack3d.back();
              break;
            case ']':
              stack3d.pop_back();
              break;
            case '+':
              back_stack->transform.rotateX(angle_X);
              break;
            case '-':
              back_stack->transform.rotateX(-angle_X);
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
          
          //Draw circle with the first origin

          //Draw circles in given positions
          //for (unsigned i = 0; i != blocks[cur_iteration].size(); ++i){
          //  Block *block_ = blocks[cur_iteration][i];
          //  //Get circle and add the current next
          //}
        }
      }
    };
  }
}

#endif