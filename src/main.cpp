
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "texture.h"
#define PI 3.14


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);


struct STRVertex
{
    glm::vec3 position;
    glm::vec3 couleur;
}; 

float int_float_color(int x)
{
    return float((x * 100) / 255);
}
glm::vec3 getColor(int r,int g, int b)
{
    return glm::vec3(int_float_color(r), int_float_color(g), int_float_color(b));
}



// LINES OF 3 AXES 
float x_axe[] = {
     -1000.0f, 0.0f, 0.0f,  //0
      1000.0f, 0.0f, 0.0f,  //1
};
float y_axe[] = {
      0.0f,-1000.0f, 0.0f,  //0
      0.0f, 1000.0f, 0.0f,  //1
};
float z_axe[] = {
      0.0f, 0.0f,-1000.0f,  //0
      0.0f, 0.0f, 1000.0f,  //1
};
unsigned int axe_ind[] = {
    0,1
};
// SQUARE
float positions[] = { // color
   -0.5f, 0.5f, 0.0f,  //0
    0.5f,  0.5f, 0.0f, //1 
   -0.5f, -0.5f, 0.0f, //2
    0.5f,  -0.5f, 0.0f, //3

};
unsigned int indices[] = {
    0,1,2,
    1,3,2
};
//-------------
float camera_posx = 0.0f;
float scale = 0.01f;
float angle = 0.1f;

int axis_rotation = 0;
float camSpeed = 0.1f;
float mouseSpeed = 2.0f;


glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camDirection = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);

float rotate_x = 0.0f;
float rotate_y = 0.0f;
float rotate_z = 0.0f;

glm::vec4 background_color = glm::vec4 (getColor(213,50,50),1.0f);

int show_shapes = 0;

float r = 0.5f;
float ambient = 0.1f;

float incr = 0.1f;
float backg_r = 0.0f;
//center sphere rotation + speed 
float center_sphere_r = 0.0f;
float center_sphere_r_speed = 0.0f;

//both diagonals speed
float cones_diagonals_rotation = 0.0f;
float cones_diagonals_rotation_speed = .0f;
float cones_dr = 0.0f; // right diagonal rotation
float cones_dl = 0.0f; // left diagonal rotqtion
float cones_dr_speed = 1.0f; // right diagonal rotation
float cones_dl_speed = 5.0f; // left diagonal rotqtion

float cone1_r = 0.0f;
float cone1_speed = 20.0f;
float cone2_r = 0.0f;
float cone2_speed = 5.0f;
float cone3_r = 0.0f;
float cone3_speed = 10.0f;
float cone4_r = 0.0f;
float cone4_speed = 3.0f;



