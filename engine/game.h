
#ifndef GAME_H
#define GAME_H
#define GLEW_STATIC
#include <GL/glew.h>
#include "camera.h"
#include <GLFW/glfw3.h>
#include <map>
#include "square_renderer.h"
#include "post_processor.h"

#define MC_W 11
#define MC_H 11

// Represents the current state of the game
enum GameState {
	GAME_ROOM1,
	GAME_ROOM2,
	GAME_ROOM3,
	END
};

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
	// Game state
	GameState              State;
	GLboolean              Keys[1024], firstMouse;
	GLuint                 Width, Height;
	GLdouble			   xpos, ypos, lastX, lastY;
	//camera
	Camera *camera;
	// Constructor/Destructor
	Game(GLuint width, GLuint height);
	~Game();
	// Initialize game state (load all shaders/textures/levels)
	void Init();
	void DrawCube(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, std::vector<square_renderer*>&, GLint state);
	// GameLoop
	void ProcessInput(GLfloat dt);
	void ProcessMouseMovement(GLfloat dt);
	void ProcessScrollMovement(GLfloat yoffset);
	void Update(GLfloat dt);
	void Render();
	GLint check_collision(glm::vec3, glm::vec3);
private:
	std::vector<GLfloat*> trigger_square;
	//I use (A,B,C) to present a plane Ax + By + Cz = 1.
	std::vector<glm::mat4x3> planes[4];
	//	std::map<GameState, GameState> next_state;
	GLfloat stare_count;
	PostProcessor* Effects;
	GLfloat heightm[MC_W +10][MC_H + 10];
	void grab();
};

#endif
