#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <string>
#include <vector>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <Sqrat/sqrat.h>

namespace fsn
{
    struct Resource
    {
        std::string mName;
        void *mData;
    };

    /// \brief Manages all of the resources
    class ResourceManager
    {
        public:
            virtual ~ResourceManager();

            /// \brief Add a resource
            void add(const std::string& name, void* data);

            /// \brief Get a resource
            void* get(const std::string& name);

            /// \brief Get a texture.
            sf::Texture *getTexture(const std::string& name);

            /// \brief Get a font.
            sf::Font *getFont(const std::string& name);

            /// \brief Get a script.
            Sqrat::Script *getScript(const std::string& name, HSQUIRRELVM master = NULL);

            static void init(){Instance=new ResourceManager;}
            static ResourceManager* get(){return Instance;}

        private:
            ResourceManager();

            static ResourceManager* Instance;

            Resource *findResource(std::string name);

            /// All of the resources
            std::vector <Resource> mResources;
    };
}

#endif // RESOURCEMANAGER_H
