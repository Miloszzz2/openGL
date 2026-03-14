#include "Program.h"

#include <fstream>
#include <cstdio>

GLuint ProgramMPGK::VAO;
GLuint ProgramMPGK::VBO;
GLuint ProgramMPGK::IBO;
GLuint ProgramMPGK::programZShaderami;
GLuint ProgramMPGK::vertexShaderId;
GLuint ProgramMPGK::fragmentShaderId;
GLint ProgramMPGK::zmiennaShader;


ProgramMPGK::ProgramMPGK(void)
{
	wysokoscOkna = 768;
	szerokoscOkna = 1024;
	polozenieOknaX = 100;
	polozenieOknaY = 100;
}


ProgramMPGK::ProgramMPGK(int wysokoscOkna, int szerokoscOkna, int polozenieOknaX, int polozenieOknaY)
{
	this->wysokoscOkna = wysokoscOkna;
	this->szerokoscOkna = szerokoscOkna;
	this->polozenieOknaX = polozenieOknaX;
	this->polozenieOknaY = polozenieOknaY;
}


ProgramMPGK::~ProgramMPGK()
{
}


void ProgramMPGK::stworzenieOkna(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(szerokoscOkna, wysokoscOkna);
	glutInitWindowPosition(polozenieOknaX, polozenieOknaY);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow("Program");
}


void ProgramMPGK::inicjalizacjaGlew()
{
	GLenum wynik = glewInit();
	if (wynik != GLEW_OK)
	{
		std::cerr << "Nie udalo sie zainicjalizowac GLEW. Blad: " << glewGetErrorString(wynik) << std::endl;
		exit(1);
	}
}


void  ProgramMPGK::wyswietl()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	static GLfloat zmiana = 0.0f;

	GLfloat kierunek = abs(sinf(zmiana));
	zmiana += 0.0005f;

	GLfloat mJ[4][4];

	mJ[0][0] = kierunek; mJ[0][1] = 0.0f;		mJ[0][2] = 0.0f;  mJ[0][3] = 0.0f;
	mJ[1][0] = 0.0f;	 mJ[1][1] = kierunek;	mJ[1][2] = 0.0f;  mJ[1][3] = 0.0f;
	mJ[2][0] = 0.0f;	 mJ[2][1] = 0.0f;		mJ[2][2] = 1.0f;  mJ[2][3] = 0.0f;
	mJ[3][0] = 0.0f;	 mJ[3][1] = 0.0f;		mJ[3][2] = 0.0f;  mJ[3][3] = 1.0f;

	glUniformMatrix4fv(macierzShader, 1, GL_TRUE, &mJ[0][0]);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (GLvoid*)(sizeof(GLfloat) * 4));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glutSwapBuffers();
}


void ProgramMPGK::usun()
{
	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);
	glDeleteProgram(programZShaderami);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);
	glDeleteVertexArrays(1, &VAO);
}


void ProgramMPGK::stworzenieVAO()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
}


void ProgramMPGK::stworzenieVBO()
{
	GLfloat wierzcholki[] = {
		-0.4f, -0.4f, 0.0f, 1.0f,
		 1.0f,  0.0f, 0.0f, 1.0f,
		 0.4f, -0.4f, 0.4f, 1.0f,
		 0.0f,  1.0f, 0.0f, 1.0f,
		-0.4f,  0.4f, 0.0f, 1.0f,
		 0.0f,  0.0f, 1.0f, 1.0f,
		 0.4f,  0.4f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 1.0f
	};

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wierzcholki), wierzcholki, GL_STATIC_DRAW);
}


void ProgramMPGK::stworzenieIBO()
{
	GLuint indeksyTab[] = {
		0, 1, 2, 1, 2, 3,
	};

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeksyTab), indeksyTab, GL_STATIC_DRAW);
}

// Odczyt - wersja fstream
std::string ProgramMPGK::wczytajFstream(const std::string& nazwaPliku, const std::string& sciezka) {
    std::string pelnaSciezka = sciezka + "/" + nazwaPliku;
    std::ifstream plik(pelnaSciezka.c_str()); // .c_str() dla lepszej kompatybilności
    std::string tresc = "";
    std::string linia = "";

    if (plik.is_open()) {
        while (getline(plik, linia)) {
            tresc += linia + "\n"; // Doklejamy linię i enter
        }
        plik.close();
    }
    return tresc;
}

// Zapis - wersja fstream
void ProgramMPGK::zapiszFstream(const std::string& nazwaPliku, const std::string& sciezka, const std::string& tresc) {
    std::string pelnaSciezka = sciezka + "/" + nazwaPliku;
    std::ofstream plik(pelnaSciezka.c_str());
    if (plik.is_open()) {
        plik << tresc;
        plik.close();
    }
}

// Odczyt - wersja FILE*
std::string ProgramMPGK::wczytajFILE(const std::string& nazwaPliku, const std::string& sciezka) {
    std::string pelnaSciezka = sciezka + "/" + nazwaPliku;
    FILE* plik = fopen(pelnaSciezka.c_str(), "r");
    std::string tresc = "";

    if (plik != NULL) {
        char znak;
        while ((znak = fgetc(plik)) != EOF) { // Czytaj aż do końca pliku (End Of File)
            tresc += znak;
        }
        fclose(plik);
    }
    return tresc;
}

// Zapis - wersja FILE*
void ProgramMPGK::zapiszFILE(const std::string& nazwaPliku, const std::string& sciezka, const std::string& tresc) {
    std::string pelnaSciezka = sciezka + "/" + nazwaPliku;
    FILE* plik = fopen(pelnaSciezka.c_str(), "w");
    if (plik != NULL) {
        fprintf(plik, "%s", tresc.c_str());
        fclose(plik);
    }
}

