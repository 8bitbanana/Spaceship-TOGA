#include "World.h"
#include "Util.h"

World::World() {
    
}

World::~World() {
    for (auto i : chunks) {delete i.second;}
    chunks.clear();
}

void World::Update(GLfloat dt, glm::vec3 shipPos) {

    auto shipChunk = ChunkCoord::from_vector(shipPos);

    int xmin  = shipChunk.x - 1;
    int xmax = shipChunk.x + 1;
    
    int zmin = shipChunk.z - 10;
    int zmax = shipChunk.z + 1;

    // Unload old out-of-bounds chunks
    for (auto i : chunks) {
        auto coord = i.first; auto chunk = i.second;
        if (coord.x < xmin |
            coord.x > xmax |
            coord.z < zmin |
            coord.z > zmax) {
            delete chunk;
            chunks.erase(coord);
        }
    }

    // Load in in new in-bounds chunks
    for (int x=xmin; x<=xmax; x++) {
        for (int z=zmin; z<=zmax; z++) {
            ChunkCoord coord = {x, z};
            if (chunks.count(coord) == 0) {
                WorldChunk* chunk = new WorldChunk(coord);
                chunks[coord] = chunk;
            }
        }
    }

    for (auto const& x : chunks) {
        auto chunk = x.second;
        chunk->Update(dt);
    }
}

void World::Draw(glm::mat4 projection, glm::mat4 view) {
    for (auto const& x : chunks) {
        auto chunk = x.second;
        chunk->Draw(projection, view);
    }
}

World::WorldChunk::WorldChunk(ChunkCoord pos) {
    //for (unsigned int i=0; i<(Util::random()%3); i++) {
    for (unsigned int i=0; i<1; i++) {
        Obstacle* obs = new Obstacle();
        glm::vec3 offset = {
            Util::random_float(0, CHUNK_SIZE),
            0,
            Util::random_float(0, CHUNK_SIZE)
        };
        offset = {0,0,0}; // stub
        obs->Position = pos.to_vector() + offset;
        obstacles.push_back(obs);
    }
}

World::WorldChunk::~WorldChunk() {
    for (auto i : obstacles) {delete i;}
    obstacles.clear();
}

void World::WorldChunk::Update(GLfloat dt) {
    for (auto obstacle : obstacles) {
        obstacle->Update(dt);
    }
}

void World::WorldChunk::Draw(glm::mat4 projection, glm::mat4 view) {
    for (auto obstacle : obstacles) {
        obstacle->Draw(projection, view);
    }
}

World::Obstacle::Obstacle() : Model("Models\\rook.obj") {
    Colour = glm::vec4(1,0,0,1);
}