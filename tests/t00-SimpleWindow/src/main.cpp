#include "UglyEngine.h"

int main()
{
	return ugly::Engine::getInstance()->run(new ugly::Application());
}