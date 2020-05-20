#include "Model.h"

#include <unordered_map>
#include <functional>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

using std::unordered_map;
using std::vector;
using std::hash;

const int CHUNK_SIZE = 10;


struct ChunkCoord {
    int x, z;
    glm::vec3 to_vector() {
        return glm::vec3(x,0,z) * (float)CHUNK_SIZE;
    }
    static ChunkCoord from_vector(glm::vec3 vector) {
        vector /= (float)CHUNK_SIZE;
        return {(int)vector.x, (int)vector.z};
    }
    bool operator == (const ChunkCoord &other) const {
        return x == other.x && z == other.z;
    }
};

// https://stackoverflow.com/a/17017281/8708443
template <> struct hash<ChunkCoord> {
    std::size_t operator()(const ChunkCoord& k) const {
        return (hash<int>()(k.x) ^ (hash<int>()(k.z) << 1));
    }
};

class World {
public:
    World();
    ~World();
    void Update(GLfloat dt, glm::vec3 shipPos);
    void Draw(glm::mat4 projection, glm::mat4 view);

private:
    class Obstacle : public Model {
    public:
        Obstacle();
    };

    class WorldChunk {
    public:
        WorldChunk(ChunkCoord pos);
        ~WorldChunk();

        void Update(GLfloat dt);
        void Draw(glm::mat4 projection, glm::mat4 view);
    private:
        vector<Obstacle*> obstacles;
    };

    unordered_map<ChunkCoord, WorldChunk*> chunks;
};