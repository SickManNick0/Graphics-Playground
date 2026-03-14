#pragma once
#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
class Shaders {

public:

	std::string shaderStringVERT,shaderStringFrag;

	GLuint SHADERprogram;
	GLuint vertexShader, fragmentShader;


	void compilingShaders()
	{
		
			vertexShader = glCreateShader(GL_VERTEX_SHADER);

			const GLchar* shaderCodeVERT[1];
			shaderCodeVERT[0] = shaderStringVERT.c_str();

			glShaderSource(vertexShader, 1, shaderCodeVERT, NULL);

			glCompileShader(vertexShader);


			GLint result = 0;
			GLchar elog[1024] = { 0 };
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);

			if (!result) {
				glGetShaderInfoLog(vertexShader, sizeof(elog), NULL, elog);
				printf("Error while compiling the vertex shader : '%s'", elog);
				return;
			}
		
		
			fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

			const GLchar* shaderCodeFRAG[1];
			shaderCodeFRAG[0] = shaderStringFrag.c_str();

			glShaderSource(fragmentShader, 1, shaderCodeFRAG, NULL);

			glCompileShader(fragmentShader);

			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);

			if (!result) {
				glGetShaderInfoLog(fragmentShader, sizeof(elog), NULL, elog);
				printf("Error while compiling the fragment shader : '%s'", elog);
				return;
			}
		
	}

	void creatingSHDRprogram() {
		SHADERprogram = glCreateProgram();

		if (!SHADERprogram)
		{
			printf("Error while creating shader Program!\n");
			return;
		}

		

		GLchar elog[1024] = { 0 };
		GLint result = 0;

		glLinkProgram(SHADERprogram);
		glGetProgramiv(SHADERprogram, GL_LINK_STATUS, &result);

		if (!result)
		{
			glGetProgramInfoLog(SHADERprogram, sizeof(elog), NULL, elog);
			printf("Error while linking shader program! : '%s'", elog);
			return;
		}

		glValidateProgram(SHADERprogram);
		glGetProgramiv(SHADERprogram, GL_VALIDATE_STATUS, &result);

		if (!result)
		{
			glGetProgramInfoLog(SHADERprogram, sizeof(elog), NULL, elog);
			printf("Error while validating shader program! : '%s'", elog);
			return;
		}

		glAttachShader(SHADERprogram, vertexShader);
		glAttachShader(SHADERprogram, fragmentShader);
	}





	Shaders(std::string VERTfilePath, std::string fragFILEPATH)
	{
		std::ifstream file(VERTfilePath);
		std::ifstream file2(fragFILEPATH);
		std::stringstream bufferORbucketOFdata;
		std::stringstream bufferORbucketOFdata2;

		if (file.is_open())
		{
			bufferORbucketOFdata << file.rdbuf();

			shaderStringVERT = bufferORbucketOFdata.str();

			file.close();
		}
		else {
			printf("Error while openning the vertex shader file!\n");
		}

		if (file2.is_open())
		{
			bufferORbucketOFdata2 << file2.rdbuf();

			shaderStringFrag = bufferORbucketOFdata2.str();

			file2.close();
		}

		else {
			printf("Error while openning the fragment shader file!\n");
		}

		compilingShaders();
		creatingSHDRprogram();
	}
};












#endif // !SHADER_H
