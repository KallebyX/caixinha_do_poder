# Contribuindo para a Caixinha do Poder

Obrigado por considerar contribuir com o projeto! Aqui estão algumas diretrizes.

## Como Contribuir

### Reportando Bugs

1. Verifique se o bug já não foi reportado nas [Issues](../../issues)
2. Use o template de bug report
3. Inclua:
   - Descrição clara do problema
   - Passos para reproduzir
   - Comportamento esperado vs atual
   - Screenshots (se aplicável)
   - Hardware utilizado

### Sugerindo Melhorias

1. Abra uma issue com a tag `enhancement`
2. Descreva claramente a melhoria proposta
3. Explique por que seria útil

### Pull Requests

1. Fork o repositório
2. Crie uma branch para sua feature (`git checkout -b feature/MinhaFeature`)
3. Faça commits com mensagens claras
4. Teste suas mudanças
5. Abra um Pull Request

## Padrões de Código

### Arduino/C++

- Indentação: 2 espaços
- Nomes de variáveis: camelCase
- Nomes de constantes: UPPER_CASE
- Comentários em português
- Documentar funções complexas

### Exemplo

```cpp
/**
 * @brief Descrição da função
 * @param parametro Descrição do parâmetro
 * @return Descrição do retorno
 */
void minhaFuncao(int parametro) {
  // Código aqui
}
```

## Estrutura do Projeto

```
caixinha_do_poder/
├── src/
│   ├── main/           # Código principal
│   ├── modules/        # Módulos auxiliares
│   └── tests/          # Códigos de teste
├── docs/               # Documentação
├── assets/             # Recursos (imagens, áudio)
└── hardware/           # Esquemáticos e modelos 3D
```

## Agradecimentos

Toda contribuição é bem-vinda, desde correções de typos até novas features!
