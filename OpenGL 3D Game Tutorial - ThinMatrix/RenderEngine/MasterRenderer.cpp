#include <glm\gtx\transform.hpp>
#include "MasterRenderer.h"

MasterRenderer::MasterRenderer(float aspectRatio)
	: m_projectionMatrix(glm::perspective(m_FOV, aspectRatio, m_NEAR_PLANE, m_FAR_PLANE)),
	m_entityRenderer(EntityRenderer(m_basicShader, m_projectionMatrix)),
	m_terrainRenderer(TerrainRenderer(m_terrainShader, m_projectionMatrix))
{
	// Don't draw faces we can't see
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	// Tell OpenGL to test for depth
	glEnable(GL_DEPTH_TEST);
	// Set clear color and clear the screen
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
	// Enable alpha blend
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Enable Antialiasing
	glEnable(GL_MULTISAMPLE);
}


MasterRenderer::~MasterRenderer()
{
}


void MasterRenderer::Render(Light& light, FPSCamera camera)
{
	// Clear buffers and activate the shader
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Load terrain shader parameters and render terrains
	m_terrainShader.Use();
	m_terrainShader.LoadLight(light, 0.1f);
	m_terrainShader.LoadViewMatrix(camera.GetViewMatrix());
	m_terrainRenderer.Render(terrains);
	m_terrainShader.UnUse();
	terrains.clear();

	// Load basic shader parameters and render entities
	m_basicShader.Use();
	m_basicShader.LoadLight(light, 0.1f);
	m_basicShader.LoadViewMatrix(camera.GetViewMatrix());
	m_entityRenderer.Render(m_entities, camera.GetPosition());
	m_basicShader.UnUse();
	m_entities.clear();
}


void MasterRenderer::ProcessTerrain(Terrain& terrain)
{
	// Add terrain to list
	terrains.push_back(terrain);
}


void MasterRenderer::ProcessEntity(Entity& entity)
{
	// Get the textured model
	TexturedModel& texturedModel = entity.GetModel();
	// If the texturedmodel is in the map this will do nothing
	// If it is not in the map it will insert it
	m_entities.insert(tMap::value_type(texturedModel, std::vector<Entity>()));
	
	// Add the entity to the entities list
	m_entities[texturedModel].push_back(entity);
}