////////////////////////////////////////////////////////////////////////////////
//
// (C) Juanmihd
//
////////////////////////////////////////////////////////////////////////////////

#include "../../octet.h"

#include "lsystem_app.h"

/// Create a box with octet
int main(int argc, char **argv) {
  // set up the platform.
  octet::app::init_all(argc, argv);

  // our application.
  octet::lsystem_app app(argc, argv);
  app.init();

  // open windows
  octet::app::run_all_apps();
}


