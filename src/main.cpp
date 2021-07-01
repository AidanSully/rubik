/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: asulliva <asulliva@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/11 17:27:16 by asulliva      #+#    #+#                 */
/*   Updated: 2021/06/22 16:38:29 by asulliva      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "cube.hpp"
#include "solver.hpp"
#include "database.hpp"

/*
**	@desc:	Function splits up arguments into seperate strings
**	@param:	const string args: arguments given
**	@ret:	vector<string> moves: vector with moves
*/
vector<string>	parse(string args)
{
	char			*token = std::strtok(const_cast<char*>(args.c_str()), " ");
	vector<string>	moves;
	while (token != nullptr) {
		moves.push_back(string(token));
		token = std::strtok(nullptr, " ");
	}
	return (moves);
}

/*
**	@desc:	function checks if moves are legal
**	@param:	vector<string> moves: move strings
**	@ret:	bool, true if legal
*/
bool			checkMoves(vector<string> moves)
{
	string			legalChars = "ULFRBD2' ";
	string			noStart = "2'";

	for (int i = 0; i < int(moves.size()); i++)
	{
		for (int j = 0; j < int(moves[i].size()); j++)
		{	
			if (find(legalChars.begin(), legalChars.end(), moves[i][j]) == legalChars.end())
			{
				printf("%c is illegal\n", moves[i][j]);
				throw "Illegal moves detected";
			}
		}
		if ((find(noStart.begin(), noStart.end(), moves[i][0]) != noStart.end()) || moves[i].size() > 2)
			throw "Illegal start character detected";
		if (moves[i].size() > 1 && ((moves[i][0] == '\'' && moves[i][0] == '2') ||
			(moves[i][1] != '\'' && moves[i][1] != '2')))
			throw "Illegal format detected";
	}
	return true;
}

vector<string>	random_moves(int len = 10)
{
	vector<string>	moves;
	vector<string>	allowed = {"F","R","U","B","L","D"};

	srand((int)time(nullptr));
	for (int i = 0; i < len; i++)
		moves.push_back(allowed[rand() % 6]);
	return moves;
}

int main(int ac, char **av)
{
	vector<string>	moves;
	Cube			c;
	Database		db;

	// Argumentparsing bullshit
	moves = random_moves();
	argparse::ArgumentParser program("rubik");
	program.add_argument("scramble")
		.help("Scramble set to use")
		.action([](const string& value) {return value;});
	program.add_argument("-g", "--generate")
		.help("Generate database")
		.default_value(false)
		.implicit_value(true);
	try {
		program.parse_args(ac, av);
	} catch (const std::runtime_error& err) {
		cout << err.what() << endl;
		cout << program;
		exit(1);
	}
	// To generate database
	if (program["-g"] == true || program["--generate"] == true)
	{
		cout << "Database generation mode\n";
		db.open_db();
		db.create_db();
		db.generate_db(c);
		exit(1);
	}
	// To solve a cube
	cout << "Solver mode\n";
	auto input = program.get<string>("scramble");
	moves = parse(input);
	for (auto move : moves)
		printf("%s\n", move.c_str());
	try {
		checkMoves(moves);
	} catch (const char *msg) {
		std::cerr << msg << std::endl;
		exit(1);
	}
	for (auto move : moves) {
		c.applyMove(move);
	}
	Solver s(&c, &db, db.database);
	s.solve();
	Cube kubus;
	if (c == kubus)
		printf("Cube is solved");
	return (0);
}
