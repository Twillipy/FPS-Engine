#include "defs.h"

void glfw_window_size_callback(GLFWwindow* window, int width, int height)
{
  s_settings *settings = (s_settings*)glfwGetWindowUserPointer(window);
  if(settings == NULL) {return;}
  
  settings->window_width = width;
  settings->window_height = height;

  glViewport(0, 0, settings->window_width, settings->window_height);
  //printf("Resize: %i %i\n", width, height);
}

void glfw_cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
  s_settings *settings = (s_settings*)glfwGetWindowUserPointer(window);
  if(settings == NULL) {return;}
  
  static double last_xpos = 300; //window_width/2;
  static double last_ypos = 300; //window_height/2;

  if(xpos != settings->window_width/2 || ypos != settings->window_height/2)
  {
    settings->camera.yaw   -= 2.5*(float)(xpos-last_xpos)/settings->window_width;
    settings->camera.M_PItch -= 2.5*(float)(ypos-last_ypos)/settings->window_height;

         if(settings->camera.M_PItch < -M_PI/2) {settings->camera.M_PItch = -M_PI/2;}
    else if(settings->camera.M_PItch >  M_PI/2) {settings->camera.M_PItch =  M_PI/2;}
         if(settings->camera.yaw < 0)       {settings->camera.yaw += 2*M_PI;}
    else if(settings->camera.yaw > 2*M_PI)    {settings->camera.yaw -= 2*M_PI;}

    glfwSetCursorPos(window, settings->window_width/2, settings->window_height/2);
  }

    last_xpos = xpos;
    last_ypos = ypos;
  //printf("%.4g %.4g\n", xpos, ypos);
  //printf("M_PItch: %.4g  Yaw: %.4g\n", settings->camera.M_PItch, settings->camera.yaw);
}

void glfw_mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  // Positive is away, Negative is towards
}

void glfw_keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  s_settings *settings = (s_settings*)glfwGetWindowUserPointer(window);
  if(settings == NULL) {return;}
  
  // Action:
  //  1 - Press
  //  2 - Repeat
  //  0 - Release
  
  static int wireframe = 0;

  if(action == GLFW_RELEASE) {return;}
  float speed;
  if(mods&GLFW_MOD_SHIFT) {speed = 5.0;}
  else {speed = 1.0;}
  
  switch(key)
  {
    case GLFW_KEY_W:
      settings->camera.pos[0] += -speed*sin(settings->camera.yaw)*cos(settings->camera.M_PItch);
      settings->camera.pos[2] += -speed*cos(settings->camera.yaw)*cos(settings->camera.M_PItch);
      settings->camera.pos[1] +=  speed*sin(settings->camera.M_PItch);
      break;
    case GLFW_KEY_S:
      settings->camera.pos[0] +=  speed*sin(settings->camera.yaw)*cos(settings->camera.M_PItch);
      settings->camera.pos[2] +=  speed*cos(settings->camera.yaw)*cos(settings->camera.M_PItch);
      settings->camera.pos[1] += -speed*sin(settings->camera.M_PItch);
      break;
    case GLFW_KEY_A:
      settings->camera.pos[0] += -speed*cos(settings->camera.yaw);
      settings->camera.pos[2] +=  speed*sin(settings->camera.yaw);
      break;
    case GLFW_KEY_D:
      settings->camera.pos[0] +=  speed*cos(settings->camera.yaw);
      settings->camera.pos[2] += -speed*sin(settings->camera.yaw);
      break;
    case GLFW_KEY_SPACE:
      settings->camera.pos[1] += speed;
      break;
    case GLFW_KEY_Z:
      settings->camera.pos[1] -= speed;
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
    case GLFW_KEY_F1:
      wireframe = !wireframe;
      if(wireframe)
      {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      }
      else
      {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }
    default:
      printf("Key: %c(%i) %i\n", key, key, GLFW_PRESS);
      break;
  }

  //printf("Pos(%f,%f)\n", settings->camera.pos[0], settings->camera.pos[2]);
}
