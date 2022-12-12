#ifndef OPENGL_3D_ENVIRONMENT_BASIS_HPP
#define OPENGL_3D_ENVIRONMENT_BASIS_HPP

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Basis
{
public:
    Shader shader = Shader("src/render/shaders/shader.vs", "src/render/shaders/shader.fs");
    GLuint VAO{}, VBO[2]{};

    Basis(float len, glm::mat4 &projection)
    {
        float pos[] {
            0.0, 0.0, 0.0,
            len, 0.0, 0.0,

            0.0, 0.0, 0.0,
            0.0, len, 0.0,

            0.0, 0.0, 0.0,
            0.0, 0.0, len};

        float col[] {
                1.0, 0.0, 0.0,
                1.0, 0.0, 0.0,

                0.0, 1.0, 0.0,
                0.0, 1.0, 0.0,

                0.0, 0.0, 1.0,
                0.0, 0.0, 1.0};

        glGenVertexArrays(1, &VAO);
        glGenBuffers(2, VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(col), col, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("model", glm::mat4(1.0f));
        shader.setFloat("const_div", 1.0f);
    }

    void draw(glm::mat4 &view)
    {
        shader.use();
        shader.setMat4("view", view);

        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, 6);
        glBindVertexArray(0);
    }
};

#endif //OPENGL_3D_ENVIRONMENT_BASIS_HPP
