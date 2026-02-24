#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

//Valores globais
#define MAX_STRING_NUMBER 7 //Para evitar estouro de memória ao converter Hexadecimal para Binário
#define MAX_STRING_VECTOR 28
#define MAX_HISTORY 10


//Definindo a estrutura do histórico, ainda não é a variável
typedef struct
{
	int sourceBase;
	int targetBase;
	char userInputString[MAX_STRING_NUMBER + 1];
	char convertedResultString[MAX_STRING_VECTOR + 1];
} historyStruct;

//Definindo a estrutura para a ordenação dos números, ainda não é a variável
typedef struct
{
	int intermediateDecimal;
	int targetBase;
	char convertedResultString[MAX_STRING_VECTOR + 1];
} sortedConversionStruct;

//Instanciando um vetor baseado na estrutura de dados do histórico
historyStruct conversionHistory[MAX_HISTORY] = { 0 };
/*
	conversionHistory[indice] =
	{
		operação mais recente (0) = {Base inserida, Número inserido, Base destino, Número convertido}
		operação anterior     (1) = {Base inserida, Número inserido, Base destino, Número convertido}
		.
		.
		.
		operação mais antiga  (N) = {Base inserida, Número inserido, Base destino, Número convertido}
	}
*/

//Instanciando um vetor baseado na estrutura de dados para números ordenados
sortedConversionStruct sortedConversionResults[MAX_HISTORY] = { 0 };
/*
	sortedConversionResults[indice] =
	{
		Menor número                    (0) = {Número inserido em decimal para comparação, Base destino, Número convertido}
		Número maior que o anterior     (1) = {Número inserido em decimal para comparação, Base destino, Número convertido}
		.
		.
		.
		Maior número atual da lista     (N) = {Número inserido em decimal para comparação, Base destino, Número convertido}
	}
*/

/*
	Agora que aprendi sobre struct pretendo refatorar o código para remover a maioria das variáveis
	para conseguir um código mais limpo e legível.
*/

void addToHistory(int srcBase, char* userInputStr, int tgtBase, char* convertedResultStr);
void addToSortedConversion(int decimalNum, int tgtBase, char* convertedResultStr);

void charToIntVector(char* userStr, int stringSize);
void clearBuffer(void);
void clearScreen(void);

void decimalToBaseN(int decimalNum, int targetBase, char* result);

void exitConveror(void);

void getUserInput(char* userStr);
int getMenuOption();

int historyMenu(void);

void intVectorToIntNumber(int stringSize, int numBase);

int nBaseToDecimal(int userBase, int userPow, int numToPow);
void newConversionMenu(void);

void showHistory(void);
void showSortedConversionList(void);
void stringCounter(int direction, int vectorLength, char* userStr);

