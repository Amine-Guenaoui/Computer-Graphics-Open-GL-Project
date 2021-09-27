
#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include "VertexArray.h"
#include "IndexBuffer.h"





#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x,__FILE__,__LINE__));

void GLClearError();

// GLCheckError
bool GLLogCall(const char* function, const char* file, int line);


//class Renderer
//{
//public:
//    void Draw(const VertexArray& va, const IndexBuffer& ib,const Shader& shader) ;
//
//
//};