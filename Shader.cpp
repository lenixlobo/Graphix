#include "Shader.h"



Shader::Shader(const char* vertexpath,const char* fragmentpath){

	std::string vertexcode;
	std::string fragmentcode;
	std::ifstream vShaderFile;
	std::ifstream fragmentFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vShaderFile.open(vertexpath,std::ios::in);
		fragmentFile.open(fragmentpath,std::ios::in);

		std::stringstream vShaderStream, vFragmentStream;

		vShaderStream << vShaderFile.rdbuf();
		vFragmentStream << fragmentFile.rdbuf();

		vShaderFile.close();
		fragmentFile.close();

		vertexcode = vShaderStream.str();
		fragmentcode = vFragmentStream.str();

	}
	catch(std::ifstream::failure e)
	{
		std::cout<<"ERROR :: SHADER :: FILE NOT READ "<<std::endl;
	}

	const char* vertexshader = vertexcode.c_str();
	const char* fragmentshader = fragmentcode.c_str();

	unsigned int vertexID, fragmentID;
	int success;
	char infoLog[512];

	//vertex Shader
	vertexID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexID,1,&vertexshader,NULL);
	glCompileShader(vertexID);
	glGetShaderiv(vertexID,GL_COMPILE_STATUS,&success);
	if (!success) {
		glGetShaderInfoLog(vertexID,512,NULL,infoLog);
		std::cout << "ERROR :: SHADER :: VERTEX :: COMPILATION FAILED \n" <<infoLog<< std::endl;
	}
	fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentID,1,&fragmentshader,NULL);
	glCompileShader(fragmentID);
	glGetShaderiv(fragmentID,GL_COMPILE_STATUS,&success);
	if (!success) {
		glGetShaderInfoLog(fragmentID,512,NULL,infoLog);
		std::cout << "ERROR :: SHADER :: FRAGMENT :: COMPILATION FAILED \n"<<infoLog << std::endl;
	}
	
	ID = glCreateProgram();
	glAttachShader(ID, vertexID);
	glAttachShader(ID, fragmentID);
	glLinkProgram(ID);

	glGetProgramiv(ID,GL_LINK_STATUS,&success);
	if (!success) {
		glGetProgramInfoLog(ID,512,NULL,infoLog);
		std::cout << "ERROR :: PROGRAM :: LINKING FAILED \n" << infoLog << std::endl;
	}

	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
	
}


Shader::~Shader()
{
}

void Shader::use(){
	glUseProgram(ID);
}

void Shader::setBool(const std::string &name,bool value) const{
	glUniform1i(glGetUniformLocation(ID,name.c_str()),(int)value);
}

void Shader::setInt(const std::string &name, int value) const{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const{
	glUniform1i(glGetUniformLocation(ID,name.c_str()),value);
}

void Shader::setMat4(const std::string &name,glm::mat4 &mat) const{
	glUniformMatrix4fv(glGetUniformLocation(ID,name.c_str()),1,GL_FALSE,&mat[0][0]);
}