int main() {
	setlocale(LC_ALL, "portuguese");

	char userInputString[MAX_STRING_NUMBER + 1] = { '\0' }; //Armazena a entrada bruta do usuário
	char previousInputNumber[MAX_STRING_VECTOR + 1] = { '\0' }; //Armazena temporariamente a entrada anterior do usuário apenas para exibição
	char convertedResultString[MAX_STRING_VECTOR + 1] = { '\0' }; //Armazena o resultado final da conversão                Ex: F  F  7 A  0
	int inputDigitsArray[MAX_STRING_VECTOR + 1] = { 0 }; //Armazena o valor decimal de cada caracter inserido pelo usuário Ex: 15 15 7 10 0

	int sourceBase = 0, targetBase = 0, intermediateDecimal = 0, finalDecimalInput = 0, stringSize = 0, historyLength = 0, currentSortedListSize = 0;
	int menuOption = 0;
	char digitCharacters[] = "0123456789ABCDEF"; //Definição do ue irá representar cada número (acho que é assim que se começa uma criptografia)

	printf("Este é um conversor de números inteiros entre bases numéricas.\n");
	printf("Ex: base de 2 (0 a 1) a base de 16 (0 a F) números.\n");
	printf("Você pode pedir uma conversão aleatória como base 6 (0 a 5).\n\n");
	printf("A conversão é de até %d dígitos para evitar estouro de memória.\n", MAX_STRING_NUMBER);
	printf("A leitura é realizada da esquerda para direita,\n");
	printf("Ex: '0123456789abcdefff' será lido '0123456'.\n\n");
	printf("Não são aceitos valores que excedam a base Hexadecimal,\n");
	printf("Ex: G a Z ou caracteres como @ . , - e etc.\n\n");
	printf("Digite exit para sair a qualquer momento.\n");
	clearScreen();

	while (1) {
		//Resetando variáveis a serem utilizadas
		sourceBase = 0, finalDecimalInput = 0, targetBase = 0;

		printf("=-=-=-=-=-=-=-=-=-= Página Inicial =-=-=-=-=-=-=-=-=-=\n");
		printf("   1 - Nova Conversão\n");
		printf("   2 - Histórico\n");
		printf("   3 - Lista ordenada\n");
		printf("exit - Sair\n");
		printf("Escolha uma opção: ");

		menuOption = getMenuOption();

		switch (menuOption) {
		case 1:
			printf("\nRealizar nova conversão...\n");
			clearScreen();
			//Função para menu de conversão dedicado, evitando identações excessivas
			newConversionMenu();
			break;
		case 2:
			//Histórico vazio
			if (conversionHistory[0].sourceBase == 0) {
				printf("\nHistórico vazio no momento, realize uma nova conversão para visualizar.\n");
				clearScreen();
				break;
			}
			/*
				exibir o histórico
				selecionar posição no histórico
				escolher entre Entrada ou Resultado
				pegar o número e a base escolhida
				realizar nova conversão a partir dos valores selecionados
			*/

			printf("\nExibir o histórico...\n");
			clearScreen();
			if (historyMenu()) {
				newConversionMenu();
			}
			break;
		case 3:
			//Lista vazia
			if (sortedConversionResults[0].targetBase == 0) {
				printf("\nLista vazia no momento, realize uma nova conversão para vizualizar.\n");
				clearScreen();
				break;
			}
			/*
				exibir a lista ordenada
				selecionar posição na lista
				escolher entre Entrada ou Saída
				pegar o número e a base escolhida
				realizar nova conversão a partir dos valores selecionados
			*/
			printf("\nExibir lista ordenada...\n");
			clearScreen();
			showSortedConversionList();
			break;
		case 4: // Sair
			exitConveror();
			break;
		default:
			printf("Entrada inválida!\n\n");
			clearScreen();
			break;
		}
	}
	printf("sai do main loop sem fechar!");
	return 0;
}

//Apenas controlar a "paginação"
void clearScreen() {
	system("pause");
	system("cls");
}

//Realizando a leitura e manipulação do primeiro vetor através de ponteiros
void getUserInput(char* userStr) {
	//Limitando a entrada para sempre ser adicionado '\0' ao final da string/char[]
	scanf("%7s", userStr);
	clearBuffer();

	// Verificando se o usuário digitou exit corretamente para sair
	if (userStr[0] == 'e' && userStr[1] == 'x' && userStr[2] == 'i' && userStr[3] == 't' && userStr[4] == '\0') {
		exitConveror();
	}
}

/*
	Vou comentar que apanhei muito pra aprender sobre este buffer em C
	eu esperava que isso já fosse tratado pelas bibliotecas ou compilador,
	esta função foi criada especificamente para limpar os inputs do usuário
	caso ele digite algo inesperado OUUUU de alguma forma leia um arquivo.

	Esta função realiza o "descarte" de tudo que estiver no buffer
	até ser encontrado uma quebra de linha (\n ou 0X0D0A para Windows) ou fim de arquivo (EOF)
	para que isto não interfira na próxima leitura de um input do usuário
	ou cause um loop infinito de um scanf("%d") esperando um tipo inteiro
	pois há um tipo char esperando alocação no buffer

	Isto poderia ser contornado com fgets e sscanf mas vamos manter as funções aprendidas em aula
*/
void clearBuffer() {
	char clearBuffer;
	while ((clearBuffer = getchar()) != '\n' && clearBuffer != EOF) {}
}

void exitConveror() {
	printf("\nEncerrando o programa...\n");
	exit(0);
}

