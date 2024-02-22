#pragma once

#include "../utils/Singleton.h"
#include <stack>
#include "GameState.h"


class GameStateMachine : public Singleton<GameStateMachine> {
	friend Singleton<GameStateMachine>;

private:
	std::stack<GameState*> gameStates_;
	GameState* deleting_ = nullptr;

	GameStateMachine() : gameStates_() { };

public:
	~GameStateMachine();
	
	// To avoid the FGM from getting copied/moved
	GameStateMachine(GameStateMachine&) = delete;
	GameStateMachine(GameStateMachine&&) = delete;
	GameStateMachine& operator=(GameStateMachine&) = delete;
	GameStateMachine& operator=(GameStateMachine&&) = delete;


	GameState* currentState() { return gameStates_.top(); };
	void pushState(GameState* state);
	void changeState(GameState* state);
	void popState();

	void run(float frameTime = 1);
};

