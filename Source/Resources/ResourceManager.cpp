#include "ResourceManager.h"


ResourceManager::ResourceManager()
{
	std::multimap<std::string, std::string> _settingsForGirl;
	_settingsForGirl.insert(std::make_pair("Width", "110"));
	_settingsForGirl.insert(std::make_pair("Height", "151"));
	_settingsForGirl.insert(std::make_pair("FrameCount", "27"));
	_settingsForGirl.insert(std::make_pair("AnimationTime", "1350"));
	_settingsForGirl.insert(std::make_pair("Color", "150 2 2"));
	_settingsForGirl.insert(std::make_pair("Color", "255 255 255"));
	_settingsForGirl.insert(std::make_pair("Color", "0 0 0"));

	std::multimap<std::string, std::string> _settingsForCat;
	_settingsForCat.insert(std::make_pair("Width", "512"));
	_settingsForCat.insert(std::make_pair("Height", "256"));
	_settingsForCat.insert(std::make_pair("FrameCount", "8"));
	_settingsForCat.insert(std::make_pair("AnimationTime", "400"));

	std::multimap<std::string, std::string> _settingsForFire;
	_settingsForFire.insert(std::make_pair("Width", "192"));
	_settingsForFire.insert(std::make_pair("Height", "192"));
	_settingsForFire.insert(std::make_pair("FrameCount", "12"));
	_settingsForFire.insert(std::make_pair("AnimationTime", "600"));

	std::multimap<std::string, std::string> _settingsForSpaceshipNormal;
	_settingsForSpaceshipNormal.insert(std::make_pair("Width", "99"));
	_settingsForSpaceshipNormal.insert(std::make_pair("Height", "154"));
	_settingsForSpaceshipNormal.insert(std::make_pair("FrameCount", "4"));
	_settingsForSpaceshipNormal.insert(std::make_pair("AnimationTime", "500"));

	std::multimap<std::string, std::string> _settingsForSpaceshipFlickering;
	_settingsForSpaceshipFlickering.insert(std::make_pair("Width", "99"));
	_settingsForSpaceshipFlickering.insert(std::make_pair("Height", "154"));
	_settingsForSpaceshipFlickering.insert(std::make_pair("FrameCount", "8"));
	_settingsForSpaceshipFlickering.insert(std::make_pair("AnimationTime", "1000"));

	_resources.insert(std::pair<std::string, Resource*>
		("piupiu", new AudioResource("piupiu", "Resources\\audio\\piupiu.wav")));
	_resources.insert(std::pair<std::string, Resource*>
		("booom", new AudioResource("booom", "Resources\\audio\\booom.wav")));
	_resources.insert(std::pair<std::string, Resource*>
		("asteroid", new TextureResource("asteroid", "Resources\\graphics\\asteroid.png")));
	_resources.insert(std::pair<std::string, Resource*>
		("smallasteroid", new PictureResource("smallasteroid", "Resources\\graphics\\smallasteroid.png")));
	_resources.insert(std::pair<std::string, Resource*>
		("girl", new ImageSequenceResource("girl", "Resources\\graphics\\girl.png", _settingsForGirl)));
	_resources.insert(std::pair<std::string, Resource*>
		("cat", new ImageSequenceResource("cat", "Resources\\graphics\\cat.png", _settingsForCat)));
	_resources.insert(std::pair<std::string, Resource*>
		("fire", new ImageSequenceResource("fire", "Resources\\graphics\\fire.png", _settingsForFire)));
	_resources.insert(std::pair<std::string, Resource*>
		("spaceshipNormal", new ImageSequenceResource("spaceshipNormal", "Resources\\graphics\\spaceship.png", _settingsForSpaceshipNormal)));
	_resources.insert(std::pair<std::string, Resource*>
		("spaceshipFlickering", new ImageSequenceResource("spaceshipFlickering", "Resources\\graphics\\spaceshipFlickering.png", _settingsForSpaceshipFlickering)));
	_resources.insert(std::pair<std::string, Resource*>
		("bullet", new TextureResource("bullet", "Resources\\graphics\\bullet.png")));
	_resources.insert(std::pair<std::string, Resource*>
		("rocket", new TextureResource("rocket", "Resources\\graphics\\rocket.png")));
}

Resource* ResourceManager::GetGeneralResource(const std::string& key)
{
	auto iter = _resources.find(key);

	assert(iter != _resources.end() && "Resource not found");

	Resource* resource = iter->second;
	if (resource->GetRefCounter() != 0)
	{
		resource->IncRefCounter();
	}
	else
	{
		resource->Load();
	}
	return resource;

}

void ResourceManager::ReleaseResource(const std::string& key)
{
	auto iter = _resources.find(key);

	assert(iter != _resources.end() && "Resource not found");
	assert(iter->second->GetRefCounter() != 0 && "Resource already released");

	iter->second->DecRefCounter();
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::ReleaseAllResources()
{
	for (auto& i : _resources) 
	{
		assert(i.second->GetRefCounter() == 0 && "Resource reference still exists");

		i.second->Unload();
	}
}
