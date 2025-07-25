#include "factory.h"
#include <QDebug>

// clang-format off
// b extern f3d::plugin* init_plugin_static_native();
extern f3d::plugin* init_plugin_static_assimp();
// clang-format on

namespace f3d
{
//----------------------------------------------------------------------------
factory* factory::instance()
{
  static factory instance;
  return &instance;
}

//----------------------------------------------------------------------------
factory::factory()
{
  // clang-format off
  // b this->StaticPluginInitializers["native"] = init_plugin_static_native;
  this->StaticPluginInitializers["assimp"] = init_plugin_static_assimp;
  // clang-format on
}

//----------------------------------------------------------------------------
const std::vector<plugin*>& factory::getPlugins()
{
  return this->Plugins;
}

//----------------------------------------------------------------------------
factory::plugin_initializer_t factory::getStaticInitializer(const std::string& pluginName)
{
  auto it = this->StaticPluginInitializers.find(pluginName);
  if (it != this->StaticPluginInitializers.end())
  {
    return it->second;
  }
  return nullptr;
}

//----------------------------------------------------------------------------
reader* factory::getReader(const std::string& fileName, std::optional<std::string> forceReader)
{
  int bestScore = -1;
  reader* bestReader = nullptr;

  for (const auto* plugin : this->Plugins)
  {
    for (const auto& reader : plugin->getReaders())
    {
      if (forceReader)
      {
        if (reader->getName() == *forceReader)
        {
          return reader.get();
        }
      }
      else if (reader->getScore() > bestScore && reader->canRead(fileName))
      {
        bestScore = reader->getScore();
        bestReader = reader.get();
      }
    }
  }

  return bestReader;
}

//----------------------------------------------------------------------------
bool factory::setReaderOption(const std::string& name, const std::string& value)
{
  for (auto& plugin : this->Plugins)
  {
    for (auto& reader : plugin->getReaders())
    {
      if (reader->setReaderOption(name, value))
      {
        return true;
      }
    }
  }
  return false;
}

//----------------------------------------------------------------------------
std::vector<std::string> factory::getAllReaderOptionNames()
{
  std::vector<std::string> names;
  for (const auto& plugin : this->Plugins)
  {
    for (const auto& reader : plugin->getReaders())
    {
      auto readerNames = reader->getAllReaderOptionNames();
      names.insert(names.end(), readerNames.begin(), readerNames.end());
    }
  }
  return names;
}

//----------------------------------------------------------------------------
void factory::load(plugin* plug)
{
  if (!this->registerOnce(plug))
  {
    qDebug() << "A plugin named \"" << plug->getName() << "\" is already registered.";
  }
}

//----------------------------------------------------------------------------
void factory::autoload()
{
  for (auto& [str, init] : this->StaticPluginInitializers)
  {
    this->registerOnce(init());
  }
}

//----------------------------------------------------------------------------
bool factory::registerOnce(plugin* plug)
{
  if (std::find(this->Plugins.begin(), this->Plugins.end(), plug) == this->Plugins.end())
  {
    this->Plugins.push_back(plug);

    qDebug() << "Loading plugin \"" << plug->getName() << "\"";
    qDebug() << "  Version: " << plug->getVersion();
    qDebug() << "  Description: " << plug->getDescription();
    qDebug() << "  Readers:";

    for (auto read : plug->getReaders())
    {
      qDebug() << "    " << read->getLongDescription();
    }

    return true;
  }
  return false;
}
}
