#include "Locator.h"


InputEvent* Locator::s_pInput = 0;
ControlEvent* Locator::s_pController = 0;
PrimitiveRenderer*Locator::s_pPrimitiveRenderer = 0;
TextureRenderer*Locator::s_pTextureRenderer = 0;
TextConsole*Locator::s_pTextConsole = 0;
AssetsManager*Locator::s_pAssets = 0;
Camera2D*Locator::s_pCamera = 0;
PhysicsFactory*Locator::s_physicsFactory = 0;
Locator::Locator()
{
}


Locator::~Locator()
{
}
