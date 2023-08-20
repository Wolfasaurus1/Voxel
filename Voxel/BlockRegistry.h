#pragma once

#include <map>
#include <string>
#include <glm/glm.hpp>


struct BlockType {
    int id;
    std::string name;
    glm::vec3 color; 
};


class BlockRegistry {
private:
    std::map<int, BlockType> blockTypes;  // Map block ID to its type

public:
    void RegisterBlockType(const BlockType& blockType) {
        blockTypes[blockType.id] = blockType;
    }

    const BlockType& GetBlockType(int id) const {
        return blockTypes.at(id);  // Note: You may want to handle the case where the ID doesn't exist
    }
};