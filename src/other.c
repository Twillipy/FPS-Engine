#include "defs.h"

void update_fps_counter(GLFWwindow* window)
{
  static double previous_seconds = 0;
  static int frame_count = 0;
  double current_seconds = glfwGetTime();
  double elapsed_seconds = current_seconds - previous_seconds;
  if(elapsed_seconds > 0.25)
  {
    previous_seconds = current_seconds;
    double fps = (double)frame_count/elapsed_seconds;
    char tmp[128];
    sprintf(tmp, "OpenGL Test - %.2f fps", fps);
    glfwSetWindowTitle(window, tmp);
    frame_count = 0;
  }
  frame_count++;
}
