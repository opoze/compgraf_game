/* Hello Transforms - código inicial fornecido em https://learnopengl.com/#!Getting-started/Hello-Triangle 
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Computação Gráfica - Jogos Digitais - Unisinos
 * Versão inicial: 7/4/2017
 * Última atualização em 14/03/2018
 *
 */

using namespace std;

#include "SceneManager.h"
#include "rhostring.hpp"


// The MAIN function, from here we start the application and run the game loop
int main()
{
	SceneManager *scene = new SceneManager;
	scene->initialize(1024, 768);

	scene->run();

	scene->finish();
	
	return 0;
}


