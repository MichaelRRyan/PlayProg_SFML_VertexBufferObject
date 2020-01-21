#include <Game.h>

static bool flip;

Game::Game() : 
	window(VideoMode(800, 600), "OpenGL Cube VBO"),
	m_rotations{ 0.0f, 0.0f, 0.0f },
	m_translations{ 0.0f, 0.0f, 0.0f },
	m_scale{ 100.0f }
{
}

Game::~Game() {}

void Game::run()
{

	initialize();

	Event event;

	while (isRunning) 
	{
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				isRunning = false;
			}
		}
		update();
		render();
	}

}

typedef struct
{
	float coordinate[3];
	float color[3];
} Vertex;

Vertex baseVertices[8];

Vertex vertices[8];

GLubyte triangles[36]
{
	1, 5, 6,
	6, 2, 1,
	4, 0, 3,
	3, 7, 4,
	3, 2, 6,
	6, 7, 3,
	0, 4, 5,
	5, 1, 0,
	0, 1, 2,
	2, 3, 0,
	4, 7, 6,
	6, 5, 4
};

/* Variable to hold the VBO identifier */
GLuint vbo[1];
GLuint index;

void Game::initialize()
{
	isRunning = true;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, window.getSize().x / window.getSize().y, 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);

	glTranslatef(0.0f, 0.0f, -10.0f);
	glEnable(GL_CULL_FACE);

	glewInit();

	/* Vertices counter-clockwise winding */

	// Coordinates
	baseVertices[0].coordinate[0] = -1.0f;
	baseVertices[0].coordinate[1] = -1.0f;
	baseVertices[0].coordinate[2] = 1.0f;

	baseVertices[1].coordinate[0] = 1.0f;
	baseVertices[1].coordinate[1] = -1.0f;
	baseVertices[1].coordinate[2] = 1.0f;

	baseVertices[2].coordinate[0] = 1.0f;
	baseVertices[2].coordinate[1] = 1.0f;
	baseVertices[2].coordinate[2] = 1.0f;

	baseVertices[3].coordinate[0] = -1.0f;
	baseVertices[3].coordinate[1] = 1.0f;
	baseVertices[3].coordinate[2] = 1.0f;

	
	baseVertices[4].coordinate[0] = -1.0f;
	baseVertices[4].coordinate[1] = -1.0f;
	baseVertices[4].coordinate[2] = -1.0f;

	baseVertices[5].coordinate[0] = 1.0f;
	baseVertices[5].coordinate[1] = -1.0f;
	baseVertices[5].coordinate[2] = -1.0f;

	
	baseVertices[6].coordinate[0] = 1.0f;
	baseVertices[6].coordinate[1] = 1.0f;
	baseVertices[6].coordinate[2] = -1.0f;

	baseVertices[7].coordinate[0] = -1.0f;
	baseVertices[7].coordinate[1] = 1.0f;
	baseVertices[7].coordinate[2] = -1.0f;
	
	// Colours
	baseVertices[0].color[0] = 1.0f;
	baseVertices[0].color[1] = 0.0f;
	baseVertices[0].color[2] = 0.0f;

	baseVertices[1].color[0] = 0.0f;
	baseVertices[1].color[1] = 1.0f;
	baseVertices[1].color[2] = 0.0f;

	baseVertices[2].color[0] = 0.0f;
	baseVertices[2].color[1] = 0.0f;
	baseVertices[2].color[2] = 1.0f;

	baseVertices[3].color[0] = 0.4f;
	baseVertices[3].color[1] = 1.0f;
	baseVertices[3].color[2] = 0.0f;

	baseVertices[4].color[0] = 0.5f;
	baseVertices[4].color[1] = 1.0f;
	baseVertices[4].color[2] = 0.0f;

	baseVertices[5].color[0] = 0.6f;
	baseVertices[5].color[1] = 1.0f;
	baseVertices[5].color[2] = 0.0f;
	
	baseVertices[4].color[0] = 0.4f;
	baseVertices[4].color[1] = 1.0f;
	baseVertices[4].color[2] = 0.0f;

	baseVertices[5].color[0] = 0.5f;
	baseVertices[5].color[1] = 1.0f;
	baseVertices[5].color[2] = 0.0f;

	baseVertices[6].color[0] = 0.6f;
	baseVertices[6].color[1] = 1.0f;
	baseVertices[6].color[2] = 0.0f;

	baseVertices[7].color[0] = 0.5f;
	baseVertices[7].color[1] = 1.0f;
	baseVertices[7].color[2] = 0.0f;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			vertices[i].coordinate[j] = baseVertices[i].coordinate[j];
			vertices[i].color[j] = baseVertices[i].color[j];
		}
	}

	/* Create a new VBO using VBO id */
	glGenBuffers(1, vbo);

	/* Bind the VBO */
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	/* Upload vertex data to GPU */
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 36, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 36, triangles, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Game::update()
{
	elapsed = clock.getElapsedTime();

	if (elapsed.asSeconds() >= 1.0f / 60.0f)
	{
		clock.restart();

		// Keyboard Input
		// Scale
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
			m_scale += 10.0f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
			m_scale -= 10.0f;

		// Rotations
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
			m_rotations.x += 0.6f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
			m_rotations.y += 0.6f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
			m_rotations.z += 0.6f;

		// Translations
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			m_translations.x += 0.2f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			m_translations.x -= 0.2f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			m_translations.y += 0.2f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			m_translations.y -= 0.2f;

		// Create an identity matrix
		cube::Matrix3f transformationMatrix{ 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };

		// Apply the tranformations to the matrix
		transformationMatrix = cube::Matrix3f::Scale3D(m_scale) * transformationMatrix;

		transformationMatrix = cube::Matrix3f::RotationX(m_rotations.x) * transformationMatrix;
		transformationMatrix = cube::Matrix3f::RotationY(m_rotations.y) * transformationMatrix;
		transformationMatrix = cube::Matrix3f::RotationZ(m_rotations.z) * transformationMatrix;

		// Apply the rotations and scale to the points
		for (int i = 0; i < 8; i++)
		{
			cube::Vector3f vector{ baseVertices[i].coordinate[0], baseVertices[i].coordinate[1], baseVertices[i].coordinate[2] };

			vector = transformationMatrix * vector;

			vertices[i].coordinate[0] = vector.x;
			vertices[i].coordinate[1] = vector.y;
			vertices[i].coordinate[2] = vector.z;
		}

		for (int i = 0; i < 8; i++)
		{
			cube::Vector3f vector{ vertices[i].coordinate[0], vertices[i].coordinate[1], 1.0f };

			vector = cube::Matrix3f::Translate(m_translations.x , m_translations.y) * vector;

			vertices[i].coordinate[0] = vector.x;
			vertices[i].coordinate[1] = vector.y;
		}
	}
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);

	/*	As the data positions will be updated by the this program on the
		CPU bind the updated data to the GPU for drawing	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 36, vertices, GL_STATIC_DRAW);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);


	glColorPointer(3, GL_FLOAT, sizeof(Vertex), (char*)NULL + 12);

	/*	Draw Triangle from VBO	(set where to start from as VBO can contain 
		model compoents that are and are not to be drawn )	*/
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (char*)NULL + 0);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (char*)NULL + 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	window.display();

}

void Game::unload()
{
	cout << "Cleaning up" << endl;

	glDeleteBuffers(1, vbo);
}

