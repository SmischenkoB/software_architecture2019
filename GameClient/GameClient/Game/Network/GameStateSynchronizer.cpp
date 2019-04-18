#include "stdafx.h"
#include "GameStateSynchronizer.h"
#include "Messages/NetMessage.h"

void GameStateSynchronizer::initNetObject()
{
	NetObjectDescriptor descriptor = { static_cast<int>(GetType()), GetID() };

	m_gameStateNetObject = NetObjectAPI::GetInstance()->createThirdPartyNetObject(descriptor);

	if (NetObjectAPI::GetInstance()->IsHost())
	{
		m_gameStateNetObject->RegisterMessageHandler(
			[this](NetMessage const& message, NetAddr const& addr)
		{
			switch (message.getNetMessageType())
			{
			case NetMessageType::PAUSE_STATE:
			{
				Game::g_pGame->setGameState(GameState::PAUSED);

				NetMessage msg;

				msg.setNetMessageType(NetMessageType::PAUSE_STATE);
				m_gameStateNetObject->SendMasterBroadcast(msg);
			}
			break;

			case NetMessageType::GAME_IN_PROGRESS_STATE:
			{
				Game::g_pGame->setGameState(GameState::IN_GAME);

				NetMessage msg;

				msg.setNetMessageType(NetMessageType::GAME_IN_PROGRESS_STATE);
				m_gameStateNetObject->SendMasterBroadcast(msg);
			}
			break;

			default:
				break;
			}
		});
	}
	else
	{
		m_gameStateNetObject->RegisterMessageHandler(
			[this](NetMessage const& message, NetAddr const& addr)
		{
			switch (message.getNetMessageType())
			{
			case NetMessageType::PAUSE_STATE:
			{
				Game::g_pGame->setGameState(GameState::PAUSED);

				NetMessage msg;

				msg.setNetMessageType(NetMessageType::PAUSE_STATE);
			}
			break;

			case NetMessageType::GAME_OVER:
			{
				Game::g_pGame->setGameState(GameState::OVER);

				NetMessage msg;

				msg.setNetMessageType(NetMessageType::GAME_OVER);
			}
			break;

			case NetMessageType::GAME_IN_PROGRESS_STATE:
			{
				Game::g_pGame->setGameState(GameState::IN_GAME);

				NetMessage msg;

				msg.setNetMessageType(NetMessageType::GAME_IN_PROGRESS_STATE);
			}
			break;

			default:
				break;
			}
		});
	}
}

void GameStateSynchronizer::synchronizeGameState()
{
	if (!NetObjectAPI::GetInstance())
	{
		return;
	}

	auto gameState = Game::g_pGame->getGameState();

	if (NetObjectAPI::GetInstance()->IsHost())
	{
		switch (gameState)
		{
		case GameState::IN_GAME:
		{
			NetMessage msg;

			msg.setNetMessageType(NetMessageType::GAME_IN_PROGRESS_STATE);
			m_gameStateNetObject->SendMasterBroadcast(msg);
		}
		break;

		case GameState::PAUSED:
		{
			NetMessage msg;

			msg.setNetMessageType(NetMessageType::PAUSE_STATE);
			m_gameStateNetObject->SendMasterBroadcast(msg);
		}
		break;

		case GameState::OVER:
		{
			NetMessage msg;

			msg.setNetMessageType(NetMessageType::GAME_OVER);
			m_gameStateNetObject->SendMasterBroadcast(msg);
		}
		break;

		default:
			break;
		}
	}
	else
	{
		switch (gameState)
		{
		case GameState::IN_GAME:
		{
			NetMessage msg;

			msg.setNetMessageType(NetMessageType::GAME_IN_PROGRESS_STATE);
			m_gameStateNetObject->SendReplicaMessageToHost(msg);
		}
		break;

		case GameState::PAUSED:
		{
			NetMessage msg;

			msg.setNetMessageType(NetMessageType::PAUSE_STATE);
			m_gameStateNetObject->SendReplicaMessageToHost(msg);
		}
		break;

		default:
			break;
		}
	}
}