void ProgramMPGK::stworzenieProgramu()
{
	programZShaderami = glCreateProgram();

	if (programZShaderami == 0)
	{
		std::cerr << "Blad podczas tworzenia programu shaderow." << std::endl;
		//system("pause");
		exit(1);
	}

	std::string vShaderSource = wczytajFstream("vertexShader.glsl", ".");
    std::string fShaderSource = wczytajFILE("fragmentShader.glsl", ".");

    const GLchar* vertexShader = vShaderSource.c_str();
    const GLchar* fragmentShader = fShaderSource.c_str();

	vertexShaderId = dodanieDoProgramu(programZShaderami, vertexShader, GL_VERTEX_SHADER);
	fragmentShaderId = dodanieDoProgramu(programZShaderami, fragmentShader, GL_FRAGMENT_SHADER);

	GLint linkowanieOK = 0;
	glLinkProgram(programZShaderami);
	glGetProgramiv(programZShaderami, GL_LINK_STATUS, &linkowanieOK);
	if (linkowanieOK == GL_FALSE) {
		GLint dlugoscLoga = 0;
		glGetProgramiv(programZShaderami, GL_INFO_LOG_LENGTH, &dlugoscLoga);
		std::vector<GLchar> log(dlugoscLoga);
		glGetProgramInfoLog(programZShaderami, dlugoscLoga, NULL, &log[0]);
		std::cerr << "Blad podczas linkowania programu shaderow." << std::endl;
		for (std::vector<GLchar>::const_iterator i = log.begin(); i != log.end(); ++i)
			std::cerr << *i;
		std::cerr << std::endl;
		glDeleteProgram(programZShaderami);
		//system("pause");
		exit(1);
	}

	GLint walidacjaOK = 0;
	glValidateProgram(programZShaderami);
	glGetProgramiv(programZShaderami, GL_VALIDATE_STATUS, &walidacjaOK);
	if (walidacjaOK == GL_FALSE) {
		GLint dlugoscLoga = 0;
		glGetProgramiv(programZShaderami, GL_INFO_LOG_LENGTH, &dlugoscLoga);
		std::vector<GLchar> log(dlugoscLoga);
		glGetProgramInfoLog(programZShaderami, dlugoscLoga, NULL, &log[0]);
		std::cerr << "Blad podczas walidacji programu shaderow." << std::endl;
		for (std::vector<GLchar>::const_iterator i = log.begin(); i != log.end(); ++i)
			std::cerr << *i;
		std::cerr << std::endl;
		glDeleteProgram(programZShaderami);
		//system("pause");
		exit(1);
	}

	glUseProgram(programZShaderami);

	macierzShader = glGetUniformLocation(programZShaderami, "macierz");
	if (macierzShader == -1)
	{
		std::cerr << "Nie znaleziono zmiennej uniform." << std::endl;
		system("pause");
		exit(1);
	}
}


GLuint ProgramMPGK::dodanieDoProgramu(GLuint programZShaderami, const GLchar* tekstShadera, GLenum typShadera)
{
	GLuint shader = glCreateShader(typShadera);

	// 35633 -> vertex shader, 35632 -> fragment shader
	const GLchar* typShaderaTekst = typShadera == 35633 ? "vertex" : "fragment";

	if (shader == 0) {
		std::cerr << "Blad podczas tworzenia " << typShaderaTekst << " shadera." << std::endl;
		//system("pause");
		exit(0);
	}

	const GLchar* tekstShaderaTab[1];
	tekstShaderaTab[0] = tekstShadera;
	GLint dlugoscShadera[1];
	dlugoscShadera[0] = strlen(tekstShadera);
	glShaderSource(shader, 1, tekstShaderaTab, dlugoscShadera);

	glCompileShader(shader);
	GLint kompilacjaOK = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &kompilacjaOK);
	if (kompilacjaOK == GL_FALSE)
	{
		GLint dlugoscLoga = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &dlugoscLoga);
		std::vector<GLchar> log(dlugoscLoga);
		glGetShaderInfoLog(shader, dlugoscLoga, NULL, &log[0]);
		std::cerr << std::endl;
		std::cerr << "Blad podczas kompilacji " << typShaderaTekst << " shadera." << std::endl;
		std::cerr << std::endl;
		std::cerr << "log: ";
		for (std::vector<GLchar>::const_iterator i = log.begin(); i != log.end(); ++i)
			std::cerr << *i;
		std::cerr << std::endl;
		glDeleteShader(shader);
		//system("pause");
		exit(1);
	}

	glAttachShader(programZShaderami, shader);
	return shader;
}

void ProgramMPGK::sprawdzenieWersji()
{
	std::cout << "Wersja GLEW: " << glewGetString(GLEW_VERSION) << std::endl;
	std::cout << "Wersja VENDOR: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Wersja REDERER: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Wersja GL: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Wersja GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}


int main(int argc, char** argv)
{
	ProgramMPGK obiektMPGK(786, 786, 100, 100);

	obiektMPGK.stworzenieOkna(argc, argv);
	obiektMPGK.inicjalizacjaGlew();
	obiektMPGK.sprawdzenieWersji();
	obiektMPGK.stworzenieVAO();
	obiektMPGK.stworzenieVBO();
	obiektMPGK.stworzenieIBO();
	obiektMPGK.stworzenieProgramu();
	glutDisplayFunc(obiektMPGK.wyswietl);
	glutIdleFunc(obiektMPGK.wyswietl);
	glutCloseFunc(obiektMPGK.usun);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glutMainLoop();

	//system("pause");
	return 0;
}