// Retorna -1 se a entrada não for um número,senao retorna o número digitado
int getMenuOption() {
	int option = -1;
	
	if (scanf("%d", &option) != 1) {
		option = -1;
	}

	clearBuffer();

	return option;
}

//Função dedicada a ser menu de conversão apenas para legibilidade
void newConversionMenu(int sourceBase, char* userStr) {
	/*
		se não há numero e base selecionados:
			pegar base
			pegar numero
			verificar se está dentro da base

		pegar base destino
		converter
		salvar no histórico
		salvar na lista ordenada
	*/

	//pega base
	while (sourceBase == 0) {
		printf("=-=-=-=-=-=-=-=-=-= Nova Conersão - Seleção Primeria Base Numérica =-=-=-=-=-=-=-=-=-=\n");
		printf("\nDe 2 a 16, escolha uma base para começar: ");
		getUserInput(userInputString);
		stringCounter(1, MAX_STRING_NUMBER, userInputString);
		charToIntVector(userInputString, stringSize);
		intVectorToIntNumber(stringSize, 10);

		if (intermediateDecimal == -1) {
			printf("\nCaracter inválido, insira novamente.\n\n");
			clearScreen();
			continue;
		}

		if (intermediateDecimal > 16) {
			printf("\nO valor '%d' excede o valor máximo 16 (Hexadecimal), insira outro valor.\n\n", intermediateDecimal);
			clearScreen();
			continue;
		}

		if (intermediateDecimal < 2) {
			printf("\nO valor '%d' excede o valor mínimo 2 (Binário), insira outro valor.\n\n", intermediateDecimal);
			clearScreen();
			continue;
		}
		sourceBase = intermediateDecimal;
		printf("\nValor inserido: '%d'\n", intermediateDecimal);
		clearScreen();
	}

	//pega o número
	while (finalDecimalInput == 0) {
		printf("=-=-=-=-=-=-=-=-=-= Nova Conersão - Número para Conversão =-=-=-=-=-=-=-=-=-=\n");
		printf("\nBase selecionada  : %d\n", sourceBase);
		printf("\nDigite um número inteiro positivo: ");
		getUserInput(userInputString);
		stringCounter(1, MAX_STRING_NUMBER, userInputString);
		charToIntVector(userInputString, stringSize);

		//checar posições de acordo com a base
		for (int i = 0; i < stringSize; i++) {
			if (inputDigitsArray[i] >= sourceBase) {
				printf("\nNúmero '%d' inserido excede a base '%d' fornecida, insira novamente.\n\n", inputDigitsArray[i], sourceBase);
				inputDigitsArray[0] = -1;
				break;
			}
		}
		if (inputDigitsArray[0] == -1) {
			printf("\nEntrada inválida!\n\n");
			clearScreen();
			continue;
		}

		/*
			Até aqui o usuário digitou corretamente dentro da base, basta converter para decimal
			somando a multiplicação do número em cada posição do array pela base elevada
			a posição do número no array
		*/
		intVectorToIntNumber(stringSize, sourceBase);
		finalDecimalInput = intermediateDecimal;

		if (finalDecimalInput == 0) {
			printf("\nValor '0' é equivalente entre todas as bases, insira novamente.\n\n");
		}
		else {
			for (int i = 0; i < MAX_STRING_VECTOR; i++) {
				previousInputNumber[i] = '\0';
			}
			for (int i = 0; i <= stringSize; i++) {
				if (userInputString[i] != '\0' && userInputString[i] != EOF) {
					previousInputNumber[i] = userInputString[i];
				}
				else {
					previousInputNumber[i] = '\0';
				}
			}
			printf("\nValor inserido: '%s'\n", previousInputNumber);
		}
		clearScreen();
	}

	//pega base destino
	while (targetBase == 0) {
		printf("=-=-=-=-=-=-=-=-=-= Nova Conersão - Seleção Base Destino =-=-=-=-=-=-=-=-=-=\n");
		printf("\nBase selecionada  : %d", sourceBase);
		printf("\nNúmero selecionado: %s\n", previousInputNumber);
		printf("\nEntre 2 e 16, digite uma base numérica de destino: ");
		getUserInput(userInputString);
		stringCounter(1, MAX_STRING_NUMBER, userInputString);
		charToIntVector(userInputString, stringSize);
		intVectorToIntNumber(stringSize, 10);

		if (intermediateDecimal == -1) {
			printf("\nCaracter inválido, insira novamente.\n\n");
			clearScreen();
			continue;
		}

		if (intermediateDecimal > 16) {
			printf("\nO valor '%d' excede o valor máximo 16 (Hexadecimal), insira outro valor.\n\n", intermediateDecimal);
			clearScreen();
			continue;
		}

		if (intermediateDecimal < 2) {
			printf("\nO valor '%d' excede o valor mínimo 2 (Binário), insira outro valor.\n\n", intermediateDecimal);
			clearScreen();
			continue;
		}

		if (sourceBase == intermediateDecimal) {
			printf("\nBase original (%d) e base destino (%d) iguais! Insira outro valor.\n\n", sourceBase, intermediateDecimal);
			clearScreen();
			continue;
		}

		targetBase = intermediateDecimal;
	}

	/*
		Transformar o número do usuário atualmente em decimal para a
		base destino através da divisão sucessiva e guardando o resto da divisão
	*/
	decimalToBaseN(finalDecimalInput, targetBase, convertedResultString);
	printf("\nSeu número foi convertido para %s\n", convertedResultString);
	addToHistory(sourceBase, previousInputNumber, targetBase, convertedResultString);
	addToSortedConversion(finalDecimalInput, targetBase, convertedResultString);
	clearScreen();
}




