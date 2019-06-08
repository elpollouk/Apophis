#include "pch.h"
#include "GameBoard.h"

GameBoard::GameBoard() :
	m_NextPlayer(PositionState::X)
{
	for (auto i = 0u; i < BOARD_SIZE; i++)
		m_Board[i] = PositionState::EMPTY;
}

GameBoard::PositionState GameBoard::Play(unsigned int position)
{
	assert(0 <= position && position < BOARD_SIZE);
	assert(m_NextPlayer == PositionState::X || m_NextPlayer == PositionState::O);
	if (m_Board[position] != PositionState::EMPTY) return PositionState::INVALID;
	m_Board[position] = m_NextPlayer;

	auto winner = CheckForWinner();
	if (winner != PositionState::EMPTY)
	{
		m_NextPlayer = PositionState::INVALID;
		return winner;
	}

	m_NextPlayer = m_NextPlayer == PositionState::X ? PositionState::O : PositionState::X;
	return PositionState::EMPTY;
}

GameBoard::PositionState GameBoard::GetState(unsigned int position) const
{
	assert(0 <= position && position < BOARD_SIZE);
	return m_Board[position];
}

char GameBoard::PositionStateToChar(GameBoard::PositionState state, char empty)
{
	switch (state)
	{
	case GameBoard::EMPTY:
		return empty;
	case GameBoard::X:
		return 'X';
	case GameBoard::O:
		return 'O';
	default:
		assert(false);
		throw Apophis::ApophisException("Invalid game state %d", (int)state);
	}
}

char GameBoard::GetStateChar(unsigned int position, char empty) const
{
	return PositionStateToChar(GetState(position), empty);
}

GameBoard::GameState GameBoard::Save() const
{
	std::string data(BOARD_SIZE, 0);
	for (auto i = 0u; i < BOARD_SIZE; i++)
		data[i] = GetStateChar(i);
	return data;
	/*GameState state = 0;
	for (auto i = 0u; i < BOARD_SIZE; i++)
	{
		state <<= 2;
		state |= (unsigned int)m_Board[i];
	}
	return state;*/
}

unsigned int GameBoard::CoordToPosition(unsigned int x, unsigned int y)
{
	assert(0 <= x && x < WIDTH);
	assert(0 <= y && y < HEIGHT);
	return (y * WIDTH) + x;
}

GameBoard::PositionState GameBoard::CheckForWinner()
{
	for (auto x = 0u; x < WIDTH; x++)
	{
		auto winner = CheckForWinnerVertical(x);
		if (winner != PositionState::EMPTY) return winner;
	}

	for (auto y = 0u; y < HEIGHT; y++)
	{
		auto winner = CheckForWinnerHorizontal(y);
		if (winner != PositionState::EMPTY) return winner;
	}

	if (GetState(0, 0) == GetState(1, 1) && GetState(1, 1) == GetState(2, 2)) return GetState(1, 1);
	if (GetState(2, 0) == GetState(1, 1) && GetState(1, 1) == GetState(0, 2)) return GetState(1, 1);

	for (auto i = 0u; i < BOARD_SIZE; i++)
		if (m_Board[i] == PositionState::EMPTY) return PositionState::EMPTY;

	return PositionState::DRAW;
}

GameBoard::PositionState GameBoard::CheckForWinnerHorizontal(unsigned int y)
{
	auto initialState = GetState(0, y);
	if (initialState == PositionState::EMPTY) return PositionState::EMPTY;
	if (initialState == GetState(1, y) && initialState == GetState(2, y)) return initialState;
	return PositionState::EMPTY;
}

GameBoard::PositionState GameBoard::CheckForWinnerVertical(unsigned int x)
{
	auto initialState = GetState(x, 0);
	if (initialState == PositionState::EMPTY) return PositionState::EMPTY;
	if (initialState == GetState(x, 1) && initialState == GetState(x, 2)) return initialState;
	return PositionState::EMPTY;
}
