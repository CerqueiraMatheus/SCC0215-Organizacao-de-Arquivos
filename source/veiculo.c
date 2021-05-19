// Pedro Lucas de Moliner de Castro - 11795784
// Matheus Henrique de Cerqueira Pinto - 11911104

#include "veiculo.h"

#include "util.h"

// Cabeçalho Veículo

int leCabecalhoVeiculoCsv(CabecalhoVeiculo *cabecalho, FILE *csv) {
    // Lê string do CSV
    char *string = leStringCsv(csv);
    if (string == NULL)
        return EOF;
    char *leitor = string;

    cabecalho->status = '0';
    cabecalho->byteProxReg = 0;
    cabecalho->nroRegistros = 0;
    cabecalho->nroRegRemovidos = 0;

    strcpy(cabecalho->descrevePrefixo, strsep(&leitor, ","));
    strcpy(cabecalho->descreveData, strsep(&leitor, ","));
    strcpy(cabecalho->descreveLugares, strsep(&leitor, ","));
    strcpy(cabecalho->descreveLinha, strsep(&leitor, ","));
    strcpy(cabecalho->descreveModelo, strsep(&leitor, ","));
    strcpy(cabecalho->descreveCategoria, strsep(&leitor, ","));

    free(string);
    return 0;
}

void escreveCabecalhoVeiculoBinario(CabecalhoVeiculo *cabecalho, FILE *binario) {
    fwrite(&cabecalho->status, sizeof(char), 1, binario);
    fwrite(&cabecalho->byteProxReg, sizeof(long long int), 1, binario);
    fwrite(&cabecalho->nroRegistros, sizeof(int), 1, binario);
    fwrite(&cabecalho->nroRegRemovidos, sizeof(int), 1, binario);

    fwrite(cabecalho->descrevePrefixo, sizeof(char), 18, binario);
    fwrite(cabecalho->descreveData, sizeof(char), 35, binario);
    fwrite(cabecalho->descreveLugares, sizeof(char), 42, binario);
    fwrite(cabecalho->descreveLinha, sizeof(char), 26, binario);
    fwrite(cabecalho->descreveModelo, sizeof(char), 17, binario);
    fwrite(cabecalho->descreveCategoria, sizeof(char), 20, binario);
}

void leCabecalhoVeiculoBinario(CabecalhoVeiculo *cabecalhoVeiculo, FILE *binario) {
    fread(&cabecalhoVeiculo->status, sizeof(char), 1, binario);
    fread(&cabecalhoVeiculo->byteProxReg, sizeof(long long int), 1, binario);
    fread(&cabecalhoVeiculo->nroRegistros, sizeof(int), 1, binario);
    fread(&cabecalhoVeiculo->nroRegRemovidos, sizeof(int), 1, binario);

    fread(cabecalhoVeiculo->descrevePrefixo, sizeof(char), 18, binario);
    fread(cabecalhoVeiculo->descreveData, sizeof(char), 35, binario);
    fread(cabecalhoVeiculo->descreveLugares, sizeof(char), 42, binario);
    fread(cabecalhoVeiculo->descreveLinha, sizeof(char), 26, binario);
    fread(cabecalhoVeiculo->descreveModelo, sizeof(char), 17, binario);
    fread(cabecalhoVeiculo->descreveCategoria, sizeof(char), 20, binario);
}

// Veículo

int leVeiculoCsv(Veiculo *veiculo, FILE *csv) {
    // Lê string do CSV
    char *string = leStringCsv(csv);
    if (string == NULL)
        return EOF;
    char *leitor = string;

    // Checa por registro removido
    if (leitor[0] == '*') {
        veiculo->removido = '0';
        leitor++;
    } else {
        veiculo->removido = '1';
    }

    strcpy(veiculo->prefixo, strsep(&leitor, ","));
    strcpy(veiculo->data, stringParaCampoString(strsep(&leitor, ",")));

    veiculo->quantidadeLugares = stringParaCampoInteiro(strsep(&leitor, ","));
    veiculo->codLinha = stringParaCampoInteiro(strsep(&leitor, ","));

    strcpy(veiculo->modelo, stringParaCampoString(strsep(&leitor, ",")));
    strcpy(veiculo->categoria, stringParaCampoString(strsep(&leitor, ",")));

    veiculo->tamanhoModelo = strlen(veiculo->modelo);
    veiculo->tamanhoCategoria = strlen(veiculo->categoria);
    veiculo->tamanhoRegistro = 31 + veiculo->tamanhoModelo + veiculo->tamanhoCategoria;

    free(string);
    return 0;
}

void escreveVeiculoBinario(Veiculo *veiculo, FILE *binario) {
    fwrite(&veiculo->removido, sizeof(char), 1, binario);
    fwrite(&veiculo->tamanhoRegistro, sizeof(int), 1, binario);

    fwrite(veiculo->prefixo, sizeof(char), 5, binario);

    // Escreve o campo data possivelmente nulo
    if (strlen(veiculo->data) == 0)
        escreveStringNuloBinario(10, binario);
    else
        fwrite(veiculo->data, sizeof(char), 10, binario);

    fwrite(&veiculo->quantidadeLugares, sizeof(int), 1, binario);
    fwrite(&veiculo->codLinha, sizeof(int), 1, binario);

    fwrite(&veiculo->tamanhoModelo, sizeof(int), 1, binario);
    fwrite(veiculo->modelo, sizeof(char), veiculo->tamanhoModelo, binario);

    fwrite(&veiculo->tamanhoCategoria, sizeof(int), 1, binario);
    fwrite(veiculo->categoria, sizeof(char), veiculo->tamanhoCategoria, binario);
}

bool leVeiculoBinario(Veiculo *veiculo, FILE *binario) {
    fread(&veiculo->removido, sizeof(char), 1, binario);
    fread(&veiculo->tamanhoRegistro, sizeof(int), 1, binario);

    // Se o registro foi removido, mata a execução
    if (foiRemovido(veiculo->removido)) return false;

    fread(veiculo->prefixo, sizeof(char), 5, binario);

    fread(veiculo->data, sizeof(char), 10, binario);
    fread(&veiculo->quantidadeLugares, sizeof(int), 1, binario);
    fread(&veiculo->codLinha, sizeof(int), 1, binario);

    fread(&veiculo->tamanhoModelo, sizeof(int), 1, binario);
    fread(veiculo->modelo, sizeof(char), veiculo->tamanhoModelo, binario);
    veiculo->modelo[veiculo->tamanhoModelo] = '\0';

    fread(&veiculo->tamanhoCategoria, sizeof(int), 1, binario);
    fread(veiculo->categoria, sizeof(char), veiculo->tamanhoCategoria, binario);
    veiculo->categoria[veiculo->tamanhoCategoria] = '\0';

    // Garante a completude do registro
    return true;
}