//Define o valor de stringSize a partir de 1 | int direction = 1 conta para direita, -1 conta para esquerda
void stringCounter(int direction, int vectorLength, char* userStr) {
	stringSize = 0;
	//printf("stringSize zerada: %d\n", stringSize);
	if (direction == 1) {
		for (int i = 0; i <= vectorLength; i++) {
			if (userStr[i] != '\0' && userStr[i] != EOF) {
				stringSize++;
				//printf("Somando stringSize: %d\n", stringSize);
			}
			else {
				//printf("stringSize final: %d\n\n", stringSize);
				break;
			}
		}
	}
	else if (direction == -1) {
		for (int i = vectorLength - 1; i >= 0; i--) {
			if (userStr[i] != '\0' && userStr[i] != EOF) {
				stringSize++;
			}
			else {
				//printf("stringSize final: %d\n\n", stringSize);
				break;
			}
		}

	}
}

//Divisão sucessiva de decimal (decimalNum) para obter o número de qualquer base numérica desejada (targetBase)
void decimalToBaseN(int decimalNum, int targetBase, char* resultStr) {
	char result[MAX_STRING_VECTOR + 1] = { '\0' };
	for (int i = 0; i < MAX_STRING_VECTOR; i++) {
		resultStr[i] = '\0';
	}

	int i = 0, remnant = 0;

	while (decimalNum > 0) {
		//Remnant recebe a sobra da divisão
		// 
		//printf("remnant = decimalNum: %d %% targetBase %d\n", decimalNum, targetBase);
		remnant = decimalNum % targetBase;
		//printf("remnant: %d\n", remnant);

		//Atribui o caracter equivale da sobra da divisão ex: 15 = F, 10 = A, ao resultado
		result[i] = digitCharacters[remnant];
		//printf("result[%d] = digitCharacters[%d]: %c\n", i, remnant, digitCharacters[remnant]);

		//Divide o número decimal recebido para calcular o valor da próxima casa númerica Ex: unidades -> dezenas -> centenas... (divisões sucessivas)
		// 
		//printf("decimalNum: %d /= targetBase: %d\n", decimalNum, targetBase);
		decimalNum /= targetBase;
		//printf("decimalNum: %d\n\n", decimalNum);
		i++;
	}
	stringCounter(1, MAX_STRING_VECTOR, result);
	for (int j = 0; j < stringSize; j++) {
		resultStr[j] = result[stringSize - j - 1];
		//printf("Valor de resultStr[%2d]: %2c | result[%2d]: %2c\n", j, resultStr[j], stringSize-j-1, result[stringSize-j-1]);
	}
	return;
}

