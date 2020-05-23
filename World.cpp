#include "World.h"
#include "Util.h"

World::World() {
    
}

World::~World() {
    for (auto i : chunks) {delete i.second;}
    chunks.clear();
}

void World::Update(GLfloat dt, glm::vec3 shipPos) {
    const int chunkStartZ = -5;

    auto shipChunk = ChunkCoord::from_vector(shipPos);
    int xmin  = shipChunk.x - 7;
    int xmax = shipChunk.x + 7;
    
    int zmin = shipChunk.z - 15;
    int zmax = shipChunk.z + 2;

    // Unload old out-of-bounds chunks
    vector<ChunkCoord> chunksToUnload;
    for (auto i : chunks) {
        auto coord = i.first; auto chunk = i.second;
        if (coord.x < xmin |
            coord.x > xmax |
            coord.z < zmin |
            coord.z > zmax) {
            chunksToUnload.push_back(coord);
        }
    }
    for (auto coord : chunksToUnload) {
        delete chunks[coord];
        chunks.erase(coord);
    }

    // Load in in new in-bounds chunks
    for (int x=xmin; x<=xmax; x++) {
        for (int z=zmin; z<=zmax; z++) {
            ChunkCoord coord = {x, z};
            if (chunks.count(coord) == 0 && z<chunkStartZ) {
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

bool World::IsCollision(glm::vec3 point) {
    ChunkCoord centerchunk = ChunkCoord::from_vector(point);
    ChunkCoord coords[9];
    coords[0] = centerchunk;
    coords[1] = {centerchunk.x+1, centerchunk.z};
    coords[2] = {centerchunk.x-1, centerchunk.z};
    coords[3] = {centerchunk.x,   centerchunk.z+1};
    coords[4] = {centerchunk.x,   centerchunk.z-1};
    coords[5] = {centerchunk.x-1, centerchunk.z-1};
    coords[6] = {centerchunk.x+1, centerchunk.z-1};
    coords[7] = {centerchunk.x-1, centerchunk.z+1};
    coords[8] = {centerchunk.x+1, centerchunk.z+1};

    for (auto coord : coords) {
        if (chunks.count(coord)>0 && chunks.at(coord)->IsCollision(point))
            return true;
    }
    return false;
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
    int a = 0;
    a++;
    for (auto obstacle : obstacles) {
        obstacle->Draw(projection, view);
    }
}

bool World::WorldChunk::IsCollision(glm::vec3 point) {
    for (auto obstacle : obstacles) {
        if (obstacle->IsCollision(point))
            return true;
    }
    return false;
}

World::Obstacle::Obstacle() : Model("rook") {
    PingColour = 0.0f;
    CollColour = 0.0f;
    Colour = glm::vec4(1,0,0,1);
}

void World::Obstacle::Update(GLfloat dt) {
    if (PingColour > 0) {
        PingColour -= dt;
    } else if (PingColour < 0) {
        PingColour = 0;
    }
    if (CollColour > 0) {
        CollColour -= dt;
    } else if (CollColour < 0) {
        CollColour = 0;
    }
    Colour = glm::vec4(1, PingColour, CollColour, 1);
}

bool World::Obstacle::IsCollision(glm::vec3 point) {
    vec3 a = {point.x, 0, point.z};
    vec3 b = {Position.x, 0, Position.z};
    //PingColour = 1.0f;
    bool result = glm::distance(a,b) < 0.75;
    if (result) {CollColour = 1.0f;}
    return result;
}