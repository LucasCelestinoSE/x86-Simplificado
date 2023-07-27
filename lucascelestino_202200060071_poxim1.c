// Como executar no terminal:
// gcc -Wall -O3 nomesobrenome_123456789012_exemplo.c -o nomesobrenome_123456789012_exemplo
// ./nomesobrenome_123456789012_exemplo entrada saida

// Numeros padronizados
	#include <stdint.h>
	#include <string.h>
	#include <math.h>
	// Biblioteca padrao
	#include <stdlib.h>
	// Entrada/saida padrao
	#include <stdio.h>
// Maximo de tam
#define MAX_LENGTH 20
// Funcao principal
int potencia(int x, int n){
    if(n == 0)
        return 1;
    else
        return x * potencia(x, n - 1);
}
int main(int argc, char* argv[]) {
	uint8_t sr = 0;
	// Exibindo a quantidade de argumentos
	printf("Quantidade de argumentos (argc): %i\n", argc);
	// Iterando sobre o(s) argumento(s) do programa
	for(uint32_t i = 0; i < argc; i++) {
		// Mostrando o argumento i
		printf("Argumento %i (argv[%i]): %s\n", i, i, argv[i]);
	}
	// Abrindo os arquivos com as permissoes corretas
	//FILE* input = fopen(argv[1], "r");

	FILE* input = fopen("intro.hex", "r");
	FILE* output = fopen("intro.out", "w");
    char linha[256];
	
    // Abre o arquivo para leitura
    //input = fopen("intro.hex", "r");
    // Lê e imprime cada linha do arquivo
   
	// 32 registradores inicializados com zero
	uint32_t R[32] = { 0 };
	// Memoria com 32 KiB inicializada com zero
	uint8_t* MEM8 = (uint8_t*)(calloc(32, 1024));
	uint32_t* MEM32 = (uint32_t*)(calloc(8, 1024));
	int linhaIndex = 0;
	while (fgets(linha, sizeof(linha), input) != NULL) {
        sscanf(linha, "%x", &MEM32[linhaIndex]);
        linhaIndex++;
    }
    #define u32_para_String
   char* u32toS(int num) {
    char* buffer = (char*)malloc(MAX_LENGTH * sizeof(char));
    if (buffer == NULL) {
        fprintf(stderr, "Erro de alocação de memória.\n");
        exit(1);
    }

    switch (num) {
        case 28:
            strcpy(buffer, "ir");
            break;
        case 29:
            strcpy(buffer, "pc");
            break;
        case 30:
            strcpy(buffer, "sp");
            break;
        case 31:
            strcpy(buffer, "sr");
            break;
        default:
            snprintf(buffer, MAX_LENGTH, "r%d", num);
    }

    return buffer;
}
char* u32toSUper(uint32_t num) {
    char* buffer = (char*)malloc(MAX_LENGTH * sizeof(char));
    if (buffer == NULL) {
        fprintf(stderr, "Erro de alocação de memória.\n");
        exit(1);
    }
	#define UperCase
    switch (num) {
        case 28:
            strcpy(buffer, "IR");
            break;
        case 29:
            strcpy(buffer, "PC");
            break;
        case 30:
            strcpy(buffer, "SP");
            break;
        case 31:
            strcpy(buffer, "SR");
            break;
        default:
            snprintf(buffer, MAX_LENGTH, "R%d", num);
    }

    return buffer;
}
	fprintf(output,"[START OF SIMULATION]\n");
	// Setando a condicao de execucao para verdadeiro
	uint8_t executa = 1;
	// Enquanto executa for verdadeiro
	#define inicioLoop 
	
	while(executa) {
		// Cadeia de caracteres da instrucao
		char instrucao[30] = { 0 };
		// Declarando operandos
		uint8_t z = 0, x = 0, i = 0, y = 0, sraux = 0;
		uint32_t pc = 0, xyl = 0;
		uint64_t temp = 0,resultado = 0;
		int64_t temps = 0;
		int32_t temps32 = 0;
		// Carregando a instrucao de 32 bits (4 bytes) da memoria indexada pelo PC (R29) no registrador IR (R28)
		// E feita a leitura redundante com MEM8 e MEM32 para mostrar formas equivalentes de acesso
		// Se X (MEM8) for igual a Y (MEM32), entao X e Y sao iguais a X | Y (redundancia)
		R[28] = ((MEM8[R[29] + 0] << 24) | (MEM8[R[29] + 1] << 16) | (MEM8[R[29] + 2] << 8) | (MEM8[R[29] + 3] << 0)) | MEM32[R[29] >> 2];
		// Obtendo o codigo da operacao (6 bits mais significativos)
		uint8_t opcode = (R[28] & (0b111111 << 26)) >> 26, opcode2 = 0;
		// Decodificando a instrucao buscada na memoria
		#define inicioSwitch
		switch(opcode) {
			// mov
			#define mov
			case 0b000000:
				// Obtendo operandos
				z = (R[28] & (0b11111 << 21)) >> 21;
				xyl = R[28] & 0x1FFFFF; //0x1FFFFF == 00000001111111111111111111, 21 bits reservados a xyl, e o resto acrescido de 0s á esquerda.
				// R[28] & 0x1FFFFF, os 21 bits se preservam e são adicionados um padding 
				// Execucao do comportamento
				R[z] = xyl;
				// Formatacao da instrucao
				sprintf(instrucao, "mov %s,%u", u32toS(z), xyl);
				// Formatacao de saida em tela (deve mudar para o arquivo de saida)
				fprintf(output,"0x%08X:\t%-25s\t%s=0x%08X\n", R[29], instrucao, u32toSUper(z), xyl);
				break;
			// l8
			case 0b011000:
				// Otendo operandos
				z = (R[28] & (0b11111 << 21)) >> 21;
				x = (R[28] & (0b11111 << 16)) >> 16;
				i = R[28] & 0xFFFF;
				// Execucao do comportamento com MEM8 e MEM32
				R[z] = MEM8[R[x] + i] | (((uint8_t*)(MEM32))[(R[x] + i) >> 2]);
				// Formatacao da instrucao
				sprintf(instrucao, "l8 r%u,[r%u%s%i]", z, x, (i >= 0) ? ("+") : (""), i);
				// Formatacao de saida em tela (deve mudar para o arquivo de saida)
				fprintf(output,"0x%08X:\t%-25s\tR%u=MEM[0x%08X]=0x%02X\n", R[29], instrucao, z, R[x] + i, R[z]);
				break;
			// l32
			case 0b011010:
				// Otendo operandos
				z = (R[28] & (0b11111 << 21)) >> 21;
				x = (R[28] & (0b11111 << 16)) >> 16;
				i = R[28] & 0xFFFF;
				// Execucao do comportamento com MEM8 e MEM32
				R[z] = ((MEM8[((R[x] + i) << 2) + 0] << 24) | (MEM8[((R[x] + i) << 2) + 1] << 16) | (MEM8[((R[x] + i) << 2) + 2] << 8) | (MEM8[((R[x] + i) << 2) + 3] << 0)) | MEM32[R[x] + i];
				// Formatacao da instrucao
				sprintf(instrucao, "l32 r%u,[r%u%s%i]", z, x, (i >= 0) ? ("+") : (""), i);
				// Formatacao de saida em tela (deve mudar para o arquivo de saida)
				fprintf(output,"0x%08X:\t%-25s\tR%u=MEM[0x%08X]=0x%08X\n", R[29], instrucao, z, (R[x] + i) << 2, R[z]);
				break;
			// bun
			case 0b110111:
				// Armazenando o PC antigo
				pc = R[29];
				// Execucao do comportamento
				R[29] = R[29] + ((R[28] & 0x3FFFFFF) << 2);
				// Formatacao da instrucao
				sprintf(instrucao, "bun %i", R[28] & 0x3FFFFFF);
				// Formatacao de saida em tela (deve mudar para o arquivo de saida)
				fprintf(output,"0x%08X:\t%-25s\tPC=0x%08X\n", pc, instrucao, R[29] + 4);
				break;
			// int
			case 0b111111:
				// Parar a execucao
				executa = 0;
				// Formatacao da instrucao
				sprintf(instrucao, "int 0");
				// Formatacao de saida em tela (deve mudar para o arquivo de saida)
				fprintf(output,"0x%08X:\t%-25s\tCR=0x00000000,PC=0x00000000\n", R[29], instrucao);
				break;
			// Fazer o resto depois (Ov, Zn, CY, SN)
			//Add
			#define add
			case 0b000010:
				
				z = (R[28] & (0b11111 << 21)) >> 21;
				x = (R[28] & (0b11111 << 16)) >> 16;
				y = (R[28] & (0b11111 << 11)) >> 11;
				R[z] = R[x] + R[y];
				resultado = R[x] + R[y];
				//Zn
				if (R[z] == 0){sr = 0b01000000;};
				//Sn
				if ((R[z] >> 31) & 1){sr = 0b00010000;};
				// Pegando os bits de (R[x][31] == R[y][31] ) && (R[z][31] != R[x][31])
				// Ov 
				if ((((R[x] >> 31) & 1) == ((R[y] >> 31) & 1)) && (((R[z] >> 31) & 1) != ((R[x] >> 31) & 1))) { sr = sr | 0b00001000; };
				// Cy
				if ((resultado < R[x]) || (resultado < R[y])){sr = 0b00000001;};
				sprintf(instrucao, "add %s,%s,%s",  u32toS(z),  u32toS(x),  u32toS(y));
				fprintf(output,"0x%08X:\t%-25s\t%s=%s+%s=0x%08X,SR=0x%08X\n", R[29], instrucao, u32toSUper(z),u32toSUper(x),u32toSUper(y), R[z],sr);
				
				break;
			//Sub
			#define sub
			case 0b000011:
				
				z = (R[28] & (0b11111 << 21)) >> 21;
				x = (R[28] & (0b11111 << 16)) >> 16;
				y = (R[28] & (0b11111 << 11)) >> 11;
				R[z] = R[x] - R[y];
				resultado = R[x] - R[y];
				//Zn
				if (R[z] == 0){sr = 0b01000000;};
				//Sn
				if ((R[z] >> 31) & 1){sr = 0b00010000;};
				// Pegando os bits de (R[x][31] == R[y][31] ) && (R[z][31] != R[x][31])
				// Ov 
				if ((((R[x] >> 31)) != ((R[y] >> 31) )) && (((R[z] >> 31)) != ((R[x] >> 31)))) { sr = 0b00001000; };
				// Cy
				if (resultado >> 32){sr = 0b00000001;};
				sprintf(instrucao, "sub %s,%s,%s",  u32toS(z),  u32toS(x),  u32toS(y));
				fprintf(output,"0x%08X:\t%-25s\t%s=%s-%s=0x%08X,SR=0x%08X\n", R[29], instrucao, u32toSUper(z),u32toSUper(x),u32toSUper(y), R[z],sr);
				
				
				break;
			#define movs
			case 0b000001:
				z = (R[28] & (0b11111 << 21)) >> 21;
				xyl = (R[28] << 11) >> 11;
				// Colocando o bit posição 20 na posição 31
				R[z] = R[28] << 11;
				//Colocando o bit de forma 0x00000001 ou 0x00000000
				// Aplicando a mascara para preencher os zeros do deslocamento >> 11 com 1
				if (R[z] >> 31) {
					R[z] = xyl | 0xFFE00000;
				} else {
					R[z] = (R[z] >> 11) & 0x000007FF;
				}
				sprintf(instrucao, "movs r%d,%d",  z, R[z]);
				fprintf(output,"0x%08X:\t%-25s\t%s=0x%08X\n", R[29], instrucao, u32toSUper(z),R[z]);
			break;
			#define cmp
			case 0b000101:
				x = (R[28] & (0b11111 << 16)) >> 16;
				y = (R[28] & (0b11111 << 11)) >> 11;
				// cmp
				xyl = R[x] - R[y];
				resultado = R[x] - R[y];
				//ZN
				if (xyl == 0){sr = 0b01000000;};
				//Sn
				if ((xyl >> 31) & 1){sr = 0b00010000;};
				// Pegando os bits de (R[x][31] == R[y][31] ) && (R[z][31] != R[x][31])
				// Ov 
				if ((((R[x] >> 31) & 1) != ((R[y] >> 31) & 1)) && (((xyl >> 31) & 1) != ((R[x] >> 31) & 1))) {  sr = 0b00001000; };
				// Cy
				if ((resultado < R[x]) || (resultado < R[y])){sr = 0b00000001;};
				sprintf(instrucao, "cmp %s,%s",  u32toS(x),  u32toS(y));
				fprintf(output,"0x%08X:\t%-25s\tSR=0x%08X\n", R[29], instrucao, sr);
			break;
			#define and
			case 0b000110:
				z = (R[28] & (0b11111 << 21)) >> 21;
				x = (R[28] & (0b11111 << 16)) >> 16;
				y = (R[28] & (0b11111 << 11)) >> 11;
				R[z] = R[x] & R[y];
				if (R[z] == 0){sr = 0x00000040;};
				if ((R[z] >> 31) & 1){sr = 0x00000010;};
				sprintf(instrucao, "and %s,%s,%s",  u32toS(z),u32toS(x), u32toS(y));
				fprintf(output,"0x%08X:\t%-25s\t%s=%s&%s=0x%08X,SR=0x%08X\n", R[29], instrucao, u32toSUper(z),u32toSUper(x),u32toSUper(y), R[z],sr);
			break;
			#define or
			case 0b000111:
				z = (R[28] & (0b11111 << 21)) >> 21;
				x = (R[28] & (0b11111 << 16)) >> 16;
				y = (R[28] & (0b11111 << 11)) >> 11;
				R[z] = R[x] | R[y];
				if (R[z] == 0){sr = 0b01000000;};
				if ((R[z] >> 31) & 1){sr = 0b01000000;};
				sprintf(instrucao, "or %s,%s,%s",  u32toS(z),u32toS(x), u32toS(y));
				fprintf(output,"0x%08X:\t%-25s\t%s=%s|%s=0x%08X,SR=0x%08X\n", R[29], instrucao, u32toSUper(z),u32toSUper(x),u32toSUper(y), R[z],sr);
			break;
			#define not
			case 0b001000:
				z = (R[28] & (0b11111 << 21)) >> 21;
				x = (R[28] & (0b11111 << 16)) >> 16;
				R[z] = ~R[x];
				if (R[z] == 0){sr = 0b01000000;};
				if ((R[z] >> 31) & 1){sr = 0b01000000;};
				sprintf(instrucao, "not %s,%s",  u32toS(z),u32toS(x));
				fprintf(output,"0x%08X:\t%-25s\t%s=~%s=0x%08X,SR=0x%08X\n", R[29], instrucao, u32toSUper(z),u32toSUper(x),R[z],sr);
			break;
			#define xor
			case 0b001001:
				z = (R[28] & (0b11111 << 21)) >> 21;
				x = (R[28] & (0b11111 << 16)) >> 16;
				y = (R[28] & (0b11111 << 11)) >> 11;
				R[z] = R[x] ^ R[y];
				if (R[z] == 0){sr = 0b01000000;};
				if ((R[z] >> 31) & 1){sr = 0b01000000;};
				sprintf(instrucao, "xor %s,%s,%s",  u32toS(z),u32toS(x), u32toS(y));
				fprintf(output,"0x%08X:\t%-25s\t%s=%s^%s=0x%08X,SR=0x%08X\n", R[29], instrucao, u32toSUper(z),u32toSUper(x),u32toSUper(y), R[z],sr);
			break;
			#define calli
			case 0b0111001:
				x = (R[28] & (0b11111 << 16)) >> 16;
				i =  R[28] & 0x03FFFFFF;
				fprintf(output,"0x%08X     0x%08X\n", R[28], i);
		
				sprintf(instrucao, "call %s,%s,%s",  u32toS(z),u32toS(x), u32toS(y));
				fprintf(output,"0x%08X:\t%-25s\t%s=%s^%s=0x%08X\n", R[29], instrucao, u32toSUper(z),u32toSUper(x),u32toSUper(y), R[z]);
			break;
			#define objetoMul
			case 0b000100:
				opcode2 = (R[28] >> 8) & 0b111;
				#define mul
				switch(opcode2){
					case 0b000:
						z = (R[28] & (0b11111 << 21)) >> 21;
						x = (R[28] & (0b11111 << 16)) >> 16;
						y = (R[28] & (0b11111 << 11)) >> 11;
						i = (R[28] << 27) >> 27;
						temp = R[x] * R[y];
						// bits mais significativos ? 
						R[i] = (temp >> 32);
						R[z] = (temp << 32) >> 32;
						temp = ((uint64_t)R[i] << 32) | R[z];
						if (temp == 0){sr = 0b01000000;};
						if (R[i] != 0){sr = 0b00000001;};
						temp = R[x] * R[y];
						
						sprintf(instrucao, "mul %s,%s,%s,%s", u32toS(i), u32toS(z),u32toS(x), u32toS(y));
						fprintf(output,"0x%08X:\t%-25s\t%s:%s=%s*%s=0x%016X,SR=0x%08X\n", R[29], instrucao, u32toSUper(i),u32toSUper(z),u32toSUper(x),u32toSUper(y), R[z],sr);
						break;
					#define sla
					case 0b011:
						z = (R[28] & (0b11111 << 21)) >> 21;
						x = (R[28] & (0b11111 << 16)) >> 16;
						y = (R[28] & (0b11111 << 11)) >> 11;
						i = (R[28] << 27) >> 27;
						temp = (((int64_t)R[z] << 32) | R[y]) * potencia(2,i+1);
						int32_t temp_low_32_bits = (int32_t)temp; // Obtém os 32 bits menos significativos de temp
						if (!z){R[z] = 0;}else{R[z] = temp >> 32;};
						
						R[x] = temp_low_32_bits;
						if (temp == 0){sr = 0b01000000;};
						if (R[z] != 0 ){sr = 0b00001000;};
						
						sprintf(instrucao, "sla %s,%s,%s,%d", u32toS(z), u32toS(x),u32toS(y), i);
						fprintf(output,"0x%08X:\t%-25s\t%s:%s=%s<<%d=0x%016X,SR=0x%08X\n", R[29], instrucao,
						u32toSUper(z),
						u32toSUper(x),
						u32toSUper(y),
						i+1,
						temp_low_32_bits,sr);
						break;
						#define sll
					case 0b001:
						z = (R[28] & (0b11111 << 21)) >> 21;
						x = (R[28] & (0b11111 << 16)) >> 16;
						y = (R[28] & (0b11111 << 11)) >> 11;
						i = (R[28] << 27) >> 27;
						temp = (((uint64_t)R[z] << 32) | R[y]) * potencia(2,i+1);
						temp_low_32_bits = (uint32_t)temp; // Obtém os 32 bits menos significativos de temp
						R[z] = temp >> 32;
						R[x] = temp_low_32_bits;
						if (temp == 0){sr = 0b01000000;};
						if (R[z] != 0 ){sr = 0x00000001;};
						sprintf(instrucao, "sll %s,%s,%s,%d", u32toS(z), u32toS(x),u32toS(y), i);
						fprintf(output,"0x%08X:\t%-25s\t%s:%s=%s:%s<<%d=0x%016X,SR=0x%08X\n",
						R[29],
						instrucao,
						u32toSUper(z),
						u32toSUper(x),
						u32toSUper(z),
						u32toSUper(y),
						i+1,R[x],
						sr);
						break;
						#define muls
					case 0b010:
						z = (R[28] & (0b11111 << 21)) >> 21;
						x = (R[28] & (0b11111 << 16)) >> 16;
						y = (R[28] & (0b11111 << 11)) >> 11;
						i = (R[28] << 27) >> 27;
						temp = R[x] * R[y];
						// bits mais significativos ? 
						R[i] = (temp >> 32);
						R[z] = (temp << 32) >> 32;
						temp = ((int64_t)R[i] << 32) | R[z];
						if (temp == 0){sr = 0b01000000;};
						if (R[i] != 0){sr = 0b00000001;};
						temp = R[x] * R[y];
						
						sprintf(instrucao, "muls %s,%s,%s,%s", u32toS(i), u32toS(z),u32toS(x), u32toS(y));
						fprintf(output,"0x%08X:\t%-25s\t%s:%s=%s*%s=0x%016X,SR=0x%08X\n", R[29], instrucao,
						u32toSUper(i),
						u32toSUper(z),
						u32toSUper(x),
						u32toSUper(y),
						R[z],
						sr);
						break;
						#define div
					case 0b100:
						z = (R[28] & (0b11111 << 21)) >> 21;
						x = (R[28] & (0b11111 << 16)) >> 16;
						y = (R[28] & (0b11111 << 11)) >> 11;
						i = (R[28] << 27) >> 27;
						
						if (R[z] == 0){sraux = sraux | 0b01000000;};
						if (R[y] == 0){sraux = sraux | 0b00100000;};
						if (R[i] != 0){sraux = sraux | 0b00000001;};
						if (sraux == 0){sr = sr;}else{ sr = sraux;};
						sprintf(instrucao, "div %s,%s,%s,%s", u32toS(i), u32toS(z),u32toS(x), u32toS(y));
						fprintf(output,"0x%08X:\t%-25s\t%s:%s=%s%%%s=0x%08X,SR=0x%08X\n", R[29], instrucao,
						u32toSUper(i),
						u32toSUper(z),
						u32toSUper(x),
						u32toSUper(y),
						R[z],
						sr);
						break;
					
				}
				break;

			// Instrucao desconhecida
			default:
				// Exibindo mensagem de erro
				fprintf(output,"Instrucao desconhecida!\n");
				// Parar a execucao
				executa = 0;
		}
		// PC = PC + 4 (proxima instrucao)
		R[29] = R[29] + 4;
	}
	// Exibindo a finalizacao da execucao
	fprintf(output,"[END OF SIMULATION]\n");
	 
	// Fechando os arquivos
	fclose(input);
	fclose(output);
	// Finalizando programa
	return 0;
}
