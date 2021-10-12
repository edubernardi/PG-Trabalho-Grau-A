/* Trabalho de Grau A - 2021/2
 * Eduardo Bernardi
 * 
 * Utilizado Framework desenvolvido pela professora Rossana Baptista Queiroz
 * para a disciplina de Processamento Gr�fico - Jogos Digitais - Unisinos
 *
 */

#include <iostream>
#include <string>
#include <assert.h>
#include <cmath>


using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

#include "Shader.h"
#include "Object.h"
#include "Sprite.h"


// Prot�tipo da fun��o de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Prot�tipos das fun��es
int loadTexture(string path);

// Dimens�es da janela (pode ser alterado em tempo de execu��o)
const GLuint WIDTH = 800, HEIGHT = 600;

int playerPositionX = 400;
int playerPositionY = 180;
bool playerFacingRight = true;
bool coinsTaken[2];

// Fun��o MAIN
int main()
{
	// Inicializa��o da GLFW
	glfwInit();

	//Muita aten��o aqui: alguns ambientes n�o aceitam essas configura��es
	//Voc� deve adaptar para a vers�o do OpenGL suportada por sua placa
	//Sugest�o: comente essas linhas de c�digo para desobrir a vers�o e
	//depois atualize (por exemplo: 4.5 com 4 e 5)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Essencial para computadores da Apple
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Cria��o da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hello PG Framework!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da fun��o de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d fun��es da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informa��es de vers�o
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Compilando e buildando o programa de shader
	//Shader* shader = new Shader("./shaders/sprite.vs", "./shaders/sprite.fs");
	//Shader* sprShader = new Shader("./shaders/sprite.vs", "./shaders/animatedsprites.fs");
	Shader* shader = new Shader("./shaders/sprite.vs", "./shaders/animatedsprites.fs");

	GLuint backgroundTex = loadTexture("./textures/background.jpg");
	GLuint playerTextureRight = loadTexture("./textures/playerA.png");
	GLuint playerTextureLeft = loadTexture("./textures/playerB.png");
	GLuint coinTexture = loadTexture("./textures/moeda.png");
	GLuint yoshiTex = loadTexture("./textures/yoshi.png");

	Sprite yoshi;
	yoshi.setSpritesheet(yoshiTex, 2, 8);
	yoshi.setPosition(glm::vec3(100, 100, 0));
	yoshi.setDimention(glm::vec3(100, 100, 1));
	yoshi.setShader(shader);

	Sprite coins[2];

	for (int i = 0; i < 2; i++) {
		coins[i].setSpritesheet(coinTexture, 1, 1);
		coins[i].setDimention(glm::vec3(80, 80, 1.0));
		coins[i].setShader(shader);
	}
	coins[0].setPosition(glm::vec3(50, 150, 0));
	coins[1].setPosition(glm::vec3(600, 200, 0));
	
	Sprite player;
	player.setSpritesheet(playerTextureRight, 1, 1);
	player.setPosition(glm::vec3(playerPositionX, playerPositionY, 0));
	player.setDimention(glm::vec3(65, 138, 1.0));
	player.setShader(shader);

	Sprite background;
	background.setSpritesheet(backgroundTex, 1, 1);
	background.setPosition(glm::vec3(400, 300, 0));
	background.setDimention(glm::vec3(800, 600, 1.0));
	background.setShader(shader);

	GLint projLoc = glGetUniformLocation(shader->Program, "projection");
	assert(projLoc > -1);

	glm::mat4 ortho = glm::mat4(1); //inicializa com a matriz identidade

	double xmin = 0.0, xmax = 800.0, ymin = 0.0, ymax = 600.0;

	// Loop da aplica��o - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as fun��es de callback correspondentes
		glfwPollEvents();

		// Definindo as dimens�es da viewport com as mesmas dimens�es da janela da aplica��o
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		ortho = glm::ortho(xmin, xmax, ymin, ymax, -1.0, 1.0);

		//Enviar a matriz de proje��o ortogr�fica para o shader
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(ortho));

		// Limpa o buffer de cor
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(10);

		background.update();
		background.draw();
		
		//verificar posi��o e atualizar
		if (playerPositionX > 800) {
			playerPositionX -= 30;
			playerFacingRight = false;
		}
		if (playerPositionX < 0) {
			playerPositionX += 30;
			playerFacingRight = true;
		}

		if (playerFacingRight) {
			player.setTexture(playerTextureRight);
		}
		else {
			player.setTexture(playerTextureLeft);
		}

		//verificar colisao
		for (int i = 0; i < 2; i++) {
			coins[i].setAngle(glfwGetTime());
			if (player.getBottomLeftVertex().x < coins[i].getTopRightVertex().x && player.getTopRightVertex().x > coins[i].getBottomLeftVertex().x) {
				coinsTaken[i] = true;
			}
			if (!coinsTaken[i]) {
				coins[i].update();
				coins[i].draw();
			}
		}

		yoshi.update();
		yoshi.draw();
		
		player.setPosition(glm::vec3(playerPositionX, playerPositionY, 0));
		player.update();
		player.draw();

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Finaliza a execu��o da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Fun��o de callback de teclado - s� pode ter uma inst�ncia (deve ser est�tica se
// estiver dentro de uma classe) - � chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		playerPositionX += 5;
		playerFacingRight = true;
	}
	
	if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		playerPositionX -= 5;
		playerFacingRight = false;
	}

}

int loadTexture(string path)
{
	GLuint texID;

	// Gera o identificador da textura na mem�ria 
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	//Ajusta os par�metros de wrapping e filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Carregamento da imagem
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}
