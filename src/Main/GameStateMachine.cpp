#include "GameStateMachine.h"

#include "../utils/sdl/InputHandler.h"
#include "../checkMemLeaks.h"


GameStateMachine::~GameStateMachine() {
	while (!gameStates_.empty()) {
		popState();
		if (deleting_ != nullptr) {
			delete deleting_;
			deleting_ = nullptr;
		}
	}

}

// Adds a new state on top of the stack
void GameStateMachine::pushState(GameState* state) {
	if(state->onEnter()) 
		gameStates_.push(state);
}

// Changes the state on top of the stack and adds the new one on top
void GameStateMachine::changeState(GameState* state) {
	// If the states are the same
    if (state == gameStates_.top()) return;

	// If the stack isn't empty
	if (!gameStates_.empty()) {
		popState();
	}

	// Then, the new state is added to the top of the stack
	pushState(state);
}

// Deletes the state on top of the stack
void GameStateMachine::popState() {
	// If the stack isn't empty
	if (!gameStates_.empty()) {
		// If the satte could be exited, it gets removed 
		// from the stack and deleted (in the next loop)
		if (gameStates_.top()->onExit()) {
			deleting_ = gameStates_.top();
			gameStates_.pop();
		}
	}
}

// Runs the current state's loop
void GameStateMachine::run(float frameTime) {
	if (!gameStates_.empty())
		gameStates_.top()->run(frameTime);

	// Deletes the previously exited state
	if(deleting_ != nullptr) {
		delete deleting_;
		deleting_ = nullptr;
	}
};