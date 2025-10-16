# Development Setup

Общие библиотеки для всех лабораторных работ по компьютерной графике.

## Содержание
- `glew-2.1.0/` - Библиотека GLEW
- `glfw-3.4.bin.WIN64/` - Библиотека GLFW

## Настройка проекта
1. Создайте новый проект в Visual Studio
2. В настройках проекта укажите пути:
В настройках раздел C/C++ → General → “Additional include directories”
   - Additional Include Directories: `$(SolutionDir)..\dev-setup\glew-2.1.0\include`
   - Additional Include Directories: `$(SolutionDir)..\dev-setup\glfw-3.4.bin.WIN64\include`
В настройках раздел Linker → Input → “additional dependencies”
   - прописываем библиотеки : "glfw3.lib;glfw3dll.lib;opengl32.lib;User32.lib;GDI32.lib;Shell32.lib;%(AdditionalDependencies)"
Добавляем путь для библиотеки “glfw3dll.lib” Linker → General → “Additional Library Directories”
   - Additional Library Directories: `$(SolutionDir)..\dev-setup\glfw-3.4.bin.WIN64\lib-vc2022`