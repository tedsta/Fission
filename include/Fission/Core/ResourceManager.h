#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <string>
#include <vector>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <Sqrat/sqrat.h>

struct Resource
{
    std::string mName;
    void *mData;
};

/// \brief Manages all of the resources
class ResourceManager
{
    public:
        ResourceManager();
        virtual ~ResourceManager();

        /// \brief Get a texture.
        sf::Texture *getTexture(const std::string& name);

        /// \brief Get a font.
        sf::Font *getFont(const std::string& name);

        /// \brief Get a script.
        Sqrat::Script *getScript(const std::string& name, HSQUIRRELVM master = NULL);

    private:
        Resource *findResource(std::string name);

        /// All of the resources
        std::vector <Resource> mResources;
};

#endif // RESOURCEMANAGER_H