//Exponenciação da entrada (numToPow) formatada dentro da base (userBase) obter um número decimal
int nBaseToDecimal(int userBase, int numberIndex, int numberForConversion) {
	if (numberForConversion >= userBase) {
		return -1;
	}

	//printf("intExponentiation(userBase: %d, userPow: %d, numToPow %d)\n", userBase, userPow, numToPow);
	/*
		Realiza a exponenciação de uma base pelo índice da posição de um número
		considere cada número um vetor ex: 101
		índice   :   2     1     0
		Número   :   1     0     1
		Base     :   2
		Conta    : 1*2^2 0*2^1 1*2^0
		Valores  :   4     0     1
		Resultado:   4  +  0  +  1 = 5
	*/
	int result = 1;

	if (numberIndex < 0) {
		return -1;
	}
	else if (numberIndex == 0) {
		return numberForConversion;
	}
	else {
		for (int i = 0; i < numberIndex; i++) {
			result *= userBase;
		}
		result *= numberForConversion;
	}
	return result;
}

//Recebe 'tamanho de vetor' e 'base numérica' | Define valor de int intermediateDecimal
void intVectorToIntNumber(int stringSize, int numBase) {
	intermediateDecimal = 0;

	//Loop exponencial
	for (int i = 0; i < stringSize; i++) {
		if (nBaseToDecimal(numBase, i, inputDigitsArray[stringSize - i - 1]) == -1) {
			//Valor excede a base fornecida ex: A = 10 em base decimal retornará -1
			intermediateDecimal = -1;
			break;
		}
		intermediateDecimal += nBaseToDecimal(numBase, i, inputDigitsArray[stringSize - i - 1]);
	}
	//printf("intermediateDecimal: %d\n", intermediateDecimal);
}

//Transforma a entrada do tipo char[ ] para int[ ] de decimais de acordo com a tabela ASCII | Define valor de inputDigitsArray[ ]
void charToIntVector(char* userStr, int stringSize) {

	/*
		Transformação de acordo com a posição decimal dos caracteres na tabela ASCII
		0 - 9 = 48 a 57
		A - F = 65 a 70
		a - f = 97 a 102
	*/
	for (int i = 0; i < stringSize; i++) {
		if (userStr[i] >= 48 && userStr[i] <= 57) {
			inputDigitsArray[i] = userStr[i] - 48;
			//printf("Valor de userStr: %d\n", inputDigitsArray[i]);
		}
		else if (userStr[i] >= 97 && userStr[i] <= 102) {
			inputDigitsArray[i] = userStr[i] - 87;
			//printf("Valor de userStr: %d\n", inputDigitsArray[i]);
		}
		else if (userStr[i] >= 65 && userStr[i] <= 70) {
			inputDigitsArray[i] = userStr[i] - 55;
			//printf("Valor de userStr: %d\n", inputDigitsArray[i]);
		}
		else {
			//Valor excede base Hexadecimal 0 a F
			//printf("Caractere '%c' inválido.\n", userStr[i]);
			inputDigitsArray[i] = -1;
			return;
		}
	}
}

//Função para inserir no histórico
void addToHistory(int srcBase, char* userInputStr, int tgtBase, char* convertedResultStr) {

	//Laço para subir todos os registros antes de adicionar o próximo
	for (int i = MAX_HISTORY - 1; i > 0; i--) {

		//Se o próximo registro estiver vazio esta operação é pulada
		if (conversionHistory[i - 1].sourceBase == 0) {
			//printf("indice %d pulado.\n", i);
			continue;
		}

		//Operação para copiar todos os dados da posição seguinte para a atual
		conversionHistory[i].sourceBase = conversionHistory[i - 1].sourceBase;
		conversionHistory[i].targetBase = conversionHistory[i - 1].targetBase;
		for (int j = 0; j <= MAX_STRING_VECTOR; j++) {
			conversionHistory[i].userInputString[j] = conversionHistory[i - 1].userInputString[j];
			conversionHistory[i].convertedResultString[j] = conversionHistory[i - 1].convertedResultString[j];
		}
	}

	conversionHistory[0].sourceBase = srcBase;
	conversionHistory[0].targetBase = tgtBase;

	//Contando o tamanho de userInputStr para atribuir corretamente os valores
	stringCounter(1, MAX_STRING_VECTOR, userInputStr);
	for (int i = 0; i < stringSize; i++) {
		conversionHistory[0].userInputString[i] = userInputStr[i];
	}
	conversionHistory[0].userInputString[stringSize] = '\0'; //Colocando \0 por garantia

	//Contando o tamanho de userInputStr para atribuir corretamente os valores
	stringCounter(1, MAX_STRING_VECTOR, convertedResultStr);
	for (int i = 0; i < stringSize; i++) {
		conversionHistory[0].convertedResultString[i] = convertedResultStr[i];
	}
	conversionHistory[0].convertedResultString[stringSize] = '\0'; //Colocando \0 por garantia

	//printf("conversionHistory[0].userInputString      : %-28s\n", conversionHistory[0].userInputString);
	//printf("conversionHistory[0].convertedResultString: %-28s\n\n", conversionHistory[0].convertedResultString);

}

