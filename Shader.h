#pragma once
#include<glm\glm\glm.hpp>
#include<string>
#include<sstream>
#include<fstream>
#include<iostream>
#include<glad\glad.h>

class Shader
{
public:
	unsigned int ID;

	Shader(const char* vertexpath,const char* fragmentpath);
	void use();
	//utility uniform functions
	void setBool(const std::string &name,bool value) const;
	void setInt(const std::string &name,int value) const;
	void setFloat(const std::string &name,float value) const;
	void setMat4(const std::string &name,glm::mat4 &mat) const;
	void setVec3(const std::string &name,glm::vec3 &value) const;
	void setVec3(const std::string &name,float x,float y,float z) const;
	~Shader();
};

