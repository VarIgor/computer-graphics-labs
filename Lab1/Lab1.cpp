#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main()
{
    // Подключаем проверку на инициализацию через проверку glfwinit()
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }

    // Настройка версии OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Создаем контекст окна
    GLFWwindow* window = glfwCreateWindow(512, 512, "Trapezoid - Variant 9", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }

    // Сделать окно текущим контекстом
    glfwMakeContextCurrent(window);

    // Инициализацию GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return 1;
    }

    // Создаем основной цикл программы
    while (!glfwWindowShouldClose(window)) {
        glClearColor(1.0f, 0.4f, 0.1f, 1.0f); // Оранжевый цвет
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_QUADS);
        glColor3f(0.7f, 0.1f, 0.6f);  // Фиолетово-розовый цвет

        // Координаты трапеции (верх шире низа)
        glVertex2f(-0.5f, 0.3f);   
        glVertex2f(0.5f, 0.3f);   
        glVertex2f(0.3f, -0.3f);  
        glVertex2f(-0.3f, -0.3f);  
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate(); // Завершение работы контекста.
    return 0;
}