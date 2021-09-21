#include <iostream>

import tgfx;

int main() {
  tgfx::tgfx_raii::init();

  tgfx::window window = tgfx::window::create_window(720, 480, "test tgfx");

  std::cout << "press enter to continue ..." << std::endl;
  std::cin.ignore();

  window.template emit<"change_size">(1280, 720);

  std::cout << "press enter to exit ..." << std::endl;
  std::cin.ignore();
}
