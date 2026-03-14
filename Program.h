#pragma once

#include <string.h>
#include <string>
#include <math.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>

GLuint macierzShader;

class ProgramMPGK
{
private:
	int wysokoscOkna;
	int szerokoscOkna;
	int polozenieOknaX;
	int polozenieOknaY;
	static GLuint VAO;
	static GLuint VBO;
	static GLuint IBO;
	static GLuint programZShaderami;
	static GLuint vertexShaderId;
	static GLuint fragmentShaderId;
	static GLint zmiennaShader;

public:
	ProgramMPGK(void);
	ProgramMPGK(int wysokoscOkna, int szerokoscOkna, int polozenieOknaX, int polozenieOknaY);
	~ProgramMPGK(void);
	void stworzenieOkna(int argc, char** argv);
	void inicjalizacjaGlew();
	static void wyswietl();
	static void usun();
	void stworzenieVAO();
	void stworzenieVBO();
	void stworzenieIBO();
	void stworzenieProgramu();
	GLuint dodanieDoProgramu(GLuint programZShaderami, const GLchar* tekstShadera, GLenum typShadera);
	void sprawdzenieWersji();

	std::string wczytajFstream(const std::string& nazwaPliku, const std::string& sciezka);
    void zapiszFstream(const std::string& nazwaPliku, const std::string& sciezka, const std::string& tresc);

    std::string wczytajFILE(const std::string& nazwaPliku, const std::string& sciezka);
    void zapiszFILE(const std::string& nazwaPliku, const std::string& sciezka, const std::string& tresc);
};
