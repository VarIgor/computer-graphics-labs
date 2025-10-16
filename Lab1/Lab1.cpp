#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main()
{
    // ���������� �������� �� ������������� ����� �������� glfwinit()
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }

    // ��������� ������ OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // ������� �������� ����
    GLFWwindow* window = glfwCreateWindow(512, 512, "Trapezoid - Variant 9", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }

    // ������� ���� ������� ����������
    glfwMakeContextCurrent(window);

    // ������������� GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return 1;
    }

    // ������� �������� ���� ���������
    while (!glfwWindowShouldClose(window)) {
        glClearColor(1.0f, 0.4f, 0.1f, 1.0f); // ��������� ����
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_QUADS);
        glColor3f(0.7f, 0.1f, 0.6f);  // ���������-������� ����

        // ���������� �������� (���� ���� ����)
        glVertex2f(-0.5f, 0.3f);   
        glVertex2f(0.5f, 0.3f);   
        glVertex2f(0.3f, -0.3f);  
        glVertex2f(-0.3f, -0.3f);  
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate(); // ���������� ������ ���������.
    return 0;
}