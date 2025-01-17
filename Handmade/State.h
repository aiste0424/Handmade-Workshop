#pragma once

/*===================================================================#
| 'State' source files last updated on 9 November 2021               |
#====================================================================#
| Class has not been fully tested. No known issues found.            |
#===================================================================*/

//TODO - Add default screens for each game state
//TODO - Cannot load resources in game states ctor
//TODO - Consider RAII to load/release resources in the ctor/dtor
//This would remove the need for OnEnter/OnExit?

class State
{

public:

	State() {}
	virtual ~State() = 0 {}

	virtual bool OnEnter() = 0;
	virtual State* Update(int deltaTime) = 0;
	virtual bool Render() = 0;
	virtual void OnExit() = 0;

};