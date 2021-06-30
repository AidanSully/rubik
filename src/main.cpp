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

void			apply_moves_db(Cube c, string move)
{
	int		amount;

	amount = move[1] - '0';
	switch (move[0])
	{
		case 'U':
			c.u(amount);
			printf("U Turn\n");
			break;
		case 'L':
			c.l(amount);
			printf("L Turn\n");
			break;
		case 'F':
			c.f(amount);
			printf("F Turn\n");
			break;
		case 'R':
			c.r(amount);
			printf("R Turn\n");
			break;
		case 'B':
			c.b(amount);
			printf("B Turn\n");
			break;
		case 'D':
			c.d(amount);
			printf("D Turn\n");
			break;
	}
}

vector<string>	read_moves_db(Cube c, string moves)
{
	vector<string>	parsed_moves;
	string			temp;
	int n = 0;

	for (size_t i = 0; i < moves.length(); i++)
	{
		temp += moves[i];
		if (temp.length() == 2)
		{
			parsed_moves.push_back(temp);
			apply_moves_db(c, parsed_moves[n]);
			n++;
			temp = "";
		}
	}
	return parsed_moves;
}


int main(int ac, char **av)
{
	vector<string>	moves;
	Cube			c;

	// Argumentparsing bullshit
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
	if (program["-g"] == true)
	{
		cout << "generate enabled\n";
		open_db();
		create_db();
		generate_db(c);
		exit(1);
	}
	// To solve a cube
	cout << "solve enabled\n";
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
	// read_db(3);
	// rowcount_db(3);
	uint64_t key = 2047;
	cout << get_value(1, key) << endl;

	return (0);
}

