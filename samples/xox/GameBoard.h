#pragma once

class GameBoard
{
public:
	static constexpr auto WIDTH = 3u;
	static constexpr auto HEIGHT = 3u;
	static constexpr auto BOARD_SIZE = WIDTH * HEIGHT;

	enum PositionState
	{
		INVALID = -1,
		EMPTY = 0,
		X = 1,
		O = 2,
		DRAW = 3,
	};

	//typedef unsigned int GameState;
	typedef std::string GameState;

	GameBoard();

	PositionState Play(unsigned int position);
	PositionState GetState(unsigned int position) const;
	GameState Save() const;

	static unsigned int CoordToPosition(unsigned int x, unsigned int y);
	PositionState GetState(unsigned int x, unsigned int y) const { return GetState(CoordToPosition(x, y)); }

	static char PositionStateToChar(GameBoard::PositionState state, char empty = ' ');
	char GetStateChar(unsigned int position, char empty = ' ') const;

private:
	PositionState CheckForWinner();
	PositionState CheckForWinnerHorizontal(unsigned int y);
	PositionState CheckForWinnerVertical(unsigned int x);

	PositionState m_NextPlayer;
	PositionState m_Board[BOARD_SIZE];
};