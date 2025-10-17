#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <string>

// ==================== ЗАДАНИЕ 1: ВСТРОЕННЫЕ ШЕЙДЕРЫ ====================

// Вершинный шейдер
const char* vertex_shader_source =
"#version 460 core\n"
"layout (location = 0) in vec3 aPos;" // attribute position
"void main(){\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}";

// Фрагментный шейдер
const char* fragment_shader_source =
"#version 460 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n" // Uniform переменная для изменения цвета
"void main(){\n"
"   FragColor = ourColor;\n"
"}";

GLuint compileEmbeddedShaders() {

    // Компиляция вершинного шейдера
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    // Проверка ошибок компиляции вершинного шейдера
    int success;
    char infoLog[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        glDeleteShader(vertex_shader);
        return 0;
    }

    // Компиляция фрагментного шейдера
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    // Проверка ошибок компиляции фрагментного шейдера
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        glDeleteShader(fragment_shader);
        return 0;
    }

    // Создание шейдерной программы
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    // Проверка ошибок линковки
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        glDeleteShader(shader_program);
        return 0;
    }

    // Удаляем шейдеры, так как они уже добавлены в программу
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}


// ==================== ЗАДАНИЕ 2: ШЕЙДЕРЫ ИЗ ФАЙЛОВ ====================

// Функция для чтения шейдера из файла
char* readShaderFromFile(const char* filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cout << "ERROR: Could not open shader file: " << filePath << std::endl;
        return nullptr;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string contentStr = buffer.str();

    // Проверяем размер
    if (contentStr.length() > 1000000) {
        std::cout << "ERROR: Shader file too large: " << filePath << std::endl;
        return nullptr;
    }

    char* content = new (std::nothrow) char[contentStr.length() + 1];
    if (!content) {
        std::cout << "ERROR: Memory allocation failed for shader: " << filePath << std::endl;
        return nullptr;
    }

    strcpy_s(content, contentStr.length() + 1, contentStr.c_str());
    return content;
}

// Функция для компиляции шейдера
GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    // Проверка ошибок компиляции
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

// Основная функция для создания шейдерной программы из файлов
GLuint createShaderProgram(const char* vertexPath, const char* fragmentPath) {
    // Читаем шейдеры из файлов
    char* vertexSource = readShaderFromFile(vertexPath);
    char* fragmentSource = readShaderFromFile(fragmentPath);

    if (!vertexSource || !fragmentSource) {
        delete[] vertexSource;
        delete[] fragmentSource;
        return 0;
    }

    // Компилируем шейдеры
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

    if (!vertexShader || !fragmentShader) {
        delete[] vertexSource;
        delete[] fragmentSource;
        return 0;
    }

    // Создаем шейдерную программу
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Проверка ошибок линковки
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        glDeleteProgram(program);
        program = 0;
    }

    // Очищаем ресурсы
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    delete[] vertexSource;
    delete[] fragmentSource;

    return program;
}

// ==================== БИБЛИОТЕКА UNIFORM ПЕРЕМЕННЫХ ====================

// Функции для установки uniform переменных (библиотека uniform)
void setUniform1i(GLuint program, const char* name, int value) {
    glUniform1i(glGetUniformLocation(program, name), value);
}

void setUniform1f(GLuint program, const char* name, float value) {
    glUniform1f(glGetUniformLocation(program, name), value);
}

void setUniform2f(GLuint program, const char* name, float x, float y) {
    glUniform2f(glGetUniformLocation(program, name), x, y);
}

void setUniform3f(GLuint program, const char* name, float x, float y, float z) {
    glUniform3f(glGetUniformLocation(program, name), x, y, z);
}

void setUniform4f(GLuint program, const char* name, float x, float y, float z, float w) {
    glUniform4f(glGetUniformLocation(program, name), x, y, z, w);
}

// ==================== ОСНОВНАЯ ПРОГРАММА ====================

int main()
{
    // Добавляем переменные для VBO, VAO, EBO и шейдерной программы
    GLuint VBO, VAO, EBO, shader_program;

    // Подключаем проверку на инициализацию через проверку glfwinit()
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }

    // Настройка версии OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    // Координаты вершин для трапеции (верх шире низа)
    float vertices[] = {
        // x,    y,    z
        -0.5f,  0.3f, 0.0f,  // верхний левый
         0.5f,  0.3f, 0.0f,  // верхний правый
         0.3f, -0.3f, 0.0f,  // нижний правый
        -0.3f, -0.3f, 0.0f   // нижний левый
    };

    // Индексы для отрисовки двух треугольников (образуют трапецию)
    unsigned int indices[] = {
        0, 1, 2,  // первый треугольник
        0, 2, 3   // второй треугольник
    };

    // Настройка буферов
    // Генерируем VAO, VBO, EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Привязываем VAO
    glBindVertexArray(VAO);

    // Привязываем и заполняем VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Привязываем и заполняем EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Настраиваем атрибуты вершин
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Отвязываем VBO и VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // ==================== ВЫБОР РЕЖИМА РАБОТЫ ====================
    // РАСКОММЕНТИРОВАТЬ НУЖНУЮ СТРОКУ ДЛЯ ВЫБОРА РЕЖИМА:

    // ЗАДАНИЕ 1: Встроенные шейдеры
    shader_program = compileEmbeddedShaders();


    // ЗАДАНИЕ 2: Шейдеры из файлов
   /* shader_program = createShaderProgram("vertex_shader.glsl", "fragment_shader.glsl");
    if (shader_program == 0) {
        std::cout << "Failed to create shader program!" << std::endl;
        glfwTerminate();
        return 1;
    }*/

    // Создаем основной цикл программы
    while (!glfwWindowShouldClose(window)) {
        // Очистка экрана
        glClearColor(1.0f, 0.4f, 0.1f, 1.0f); // Оранжевый фон
        glClear(GL_COLOR_BUFFER_BIT);

        // Используем свою шейдерную программу
        glUseProgram(shader_program);

        // Анимированный цвет
        float timeValue = static_cast<float>(glfwGetTime());
        float greenValue = (sinf(timeValue) / 2.0f) + 0.5f; // Значение от 0.0 до 1.0
        float redValue = (cosf(timeValue) / 2.0f) + 0.5f;   // Значение от 0.0 до 1.0

        // ==================== ВЫБОР МЕТОДА УСТАНОВКИ UNIFORM ====================

        // ЗАДАНИЕ 1: Стандартный метод
        // Получаем location uniform переменной и устанавливаем значение
        int vertexColorLocation = glGetUniformLocation(shader_program, "ourColor");
        glUniform4f(vertexColorLocation, redValue, greenValue, 0.3f, 1.0f);


        ////ЗАДАНИЕ 2: Используем библеотеку UNIFORM
        //setUniform4f(shader_program, "ourColor", redValue, greenValue, 0.3f, 1.0f);

        // Рисуем трапецию
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Освобождаем ресурсы
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shader_program);

    glfwTerminate(); // Завершение работы контекста.

    std::cout << "SUCCESS: Program terminated correctly!" << std::endl;
    return 0;
}
