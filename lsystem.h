////////////////////////////////////////////////////////////////////////////////
//
// (C) Juanmihd
//
////////////////////////////////////////////////////////////////////////////////

#ifndef LSYSTEM_INCLUDED
#define LSYSTEM_INCLUDED

namespace octet{

  /// @brief This is the lsystem class. It will create an object that will control the l_system
  class lsystem{
    dynarray<char> initial_word;
    dynarray<dynarray<char>> word;
    dynarray<dynarray<char>> set_rules;
    dynarray<int> symbols;
    dynarray<int> symbols_rev;
    int iteration;
    float angle;
    //Things to handle the file

    int read_word(char* word){

    }

    bool decode_file(){
      unsigned int buffer_size = buffer.size();

      //Check if the first word is "redefine"
      //Read the angle
      //Read the distance
      //Read initial axiom
      //Read number and type of rules
      //Read rules
      
      return true;
    }

  public:
    lsystem(){}

    bool load_file(char * file_name){
      dynarray<uint8_t> buffer;
      app_utils::get_url(buffer, file_name);
      return decode_file(buffer);
    }

    void nextStep();
  };

}

#endif