int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    //setting opengl version 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1024, 768, "Projet OpenGL SI MIV 2021 GUENAOUI + NAIT KACI ", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */ 
    glfwMakeContextCurrent(window);
    //synchronise the swap
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) 
        std::cout << "Error !" << std::endl ;
    


    // second object sphere
    //generates all vertices of the sphere for the given radius, sectors and stacks. 
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;


    int radius = 1;
    int sectorCount = 38;
    int stackCount = 38;
    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
    float s, t;                                     // vertex texCoord

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // normalized vertex normal (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);

            // vertex tex coord (s, t) range between [0, 1]
            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            texCoords.push_back(s);
            texCoords.push_back(t);
        }
    }

    //indices
    std::vector<int> indices_s;
    std::vector<int> lineIndices;
    int k1, k2;
    for (int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if (i != 0)
            {
                indices_s.push_back(k1);
                indices_s.push_back(k2);
                indices_s.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if (i != (stackCount - 1))
            {
                indices_s.push_back(k1 + 1);
                indices_s.push_back(k2);
                indices_s.push_back(k2 + 1);
            }

            // store indices for lines
            // vertical lines for all stacks, k1 => k2
            lineIndices.push_back(k1);
            lineIndices.push_back(k2);
            if (i != 0)  // horizontal lines except 1st stack, k1 => k+1
            {
                lineIndices.push_back(k1);
                lineIndices.push_back(k1 + 1);
            }
        }
    }

    
    int vertices_length = vertices.size();
    int normals_length = normals.size();
    int texCoords_length = texCoords.size();
    int sphere_indices_length = indices_s.size();
    int sphere_line_indices_length = lineIndices.size();

    float  * sphere_vertexes = new float[vertices_length];
    for (int i = 0; i < vertices_length; i++)
        sphere_vertexes[i] = vertices[i];
    
    float* sphere_normals = new float[normals_length];
    for (int i = 0; i < normals_length; i++)
        sphere_normals[i] = normals[i];

    float* sphere_text_coords = new float[texCoords_length];
    for (int i = 0; i < texCoords_length; i++)
        sphere_text_coords[i] = texCoords[i];
    
    unsigned int * sphere_indices = new unsigned int[sphere_indices_length];
    for (int i = 0; i < sphere_indices_length; i++)
        sphere_indices[i] = indices_s[i];
    
    float* sphere_line_coords = new float[sphere_line_indices_length];
    for (int i = 0; i < sphere_line_indices_length; i++)
        sphere_line_coords[i] = lineIndices[i];
    //---------end of sphere

    //-------- cube 

    unsigned int cube_vertexes_length = 8;
    int cube_indices_length = 36;
    float cube_vertexes[] = { // color
       -0.5f,  0.5f, -0.5f,  //0
        0.5f,  0.5f, -0.5f, //1 
       -0.5f, -0.5f, -0.5f, //2
        0.5f, -0.5f, -0.5f, //3
       -0.5f,  0.5f,  0.5f,  //4
        0.5f,  0.5f,  0.5f, //5
       -0.5f, -0.5f,  0.5f, //6
        0.5f, -0.5f,  0.5f, //7

    };
    unsigned int cube_indices[] = {
        0,1,2, // 
        1,3,2, // 1
        4,5,7,
        4,6,7,// 2
        0,4,5,
        0,5,1,//3
        1,5,3,
        3,5,7,//4
        0,4,6,
        0,6,2,//5
        2,7,3,
        2,7,6//6

    };

    //--------- end of cube 

    //--------- Cone 
    unsigned int cone_vertexes_length = 5;
    int cone_indices_length = 18;
    float cone_vertexes[] = { // color
        0.0f, -0.5f, -0.5f,  //0
        0.0f, -0.5f,  0.5f, //1 
        0.0f,  0.5f,  0.5f, //2
        0.0f,  0.5f, -0.5f, //3
        1.0f,  0.0f,  0.0f,  //4
        

    };
    unsigned int cone_indices[] = {
        0,1,2,
        0,3,2,
        0,1,4,
        3,2,4,
        1,2,4,
        0,3,4

    };
 
    VertexArray va;
    VertexBuffer vb(positions, 4 * 3 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(3);
    va.addBUffer(vb, layout);
    IndexBuffer ib(indices,6);
    //shader declar
    
    Shader shader("res/shaders/shad.shader");
    shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);


    // #vertex array 
    //unbounding just make the values 0 in the bind functions go to BAHLOUL
    va.Unbind();
    shader.UnBind();
    vb.Bind();
    ib.Unbind();


    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

    

    float speed = 3.0f; // 3 units / second
    float mouseSpeed = 0.005f;
    // Get mouse position
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);//getting mouse positions//


    //Mercury path
    float mercury_angle = 0.0f;
    float mercury_speed = 0.2f;

    //Venus path
    float venus_angle = 0.1f;
    float venus_speed= 0.05f;
    // earth path
    float earth_angle = 0.1f;
    float earth_speed = 0.03f;

    //moon path
    float moon_angle = 0.0f;
    float moon_speed = .1f;


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        //glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, background_color[3]);
   
        
      
        //interaction functions call back 
         //keyboard and mouse call 
        glfwSetKeyCallback(window, key_callback);
        glfwSetCursorPosCallback(window, cursor_position_callback);

        //camera and matrix
        glm::vec3 camX = normalize(cross(camUp, camDirection));


        glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 200.0f);
        //glm::mat4 View = glm::lookAt(glm::vec3(camX, 0, camZ), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        glm::mat4 View = glm::lookAt(camPos, camX, camUp);
        glm::mat4 Model = glm::mat4(1.0f);
        //translate
        Model = glm::translate(Model, glm::vec3(-2.0f, 0.0f, -5.0f));
        //scale
        Model = glm::scale(Model, glm::vec3(1.0f * scale, 1.0f * scale, scale * 1.0f));


        /*glm::mat4 Projection = glm::perspective(glm::radians(60.0f), 4.0f / 3.0f, 0.1f, 100.0f);
        glm::mat4 View = glm::lookAt(glm::vec3(0, 0, 4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        glm::mat4 Model = glm::mat4(1.0f);*/
        Model = glm::rotate(Model, glm::radians(rotate_x), glm::vec3(1, 0, 0));
        Model = glm::rotate(Model, glm::radians(rotate_y), glm::vec3(0, 1, 0));
        Model = glm::rotate(Model, glm::radians(rotate_z), glm::vec3(0, 0, 1));
        glm::mat4 MVP = Projection * View * Model;
        shader.SetUniformMat4("u_MVP", MVP);


        //Texture texture("res/textures/0.raw");
        //texture.Bind();
        shader.Bind();
        //shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
        

        shader.SetUniform4f("u_Color", 1.0f, 0.3f, 0.8f, 1.0f);
      
        
        //lightning 

        //shader.SetUniform3f("lightPos", 1.0f, 1.0f, 5.0f);
        shader.SetUniform4f("lightColor", 1.0f, 1.0f, 1.0f, 1.0f);
        shader.SetUniform1f("ambient_value", ambient);
        //shader.SetUniform3f("viewPos", 0.0f, 0.0f, 2.0f);
        //shader.SetUniform1i("u_Texture", 0);
      
        VertexArray va;
        va.Bind();
        //drawing the axes
        //x_axis
        //shader.SetUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);
        //VertexBuffer vbxa(x_axe, 2 * 3 * sizeof(float));
        //VertexBufferLayout layout_xaxis;
        //layout_xaxis.Push<float>(3);
        //vbxa.Bind();
        //va.addBUffer(vbxa, layout_xaxis);
        //IndexBuffer ibxa(axe_ind, 2);
        //ibxa.Bind();
        //GLCall(glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, nullptr));
        ////y_axis
        //shader.SetUniform4f("u_Color", 0.0f, 1.0f, 0.0f, 1.0f);
        //VertexBuffer vbya(y_axe, 2 * 3 * sizeof(float));
        //VertexBufferLayout layout_yaxis;
        //layout_yaxis.Push<float>(3);
        //vbxa.Bind();
        //va.addBUffer(vbya, layout_yaxis);
        //IndexBuffer ibya(axe_ind, 2);
        //ibya.Bind();
        //GLCall(glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, nullptr));
        ////z_axis
        //shader.SetUniform4f("u_Color", 0.0f, 0.0f, 1.0f, 1.0f);
        //VertexBuffer vbza(z_axe, 2 * 3 * sizeof(float));
        //VertexBufferLayout layout_zaxis;
        //layout_zaxis.Push<float>(3);
        //vbza.Bind();
        //va.addBUffer(vbza, layout_yaxis);
        //IndexBuffer ibza(axe_ind, 2);
        //ibza.Bind();
        //GLCall(glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, nullptr));

        
        //transforms for the background
        //will make its radius big to fill background ( sky box ) 
        glm::mat4 ball_model = glm::translate(Model, glm::vec3(0.0f,0.0f , 0.0f));
        ball_model = glm::scale(ball_model, glm::vec3(20.0f, 20.0f, -20.0f));
        ball_model = glm::rotate(ball_model, glm::radians(backg_r), glm::vec3(0, 0, 1));
        MVP = Projection * View * ball_model;
        shader.SetUniformMat4("u_MVP", MVP);
        //drawing the objects
        //background box 
        shader.SetUniform4f("u_Color", 0.0f, 0.5f, 5.0f, 1.0f);
        //drawing ball
        VertexBuffer vbsphere(sphere_vertexes, (vertices_length) * sizeof(float));
        VertexBufferLayout layout_sphere;
        layout_sphere.Push<float>(3);
        vbsphere.Bind();
        va.addBUffer(vbsphere, layout_sphere);
        IndexBuffer ibsphere(sphere_indices, sphere_indices_length);
        ibsphere.Bind();
        GLCall(glDrawElements(GL_POINTS, sphere_indices_length, GL_UNSIGNED_INT, (void*)0));
        //done with background
        

        
        //transforms for the center sphere 
        float center_radius = 5.0f;
        glm::mat4 center_sphere = glm::translate(Model, glm::vec3(0.0f, 0.0f, 0.0f));
        center_sphere = glm::rotate(center_sphere, glm::radians(center_sphere_r), glm::vec3(0, 1, 0));
        //center_sphere = glm::scale(center_sphere, glm::vec3(center_radius, center_radius, center_radius));
        MVP = Projection * View * center_sphere;
        shader.SetUniformMat4("u_MVP", MVP);
        //drawing the sphere
        shader.SetUniform4f("u_Color", 1.f, .6f, 0.3f, 1.0f);


       
        VertexBuffer vbsphere1(sphere_vertexes, (vertices_length) * sizeof(float));
        VertexBufferLayout layout_sphere1;
        layout_sphere1.Push<float>(3);
        VertexBuffer vbnormals(sphere_normals, (normals_length)*sizeof(float));
        VertexBufferLayout layout_sphere1_normals;
        layout_sphere1_normals.Push<float>(3);

        vbsphere1.Bind();
        vbnormals.Bind();
        va.addBUffer(vbsphere1, layout_sphere1);
        va.addBUffer(vbnormals, layout_sphere1_normals);
        IndexBuffer ibsphere1(sphere_indices, sphere_indices_length );
        ibsphere1.Bind();
        GLCall(glDrawElements(GL_TRIANGLES, sphere_indices_length, GL_UNSIGNED_INT, nullptr));


        //showing the test shapes 
        if (show_shapes == 0) {
            //drawing the cubes right +  left  
            glm::mat4 cubes = glm::translate(Model, glm::vec3(0.0f, 0.0f, 0.0f));
            cubes = glm::rotate(cubes, glm::radians(center_sphere_r), glm::vec3(0, 0, 1));
            MVP = Projection * View * cubes;
            shader.SetUniformMat4("u_MVP", MVP);

            //drawing the cube the right 
            glm::mat4 cube_right = glm::translate(cubes, glm::vec3(2.0f, 0.0f, 0.0f));
            cube_right = glm::rotate(cube_right, glm::radians(0.0f), glm::vec3(0, 1, 0));
            //center_sphere = glm::scale(center_sphere, glm::vec3(center_radius, center_radius, center_radius));
            MVP = Projection * View * cube_right;
            shader.SetUniformMat4("u_MVP", MVP);


            shader.SetUniform4f("u_Color", 0.0f, 0.5f, 1.f, 0.3f);
            VertexBuffer vbcube(cube_vertexes, 8 * 3 * sizeof(float));
            VertexBufferLayout layout_cube;
            layout_cube.Push<float>(3);
            vbcube.Bind();
            va.addBUffer(vbcube, layout_cube);
            IndexBuffer ibcube(cube_indices, cube_indices_length);
            ibcube.Bind();
            GLCall(glDrawElements(GL_TRIANGLES, cube_indices_length, GL_UNSIGNED_INT, nullptr));
            //drawing the left cube 

            glm::mat4 cube_left = glm::translate(cubes, glm::vec3(-2.0f, 0.0f, 0.0f));
            cube_left = glm::rotate(cube_left, glm::radians(0.0f), glm::vec3(0, 1, 0));
            //center_sphere = glm::scale(center_sphere, glm::vec3(center_radius, center_radius, center_radius));
            MVP = Projection * View * cube_left;
            shader.SetUniformMat4("u_MVP", MVP);


            shader.SetUniform4f("u_Color", 0.0f, 0.3f, 0.8f, .3f);
            VertexBuffer vbcubeleft(cube_vertexes, 8 * 3 * sizeof(float));
            VertexBufferLayout layout_cube_left;
            layout_cube_left.Push<float>(3);
            vbcubeleft.Bind();
            va.addBUffer(vbcubeleft, layout_cube_left);
            IndexBuffer ibcubeleft(cube_indices, cube_indices_length);
            ibcubeleft.Bind();
            GLCall(glDrawElements(GL_TRIANGLES, cube_indices_length, GL_UNSIGNED_INT, nullptr));
            //done with the cubes 

            //drawing the cones diagonal one

            glm::mat4 cones = glm::translate(Model, glm::vec3(0.0f, 0.0f, 0.0f));
            cones = glm::rotate(cones, glm::radians(cones_diagonals_rotation), glm::vec3(0, 1, 0));
            MVP = Projection * View * cones;
            shader.SetUniformMat4("u_MVP", MVP);

            //right diagonal 
            glm::mat4 cones_right_diagonal = glm::translate(cones, glm::vec3(0.0f, 0.0f, 0.0f));
            cones_right_diagonal = glm::rotate(cones_right_diagonal, glm::radians(45.0f), glm::vec3(0, 0, 1));
            cones_right_diagonal = glm::rotate(cones_right_diagonal, glm::radians(cones_dr), glm::vec3(0, 1, 0));
            MVP = Projection * View * cones_right_diagonal;
            shader.SetUniformMat4("u_MVP", MVP);
            //drawing first cone of the right diagonal
            glm::mat4 cone_left = glm::translate(cones_right_diagonal, glm::vec3(2.0f, 0.0f, 0.0f));
            cone_left = glm::rotate(cone_left, glm::radians(cone1_r), glm::vec3(1, 0, 0));
            //center_sphere = glm::scale(center_sphere, glm::vec3(center_radius, center_radius, center_radius));
            MVP = Projection * View * cone_left;
            shader.SetUniformMat4("u_MVP", MVP);


            shader.SetUniform4f("u_Color", 0.3f, 0.0f, 0.8f, .3f);
            VertexBuffer vbconeleft(cone_vertexes, 5 * 3 * sizeof(float));
            VertexBufferLayout layout_cone_left;
            layout_cone_left.Push<float>(3);
            vbconeleft.Bind();
            va.addBUffer(vbconeleft, layout_cone_left);
            IndexBuffer ibconeleft(cone_indices, cone_indices_length);
            ibconeleft.Bind();
            GLCall(glDrawElements(GL_TRIANGLES, cone_indices_length, GL_UNSIGNED_INT, nullptr));



            //drawing second cone of the right diagonal
            glm::mat4 cone_right = glm::translate(cones_right_diagonal, glm::vec3(-2.0f, 0.0f, 0.0f));
            cone_right = glm::rotate(cone_right, glm::radians(cone2_r), glm::vec3(1, 0, 0));
            cone_right = glm::scale(cone_right, glm::vec3(-1.0f, 1.0f, 1.0f));
            //center_sphere = glm::scale(center_sphere, glm::vec3(center_radius, center_radius, center_radius));
            MVP = Projection * View * cone_right;
            shader.SetUniformMat4("u_MVP", MVP);


            shader.SetUniform4f("u_Color", 0.3f, 0.0f, 0.8f, .3f);
            VertexBuffer vbconeright(cone_vertexes, 5 * 3 * sizeof(float));
            VertexBufferLayout layout_cone_right;
            layout_cone_right.Push<float>(3);
            vbconeright.Bind();
            va.addBUffer(vbconeright, layout_cone_right);
            IndexBuffer ibconeright(cone_indices, cone_indices_length);
            ibconeright.Bind();
            GLCall(glDrawElements(GL_TRIANGLES, cone_indices_length, GL_UNSIGNED_INT, nullptr));

            // done with first diagonal 


            //left diagonal
            glm::mat4 cones_left_diagonal = glm::translate(cones, glm::vec3(0.0f, 0.0f, 0.0f));
            cones_left_diagonal = glm::rotate(cones_left_diagonal, glm::radians(-45.0f), glm::vec3(0, 0, 1));
            cones_left_diagonal = glm::rotate(cones_left_diagonal, glm::radians(cones_dl), glm::vec3(0, 1, 0)); // rotating
            MVP = Projection * View * cones_left_diagonal;
            shader.SetUniformMat4("u_MVP", MVP);
            //drawing first cone of the left diagonal
            glm::mat4 cone_left_1 = glm::translate(cones_left_diagonal, glm::vec3(2.0f, 0.0f, 0.0f));
            cone_left_1 = glm::rotate(cone_left_1, glm::radians(cone3_r), glm::vec3(1, 0, 0));
            //center_sphere = glm::scale(center_sphere, glm::vec3(center_radius, center_radius, center_radius));
            MVP = Projection * View * cone_left_1;
            shader.SetUniformMat4("u_MVP", MVP);


            shader.SetUniform4f("u_Color", 0.3f, 0.0f, 0.8f, .3f);
            VertexBuffer vbconeleft_1(cone_vertexes, 5 * 3 * sizeof(float));
            VertexBufferLayout layout_cone_left_1;
            layout_cone_left_1.Push<float>(3);
            vbconeleft_1.Bind();
            va.addBUffer(vbconeleft_1, layout_cone_left_1);
            IndexBuffer ibconeleft_1(cone_indices, cone_indices_length);
            ibconeleft_1.Bind();
            GLCall(glDrawElements(GL_TRIANGLES, cone_indices_length, GL_UNSIGNED_INT, nullptr));



            //drawing second cone of the left diagonal 
            glm::mat4 cone_right_1 = glm::translate(cones_left_diagonal, glm::vec3(-2.0f, 0.0f, 0.0f));
            cone_right_1 = glm::rotate(cone_right_1, glm::radians(cone4_r), glm::vec3(1, 0, 0));
            cone_right_1 = glm::scale(cone_right_1, glm::vec3(-1.0f, 1.0f, 1.0f));
            //center_sphere = glm::scale(center_sphere, glm::vec3(center_radius, center_radius, center_radius));
            MVP = Projection * View * cone_right_1;
            shader.SetUniformMat4("u_MVP", MVP);


            shader.SetUniform4f("u_Color", 0.3f, 0.0f, 0.8f, .3f);
            VertexBuffer vbconeright_1(cone_vertexes, 5 * 3 * sizeof(float));
            VertexBufferLayout layout_cone_right_1;
            layout_cone_right_1.Push<float>(3);
            vbconeright_1.Bind();
            va.addBUffer(vbconeright_1, layout_cone_right_1);
            IndexBuffer ibconeright_1(cone_indices, cone_indices_length);
            ibconeright_1.Bind();
            GLCall(glDrawElements(GL_TRIANGLES, cone_indices_length, GL_UNSIGNED_INT, nullptr));
        }


        //solaire System --------------------------------------------------------------------------------
        //murcury path 
        float d = 2.0f;
        float mr_x = d * sin(mercury_angle);
        float mr_z = d * cos(mercury_angle);
        glm::mat4 mercury_mat = glm::translate(Model, glm::vec3(mr_x, 0.0f, mr_z));
        //mercury_mat = glm::rotate(mercury_mat, glm::radians(mercury_angle), glm::vec3(0, 1, 0)); // it turns on it self
        mercury_mat = glm::scale(mercury_mat, glm::vec3(0.10f, 0.10f, 0.1f));
        MVP = Projection * View * mercury_mat;
        shader.SetUniformMat4("u_MVP", MVP);

        //drawing Mercury 
        shader.SetUniform4f("u_Color", 0.48f, .32f, 0.14f, 1.0f);
        //drawing ball
        VertexBuffer vbMercury(sphere_vertexes, (vertices_length) * sizeof(float));
        VertexBufferLayout layout_mercury;
        layout_mercury.Push<float>(3);
        vbMercury.Bind();
        va.addBUffer(vbMercury, layout_mercury);
        IndexBuffer ibMercury(sphere_indices, sphere_indices_length);
        ibMercury.Bind();
        GLCall(glDrawElements(GL_TRIANGLES, sphere_indices_length, GL_UNSIGNED_INT, (void*)0));


        //drawing VENUS 
        float venus_d = 3.0f;
        float venus_x = venus_d * sin(venus_angle);
        float venus_z = venus_d * cos(venus_angle);
        glm::mat4 venus_mat = glm::translate(Model, glm::vec3(venus_x, 0.0f,venus_z));
        venus_mat = glm::rotate(venus_mat, glm::radians(venus_speed), glm::vec3(0, 1, 0));
        venus_mat = glm::scale(venus_mat, glm::vec3(0.15f, 0.15f, 0.15f));
        MVP = Projection * View * venus_mat;
        shader.SetUniformMat4("u_MVP", MVP);



        //drawing VENUS 
        shader.SetUniform4f("u_Color", 0.68f, .36f, 0.07f, 1.0f);
        //drawing ball
        VertexBuffer vbVenus(sphere_vertexes, (vertices_length) * sizeof(float));
        VertexBufferLayout layout_Venus;
        layout_Venus.Push<float>(3);
        vbVenus.Bind();
        va.addBUffer(vbVenus, layout_mercury);
        IndexBuffer ibVenus(sphere_indices, sphere_indices_length);
        ibVenus.Bind();
        GLCall(glDrawElements(GL_TRIANGLES, sphere_indices_length, GL_UNSIGNED_INT, (void*)0));


        //drawing EARTH 
        float earth_d = 4.0f;
        float earth_x = earth_d * sin(earth_angle);
        float earth_z = earth_d * cos(earth_angle);
        glm::mat4 earth_mat = glm::translate(Model, glm::vec3(earth_x, 0.0f, earth_z));
        earth_mat = glm::rotate(earth_mat, glm::radians(earth_angle), glm::vec3(0, 1, 0));
        earth_mat = glm::scale(earth_mat, glm::vec3(0.20f, 0.20f, 0.20f));
        MVP = Projection * View * earth_mat;
        shader.SetUniformMat4("u_MVP", MVP);



        //drawing 
        shader.SetUniform4f("u_Color", 0.03f, 0.73f, 0.86f, 1.0f);
        //drawing ball
        VertexBuffer vbEarth(sphere_vertexes, (vertices_length) * sizeof(float));
        VertexBufferLayout layout_Earth;
        layout_Earth.Push<float>(3);
        vbEarth.Bind();
        va.addBUffer(vbEarth, layout_Earth);
        IndexBuffer ibEarth(sphere_indices, sphere_indices_length);
        ibEarth.Bind();
        GLCall(glDrawElements(GL_TRIANGLES, sphere_indices_length, GL_UNSIGNED_INT, (void*)0));


        //drawing earth moon
        
        float moon_d = 3.0f;
        float moon_x = moon_d * sin(moon_angle);
        float moon_z = moon_d * cos(moon_angle);
        glm::mat4 moon_mat = glm::translate(earth_mat, glm::vec3(moon_x, 0.0f, moon_z));
        moon_mat = glm::rotate(moon_mat, glm::radians(earth_angle), glm::vec3(0, 1, 0)); // turns on himeself
        moon_mat = glm::scale(moon_mat, glm::vec3(0.20f, 0.20f, .20f));
        MVP = Projection * View * moon_mat;
        shader.SetUniformMat4("u_MVP", MVP);
        //drawing 
        shader.SetUniform4f("u_Color", 0.52f, 0.84f, 0.65f, 1.0f);
        //drawing ball
        VertexBuffer vbMoon(sphere_vertexes, (vertices_length) * sizeof(float));
        VertexBufferLayout layout_Moon;
        layout_Moon.Push<float>(3);
        vbMoon.Bind();
        va.addBUffer(vbMoon, layout_Moon);
        IndexBuffer ibMoon(sphere_indices, sphere_indices_length);
        ibMoon.Bind();
        GLCall(glDrawElements(GL_TRIANGLES, sphere_indices_length, GL_UNSIGNED_INT, (void*)0));


        //going back to original matrix
        MVP = Projection * View * Model;
        shader.SetUniformMat4("u_MVP", MVP);

        r += incr;
        backg_r += 0.01f;
        center_sphere_r += center_sphere_r_speed;
        
        cones_diagonals_rotation += cones_diagonals_rotation_speed;
        
        cones_dr += cones_dr_speed;
        cones_dl += cones_dl_speed;

        cone1_r += cone1_speed;
        cone2_r += cone2_speed;
        cone3_r += cone3_speed;
        cone4_r += cone4_speed;

        if (r > 0.5f || r < 0.0f)
            incr =  - incr ;
        if (backg_r > 360.f)
            backg_r = 0.0f;
        if (center_sphere_r > 360.f)
            center_sphere_r = 0.0f;
        if (cones_dr > 360.f)
            cones_dr = 0.0f;
        if (cones_dl > 360.f)
            cones_dl = 0.0f;
        

        //planets paths 
        mercury_angle += mercury_speed/2;
        venus_angle += venus_speed/2;
        earth_angle += earth_speed/2;
        moon_angle += moon_speed/2;
        ////drawing square
        //shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f)
        //VertexBuffer vb(positions, 6 * 4 * sizeof(float));
        //VertexBufferLayout layout;
        //layout.Push<float>(3);
        //vb.Bind();
        //va.addBUffer(vb, layout);
        //IndexBuffer ib(indices, 6);
        //ib.Bind();
        //GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        
        

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    

    glfwTerminate();
    return 0;





}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    
    if (key == GLFW_KEY_E && action == GLFW_PRESS)
        std::cout << "pess" + key << std::endl;
    if (action == GLFW_PRESS)
        switch (key) {
        case GLFW_KEY_W:
            //scale up
            //scale += 0.1f;
            //camPos += camSpeed * camDirection;
            camPos = glm::vec3(camPos.x, camPos.y + camSpeed, camPos.z);
            break;
        case GLFW_KEY_S:
            //scale down 
             //scale -= 0.1f;
             //camPos -= camSpeed * camDirection;
            camPos = glm::vec3(camPos.x, camPos.y - camSpeed, camPos.z);
            break;
        case GLFW_KEY_D:
            //scale down 
             //scale -= 0.1f;
             //camPos -= camSpeed * camDirection;
            camPos = glm::vec3(camPos.x + camSpeed, camPos.y, camPos.z);
            break;
        case GLFW_KEY_A:
            //scale down 
             //scale -= 0.1f;
             //camPos -= camSpeed * camDirection;
            camPos = glm::vec3(camPos.x - camSpeed, camPos.y, camPos.z);
            break;
        case GLFW_KEY_Y:
            /* rotate around X */ 
            //rotate_x += 1.0f;
            axis_rotation = 0;
            std::cout << "rotation X " << rotate_x << std::endl;
            break;
        case GLFW_KEY_U:
            /* rotate around Y */
            //rotate_y += 1.0f;
            axis_rotation = 1;
            std::cout << "rotation Y " << rotate_y << std::endl;
            break;
        case GLFW_KEY_I:
            /* rotate around Z */
            //rotate_z += 1.0f;
            axis_rotation = 2;
            std::cout << "rotation Z " << rotate_z << std::endl;
            break;
        case GLFW_KEY_DOWN:
            center_sphere_r_speed -= 0.1f;
            break;
        case GLFW_KEY_UP:
            center_sphere_r_speed += 0.1f;
            break;
        case GLFW_KEY_RIGHT:
            cones_diagonals_rotation_speed += 0.1f;
            break;
        case GLFW_KEY_LEFT:
            cones_diagonals_rotation_speed -= 0.1f;
            break;
        case GLFW_KEY_L:
            ambient += 0.1f;
            break;
        case GLFW_KEY_K:
            ambient -= 0.1f;
            break;
        case GLFW_KEY_Z:
            if (show_shapes == 0) {
                show_shapes = 1;
                break;
            }
            show_shapes = 0;
            break;
        //case GLFW_KEY_X:
        //    /* rotate around Z */
        //    mouseSpeed += 0.1f;
        //    std::cout << "mouse speed " << mouseSpeed << std::endl;
        //    break;
        //case GLFW_KEY_Z:
        //    /* rotate around Z */
        //    mouseSpeed -= 0.1f;
        //    std::cout << "mouse speed  " << mouseSpeed << std::endl;
        //    break;
        case GLFW_KEY_Q: /* Escape */ exit(0);
            break;
        default:
            axis_rotation = -1;
            break;

        }
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    std::cout << xpos << " : " << ypos << " ;::::" << std::endl;
    double new_xpos, new_ypos;
    glfwGetCursorPos(window, &new_xpos, &new_ypos);
    int move = -1;
    
    int stateL = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (stateL == GLFW_PRESS)
    {   
       /* if ((new_xpos - xpos) > 0) {
            //std::cout << " CLICKED " << std::endl;
            move = 0;
            rotate_y += mouseSpeed;
        }
        if ((new_xpos - xpos) < 0) {
            //std::cout << " CLICKED " << std::endl
            move = 1;
            rotate_y -= mouseSpeed;
        }
        if ((new_ypos - ypos) > 0) {
            //std::cout << " CLICKED " << std::endl;
            rotate_x += mouseSpeed;

            move = 2;
        }
        if ((new_ypos - ypos) < 0) {
            //std::cout << " CLICKED " << std::endl;
            
            rotate_x -= mouseSpeed;
            move = 3;
        }*/
        switch (axis_rotation)
        {
        case 0:
            rotate_x += mouseSpeed;
            break;
        case 1:
            rotate_y += mouseSpeed;
            break;
        case 2:
            rotate_z += mouseSpeed;
            break;

        default:
            break;
        }
        std::cout << "rotation X " << rotate_x << std::endl;
        std::cout << "rotation Y " << rotate_y << std::endl;
        std::cout << "rotation Z " << rotate_z << std::endl;
        
        /*   camera_posx += 1.0f;
           angle += 0.1f;
           camPos += camSpeed * camDirection;*/

    }
    int stateR = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
    if (stateR == GLFW_PRESS)
    {
        //std::cout << " CLICKED " << std::endl;
        scale += 0.01f;
        std::cout << "scale  " << scale<< std::endl;

        /*   camera_posx += 1.0f;
           angle += 0.1f;
           camPos += camSpeed * camDirection;*/

    }
    int stateM = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
    if (stateM == GLFW_PRESS)
    {
        //std::cout << " CLICKED " << std::endl;
        scale -= 0.01f;
        if (scale < 0)
            scale = 0.1f;
        std::cout << "scale  " << scale << std::endl;

        /*   camera_posx += 1.0f;
           angle += 0.1f;
           camPos += camSpeed * camDirection;*/

    }


}