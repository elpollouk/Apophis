#include "pch.h"
#include "Bootstrap.h"
#include "GameBoard.h"
#include "vectorutils.h"

std::default_random_engine generator;
std::uniform_int_distribution<unsigned int> distribution(0, 997);

using namespace Apophis;
using namespace SampleUtils;

Apophis::Vector FeaturizeState(const std::string& state);

char State(const GameBoard& board, unsigned int position)
{
	return board.GetStateChar(position, '1' + position);
}

void ShowBoard(const GameBoard& board)
{
	std::cout << "   |   |   \n";
	std::cout << " " << State(board, 0) << " | " << State(board, 1) << " | " << State(board, 2) << " \n";
	std::cout << "   |   |   \n";
	std::cout << "---+---+---\n";
	std::cout << "   |   |   \n";
	std::cout << " " << State(board, 3) << " | " << State(board, 4) << " | " << State(board, 5) << " \n";
	std::cout << "   |   |   \n";
	std::cout << "---+---+---\n";
	std::cout << "   |   |   \n";
	std::cout << " " << State(board, 6) << " | " << State(board, 7) << " | " << State(board, 8) << " \n";
	std::cout << "   |   |   \n\n";
}

void EvaluateState(Apophis::Component::Network& network, const GameBoard& board)
{
	auto input = FeaturizeState(board.Save());
	auto output = network.Calculate(input);

	std::cout << "X Win = " << output[0] << "\n";
	std::cout << "O Win = " << output[1] << "\n";
	std::cout << "Draw  = " << output[2] << "\n";
}

bool HumanGame()
{
	GameBoard board;
	Apophis::Component::Network network(*IO::LoadJson("Data/network.json"));
	std::cout << "\nNew Game!\n";

	while (true)
	{
		ShowBoard(board);
		std::cout << "Position to play (1-9, q)> ";
		std::string strPosition;
		std::cin >> strPosition;

		if (strPosition == "q") return false;

		auto position = std::stoi(strPosition) - 1;
		if (position < 0 || GameBoard::BOARD_SIZE <= position)
		{
			std::cout << "Position out of range\n";
			continue;
		}

		auto result = board.Play(position);
		switch (result)
		{
		case GameBoard::DRAW:
			ShowBoard(board);
			std::cout << "Draw\n";
			EvaluateState(network, board);
			return true;

		case GameBoard::O:
		case GameBoard::X:
			ShowBoard(board);
			std::cout << GameBoard::PositionStateToChar(result) << " wins!\n";
			EvaluateState(network, board);
			return true;

		case GameBoard::EMPTY:
			break;

		default:
			std::cout << "Invalid position\n";
		}
	}
}

void Record(const GameBoard& board, std::set<GameBoard::GameState>& set)
{
	set.insert(board.Save());
}

void RandomGame(std::set<GameBoard::GameState>& gamesX, std::set<GameBoard::GameState>& gamesO, std::set<GameBoard::GameState>& gamesDraw, std::set<GameBoard::GameState>& gamesIncomplete)
{
	GameBoard board;

	while (true)
	{
		auto potition = distribution(generator) % 9;
		auto winner = board.Play(potition);
		switch (winner)
		{
		case GameBoard::DRAW:
			Record(board, gamesDraw);
			return;

		case GameBoard::X:
			Record(board, gamesX);
			return;

		case GameBoard::O:
			Record(board, gamesO);
			return;
		}
	}
}

Apophis::Vector FeaturizeState(const std::string& state)
{
	Apophis::Vector input(18);
	for (auto i = 0; i < GameBoard::BOARD_SIZE; i++)
	{
		auto j = i * 2;
		switch (state[i])
		{
		case 'X':
			input[j] = 1.f;
			input[j + 1] = 0.f;
			break;

		case 'O':
			input[j] = 0.f;
			input[j + 1] = 1.0f;
			break;

		default:
			input[j] = 0.f;
			input[j + 1] = 0.f;
			break;
		}
	}
	return input;
}

void SaveExample(Apophis::ExampleSet& exampleSet, const std::string& state, int classification)
{
	Apophis::Vector input = FeaturizeState(state);
	Apophis::Vector output = SampleUtils::Vector::OneHot(3, classification);

	exampleSet.AddExample(input, output);
}

void Train(const Apophis::IExampleProvider& trainingExamples, const Apophis::ExampleSet& testExamples)
{
	Training::BackPropNetwork network(testExamples.InputSize, 0.0001f, 0.5f);
	network.AddLayer<TransferFunction::Relu>(16);
	network.AddLayer<TransferFunction::Relu>(16);
	network.AddLayer<TransferFunction::Relu>(3);

	Training::Evaluator evaluator(network, Training::Loss::SquaredError, testExamples);
	Training::StoppingCondition::AnyStoppingCondition stoppingConditions;
	//stoppingConditions.Add<LossLessThan>(TRAINING_ERROR);
	stoppingConditions.Add<Training::StoppingCondition::NumTrainingIterations>(20000000);

	Training::Trainer trainer(network, evaluator);

	std::cout << "Training...\n";
	trainer.Run(trainingExamples, stoppingConditions);

	std::cout << "Training Count = " << trainer.GetMetrics().Get<int>(Data::METRIC_TRAINING_ITERATIONS) << "\n";
	std::cout << "Error = " << trainer.GetMetrics().Get<float>(Data::METRIC_TRAINING_LOSS) << "\n";

	IO::SaveNetwork(network, "Data/network.json");
}

void GenerateExamplesAndTrain()
{
	std::cout << "XOX Sample\n\n";

	std::set<GameBoard::GameState> gamesX;
	std::set<GameBoard::GameState> gamesO;
	std::set<GameBoard::GameState> gamesDraw;
	std::set<GameBoard::GameState> gamesIncomplete;

	for (auto i = 0; i < 100000; i++)
	{
		generator.seed(i * 997);
		RandomGame(gamesX, gamesO, gamesDraw, gamesIncomplete);
	}

	Apophis::ExampleSet examplesX(18, 3);
	Apophis::ExampleSet examplesO(18, 3);
	Apophis::ExampleSet examplesDraw(18, 3);
	Apophis::ExampleSet examplesAll(18, 3);

	for (const auto& state : gamesX)
	{
		SaveExample(examplesX, state, 0);
		SaveExample(examplesAll, state, 0);
	}

	for (const auto& state : gamesO)
	{
		SaveExample(examplesO, state, 1);
		SaveExample(examplesAll, state, 1);
	}

	for (const auto& state : gamesDraw)
	{
		SaveExample(examplesDraw, state, 2);
		SaveExample(examplesAll, state, 1);
	}

	//IO::SaveExamples(examples, "Data/endstates.json");

	Apophis::MultiExampleSet examples({ &examplesX, &examplesO, &examplesDraw });
	Train(examples, examplesAll);
}

int SampleMain(int argc, const char** argv)
{
	while (HumanGame()) {}
	//GenerateExamplesAndTrain();
	return 0;
}
