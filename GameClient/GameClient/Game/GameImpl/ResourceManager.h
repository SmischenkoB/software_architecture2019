#pragma once

enum class Sprites
{
	DESK
	, CHECKER
	, WHITE
	, LOADING
};

struct AnimationPack
{
	Sprite* sprite = nullptr;
	int numberOfSprites = 0;
	int numberOfHorizontalSprites = 0;
	int numberOfVerticalSprites = 0;
};

class ResourceManager
{
public:
	~ResourceManager();

	static ResourceManager* getInstance();
	static void deleteInstance();

	Sprite* getResource(Sprites name);
	Sprites getKeyOfSpriteByPath(std::string const &str);

	void preload();

	static bool preloadFinished();

private:
	ResourceManager();

	void loadResource(Sprites name, const std::string& path);
	void loadResources();

	static bool m_preloadFinished;
	static ResourceManager* m_instance;

	std::unordered_map<Sprites, Sprite*> m_Resources;

};

