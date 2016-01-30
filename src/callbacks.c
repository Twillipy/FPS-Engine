#include "defs.h"

void glfw_window_size_callback(GLFWwindow* window, int width, int height)
{
  window_width = width;
  window_height = height;

  glViewport(0, 0, window_width, window_height);
  //printf("Resize: %i %i\n", width, height);
}

void glfw_cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
  static double last_xpos = 300; //window_width/2;
  static double last_ypos = 300; //window_height/2;

  if(xpos != window_width/2 || ypos != window_height/2)
  {
    camera.yaw   -= 2.5*(float)(xpos-last_xpos)/window_width;
    camera.pitch -= 2.5*(float)(ypos-last_ypos)/window_height;

         if(camera.pitch < -PI/2) {camera.pitch = -PI/2;}
    else if(camera.pitch >  PI/2) {camera.pitch =  PI/2;}
         if(camera.yaw < 0)       {camera.yaw += 2*PI;}
    else if(camera.yaw > 2*PI)    {camera.yaw -= 2*PI;}

    glfwSetCursorPos(window, window_width/2, window_height/2);
  }

    last_xpos = xpos;
    last_ypos = ypos;
  //printf("%.4g %.4g\n", xpos, ypos);
  //printf("Pitch: %.4g  Yaw: %.4g\n", camera.pitch, camera.yaw);
}

void glfw_mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  // Positive is away, Negative is towards
}

void glfw_keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  // Action:
  //  1 - Press
  //  2 - Repeat
  //  0 - Release

  if(action == GLFW_RELEASE) {return;}
  float speed;
  if(mods&GLFW_MOD_SHIFT) {speed = 5.0;}
  else {speed = 1.0;}
  
  switch(key)
  {
    case GLFW_KEY_W:
      camera.pos[0] += -speed*sin(camera.yaw)*cos(camera.pitch);
      camera.pos[2] += -speed*cos(camera.yaw)*cos(camera.pitch);
      camera.pos[1] +=  speed*sin(camera.pitch);
      break;
    case GLFW_KEY_S:
      camera.pos[0] +=  speed*sin(camera.yaw)*cos(camera.pitch);
      camera.pos[2] +=  speed*cos(camera.yaw)*cos(camera.pitch);
      camera.pos[1] += -speed*sin(camera.pitch);
      break;
    case GLFW_KEY_A:
      camera.pos[0] += -speed*cos(camera.yaw);
      camera.pos[2] +=  speed*sin(camera.yaw);
      break;
    case GLFW_KEY_D:
      camera.pos[0] +=  speed*cos(camera.yaw);
      camera.pos[2] += -speed*sin(camera.yaw);
      break;
    case GLFW_KEY_SPACE:
      camera.pos[1] += speed;
      break;
    case GLFW_KEY_Z:
      camera.pos[1] -= speed;
      break;
    case GLFW_KEY_ESCAPE:
      glfwSetWindowShouldClose(window, 1);
      break;
    case GLFW_KEY_F12:
      printf("Toggle: Fullscreen\n");
      /*
      glfwDestroyWindow(window);

      // Windowed
      window = glfwCreateWindow(window_width, window_height, "OpenGL Test", NULL, NULL);

      // Fullscreen
      GLFWmonitor* mon = glfwGetPrimaryMonitor();
      const GLFWvidmode* vmode = glfwGetVideoMode(mon);
      window = glfwCreateWindow(vmode->width, vmode->height, "Extended GL Init", mon, NULL);

      glfwMakeContextCurrent(window);
      glfwSetWindowSizeCallback(window, glfw_window_size_callback);
      glfwSetCursorPosCallback(window, glfw_cursor_position_callback);
      glfwSetScrollCallback(window, glfw_mouse_scroll_callback);
      glfwSetKeyCallback(window, glfw_keyboard_callback);
      */
      break;
    default:
      printf("Key: %c(%i) %i\n", key, key, GLFW_PRESS);
      break;
  }

  //printf("Pos(%f,%f)\n", camera.pos[0], camera.pos[2]);
}
