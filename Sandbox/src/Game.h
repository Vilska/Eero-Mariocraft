#pragma on

#include "Eero.h"

#include "Scenes/MenuScene.h"
#include "Scenes/PlayScene.h"

namespace Eero {

	class Game : public Layer
	{
	public:
		virtual void OnAttach() override;

		virtual void OnUpdate(float deltaTime) override;
	private:
		std::shared_ptr<MenuScene> m_MenuScene;
		std::shared_ptr<PlayScene> m_PlayScene;
	};

}