//Exibir o histórico
void showHistory() {
	historyLength = 0;

	printf("=-=-=-=-=-=-=-=-=-= Histórico =-=-=-=-=-=-=-=-=-=");
	for (int i = MAX_HISTORY - 1; i >= 0; i--) {
		//Se o registro estiver vazio esta operação é pulada
		if (conversionHistory[i].sourceBase == 0) {
			//printf("indice %d pulado\n", i+1);
			continue;
		}
		historyLength++;
		printf("\n%dº posição do histórico\n", i + 1);
		printf("          Base | Número\n");
		printf("ENTRADA  : %2d  |  %-28s\n", conversionHistory[i].sourceBase, conversionHistory[i].userInputString);
		printf("RESULTADO: %2d  |  %-28s\n", conversionHistory[i].targetBase, conversionHistory[i].convertedResultString);
	}
	printf("\nTotal de posições: %d\n", historyLength);
	printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
}

void getHistoryValues(int historyPosition) {
	while (1) {
		printf("\n%dº posição do histórico\n", historyPosition + 1);
		printf("             Base | Número\n");
		printf("1- ENTRADA  : %2d  |  %-28s\n", conversionHistory[historyPosition].sourceBase, conversionHistory[historyPosition].userInputString);
		printf("2- RESULTADO: %2d  |  %-28s", conversionHistory[historyPosition].targetBase, conversionHistory[historyPosition].convertedResultString);
		printf("\n\nSelecione 1- ENTRADA ou 2- RESULTADO: ");
		getUserInput(userInputString);
		stringCounter(1, MAX_STRING_NUMBER, userInputString);
		charToIntVector(userInputString, stringSize);
		intVectorToIntNumber(stringSize, 10);

		switch (intermediateDecimal) {
		case 1:
			stringCounter(1, MAX_STRING_VECTOR, conversionHistory[historyPosition].userInputString);
			charToIntVector(conversionHistory[historyPosition].userInputString, stringSize);
			intVectorToIntNumber(stringSize, conversionHistory[historyPosition].sourceBase);

			for (int i = 0; i < stringSize; i++) {
				previousInputNumber[i] = conversionHistory[historyPosition].userInputString[i];
			}
			previousInputNumber[stringSize] = '\0';
			for (int i = stringSize; i < MAX_STRING_VECTOR; i++) {
				previousInputNumber[i] = 0;
			}
			//printf("\npreviousInputNumber: %s\n", previousInputNumber);

			finalDecimalInput = intermediateDecimal;
			sourceBase = conversionHistory[historyPosition].sourceBase;

			printf("\nValores selecionados:\nBase  : %d\nNúmero: %s\n\n", sourceBase, conversionHistory[historyPosition].userInputString);
			clearScreen();
			return;
			break;
		case 2:
			stringCounter(1, MAX_STRING_VECTOR, conversionHistory[historyPosition].convertedResultString);
			charToIntVector(conversionHistory[historyPosition].convertedResultString, stringSize);
			intVectorToIntNumber(stringSize, conversionHistory[historyPosition].targetBase);

			for (int i = 0; i < stringSize; i++) {
				previousInputNumber[i] = inputDigitsArray[i] + 48;
			}
			previousInputNumber[stringSize] = '\0';
			for (int i = stringSize; i < MAX_STRING_VECTOR; i++) {
				previousInputNumber[i] = 0;
			}
			printf("\npreviousInputNumber: %s\n", previousInputNumber);

			finalDecimalInput = intermediateDecimal;
			sourceBase = conversionHistory[historyPosition].targetBase;

			printf("\nValores selecionados:\nBase  : %d\nNúmero: %s\n\n", conversionHistory[historyPosition].targetBase, conversionHistory[historyPosition].convertedResultString);
			clearScreen();
			return;
			break;
		default:
			printf("Entrada inválida!\n\n");
			clearScreen();
			break;
		}
	}
}

