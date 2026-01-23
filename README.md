# Universal Number Converter (C)

Este projeto é um conversor de bases numéricas universal desenvolvido puramente em **C**. O diferencial desta aplicação é a implementação de algoritmos de conversão sem o auxílio de bibliotecas externas de manipulação de strings, focando em lógica de baixo nível, aritmética computacional e manipulação direta de memória.

## Funcionalidades

- **Conversão Universal:** Converte números entre qualquer base $X$ e $Y$ (ex: Binário para Hexadecimal, Decimal para Base 36, etc.);
- **Lógica "baixo nível":** Desenvolvimento focado no entendimento de como o computador processa dados, utilizando:
  - Divisões sucessivas para obtenção de restos;
  - Exponenciações para cálculo de valores posicionais (unidade, dezena, centena, etc.);
  - Manipulação de `char arrays` e **ponteiros** para gerenciamento de strings.
- **Independência de bibliotecas prontas ou feitas para isso:** O core do algoritmo foi desenhado para ser portátil e independente.

## Como Funciona

O algoritmo opera em dois estágios principais para garantir a precisão em qualquer base numérica:

1. **Normalização:** O número de entrada da base origem é convertido para **Base 10 (Decimal)** através de somas de potências;

$$N_{10} = \sum (dígito \cdot base ^{posicão})$$

2. **Conversão de Destino:** O valor decimal é então processado por divisões sucessivas pela base destino, onde os restos compõem o novo número (cada resto em uma casa numérica - unidade, dezena, centena...).

## Quer brincar com meu código?

Primeiro você precisa ter Git instalado em sua máquina, em seguida dentro de uma pasta você poderá utilizar `git clone` para transferir os arquivos para sua pasta.

```Bash
# Primeiro selecione uma pasta em sua máquina
cd sua-pasta-ou-area-de-trabalho

# Clonar o repositório
git clone https://github.com/Nicolas-Makiniks/Number-Converter.git
```

---

## Roadmap de Desenvolvimento

Atualmente o projeto está em desenvolvimento inicial, estou criando os arquivos para o repositório.

## Licença

Este projeto está sob a licença CC0 (Creative Commons Zero). Você é livre para copiar, modificar e distribuir o código, mesmo para fins comerciais, sem necessidade de permissão ou atribuição.

Projeto desenvolvido como parte dos estudos de Ciência da Computação na FAM.
