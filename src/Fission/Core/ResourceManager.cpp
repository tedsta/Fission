#include <Fission/Core/ResourceManager.h>

#include <iostream>

ResourceManager* ResourceManager::Instance;

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
    //dtor
}

void ResourceManager::add(const std::string& name, void* data)
{
    Resource rc;
    rc.mName = name;
    rc.mData = data;
    mResources.push_back(rc);
}

void* ResourceManager::get(const std::string& name)
{
    Resource *rc = findResource(name);

    if (rc)
        return rc->mData;
    return NULL;
}

sf::Texture *ResourceManager::getTexture(const std::string& name)
{
    sf::Texture *texture = NULL;
    Resource *resource = findResource(name);

    if (resource) //resource exists - use it
        texture = (sf::Texture*)resource->mData;
    else //it's a new resource - load it
    {
        //new resource
        Resource rc;
        rc.mName = name;

        //load the texture
        texture = new sf::Texture;
        texture->loadFromFile(name);
        texture->setRepeated(true); //repeat by default
        rc.mData = texture;

        //add the resource
        mResources.push_back(rc);
    }

    return texture;
}

sf::Font *ResourceManager::getFont(const std::string& name)
{
    sf::Font *font = NULL;
    Resource *resource = findResource(name);

    if (resource) //resource exists - use it
        font = (sf::Font*)resource->mData;
    else //it's a new resource - load it
    {
        //new resource
        Resource rc;
        rc.mName = name;

        //load the font
        font = new sf::Font;
        font->loadFromFile(name);
        rc.mData = font;

        //add the resource
        mResources.push_back(rc);
    }

    return font;
}

Sqrat::Script *ResourceManager::getScript(const std::string& name, HSQUIRRELVM master)
{
    Sqrat::Script *script = NULL;
    Resource *resource = findResource(name);

    if (resource) //resource exists - use it
        script = (Sqrat::Script*)resource->mData;
    else //it's a new resource - load it
    {
        //new resource
        Resource rc;
        rc.mName = name;

        //load the script
        script = new Sqrat::Script(master);

        std::string err;
        if (!script->CompileFile(name, err)) //there is an error
            std::cout << "Squirrel Compile Error: " << err << std::endl;
        else if (!script->Run(err))
            std::cout << "Squirrel Runtime Error: " << err << std::endl;

        rc.mData = script;

        //add the resource
        mResources.push_back(rc);
    }

    return script;
}

Resource *ResourceManager::findResource(std::string name)
{
    for (Resource& resource : mResources)
        if (resource.mName == name)
            return &resource;

    return NULL;
}
