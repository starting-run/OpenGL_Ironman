class Skybox {
public:
	Skybox();
	~Skybox();
	void draw(bool blend = false);

private:
	typedef unsigned int uint;
	typedef const char * pcStr;
	uint textures[6];
	static uint _loadTexture(pcStr filename);
};