int historyMenu() {
	int historyPosition = 0;
	int userInputOutput = 0;

	while (1) {
		showHistory();
		printf("   1 - Nova conversão a partir do histórico\n");
		printf("   2 - Voltar\n");
		printf("exit - Sair\n");
		printf("Escolha uma opção: ");
		getUserInput(userInputString);
		stringCounter(1, MAX_STRING_NUMBER, userInputString);
		charToIntVector(userInputString, stringSize);
		intVectorToIntNumber(stringSize, 10);

		switch (intermediateDecimal) {
		case 1:
			printf("\nNova conversão a partir do histórico...\n");
			clearScreen();

			while (1) {
				showHistory();
				printf("Escolha uma posição do histórico: ");
				getUserInput(userInputString);
				stringCounter(1, MAX_STRING_NUMBER, userInputString);
				charToIntVector(userInputString, stringSize);
				intVectorToIntNumber(stringSize, 10);
				historyPosition = intermediateDecimal;

				if (historyPosition <= historyLength) {
					printf("\nPosição %d selecionada...\n", historyPosition);
					clearScreen();

					historyPosition--;
					getHistoryValues(historyPosition);
					return 1;
				}
				printf("Entrada inválida!\n\n");
				clearScreen();
			}
			break;
		case 2:
			printf("\nVoltando para o menu principal...\n");
			clearScreen();
			return 0;
			break;
		case 3:
			exitConveror();
			break;
		default:
			printf("Entrada inválida!\n\n");
			break;
		}
		clearScreen();
	}
}

/*
	A meta era fazer verificação binária O(log n), mas devido a realocação dos valores dentro do vetor
	o custo de operação se torna O(n) sendo n os elementos dentro do array...


	Alocação dos valores para direita O(n), depende da posição encontrada na virificação...
	no pior caso será a primeira posição e o array estará cheio	e
	consequentemente movendo TODOS os elementos do array para direita...

	Alocação do valor inserido O(1) pq já sabemos a posição pela verificação

	resultado final: operação O(n) pois é a maior grandeza das operações
*/
void addToSortedConversion(int decimalNum, int tgtBase, char* convertedResultStr) {
	int insertIndex = currentSortedListSize; //Inicia já com o index da última posição

	//Loop para achar o index de um número maior que a entrada
	for (int i = 0; i < currentSortedListSize; i++) {
		if (decimalNum < sortedConversionResults[i].intermediateDecimal) {
			insertIndex = i;
			break;
		}
	}

	//Movendo todos os elementos maiores que a entrada para direita
	for (int i = currentSortedListSize; i > insertIndex; i--) {
		sortedConversionResults[i] = sortedConversionResults[i - 1];
	}

	//Salva os dados na posição de insertIndex
	sortedConversionResults[insertIndex].targetBase = tgtBase;
	sortedConversionResults[insertIndex].intermediateDecimal = decimalNum;
	strncpy_s(sortedConversionResults[insertIndex].convertedResultString, MAX_STRING_VECTOR + 1, convertedResultStr, MAX_STRING_VECTOR + 1);

	if (currentSortedListSize < MAX_HISTORY) {
		currentSortedListSize++;
	}
}

void showSortedConversionList() {
	printf("=-=-=-=-=-=-=-=-=-= Lista Ordenada =-=-=-=-=-=-=-=-=-=\n");
	for (int i = currentSortedListSize - 1; i >= 0; i--) {
		printf("\n%dº posição do histórico\n", i + 1);
		printf("\tBase  |  Número\n");
		printf("\t%2d    |  %-28s\n", sortedConversionResults[i].targetBase, sortedConversionResults[i].convertedResultString);
	}
	printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
	clearScreen();
}