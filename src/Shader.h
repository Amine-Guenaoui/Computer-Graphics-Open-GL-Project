
#include <string>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;

};

class Shader
{
private:
		std::string m_FilePath;
		unsigned int m_RendererID;
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void UnBind() const;

	// set Uniforms
	void SetUniform1i(const std::string& name, int val);
	void SetUniform1f(const std::string& name, float val);
	void SetUniformMat4(const std::string& name, glm::mat4 f0);
	void SetUniform4f(const std::string& name, float v0, float v1, float f2, float f3);
	void SetUniform3f(const std::string& name, float f0, float f1, float f2);
private:
	ShaderProgramSource ParseShader(const std::string& filePath);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmenttShader);
	unsigned int CompileShader(const std::string& source, unsigned int type);
	int GetUniformLocation(const std::string& name);

};